#ifndef MESSAGE_H
#define MESSAGE_H
#include "types.hpp"
#include <string>

namespace DISTPROJ {

  enum MessageType {FinishMessage_t, PrepareMessage_t};
  
  struct Ballot;
  class Quorum;
  class Message {

  public:
    virtual MessageType type();
    virtual std::string toJSON();
    Message fromJSON(std::string s);
    virtual unsigned int getSlot();

  };
  
  class PrepareMessage : public Message {

  public:
    PrepareMessage(NodeID _v, unsigned int _slotID, Ballot& _b, Ballot& _p,
                   Ballot& _p_0, Ballot& _c, Quorum& _d)
      : v(_v), slotID(_slotID), b(_b), p(_p), p_0(_p_0), c(_c), d(_d) {};

    MessageType type() { return PrepareMessage_t; };
    unsigned int getSlot() { return slotID; };
    std::string toJSON();

  private:
    NodeID v;
    unsigned int slotID;
    Ballot& b,  & p,& p_0,& c;
    Quorum& d;

    
  };
  
  class FinishMessage : public Message {
    
  public:
    FinishMessage(NodeID _v, unsigned int _slotID, Ballot* _b, Quorum* _d)
      : v(_v), slotID(_slotID), b(_b), d(_d) {};

    MessageType type() { return FinishMessage_t; };
    unsigned int getSlot() { return slotID; };
    std::string toJSON();

  private:
    NodeID v;
    unsigned int slotID;
    Ballot* b;
    Quorum* d;

  };

}

#endif
