#include "node.hpp"
#include "quorum.hpp"
#include "message.hpp"
#include "slot.hpp"
#include <memory>
using namespace DISTPROJ;
#define NILBALLOT Ballot{0, ""}
Slot::Slot(int id, LocalNode* m) : phi(PREPARE), node(m){
  state.slotNum = id;
  state.b = NILBALLOT;
  state.p = NILBALLOT;
  state.p_ = NILBALLOT;
  state.c = NILBALLOT;
}

std::shared_ptr<PrepareMessage> Slot::Prepare() {
  auto p = std::make_shared<PrepareMessage>(node->GetNodeID(), state.slotNum, state.b, state.p, state.p_, state.c, node->GetQuorumSet(),0); 
  return p;
}

std::shared_ptr<FinishMessage> Slot::Finish() {
  auto p = std::make_shared<FinishMessage>(node->GetNodeID(), state.slotNum, state.b, node->GetQuorumSet());
  return p;
}

void Slot::lastDefined(NodeID from, std::shared_ptr<Message>* last) {
  try {
    *last = M.at(from);
  }catch(std::out_of_range){
    *last = std::make_shared<PrepareMessage>(from, 0, NILBALLOT,NILBALLOT,NILBALLOT,NILBALLOT, Quorum{},0);
    M[from] = *last;
  }

}

