#include "message.hpp"
#include "slot.hpp"
#include "quorum.hpp"
#include <string>
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"


using namespace DISTPROJ;




// template<class Archive>
// void Message::serialize(Archive & archive) {
//   auto pm = (PrepareMessage*) this;
//   auto fm = (FinishMessage*) this;
//   switch (t) {
//   case PrepareMessage_t:
// 	pm->serialize(archive);
// 	break;
//   case FinishMessage_t:
// 	fm->serialize(archive);
// 	break;
//   default:
// 	archive("grave error: Tried to serialize raw message");
// 	break;
//   }
// }


bool FinishMessage::isBiggerNumberThan( std::shared_ptr<Message> m) {
  auto message = std::static_pointer_cast<FinishMessage>(m);
  switch (m->type()){
  case FinishMessage_t:
	return b.num > message->b.num;
  default:
	return true;
  }
}


bool PrepareMessage::isBiggerNumberThan( std::shared_ptr<Message> m) {
  auto message = std::static_pointer_cast<PrepareMessage>(m);
  auto first = b.num > message->b.num;
  auto first_continue = (b.num == message->b.num);
  auto second = p.num > message->p.num;
  auto second_continue = (p.num == message->p.num);
  auto third = p_.num > message->p_.num;
  auto third_continue = (p_.num == message->p_.num);
  auto fourth = c.num > message->c.num;

  switch (m->type()){
  case FinishMessage_t:
	return false;
  case PrepareMessage_t:
	return first || ( first_continue && (second || (second_continue && (third || (third_continue && fourth))))); // See SCP pg 29
  default:
	return true; 
  }
}

