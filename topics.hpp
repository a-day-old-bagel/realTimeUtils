#pragma once

#include <string>
#include <atomic>
#include <memory>
#include <functional>
#include "delegate.hpp"

// Get a std::unique_ptr to a closure-subscription without all the syntax
#define RTU_MAKE_SUB_UPTR(tpc, fnc, inst) std::make_unique<rtu::topics::Subscription>(tpc, RTU_MTHD_DLGT(&fnc, inst));

// Same as above but for the simple subscriptions to normal functions
#define RTU_MAKE_SSUB_UPTR(tpc, fnc) std::make_unique<rtu::topics::Subscription>(tpc, RTU_FUNC_DLGT(&fnc));

// Create a static subscription inside a function so that no further state creation is needed.
// This can function as a "one-liner" topic subscription. Be careful, as all the caveats of a static variable apply.
#define RTU_STATIC_SUB(name, tpc, fnc, inst) static std::unique_ptr<rtu::topics::Subscription> name; \
if ( ! name) { name = RTU_MAKE_SUB_UPTR(tpc, fnc, inst); }

// Create a static simple subscription inside a function so that no further state creation is needed.
// This can function as a "one-liner" topic subscription. Be careful, as all the caveats of a static variable apply.
#define RTU_STATIC_SSUB(name, tpc, fnc) static std::unique_ptr<rtu::topics::Subscription> name; \
if ( ! name) { name = RTU_MAKE_SSUB_UPTR(tpc, fnc); }

#if defined(__clang__) || defined(__GNUC__)
#define RTU_FMTARGS(FMT) __attribute__((format(printf, FMT, FMT+1))) // Apply printf-style warnings to user functions.
#define RTU_FMTLIST(FMT) __attribute__((format(printf, FMT, 0)))
#else
#define RTU_FMTARGS(FMT)
#define RTU_FMTLIST(FMT)
#endif

#define RTU_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR)/sizeof(*_ARR))) // Size of static C-style array. Don't use on pointers!

namespace rtu::topics {
	/**
	 * Topics, publishers, and subscriptions should really only be used as a last resort in most applications since they
	 * obscure the control flow of your program and can easily be misused out of laziness. Only choose to implement
	 * your behavior this way if there is no obviously clean way to communicate between two parts of your code. In other
	 * words, generally use this if your low coupling philosophy would be seriously offended otherwise.
	 * Some will argue that needing a technique like this is a symptom of bad design, and they may be right.
	 * But perfect design never makes it to production, and they may be wrong besides. Maybe a completely decoupled
	 * event-based/message-passing design approaches a sort of perfection in its simplicity and extensibility.
	 * So maybe disregard this whole message.
	 */

	void publishf(const std::string &topic, const char *fmt, ...) RTU_FMTARGS(2);

  void publishp(const std::string &topic, void *data);

  void publish(const std::string& topic);

  template <typename DataType>
  void publish(const std::string& topic, const DataType&& data) {
	  publishp(topic, (void *) &data);
  }

  template <typename DataType>
  void publish(const std::string& topic, const DataType& data) {
	  publishp(topic, (void *) &data);
  }

	typedef std::function<void(void*)> FAction;
	typedef std::function<void()> FSimpleAction;
  typedef Delegate<void(void*)> DAction;
  typedef Delegate<void()> DSimpleAction;

  class Subscription {
    public:
      Subscription(const std::string &topic, const DAction &dAction);
      Subscription(const std::string &topic, const DSimpleAction &dSimpleAction);
		  Subscription(const std::string &topic, const FAction &fAction);
		  Subscription(const std::string &topic, const FSimpleAction &fSimpleAction);
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

	typedef std::unique_ptr<Subscription> SubUptr;
}
