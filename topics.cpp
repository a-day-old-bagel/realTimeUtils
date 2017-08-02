
#include <unordered_map>
#include "topics.hpp"

namespace rtu {
  namespace topics {

    namespace {
      struct Topic {
        std::unordered_map<uint32_t, Action> subscriptions;
        void publish(void* data) {
          for (auto sub : subscriptions) {
            sub.second(data);
          }
        }
      };
      std::unordered_map<std::string, Topic> activeTopics;
    }

    Subscription::Subscription(std::string topic, Action action) : topic(topic), id(++nextId) {
      activeTopics[topic].subscriptions[id] = action;
      printf("SUB: %s %u\n", topic.c_str(), id);
    }

    Subscription::~Subscription() {
      activeTopics[topic].subscriptions.erase(id);
      printf("~SUB: %s %u\n", topic.c_str(), id);
    }

    std::atomic<uint32_t> Subscription::nextId(0);

    void publish(std::string topic, void* data) {
      activeTopics[topic].publish(data);
    }

  }
}