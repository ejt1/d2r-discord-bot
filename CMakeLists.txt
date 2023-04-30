cmake_minimum_required(VERSION 3.22)

if (NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 23)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(VCPKG_HOST_TRIPLET "x64-windows" CACHE STRING "")
set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")

project(d2r-discord-bot)

if (WIN32)
  add_definitions(-D_WIN32_WINNT=0x0601)
endif()

add_subdirectory(src)
