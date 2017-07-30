
#pragma once

namespace rtu {
  namespace keyInput {

    template<typename lastKeyCode>
    static bool anyPressed(const uint8_t *keyStates, lastKeyCode key) {
      return keyStates[key];
    }

    template<typename firstKeyCode, typename... keyCode>
    static bool anyPressed(const uint8_t *keyStates, firstKeyCode firstKey, keyCode... keys) {
      return keyStates[firstKey] || anyPressed(keyStates, keys...);
    }

#   define RTU_DO_ON_KEYS(action, keyStates, ...) if(rtu::keyInput::anyPressed(keyStates, __VA_ARGS__)) { action; }

  }
}
