cmake_minimum_required (VERSION 3.8)

project(clinq VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)

include_directories(ThirdParty/catch)

if(WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()