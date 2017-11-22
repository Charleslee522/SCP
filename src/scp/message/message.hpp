#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include "ballot.hpp"
#include "quorum.hpp"
#include "common.hpp"
#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"

namespace DISTPROJ {

  enum MessageType {FinishMessage_t=0, PrepareMessage_t=1};
  
  class Slot;
  class Message {
  private:
    MessageType t;

  public:
    Message(MessageType t) : t(t){};
    MessageType type() { return t;};
    virtual unsigned int getSlot() = 0;

    virtual bool isBiggerNumberThan(std::shared_ptr<Message> x) = 0;
	
  };

}

#endif
