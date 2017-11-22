#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>
#include <array>
#include <vector>

#include "RPC.hpp"
#include "fakeRPC.hpp"
#include "node.hpp"
#include "server.hpp"
#include "client.hpp"

using namespace DISTPROJ;
using namespace DISTPROJ::Application::KVStellar;
using namespace std;

unsigned long long uuid = 0;

void testOriginal();
void test3QuorumSet(double threshold);
void testSameMessage(double threshold);
void testOnlyOneQuorum(int n_nodes, double threshold);
void printCmdUsage();

int main(int argc, char *argv[]) {
    if(argc < 3) {
        printCmdUsage();
        return 0;
    }
    int n_nodes = atoi(argv[1]);
    double threshold = atof(argv[2]);
    if(n_nodes < 2) {
        printCmdUsage();
        return 0;
    }
    testOnlyOneQuorum(n_nodes, threshold);
}

void printCmdUsage() {
    printf("Usage: ./comet (n_nodes in one quorum >= 2) (threshold)\n");        
    printf("Example: ./comet 3 0.66\n");
}

void testOriginal() {
    const int SERVER_N = 6;
    const int CLIENT_N = 2;
  // Create transport layer.
  shared_ptr<FakeRPCLayer> rpc =  make_shared<FakeRPCLayer>();

  // Create servers and quorum sets.
  array< shared_ptr<ServerKV>, SERVER_N> servers;
  for (auto i =0; i < SERVER_N; i++)
    servers[i] = make_shared<ServerKV>(rpc, 0.8);
  set<NodeID> s;
  for (auto i =0; i < SERVER_N; i++)
    s.insert(servers[i]->GetNodeID());
  for (auto i =0; i < SERVER_N; i++)
    servers[i]->AddPeers(s);

  // Create client group 0 -- talk to server 0.
  array<ClientKV*, CLIENT_N> clients0;
  for (auto i=0; i < CLIENT_N; i++)
    clients0[i] = new ClientKV(servers[0], "");

  // Create client group 1 -- talk to server 1.
  array<ClientKV*, CLIENT_N> clients1;
  for (auto i=0; i < CLIENT_N; i++)
    clients1[i] = new ClientKV(servers[1], "");

  // Basic tests.
  printf("Runnning basic tests on comet key-value store.\n");

  // Test put on one client and get on another (both talk to the same server.).
  clients0[0]->Put("1", "Test");
  if (clients0[1]->Get("1") == "Test") {
    printf("Test PASSED\n");
  } else {
    printf("Test FAILED\n");
  }

  // Test get on client talking to another server.
  if (clients1[0]->Get("1") == "Test") {
    printf("Test PASSED\n");
  } else {
    printf("Test FAILED\n");
  }

  return;
}

void test3QuorumSet(double threshold) {
    const int SERVER_N = 11;
    const int SET1_SN = 5;
    const int SET2_SN = 7;

    printf("threshold: %lf\n", threshold);

    shared_ptr<FakeRPCLayer> rpc = make_shared<FakeRPCLayer>();

    array<shared_ptr<ServerKV>, SERVER_N> servers;

    for(auto i = 0; i < SERVER_N; ++i) {
        servers[i] = make_shared<ServerKV>(rpc, threshold);
    }

    set<NodeID> nodesSet1;
    for(auto i = 0; i < SET2_SN; ++i) {
        nodesSet1.insert(servers[i]->GetNodeID());
    }

    set<NodeID> nodesSet2;
    for(auto i = 5; i < SERVER_N; ++i) {
        nodesSet2.insert(servers[i]->GetNodeID());
    }

    for(auto i = 0; i < SET2_SN; ++i) {
        servers[i]->AddPeers(nodesSet1);
    }

    for(auto i = SET2_SN; i < SERVER_N; ++i) {
        servers[i]->AddPeers(nodesSet2);
    }
    
    shared_ptr<ClientKV> client1 = make_shared<ClientKV>(servers[0], "");
    shared_ptr<ClientKV> client2 = make_shared<ClientKV>(servers[SERVER_N - 1], "");
    
    // printf("Put 1 to servers[0]\n");
    // client1->Put("1", "Value 1");

    // printf("servers[9] has 1? ");
    // cout << client2->Get("1") << endl;

    printf("Put (2, Value 2) to servers[%d]\n", SERVER_N - 1);
    client2->Put("2", "Value 2");

    printf("servers[0] has Value 2? ");
    cout << client1->Get("2") << endl;

    printf("servers[0] has Value 1? ");
    cout << client1->Get("1") << endl;

    return;
}

