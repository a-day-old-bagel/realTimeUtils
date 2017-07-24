
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
      printf("SUBSCRIPTION CREATED FOR TOPIC: %s\n", topic.c_str());
      activeTopics[topic].subscriptions[id] = action;
    }

    Subscription::~Subscription() {
      printf("SUBSCRIPTION CANCELLED FOR TOPIC: %s\n", topic.c_str());
      activeTopics[topic].subscriptions.erase(id);
    }

    std::atomic<uint32_t> Subscription::nextId(0);

    void publish(std::string topic, void* data) {
      if (topic.compare("key_held_space") == 0) {
        printf("SPACE topic: %u subscibers.\n", activeTopics.count(topic));
      }
      activeTopics[topic].publish(data);
    }

  }
}