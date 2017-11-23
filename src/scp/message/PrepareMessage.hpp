#ifndef PREPARE_MESSAGE_H
#define PREPARE_MESSAGE_H

#include <string>

#include "message.hpp"
#include "ballot.hpp"
#include "quorum.hpp"
#include "common.hpp"
#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"

namespace DISTPROJ {

  class Slot;

  class PrepareMessage : public Message {

  public:
	PrepareMessage() : PrepareMessage(0,0,Ballot{}, Ballot{}, Ballot{}, Ballot{}, Quorum{}, 0){};
    PrepareMessage(NodeID _v, SlotNum _slotID, Ballot _b, Ballot _p,
                   Ballot _p_, Ballot _c, Quorum _d, Nonce n)
      :  Message(PrepareMessage_t),v(_v), slotID(_slotID), b(_b), p(_p), p_(_p_), c(_c), d(_d), nonce(n) {};

    template<class Archive>
    void serialize(Archive & archive) {
      archive(CEREAL_NVP(v), CEREAL_NVP(slotID), CEREAL_NVP(b),
              CEREAL_NVP(p), CEREAL_NVP(p_), CEREAL_NVP(c),
              CEREAL_NVP(d), CEREAL_NVP(nonce));
    };
    unsigned int getSlotId() { return slotID; };
    NodeID from() {return v;};

    bool isBiggerNumberThan( std::shared_ptr<Message> x);
  private:
    NodeID v;
    unsigned int slotID;
    Ballot b, p, p_, c;
    Quorum d;
    Nonce nonce;

    friend Slot;
    
  };

}

CEREAL_REGISTER_POLYMORPHIC_RELATION(DISTPROJ::Message, DISTPROJ::PrepareMessage);
CEREAL_REGISTER_TYPE_WITH_NAME(DISTPROJ::PrepareMessage, "Prepare");

#endif
