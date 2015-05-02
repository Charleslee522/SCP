#ifndef NODE_H
#define NODE_H

#include <set>
#include <map>
#include <cstdlib>
#include <cstdint>
#include <thread>

#include "common.hpp"
#include "quorum.hpp"

namespace DISTPROJ {

  class RPCLayer;
  class Message;
  class Slot;

  class Node {

  public:
    Node(NodeID _id, RPCLayer& _rpc);
    Node(NodeID _id, RPCLayer& _rpc, Quorum _quorumSet);

    NodeID GetNodeID();
    Quorum GetQuorumSet();
    void PrintQuorumSet();

  protected:
    NodeID id;
    RPCLayer& rpc;
    Quorum quorumSet;

  };

  class LocalNode : public Node {

  public:
    LocalNode(NodeID _id, RPCLayer& _rpc)
      : Node(_id, _rpc) {};
    LocalNode(NodeID _id, RPCLayer& _rpc, Quorum _quorumSet) 
      : Node(_id, _rpc, _quorumSet) {}; 

    void AddKnownNode(NodeID v);
    void AddNodeToQuorum(NodeID v);
    void UpdateQurorum(Quorum quorumSet);

    void Start();
    void SendMessage(Message& msg);
    void ProcessMessage(Message& msg);

    void DumpLog();

  private:
    void Tick();
    std::thread * t;
    std::map<unsigned int, Slot*> log;
    std::set<NodeID> knownNodes;

  };

}

#endif
