#include "FinishMessage.hpp"
#include "slot.hpp"
//#include "quorum.hpp"
// #include <string>
// #include "cereal/archives/json.hpp"
// #include "cereal/types/polymorphic.hpp"

using namespace DISTPROJ;

bool FinishMessage::isBiggerNumberThan( std::shared_ptr<Message> m) {
  auto message = std::static_pointer_cast<FinishMessage>(m);
  switch (m->type()){
  case FinishMessage_t:
	return b.num > message->b.num;
  default:
	return true;
  }
}