void Slot::handle(std::shared_ptr<Message> _msg){
  // Add the message to be the last message seen
  // Handle the response
  std::shared_ptr<Message> last;
#ifdef VERBOSE
  Dump();
#endif
  switch (_msg->type()) {
  case PrepareMessage_t:
    {
      auto pmsg = std::static_pointer_cast<PrepareMessage>( _msg);
      auto from = pmsg->from();

      if (node->GetQuorumSet().members.find(from) == node->GetQuorumSet().members.end()) {
        // Don't let message from nodes not in one of your quorum slices
        // change your state. Don't process the message.
        // TODO : Check that this is actually the behavior we want.
        return;
      }

      // Check if we are already done.
      lastDefined(from, &last);
      if (pmsg->follows(last)) {
        if (phi == EXTERNALIZE && from != node->GetNodeID()) {
          node->SendMessageTo(Finish(),from);
          return;
        }
        handle(pmsg);
        M[from] = pmsg;
      }
    }
    break;
  case FinishMessage_t:
    {
      auto fmsg = std::static_pointer_cast<FinishMessage>( _msg);
      auto from = fmsg->from();
      
      if (node->GetQuorumSet().members.find(from) == node->GetQuorumSet().members.end()) {
        // Don't let message from nodes not in one of your quorum slices
        // change your state. Don't process the message.
        // TODO : Check that this is actually the behavior we want.
        return;
      }

      // Check if we are already done.
      lastDefined(from, &last);
      if (fmsg->follows(last)) {
        if (phi == EXTERNALIZE && from != node->GetNodeID()) {
          node->SendMessageTo(Finish(),from);
          return;
        }
        handle(fmsg);
        M[from] = fmsg;
      }
    }
    break;
  default:
    exit(EXIT_FAILURE);
    break;
  }
#ifdef VERBOSE
  Dump();
#endif
}
void Slot::handle(std::shared_ptr<PrepareMessage> msg) {
#ifdef VERBOSE
  printf("PREPARE\n");
#endif
  bool returnNow = false;
  // If phase is not prepare, return
  if (phi != PREPARE ) {
    // Send 1-to-1 finish message to from node.
    return;
  }

  // Definition -- vote:
  //  node v votes for a iff
  //  1) v asserts a is valid/consistent
  //  2) v promises not to vote against a.

  // First case: We've never voted for anything. I.E. b = 0;
  // Vote for b but don't accept yet.
  if (state.b == NILBALLOT) {
    state.b.value = msg->b.value;
    state.b.num = 1;
    // Send out vote for b.
    node->SendMessage(Prepare());
    return;
  }

  // if( true /* && a message allows v to accept that new ballots are prepared by either of accepts 2 criteria */) {
  // if prepared ballot then set p

  // Definition -- Accept:
  //  node v accepts statement a (the value in b) iff it has never accepted
  //  a contradicting statment and
  //  1) there exists U s.t. node v is in U and everyone in U has voted for
  //    or accepted a. OR
  //  2) Each member of a v-blocking set claims to accept a.

  // Check that we haven't accepted a contradicting statement.

  // NOTE : the > operator does not accomplish the logic below.
  if (compatible(msg->b, state.p) || state.p == NILBALLOT){ 
    // Now check that one of our quorum slices has all voted for or 
    // accepted b.
    auto b_voted_or_accepted = node->quorumSet.threshold;
    for (auto kp : M) {
      auto m = kp.second;
      switch (m->type()) {
      case FinishMessage_t:
        if ((std::static_pointer_cast<FinishMessage>(m))->b == msg->b){
          b_voted_or_accepted--;
        }
        break;
      case PrepareMessage_t:
        if ( (std::static_pointer_cast<PrepareMessage>(m))->b == msg->b ||
             (std::static_pointer_cast<PrepareMessage>(m))->p == msg->b){
          b_voted_or_accepted--;
        }
        break;
      }
      // This can be moved outside of the for loop -- this let's it
      // duck out as soon as threshold crossed
      if ( b_voted_or_accepted == 0 ) {
        state.p = state.b;
        returnNow = true;
        break;
      }
    }
  } else {
    // Statement contradicted. Check for v-blocking.
    auto b_vblock_vote = node->quorumSet.threshold;
    for (auto kp : M) {
      auto m = kp.second;
      switch (m->type()) {
      case FinishMessage_t:
        if ((std::static_pointer_cast<FinishMessage>(m))->b == msg->b){
          b_vblock_vote--;
        }
        break;
      case PrepareMessage_t:
        if ((std::static_pointer_cast<PrepareMessage>(m))->p == msg->b){
          b_vblock_vote--;
        }
        break;
      }

      if (b_vblock_vote  == 0) {
        // v-blocking set found so vote the contradicting ballot.
        state.p_ = state.p;
        state.p = Ballot{};
        state.b.value = msg->b.value;
        state.b.num += 1;
        returnNow = true;
        break;
      }
    }

  }

  // If a c ballot exists but p >!~ c or p_ >!~ c, clear c.
  if (state.c.num != 0 && (state.p > state.c || state.p_ > state.c)) {
    state.c = Ballot{};
    returnNow = true;
  }

  if (returnNow) {
    node->SendMessage(Prepare());
  }

  if ( state.b != state.c && state.b == state.p /* V confirms b is prepared */ ) {
    auto b_prepared = node->quorumSet.threshold;
    for(auto kp : M) {
      auto m = kp.second;
      switch (m->type()){
      case FinishMessage_t:
        if ((std::static_pointer_cast<FinishMessage>(m))->b == state.p){
          b_prepared--;
        }
        break;
      case PrepareMessage_t:
        if ( (std::static_pointer_cast<PrepareMessage>(m))->b == state.p){
          b_prepared--;
        }
        break;
      }

      if (b_prepared  == 0 ){
        state.c = state.b;
        node->SendMessage(Finish());
        break;
      }
    }

  }


}
void Slot::handle(std::shared_ptr<FinishMessage> msg) {
#ifdef VERBOSE
  printf("Finish\n");
#endif
  // Finish message implies every statement implied by Prepare v i b b 0 b D.
  auto p = std::make_shared<PrepareMessage>(node->GetNodeID(), state.slotNum, state.b, state.b, Ballot{}, state.b, node->GetQuorumSet(),0); 
  // handle(p);
  if (phi == PREPARE && state.b == state.p && state.b == state.c && msg->b == state.b) { // RULE 3
    phi = FINISH;
    // TODO (JHH) : Figure what if anything needs to happen here.
    //return; /// ???????????????????>?>?????????????????questionmark??
  }
  if (phi == FINISH && state.b == state.p && state.b == state.c && msg->b == state.b){ // RULE 4
    // Check that this node ~confirms~ b.
    auto b_commit = node->quorumSet.threshold;
    for (auto kp : M) {
      auto m = kp.second;
      switch (m->type()) {
      case FinishMessage_t:
        if ((std::static_pointer_cast<FinishMessage>(m))->b == state.c){ // Finish -> b == Prepare -> c
          b_commit--;
        }
        break;
      case PrepareMessage_t:
        if ( (std::static_pointer_cast<PrepareMessage>(m))->c == state.c){
          b_commit--;
        }
        break;
      }
#ifdef VERBOSE
      printf("Externalizing need %d", b_commit);
#endif
      if (b_commit == 0) {
        phi = EXTERNALIZE;
        break;
      }
    }
  } else {
    // TODO : Might need to check for a v-blocking set and go back into the 
    // prepare state.
  }

}
// Dump state / received message inforamtion.
void Slot::Dump(){
  printf("Dumping id: %llu\n    slot: %u, b: %d, p: %d, p_: %d, c:%d \n%s\n, Phase %s\n",node->GetNodeID(),state.slotNum, state.b.num,state.p.num,state.p_.num, state.c.num,state.c.value.c_str(), Phase_s().c_str());
}

