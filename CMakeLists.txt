
cmake_minimum_required( VERSION 3.9.2 FATAL_ERROR )
cmake_policy( VERSION 3.9.2...3.17 )

set( CMAKE_DISABLE_IN_SOURCE_BUILD ON )
set( CMAKE_DISABLE_SOURCE_CHANGES ON )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

project( realtimeutils VERSION 0.01.01.01 LANGUAGES CXX C )

add_library( realtimeutils STATIC
  eventResponseMap.cpp eventResponseMap.hpp
  keyInput.hpp
  ouroboros.hpp
  stringHelpers.cpp stringHelpers.hpp
  topics.cpp topics.hpp
  )
target_link_libraries( realtimeutils
  )
target_include_directories( realtimeutils PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/readerwriterqueue
  )
