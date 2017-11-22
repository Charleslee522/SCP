#include <iostream>
#include <array>

#include "RPC.hpp"
#include "fakeRPC.hpp"
#include "PrepareMessage.hpp"
#include "FinishMessage.hpp"
#include "quorum.hpp"
#include "node.hpp"
#include "ballot.hpp"
#include <chrono>
#include <thread>
#include <fstream>
#include "stellarkv.hpp"

using namespace DISTPROJ;
using namespace DISTPROJ::Application::StellarKV;
using namespace std;
const int N = 4;
const float THRESHOLD = 0.8;
unsigned long long uuid = 0;

void printStatus(std::array<shared_ptr<StellarKV>, N>& nodes, std::string key) {
  for (;;this_thread::sleep_for(chrono::seconds(1))){
    auto count = N;
    for (auto n : nodes) {
      auto r = n->Get(key);
      if (r.second){
        printf("Key was set to (%s) on %llu\n",r.first.second.c_str(), n->GetNodeID());
        count--;
      }
    }
    if (count == 0) {
      break;
    }
  }

}

int func() {
  std::array<shared_ptr<StellarKV>, N> nodes;
  // Create transport layer.
  shared_ptr<FakeRPCLayer> rpc = make_shared<FakeRPCLayer>();
  // Create nodes.
  for (auto i =0; i < N; ++i)
    nodes[i] = make_shared<StellarKV>(rpc, THRESHOLD);
  set<NodeID> s;
  for (auto i =0; i < N; ++i)
    s.insert(nodes[i]->GetNodeID());

  for (auto i =0; i < N; ++i) {
    nodes[i]->AddPeers(s);
  }
  //printf("Need to get %i nodes to agree out of %i nodes\n",nodes[0]->GetThreshold(), N);
  nodes[0]->Put("test", "MESSAGE");
  printStatus(nodes, "test");

  nodes[0]->Put("test2", "MESSAGE2");
  nodes[1]->Put("test2", "MESSAGE2");
  printStatus(nodes, "test2");
  
  nodes[0]->Put("test3", "MESSAGE3.1");
  nodes[1]->Put("test3", "MESSAGE3.2");
  printStatus(nodes, "test3");

  printf("COMPLETE\n");
  return 0;
}



 int main(int argc, char *argv[]) {
   std::array<shared_ptr<StellarKV>, N+1> nodes;
   // Create transport layer.
   shared_ptr<FakeRPCLayer> rpc = make_shared<FakeRPCLayer>();

   // Create nodes.
   for (auto i = 0; i < N; ++i){
     nodes[i] = make_shared<StellarKV>(rpc, THRESHOLD);
   }

   // Print IDs.
   for (auto i = 0; i < N; ++i)
     std::cout << i << "'th node id: " << nodes[i]->GetNodeID() << "\n";

   // Create quorum sets.
   Quorum qs;
   qs.threshold = 3;

   for(auto i = 0; i < N; ++i) {
    qs.members.insert(nodes[i]->GetNodeID());
   }

   // Add quorum sets to nodes.
   for (auto i = 0; i < N-1; ++i)
     nodes[i]->getNode()->UpdateQuorum(qs);

   // Print a nodes quorum set threshold.
   nodes[0]->getNode()->PrintQuorumSet();

   // Update the quorum set.
   nodes[N] = make_shared<StellarKV>(rpc, THRESHOLD);
   nodes[0]->getNode()->AddNodeToQuorum(nodes[N]->GetNodeID());
   
   printf("Add Node\n");
   nodes[0]->getNode()->PrintQuorumSet();
/*
   printf("Update Quorum\n");
   nodes[1]->getNode()->UpdateQuorum(qs);
   nodes[0]->getNode()->PrintQuorumSet();
   nodes[1]->getNode()->PrintQuorumSet();


   nodes[1]->Put("key to node 0", " message from node 0");
*/   // Make a sample message.
 //  Ballot dummyBallot;
 //  dummyBallot.num = 1;
 //  dummyBallot.value = "";

   // auto samplePrepareMsg = std::make_shared<PrepareMessage>(
   //     nodes[0]->GetNodeID(),
   //     0,
   //     dummyBallot,
   //     dummyBallot,
   //     dummyBallot,
   //     dummyBallot,
   //     nodes[0]->GetQuorumSet());

   // nodes[0]->SendMessage(samplePrepareMsg);
   //nodes[0]->getNode()->Propose("hello");
   //std::this_thread::yield();
  // std::this_thread::sleep_for(std::chrono::seconds(1));
   // printf("sending duplicate message\n");
   // nodes[0]->SendMessage(samplePrepareMsg);

   // nodes[1]->ReceiveMessage();
/*
   for (auto i = 0; i < N; ++i)
     nodes[i]->getNode()->Start();

   printf("Implementation coming soon\n");
   while (true){

   }
*/
   return 0;
 }
