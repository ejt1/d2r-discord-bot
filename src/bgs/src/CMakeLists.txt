find_package(OpenSSL REQUIRED)
find_package(spdlog CONFIG REQUIRED)

set(BGS_SRCS
  "${CMAKE_CURRENT_SOURCE_DIR}/Rpc/Channel.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Rpc/Packet.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Rpc/Request.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Rpc/Response.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Rpc/Router.cpp"
  
  "${CMAKE_CURRENT_SOURCE_DIR}/Transport/Connection.cpp")

add_library(bgs ${BGS_SRCS})
add_library(bgs::bgs ALIAS bgs)

# because range-based co_await is removed in the final coroutine standard
# we have to make boost use the <expermental/coroutine> include, also set
# the /await compile option which is required.
if (WIN32)
target_compile_definitions(bgs
  PUBLIC
    BOOST_ASIO_DISABLE_STD_COROUTINE=1)
target_compile_options(bgs PUBLIC /await)

# add /bigobj compile option to allow larger sections
# required when heavily using templating
target_compile_options(bgs PUBLIC /bigobj)
endif()

target_include_directories(bgs PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_precompile_headers(bgs PUBLIC pch.h)

add_dependencies(bgs bgs-sdk)

target_link_libraries(bgs
  PUBLIC
    bgs::bgs-sdk

    OpenSSL::SSL
    OpenSSL::Crypto
    spdlog::spdlog)

install(
  IMPORTED_RUNTIME_ARTIFACTS
    fmt::fmt
    spdlog::spdlog
  DESTINATION
    ${CMAKE_INSTALL_PREFIX})
