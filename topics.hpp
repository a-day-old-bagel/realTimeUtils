#pragma once

#include <string>
#include <atomic>
#include <memory>
#include "delegate.hpp"

// Get a std::unique_ptr to a subscription without all the syntax
#define RTU_MAKE_SUB_UNIQUEPTR(tpc, fnc, inst) std::make_unique<Subscription>(tpc, RTU_MTHD_DLGT(&fnc, inst));

namespace rtu {
  namespace topics {

    void publishPtr(const std::string &topic, void *data);

    void publish(const std::string& topic);

    template <typename DataType>
    void publish(const std::string& topic, const DataType&& data) {
      publishPtr(topic, (void *) &data);
    }

    template <typename DataType>
    void publish(const std::string& topic, const DataType& data) {
      publishPtr(topic, (void *) &data);
    }

    typedef Delegate<void(void*)> Action;
    typedef Delegate<void()> SimpleAction;

    class Subscription {
      public:
        Subscription(const std::string &topic, const Action &action);
        Subscription(const std::string &topic, const SimpleAction &simpleAction);
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
