#ifndef SLOT_H
#define SLOT_H

#include <string>
#include <map>
#include <mutex>
#include <sstream>

#include "ballot.hpp"
#include "quorum.hpp"
#include "common.hpp"

namespace DISTPROJ {

  class Message;
  class PrepareMessage;
  class FinishMessage;
  class LocalNode;
  enum Phase { PREPARE, FINISH, EXTERNALIZE };

  struct SlotState {
    Ballot b;   // ballot
    Ballot p;   // prepare
    Ballot p_;  // prepare backup
    Ballot c;   // commit
    unsigned int slotNum; // slot sequence number
  };


  class Slot {

  public:
    Slot(int id, LocalNode* m);

    void handle(std::shared_ptr<Message> msg);

    // Dump state / received message inforamtion.
    void Dump();
    std::string printSlot();
    Phase GetPhase() { return slotPhase;};
    std::string GetValue() { return state.c.value;};

    std::string Phase_s() {

    const static std::map<Phase, std::string> phase= {{PREPARE, "Prepare"}, {FINISH, "Finish"}, {EXTERNALIZE, "Externalize"}};
    return phase.at(slotPhase);
    };
  private:
    void handle(std::shared_ptr<PrepareMessage> msg);
    void handle(std::shared_ptr<FinishMessage> msg);
    void getMessageFromMap(NodeID n, std::shared_ptr<Message>* m);
    bool isMember(NodeID from);

    SlotState state;
    Phase slotPhase;
    std::map<NodeID, std::shared_ptr<Message>> messageMap_;
    LocalNode * node;
    std::mutex mtx;

    std::shared_ptr<PrepareMessage> Prepare();
    std::shared_ptr<FinishMessage> Finish();

  };
}

#endif
