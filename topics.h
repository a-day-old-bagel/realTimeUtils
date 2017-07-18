#pragma once

#include <string>
#include <atomic>
#include "delegate.h"

namespace rtu {
  namespace topics {
    void publish(std::string topic, void* data);
    typedef Delegate<void(void*)> Action;
    class Subscription {
      public:
        Subscription(const std::string && topic, const Action && action);
        virtual ~Subscription();
      private:
        std::string topic;
        uint32_t id;
        static std::atomic<uint32_t> nextId;
    };
  }
}
