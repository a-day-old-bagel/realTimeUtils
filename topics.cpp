
#include <unordered_map>
#include <memory>
#include <cstdarg>
#include "topics.hpp"
#include "stringHelpers.hpp"

namespace rtu::topics {

  namespace {
    struct AnyAction {
        bool isSimple, isDelegate;
		    DAction dAction;
		    DSimpleAction dSimpleAction;
		    FAction fAction;
		    FSimpleAction fSimpleAction;
    };
    struct Topic {
      std::unordered_map<uint32_t, AnyAction> subscriptions;
      void publish(void* data) {
        for (const auto &sub : subscriptions) {
        	if (sub.second.isDelegate) {
		        sub.second.isSimple ? sub.second.dSimpleAction() : sub.second.dAction(data);
        	} else {
		        sub.second.isSimple ? sub.second.fSimpleAction() : sub.second.fAction(data);
        	}
        }
      }
    };
    std::unordered_map<std::string, Topic> activeTopics;
  }

  Subscription::Subscription(const std::string &topic, const DAction &dAction) : topic(topic), id(++nextId) {
    activeTopics[topic].subscriptions[id].dAction = dAction;
    activeTopics[topic].subscriptions[id].isSimple = false;
    activeTopics[topic].subscriptions[id].isDelegate = true;
  }

  Subscription::Subscription(const std::string &topic, const DSimpleAction &dSimpleAction) : topic(topic), id(++nextId){
    activeTopics[topic].subscriptions[id].dSimpleAction = dSimpleAction;
    activeTopics[topic].subscriptions[id].isSimple = true;
	  activeTopics[topic].subscriptions[id].isDelegate = true;
  }

	Subscription::Subscription(const std::string &topic, const FAction &fAction) : topic(topic), id(++nextId) {
		activeTopics[topic].subscriptions[id].fAction = fAction;
		activeTopics[topic].subscriptions[id].isSimple = false;
		activeTopics[topic].subscriptions[id].isDelegate = false;
	}
	
	Subscription::Subscription(const std::string &topic, const FSimpleAction &fSimpleAction) : topic(topic), id(++nextId){
		activeTopics[topic].subscriptions[id].fSimpleAction = fSimpleAction;
		activeTopics[topic].subscriptions[id].isSimple = true;
		activeTopics[topic].subscriptions[id].isDelegate = false;
	}

  Subscription::~Subscription() {
    activeTopics[topic].subscriptions.erase(id);
  }

  std::atomic<uint32_t> Subscription::nextId(0);

	void publishf(const std::string &topic, const char *fmt, ...) {
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, RTU_ARRAYSIZE(buf), fmt, args);
		buf[RTU_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		publish(topic, buf);
	}

  void publishp(const std::string &topic, void *data) {
    activeTopics[topic].publish(data);
  }

  void publish(const std::string& topic) {
    activeTopics[topic].publish(nullptr);
  }

}