void testSameMessage(double threshold) {
    const int SERVER_N = 11;
    const int SET1_SN = 5;
    const int SET2_SN = 7;

    printf("threshold: %lf\n", threshold);

    shared_ptr<FakeRPCLayer> rpc = make_shared<FakeRPCLayer>();

    array<shared_ptr<ServerKV>, SERVER_N> servers;

    for(auto i = 0; i < SERVER_N; ++i) {
        servers[i] = make_shared<ServerKV>(rpc, threshold);
    }

    set<NodeID> nodesSet1;
    for(auto i = 0; i < SET2_SN; ++i) {
        nodesSet1.insert(servers[i]->GetNodeID());
    }

    set<NodeID> nodesSet2;
    for(auto i = 5; i < SERVER_N; ++i) {
        nodesSet2.insert(servers[i]->GetNodeID());
    }

    for(auto i = 0; i < SET2_SN; ++i) {
        servers[i]->AddPeers(nodesSet1);
    }

    for(auto i = SET2_SN; i < SERVER_N; ++i) {
        servers[i]->AddPeers(nodesSet2);
    }
    
    shared_ptr<ClientKV> client1 = make_shared<ClientKV>(servers[0], "");
    shared_ptr<ClientKV> client2 = make_shared<ClientKV>(servers[SERVER_N - 1], "");
    
    printf("Put (1, Value 1) to servers[0]\n");
    client1->Put("1", "Value 1");

    printf("servers[%d] has Value 1? ", SERVER_N - 1);
    cout << client2->Get("1") << endl;

    printf("Put (2, Value 2) to servers[%d]\n", SERVER_N - 1);
    client2->Put("2", "Value 2");

    printf("servers[0] has Value 1? ");
    cout << client1->Get("1") << endl;

    printf("servers[0] has Value 2? ");
    cout << client1->Get("2") << endl;

    shared_ptr<ClientKV> client3 = make_shared<ClientKV>(servers[SET1_SN], "");
    printf("servers[%d] has Value 1? ", SET1_SN);
    cout << client3->Get("1") << endl;
    printf("servers[%d] has Value 2? ", SET1_SN);
    cout << client3->Get("2") << endl;

    return;
}

void testOnlyOneQuorum(int N_NODE, double threshold) {
    shared_ptr<FakeRPCLayer> rpc = make_shared<FakeRPCLayer>();

    vector< shared_ptr<ServerKV> > servers;

    for(auto i = 0; i < N_NODE; ++i) {
        servers.emplace_back(make_shared<ServerKV>(rpc, threshold));
    }

    set<NodeID> nodesSet1;
    for(auto i = 0; i < N_NODE; ++i) {
        nodesSet1.insert(servers.at(i)->GetNodeID());
    }

    for(auto i = 0; i < N_NODE; ++i) {
        servers.at(i)->AddPeers(nodesSet1);
    }
    
    shared_ptr<ClientKV> client1 = make_shared<ClientKV>(servers.front(), "");
    shared_ptr<ClientKV> client2 = make_shared<ClientKV>(servers.back(), "");
    
    printf("[NODE %llu] Put (1, Value 1)\n", servers.front()->GetNodeID());    
    client1->Put("1", "Value 1");

    printf("[NODE %llu] Get (1) = ", servers.back()->GetNodeID());
    cout << "[NODE " << servers.back()->GetNodeID() << "]" << client2->Get("1") << endl;

    return;
}

