
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
      void setAction(std::string topic, rtu::topics::Action action);
      void setAction(std::string topic, rtu::topics::SimpleAction simpleAction);
  };

}
