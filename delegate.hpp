/**
 * A Delegate in this case is a container for a stateful call to a function or member method. A delegate can be stored
 * or passed around and called from anywhere without losing context or causing other headaches (most of the time).
 *
 * "Why not just use std::function?" No real reason. This is simple and fast and I'm used to it.
 */
#pragma once

namespace rtu {
   /*
    * A note on type formattings: it's pretty much the same as for std::function (template arguments read like a
    * funciton signature: <return_type(param_type_0, param_type_1, ... , param_type_n)>
    * For example, for a function that returns float and takes an int and a char, a delegate type would be:
    * Delegate<float(int, char)>
    */

   /*
    * The following macro is all you need to make delegates.  Usage:
    * RTU_MTHD_DLGT(method, obj) makes a delegate to a member method, taking the method and class instance (object),
    * like so:
    * Given the code:
    *   int MyClass { public: int myMethod(const char a) { //do something } };
    *   MyClass myClassInstance;
    * A delegate for a call to myClassInstance.myMethod would be declared as:
    *   Delegate<int(const char)> myDelegate = RTU_MTHD_DLGT(&MyClass::myMethod, &myClassInstance)
    * and later to invoke the delegate, just call myDelegate()
    *
    * RTU_FUNC_DLGT does the same but for a normal function without the object reference:
    *   void function(const char* str) { ... }
    *   Delegate<void(const char*)> delegate = RTU_FUNC_DLGT(function);
    *   delegate("a string");
    */
# ifndef RTU_MTHD_DLGT // deletage to member method (template keyword fixes problems with methods of template classes)
#   define RTU_MTHD_DLGT(func, instRef) (rtu::NewDelegate(func).template Create<func>(instRef))
# endif
# ifndef RTU_FUNC_DLGT // delegate to function
#   define RTU_FUNC_DLGT(func) (rtu::NewDelegate_NoClass(func).template CreateForFunction<func>())
# endif

  // And here are come all the guts...
  template<typename returnType, typename... params>
  class Delegate; // this advance declaration allows for the templating ahead.
  // main Delegate class definition
  template<typename returnType, typename... params>
  class Delegate<returnType(params...)> {
      typedef returnType (*PtrToFunc)(void* callee, params...);
    public:
      Delegate() {}
      Delegate(void* callee, PtrToFunc function) : calleePtr(callee) , callbackFuncPtr(function) {}
      returnType operator()(params... args) const {
        return (*callbackFuncPtr)(calleePtr, args...);
      }
      bool operator==(const Delegate& rhs) const {
        return (calleePtr == rhs.calleePtr) && (callbackFuncPtr == rhs.callbackFuncPtr);
      }
    private:
      void* calleePtr;
      PtrToFunc callbackFuncPtr;
  };

  // Delegate spawner makes delegates, handles all the casting required for delegate operation.
  template<typename className, typename returnType, typename... params>
  struct DelegateSpawner {
    template<returnType (className::*func)(params...)>
    static returnType MethodCaller(void* o, params... xs){
      return (static_cast<className*>(o)->*func)(xs...);
    }
    template <returnType (*classFuncPtr)(params...)>
    static returnType FunctionCaller(void*, params... xs){
      return (classFuncPtr)(xs...);
    }
    template<returnType (className::*func)(params...)>
    inline static Delegate<returnType(params...)> Create(className* o){
      return Delegate<returnType(params...)>(o, &DelegateSpawner::MethodCaller<func>);
    }
    template<returnType (*classFuncPtr)(params...)>
    inline static Delegate<returnType(params...)> CreateForFunction(){
      return Delegate<returnType(params...)>(0L, &DelegateSpawner::FunctionCaller<classFuncPtr>);
    }
  };

  // helper function that returns delegate spawner of member method delegates
  template<typename className, typename returnType, typename... params>
  DelegateSpawner<className, returnType, params... > NewDelegate(returnType (className::*)(params...)){
    return DelegateSpawner<className, returnType, params...>();
  }
  class noType{}; // noType class used in the function below
  // helper function that returns delegate spawner of function delegates
  template<typename returnType, typename... params>
  DelegateSpawner<noType, returnType, params... > NewDelegate_NoClass(returnType (*TFnctPtr)(params...)){
    return DelegateSpawner<noType, returnType, params...>();
  }
}
