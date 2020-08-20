
#pragma once

#include <string>
#include <unordered_map>
#include "topics.hpp"

namespace rtu {

  class EventResponseMap {
    private:
      std::unordered_map<std::string, rtu::topics::Subscription> userInputSubs;
    public:
      EventResponseMap() = default;
      void setAction(const std::string& topic, const rtu::topics::DAction& dAction);
      void setAction(const std::string& topic, const rtu::topics::DSimpleAction& dSimpleAction);
		  void setAction(const std::string& topic, const rtu::topics::FAction& fAction);
		  void setAction(const std::string& topic, const rtu::topics::FSimpleAction& fSimpleAction);
  };

}
