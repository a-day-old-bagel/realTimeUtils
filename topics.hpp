#pragma once

#include <string>
#include <atomic>
#include "delegate.hpp"

namespace rtu {
  namespace topics {
    void publish(std::string topic, void* data);
    typedef Delegate<void(void*)> Action;
    class Subscription {
      public:
        Subscription(std::string topic, Action action);
        Subscription(const Subscription&) = delete; // no copies
        Subscription& operator=(const Subscription&) = delete; // no self-assignments
        Subscription(Subscription&&) = delete; // no moves
        Subscription& operator=(Subscription&&) = delete; // no move assignments
        virtual ~Subscription();
      private:
        std::string topic;
        uint32_t id;
        static std::atomic<uint32_t> nextId;
    };
  }
}
