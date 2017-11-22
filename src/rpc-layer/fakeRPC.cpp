#include <vector>
#include <sstream>
#include <cstdio>

#include "queue.hpp"
#include "RPC.hpp"
#include "node.hpp"
#include "slot.hpp"
#include "fakeRPC.hpp"
#include "message.hpp"

using namespace DISTPROJ;

// FakeRPCLayer

FakeRPCLayer::FakeRPCLayer(){}

void FakeRPCLayer::AddNode(NodeID node) {
  messageQueues[node] = new Queue<std::string>();
}

MessageClient* FakeRPCLayer::GetClient(NodeID id) {
  return new MessageClient(id, this); 
}

void FakeRPCLayer::Send(std::shared_ptr<Message> msg, NodeID id, NodeID peerID) {
#ifdef VERBOSE
  if(msg->type() == PrepareMessage_t) {
    printf("[NODE %llu] Send Prepare Message To %llu\n", id, peerID);
  } else {
    printf("[NODE %llu] Send Finish Message To %llu\n", id, peerID);    
  }
#endif
// if stoping message to myself, system stucked.
//   if(id == peerID) {
// #ifdef VERBOSE
//   printf("[NODE %llu] Don't Send Message To Myself\n", id);
// #endif
//     return;
//   }
  std::ostringstream ss;
  {
    cereal::JSONOutputArchive archive(ss);
    archive(CEREAL_NVP(msg), CEREAL_NVP(id));
  }
  messageQueues[peerID]->Add(ss.str());
}

bool FakeRPCLayer::Receive(std::shared_ptr<Message>* msg, NodeID id) {
  // We only have 1 thread dequeing so this is chill.
  if (messageQueues[id]->Empty()) {
    return false;
  } else {
    std::istringstream ss;
    NodeID receiveID;
    ss.str(messageQueues[id]->Get());
    {
      cereal::JSONInputArchive archive(ss);
      archive(*msg, receiveID);
    }
#ifdef VERBOSE
  if((*msg)->type() == PrepareMessage_t) {
    printf("[NODE %llu] Receive Prepare Message From %llu\n", id, receiveID);
  } else {
    printf("[NODE %llu] Receive Finish Message From %llu\n", id, receiveID);    
  }
#endif
    return true && *msg; // implicitly checks for validity
  }
}

void FakeRPCLayer::Broadcast(std::shared_ptr<Message> msg, NodeID id, std::set<NodeID> peers) {
  // Client messages itself.
  for (auto peer : peers) {
    Send(msg, id, peer);
  }
}

// MessageClient

MessageClient::MessageClient(NodeID id, RPCLayer* r) : id(id), rpc(r) {
}

void MessageClient::Send(std::shared_ptr<Message> msg, NodeID peerID) {
  rpc->Send(msg, id, peerID);
}

bool MessageClient::Receive(std::shared_ptr<Message>* msg) {
  return rpc->Receive(msg, id);
}

void MessageClient::Broadcast(std::shared_ptr<Message> msg, std::set<NodeID> peers) {
  rpc->Broadcast(msg, id, peers);
}
