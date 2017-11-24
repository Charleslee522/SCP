
#include <iostream>
#include <set>

#include "PrepareMessage.hpp"
#include "FinishMessage.hpp"
#include "fakeRPC.hpp"
#include "quorum.hpp"
#include "slot.hpp"
#include "node.hpp"
#include <chrono>
#include <stdio.h>
using namespace DISTPROJ;

Node::Node(NodeID _id, RPCLayer& _rpc) 
  : id(_id), rpc(_rpc), t(nullptr) {
  rpc.AddNode(id);
}

Node::Node(NodeID _id, RPCLayer& _rpc, Quorum _quorumSet) 
  : id(_id), rpc(_rpc), quorumSet(_quorumSet), t(nullptr) {
  rpc.AddNode(id);
}

NodeID Node::GetNodeID() { 
  return id;
}

Quorum Node::GetQuorumSet() {
  return quorumSet;
}

void Node::PrintQuorumSet() {
  printf("Printing quorum set for node %llu \n", id);
  printf("Threshold: %d\n", quorumSet.threshold);
  printf("Quorum members : ");
  std::set<NodeID>::iterator iter;
  for (iter=quorumSet.members.begin(); iter != quorumSet.members.end(); ++iter) {
    std::cout << (*iter) << ", ";
  }
  std::cout << std::endl;
}

LocalNode::LocalNode(NodeID _id, RPCLayer& _rpc)
  : Node(_id, _rpc) {
  mc = _rpc.GetClient(_id);
};
LocalNode::LocalNode(NodeID _id, RPCLayer& _rpc, Quorum _quorumSet) 
  : Node(_id, _rpc, _quorumSet) {
  mc = _rpc.GetClient(_id);
}; 

void LocalNode::Tick() {
  std::shared_ptr<Message> m;
  while (true){
    std::lock_guard<std::mutex> lock(mtx);
    if (ReceiveMessage(&m)) {
      ProcessMessage(m);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void LocalNode::Start() {
#ifdef VERBOSE
  printf("[NODE %llu] Start\n", id);
#endif
  if (t == nullptr) {
    t = new std::thread(&LocalNode::Tick, this);
  }
}

void LocalNode::AddKnownNode(NodeID v) {
  knownNodes.insert(v);
}

void LocalNode::UpdateQuorum(Quorum _quorumSet) {
  quorumSet = _quorumSet;
}

void LocalNode::AddNodeToQuorum(NodeID v) {
  quorumSet.members.insert(v);
}
void LocalNode::RemoveNodeFromQuorum(NodeID v) {
  quorumSet.members.erase(v);
}
int LocalNode::QuorumSize(){
  return quorumSet.members.size();

}
void LocalNode::SetThreshold(int t){
  if (t > QuorumSize()) {
    quorumSet.threshold = QuorumSize();
  }
  else{
    quorumSet.threshold = t;
  }
}
int LocalNode::GetThreshold(){
  return quorumSet.threshold;
}

SlotNum LocalNode::Propose(std::string value){
  auto i = NewSlot();
  Propose(value, i);
  return i;
}

void LocalNode::Propose(std::string value, SlotNum sn){
  std::lock_guard<std::mutex> lock(mtx);
  auto b = Ballot{1, value};
  auto nonce = generateNonce(&b, sn);
#ifdef VERBOSE
  printf("[NODE %llu] Nonce Found %llu\n", id, nonce);
#endif
  auto m = std::make_shared<PrepareMessage>(id, sn, b, Ballot{}, Ballot{}, Ballot{}, quorumSet, 0); /* TODO; resending etc */
  SendMessage(m);
#ifdef VERBOSE
  {
    //std::lock_guard<std::mutex> lock(mtx);
    printf("[NODE %llu] Broadcast(SendMessage) to every validator in LocalNode::Propose()\n", id);
    this->PrintQuorumSet();
  }
#endif
}

SlotNum LocalNode::NewSlot(){
  auto a = maxSlotId;
  //maxSlotId++;
  return a;
}

void LocalNode::SendMessage(std::shared_ptr<Message> msg) {
#ifdef VERBOSE
  printf("[NODE %llu] Broadcast Message", this->id);
  auto slot = log.find(msg->getSlotId());
  if(slot != log.end()) {
    slot->second->Dump();
  } else {
    printf("[NODE %llu] No slot(%d) in memory\n", this->id, msg->getSlotId());
  }
#endif
  mc->Broadcast(msg, GetQuorumSet().members);
}

void LocalNode::SendMessageTo(std::shared_ptr<Message> msg, NodeID i) {
#ifdef VERBOSE
  printf("[NODE %llu] Send Message To %llu", this->id, i);
  auto slot = log.find(msg->getSlotId());
  if(slot != log.end()) {
    slot->second->Dump();
  } else {
    printf("[NODE %llu] No slot(%d) in memory\n", this->id, msg->getSlotId());
  }
#endif
  mc->Send(msg, i);
}

bool LocalNode::ReceiveMessage(std::shared_ptr<Message>* msg) {
  bool received = mc->Receive(msg);
  if (received && msg) {
    return true;
  }
  return false;
}

void LocalNode::ProcessMessage(std::shared_ptr<Message> msg) {
  auto slotId = msg->getSlotId();
  if (log.find(slotId) == log.end()) {
    log[slotId] = std::make_shared<Slot>(slotId, this);
    if (slotId > maxSlotId) {
      maxSlotId = slotId;
    }
  }
#ifdef VERBOSE
  {
    //std::lock_guard<std::mutex> lock(mtx);    
    //printf("[NODE %llu] DumpLog\n", this->id);
    //this->DumpLog();
    printf("[NODE %llu] Received message: %s\n", this->id, log[slotId]->printSlot().c_str());
  }
#endif  
  log[slotId]->handle(msg);
}

void LocalNode::DumpLog() {
  for (auto slot : log) {
    slot.second->Dump();
  }
}



std::pair<std::string, bool> LocalNode::View(SlotNum s){
  std::lock_guard<std::mutex> lock(mtx);
  try{
#ifdef VERBOSE
    printf("[NODE %llu] Phase %s in View Method\n", id, log.at(s)->Phase_s().c_str());
#endif
    bool b = log.at(s)->GetPhase() == EXTERNALIZE;
    return std::pair<std::string, bool>(log.at(s)->GetValue(), b);
  } catch (std::out_of_range){
    return std::pair<std::string, bool>("", false);
  }
}
