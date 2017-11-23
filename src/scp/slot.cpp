#include "node.hpp"
#include "quorum.hpp"
#include "PrepareMessage.hpp"
#include "FinishMessage.hpp"
#include "slot.hpp"
#include <memory>
using namespace DISTPROJ;
#define NILBALLOT Ballot{0, ""}
Slot::Slot(int id, LocalNode* m) : slotPhase(PREPARE), node(m){
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

void Slot::getMessageFromMap(NodeID from, std::shared_ptr<Message>* last) {
  try {
    *last = messageMap_.at(from);
  }catch(std::out_of_range){
    *last = std::make_shared<PrepareMessage>(from, 0, NILBALLOT,NILBALLOT,NILBALLOT,NILBALLOT, Quorum{},0);
    messageMap_[from] = *last;
  }
}

bool Slot::isMember(NodeID from) {
  return node->GetQuorumSet().members.find(from) != node->GetQuorumSet().members.end();
        // Don't let message from nodes not in one of your quorum slices
        // change your state. Don't process the message.
        // TODO : Check that this is actually the behavior we want.
}

void Slot::handle(std::shared_ptr<Message> _msg){
  // Add the message to be the last message seen
  // Handle the response
  std::shared_ptr<Message> last;
#ifdef VERBOSE
{
  std::lock_guard<std::mutex> lock(mtx);
  printf("[NODE %llu] Before Handle Message", node->id);
  Dump();
}
#endif
  switch (_msg->type()) {
  case PrepareMessage_t:
    {
      auto pmsg = std::static_pointer_cast<PrepareMessage>(_msg);
      auto from = pmsg->from();

      if(! isMember(from)) { return; }

      // Check if we are already done.
      getMessageFromMap(from, &last);
      if (pmsg->isBiggerNumberThan(last)) { // handle bigger sequence number message only
        if (slotPhase == EXTERNALIZE && from != node->GetNodeID()) {  // ignore message from myself
          node->SendMessageTo(Finish(),from);
          return;
        }
        handle(pmsg);
#ifdef VERBOSE
        printf("[NODE %llu] push PrepareMessage from %llu to messageMap\n", node->id, from);
#endif
        messageMap_[from] = pmsg;
      }
    }
    break;
  case FinishMessage_t:
    {
      auto fmsg = std::static_pointer_cast<FinishMessage>(_msg);
      auto from = fmsg->from();
      
      if(! isMember(from)) { return; }

      // Check if we are already done.
      getMessageFromMap(from, &last);
      if (fmsg->isBiggerNumberThan(last)) { // handle bigger sequence number message only
        if (slotPhase == EXTERNALIZE && from != node->GetNodeID()) {  // ignore message from myself
          node->SendMessageTo(Finish(),from);
          return;
        }
        handle(fmsg);
#ifdef VERBOSE
  printf("[NODE %llu] push FinishMessage from %llu to messageMap\n", node->id, from);
#endif
        messageMap_[from] = fmsg;
      }
    }
    break;
  default:
    exit(EXIT_FAILURE);
    break;
  }
#ifdef VERBOSE
{
  std::lock_guard<std::mutex> lock(mtx);
  printf("[NODE %llu] After Handle Message", node->id);
  Dump();
}
#endif
}

void Slot::handle(std::shared_ptr<PrepareMessage> msg) {
#ifdef VERBOSE
  printf("[NODE %llu] PREPARE\n", node->id);
#endif
  bool returnNow = false;
  // If phase is not prepare, return
  if (slotPhase != PREPARE ) {
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
#ifdef VERBOSE
    printf("[NODE %llu] ballot is NULL(init), value: %s\n", node->id, msg->b.value.c_str());    
#endif
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
#ifdef VERBOSE
    printf("[NODE %llu] quorumSet.threshold: %d\n", node->id, node->quorumSet.threshold);
#endif
    for (auto kp : messageMap_) {
      auto m = kp.second;
      switch (m->type()) {
      case FinishMessage_t:
        if ((std::static_pointer_cast<FinishMessage>(m))->b == msg->b){
#ifdef VERBOSE
          printf("[NODE %llu] b_voted_or_accepted(%d -> %d); because of <FinishMessage>m->b == msg->b\n", node->id, b_voted_or_accepted, b_voted_or_accepted - 1);
#endif
          b_voted_or_accepted--;
        }
        break;
      case PrepareMessage_t:
        if ( (std::static_pointer_cast<PrepareMessage>(m))->b == msg->b ||
             (std::static_pointer_cast<PrepareMessage>(m))->p == msg->b){
#ifdef VERBOSE
              if((std::static_pointer_cast<PrepareMessage>(m))->b == msg->b) {
                printf("[NODE %llu] b_voted_or_accepted(%d -> %d); because of <PrepareMessage>m->b == msg->b\n", node->id, b_voted_or_accepted, b_voted_or_accepted - 1);
              } else {
                printf("[NODE %llu] b_voted_or_accepted(%d -> %d); because of <PrepareMessage>m->p == msg->b\n", node->id, b_voted_or_accepted, b_voted_or_accepted - 1);
              }
#endif
          b_voted_or_accepted--;
        }
        break;
      default:
        // do nothing
        break;
      }
      // This can be moved outside of the for loop -- this let's it
      // duck out as soon as threshold crossed
      if ( b_voted_or_accepted == 0 ) {
        state.p = state.b;
        returnNow = true;
#ifdef VERBOSE
        printf("[NODE %llu] returnNow = true(b_voted_or_accepted == 0), state.p = state.b;\n", node->id);
#endif
        break;
      }
    }
  } else {
    // Statement contradicted. Check for v-blocking.
#ifdef VERBOSE
        printf("[NODE %llu] check v-blocking\n", node->id);
#endif
    auto b_vblock_vote = node->quorumSet.threshold;
    for (auto kp : messageMap_) {
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
#ifdef VERBOSE
        printf("[NODE %llu] returnNow 1(b_vblock_vote)\n", node->id);
#endif
        break;
      }
    }

  }

  // If a c ballot exists but p >!~ c or p_ >!~ c, clear c.
  if (state.c.num != 0 && (state.p > state.c || state.p_ > state.c)) {
    state.c = Ballot{};
#ifdef VERBOSE
    std::cout << "[NODE " << node->id << "] returnNow 2" << std::endl;
#endif
    returnNow = true;
  }

  if (returnNow) {
#ifdef VERBOSE
    std::cout << "[NODE " << node->id << "] returnNow -> Send Prepare Message" << std::endl;
#endif
    node->SendMessage(Prepare()); // meaningless line...
  }

  if ( state.b != state.c && state.b == state.p /* V confirms b is prepared */ ) {
#ifdef VERBOSE
    printf("[NODE %llu] prepared but not committed(state.b != state.c && state.b == state.p)\n", node->id);
#endif
    auto b_prepared = node->quorumSet.threshold;
    for(auto kp : messageMap_) {
      auto m = kp.second;
      switch (m->type()){
      case FinishMessage_t:
        if ((std::static_pointer_cast<FinishMessage>(m))->b == state.p){
#ifdef VERBOSE
          printf("[NODE %llu] b_prepared(%d -> %d); because of <FiniishMessage>m->b == state.b\n", node->id,b_prepared, b_prepared - 1);
#endif
          b_prepared--;
        }
        break;
      case PrepareMessage_t:
        if ( (std::static_pointer_cast<PrepareMessage>(m))->b == state.p){
#ifdef VERBOSE
          printf("[NODE %llu] b_prepared(%d -> %d); because of <PrepareMessage>m->b == state.b\n", node->id,b_prepared, b_prepared - 1);
#endif
          b_prepared--;
        }
        break;
      }

      if (b_prepared == 0 ){
        state.c = state.b;
#ifdef VERBOSE
        std::cout << "[NODE " << node->id << "] returnNow -> Send Finish Message because of (b_prepared == 0), state.c = state.b;" << std::endl;
#endif
        node->SendMessage(Finish());
        break;
      }
    }
  }
}
void Slot::handle(std::shared_ptr<FinishMessage> msg) {
#ifdef VERBOSE
  printf("[NODE %llu] Finish\n", node->id);
#endif
  // Finish message implies every statement implied by Prepare v i b b 0 b D.
  // auto p = std::make_shared<PrepareMessage>(node->GetNodeID(), state.slotNum, state.b, state.b, Ballot{}, state.b, node->GetQuorumSet(),0); 
  // handle(p);
  if (slotPhase == PREPARE && state.b == state.p && state.b == state.c && state.b == msg->b) { // RULE 3
    slotPhase = FINISH;
#ifdef VERBOSE
  printf("[NODE %llu] slotPhase to FINISH\n", node->id);
#endif
    // TODO (JHH) : Figure what if anything needs to happen here.
    //return; /// ???????????????????>?>?????????????????questionmark??
  }
  if (slotPhase == FINISH && state.b == state.p && state.b == state.c && state.b == msg->b){ // RULE 4
    // Check that this node ~confirms~ b.
    auto b_commit = node->quorumSet.threshold;
    for (auto kp : messageMap_) {
      auto m = kp.second;
      switch (m->type()) {
      case FinishMessage_t:
        if ((std::static_pointer_cast<FinishMessage>(m))->b == state.c){ // Finish -> b == state -> c
#ifdef VERBOSE
          printf("[NODE %llu] b_commit(%d -> %d); because of <FinishMessage>m->b == state.c(==msg->b)\n", node->id, b_commit, b_commit - 1);
#endif
          b_commit--;
        }
        break;
      case PrepareMessage_t:
        if ((std::static_pointer_cast<PrepareMessage>(m))->c == state.c){
#ifdef VERBOSE
          printf("[NODE %llu] b_commit(%d -> %d); because of <PrepareMessage>m->c == state.c(==msg->b)\n", node->id, b_commit, b_commit - 1);
#endif
          b_commit--;
        }
        break;
      }
#ifdef VERBOSE
      printf("[NODE %llu] Externalizing need if (c = %d) == 0\n", node->id, b_commit);
#endif
      if (b_commit == 0) {
#ifdef VERBOSE
  printf("[NODE %llu] slotPhase to EXTERNALIZE\n", node->id);
#endif
        slotPhase = EXTERNALIZE;
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
  std::cout << this->printSlot() << std::endl;
}

std::string Slot::printSlot() {
  std::stringstream stream;
  stream << " [Phase " << Phase_s() << "] slot: " << state.slotNum << ", b: " << state.b.num << 
  ", p: " << state.p.num << ", p_: " << state.p_.num << ", c:" << state.c.num << "\n\n";
  return stream.str();
}
