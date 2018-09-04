#pragma once

#include <string>
#include <sstream>
#include <array>
#include "delegate.hpp"

namespace rtu {
  template <typename T, typename indexType, indexType numSlots>
  class Ouroboros {
    public:

      typedef rtu::Delegate<T&(T&, const indexType&)> Ctor;
      typedef rtu::Delegate<void(T&, const indexType&)> Dtor;
      static Ctor defaultCtor;
      static Dtor defaultDtor;
      explicit Ouroboros(const Ctor & ctor = defaultCtor, const Dtor & dtor = defaultDtor);
      const indexType getNumSlots() const;
      const indexType getHeadSlot() const;
      const indexType getTailSlot() const;
      const bool isEmpty() const;
      const bool isFull() const;
      const bool isValid() const;
      const bool isValid(indexType index) const;
      const bool firstIsFresherThanSecond(indexType first, indexType second) const;
      const bool setEmptyIndex(indexType index);
      T & capitate();
      void decaudate();
      void decaudate(indexType upToButNot);
      T & operator [] (indexType index);
      const T & operator [] (indexType index) const;
      const std::string toDebugString() const;

      static indexType getNext(indexType index);
      static indexType getPrev(indexType index);

    private:

      std::array<T, numSlots> raw;
      Ctor ctor;
      Dtor dtor;
      indexType nextHead = 0;
      indexType currentTail = 0;
      bool emptyFlag = true;
      bool fullFlag = false;
      bool errorFlag = false;

      static T& defaultCtorImpl(T&, const indexType&);
      static void defaultDtorImpl(T&, const indexType&);
      indexType advanceHead();
      indexType advanceTail();
      void haveBadTime();
  };

  template<typename T, typename indexType, indexType numSlots>
  T &Ouroboros<T, indexType, numSlots>::defaultCtorImpl(T &in, const indexType &) {
    return in;
  }

  template<typename T, typename indexType, indexType numSlots>
  void Ouroboros<T, indexType, numSlots>::defaultDtorImpl(T &, const indexType &) {}

  template<typename T, typename indexType, indexType numSlots>
  typename Ouroboros<T, indexType, numSlots>::Ctor Ouroboros<T, indexType, numSlots>::defaultCtor =
      RTU_FUNC_DLGT(Ouroboros::defaultCtorImpl);

  template<typename T, typename indexType, indexType numSlots>
  typename Ouroboros<T, indexType, numSlots>::Dtor Ouroboros<T, indexType, numSlots>::defaultDtor =
      RTU_FUNC_DLGT(Ouroboros::defaultDtorImpl);

  template<typename T, typename indexType, indexType numSlots>
  Ouroboros<T, indexType, numSlots>::Ouroboros(const Ctor &ctor, const Dtor &dtor) : ctor(ctor), dtor(dtor) {}

  template<typename T, typename indexType, indexType numSlots>
  const indexType Ouroboros<T, indexType, numSlots>::getNumSlots() const {
    return numSlots;
  }

  template<typename T, typename indexType, indexType numSlots>
  const indexType Ouroboros<T, indexType, numSlots>::getHeadSlot() const {
    return nextHead ? nextHead - 1 : numSlots - 1;
  }

  template<typename T, typename indexType, indexType numSlots>
  const indexType Ouroboros<T, indexType, numSlots>::getTailSlot() const {
    return currentTail;
  }

  template<typename T, typename indexType, indexType numSlots>
  const bool Ouroboros<T, indexType, numSlots>::isEmpty() const {
    return emptyFlag;
  }

  template<typename T, typename indexType, indexType numSlots>
  const bool Ouroboros<T, indexType, numSlots>::isFull() const {
    return fullFlag;
  }

  template<typename T, typename indexType, indexType numSlots>
  const bool Ouroboros<T, indexType, numSlots>::isValid() const {
    return !errorFlag;
  }

