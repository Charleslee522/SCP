#ifndef FINISH_MESSAGE_H
#define FINISH_MESSAGE_H

#include <string>

#include "ballot.hpp"
#include "quorum.hpp"
#include "common.hpp"
#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"

#include "message.hpp"

namespace DISTPROJ {

  class Slot;

  class FinishMessage : public Message {
    
  public:
	FinishMessage(): FinishMessage(0,0,Ballot{}, Quorum{}){};
    FinishMessage(NodeID _v, unsigned int _slotID, Ballot _b, Quorum _d)
      : Message(FinishMessage_t), v(_v), slotID(_slotID), b(_b), d(_d)  {};



    template<class Archive>
    void serialize(Archive & archive) {
      archive(CEREAL_NVP(v),CEREAL_NVP(slotID), CEREAL_NVP(b),CEREAL_NVP(d)); // serialize things by passing them to the archive
    };
    unsigned int getSlotId() { return slotID; };
    NodeID from() {return v;};
    bool isBiggerNumberThan( std::shared_ptr<Message> x);
  private:
    NodeID v;
    unsigned int slotID;
    Ballot b;
    Quorum d;

    friend Slot;
  };

}

CEREAL_REGISTER_POLYMORPHIC_RELATION(DISTPROJ::Message, DISTPROJ::FinishMessage);
CEREAL_REGISTER_TYPE_WITH_NAME(DISTPROJ::FinishMessage, "Finish");

#endif
