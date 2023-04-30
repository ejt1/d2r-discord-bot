###############################################################################
# Protobuf bgs generator plugin
###############################################################################
project(protoc-gen-bgs)

set(PROTOBUF_IMPORT_DIRS
  "${CMAKE_CURRENT_SOURCE_DIR}/../protobuf-2.6.1/src")

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS
  "${CMAKE_CURRENT_SOURCE_DIR}/bgs/low/pb/client/global_extensions/field_options.proto"
  "${CMAKE_CURRENT_SOURCE_DIR}/bgs/low/pb/client/global_extensions/message_options.proto"
  "${CMAKE_CURRENT_SOURCE_DIR}/bgs/low/pb/client/global_extensions/method_options.proto"
  "${CMAKE_CURRENT_SOURCE_DIR}/bgs/low/pb/client/global_extensions/range.proto"
  "${CMAKE_CURRENT_SOURCE_DIR}/bgs/low/pb/client/global_extensions/routing.proto"
  "${CMAKE_CURRENT_SOURCE_DIR}/bgs/low/pb/client/global_extensions/service_options.proto")

set(PROTOC_BGS_SOURCES
  ${PROTO_SRCS}

  "${CMAKE_CURRENT_SOURCE_DIR}/src/compiler/file.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/compiler/generator.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/compiler/service.cpp"

  "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_executable(${PROJECT_NAME} ${PROTOC_BGS_SOURCES})
add_executable(protobuf::protoc-gen-bgs ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE
  protobuf::protobuf)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_BINARY_DIR}")
    
###############################################################################
# Protobuf extensions interface library
###############################################################################
add_library(proto-extensions STATIC ${PROTO_SRCS})
add_library(protobuf::proto-extensions ALIAS proto-extensions)

target_link_libraries(proto-extensions PRIVATE protobuf::protobuf)

target_include_directories(proto-extensions PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