  template<typename T, typename indexType, indexType numSlots>
  const bool Ouroboros<T, indexType, numSlots>::isValid(indexType index) const {
    return ! errorFlag && ! isEmpty() && firstIsFresherThanSecond(index, currentTail);
  }

  template<typename T, typename indexType, indexType numSlots>
  const bool Ouroboros<T, indexType, numSlots>::firstIsFresherThanSecond(indexType first, indexType second) const {
    if (nextHead > second) {
      return nextHead > first && second <= first;
    } else {
      return nextHead > first || second <= first;
    }
  }

  template<typename T, typename indexType, indexType numSlots>
  const bool Ouroboros<T, indexType, numSlots>::setEmptyIndex(indexType index) {
    if (isEmpty()) {
      nextHead = index;
      currentTail = index;
      return true;
    } else {
      return false;
    }
  }

  template<typename T, typename indexType, indexType numSlots>
  T &Ouroboros<T, indexType, numSlots>::capitate() {
    ctor(raw[nextHead], nextHead);
    advanceHead();
    return raw[getHeadSlot()];
  }

  template<typename T, typename indexType, indexType numSlots>
  inline void Ouroboros<T, indexType, numSlots>::decaudate() {
    dtor(raw[currentTail], currentTail);
    advanceTail();
  }

  template<typename T, typename indexType, indexType numSlots>
  void Ouroboros<T, indexType, numSlots>::decaudate(indexType upToButNot) {
    for (; currentTail != upToButNot; decaudate());
  }

  template<typename T, typename indexType, indexType numSlots>
  T &Ouroboros<T, indexType, numSlots>::operator[](indexType index) {
    return raw[index % numSlots]; // Wraps around. No error case, but invalid slots may be returned.
  }

  template<typename T, typename indexType, indexType numSlots>
  const T &Ouroboros<T, indexType, numSlots>::operator[](indexType index) const {
    return raw[index % numSlots]; // Wraps around. No error case, but invalid slots may be returned.
  }

  template<typename T, typename indexType, indexType numSlots>
  const std::string Ouroboros<T, indexType, numSlots>::toDebugString() const {
    std::stringstream out;
    for (indexType i = 0; i < numSlots; ++i) {
      int numeral = isValid(i);
      if (nextHead == i) {
        numeral += 3;
      }
      if (currentTail == i) {
        numeral += 5;
      }
      out << numeral;
    }
    return out.str();
  }

  template<typename T, typename indexType, indexType numSlots>
  indexType Ouroboros<T, indexType, numSlots>::getNext(indexType index) {
    if (index < 0 || ++index >= numSlots) {
      return 0;
    } else {
      return index; // already incremented
    }
  }

  template<typename T, typename indexType, indexType numSlots>
  indexType Ouroboros<T, indexType, numSlots>::getPrev(indexType index) {
    if (index <= 0 || --index >= numSlots) {
      return numSlots - 1;
    } else {
      return index; // already decremented
    }
  }

  template<typename T, typename indexType, indexType numSlots>
  indexType Ouroboros<T, indexType, numSlots>::advanceHead() {
    if (nextHead == currentTail) {
      if (fullFlag) { haveBadTime(); }
      else { emptyFlag = false; }
    }
    nextHead = getNext(nextHead);
    if (nextHead == currentTail) { fullFlag = true; }
    return nextHead;
  }

  template<typename T, typename indexType, indexType numSlots>
  indexType Ouroboros<T, indexType, numSlots>::advanceTail() {
    if (nextHead == currentTail) {
      if (emptyFlag) { haveBadTime(); }
      else { fullFlag = false; }
    }
    currentTail = getNext(currentTail);
    if (nextHead == currentTail) { emptyFlag = true; }
    return currentTail;
  }

  template<typename T, typename indexType, indexType numSlots>
  void Ouroboros<T, indexType, numSlots>::haveBadTime() {
    // If you like exceptions, here is the place to throw one.
    errorFlag = true;
  }
}
