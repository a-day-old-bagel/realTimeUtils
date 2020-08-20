
#include "eventResponseMap.hpp"

using namespace rtu::topics;

namespace rtu {

  void EventResponseMap::setAction(const std::string& topic, const DAction& dAction) {
    if (userInputSubs.count(topic)) {
      userInputSubs.erase(topic);
    }
    userInputSubs.emplace(std::piecewise_construct, std::make_tuple(topic), std::make_tuple(topic, dAction));
  }

  void EventResponseMap::setAction(const std::string& topic, const DSimpleAction& dSimpleAction) {
    if (userInputSubs.count(topic)) {
      userInputSubs.erase(topic);
    }
    userInputSubs.emplace(std::piecewise_construct, std::make_tuple(topic), std::make_tuple(topic, dSimpleAction));
  }

	void EventResponseMap::setAction(const std::string& topic, const FAction& fAction) {
		if (userInputSubs.count(topic)) {
			userInputSubs.erase(topic);
		}
		userInputSubs.emplace(std::piecewise_construct, std::make_tuple(topic), std::make_tuple(topic, fAction));
	}

	void EventResponseMap::setAction(const std::string& topic, const FSimpleAction& fSimpleAction) {
		if (userInputSubs.count(topic)) {
			userInputSubs.erase(topic);
		}
		userInputSubs.emplace(std::piecewise_construct, std::make_tuple(topic), std::make_tuple(topic, fSimpleAction));
	}

}
