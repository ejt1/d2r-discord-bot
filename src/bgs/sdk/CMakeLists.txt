project(bgs-sdk)

# add protobuf import directory for extensions
set(PROTOBUF_IMPORT_DIRS
  "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/protobuf-2.6.1/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/protoc-gen-bgs")

PROTOBUF_GENERATE_BGS(PROTO_SRCS PROTO_HDRS
    bgs/low/pb/client/api/client/v2/attribute_types.proto
    bgs/low/pb/client/api/client/v2/game_utilities_service.proto
    
    bgs/low/pb/client/account_service.proto
    bgs/low/pb/client/account_types.proto
    bgs/low/pb/client/attribute_types.proto
    bgs/low/pb/client/authentication_service.proto
    bgs/low/pb/client/connection_service.proto
    bgs/low/pb/client/content_handle_types.proto
    bgs/low/pb/client/entity_types.proto
    bgs/low/pb/client/resource_service.proto
    bgs/low/pb/client/role_types.proto
    bgs/low/pb/client/rpc_config.proto
    bgs/low/pb/client/rpc_types.proto
    bgs/low/pb/client/semantic_version.proto
    bgs/low/pb/client/session_service.proto
    bgs/low/pb/client/session_types.proto
    
    external/v1/d2r_connection.proto
    external/v1/diablo_clone.proto
    external/v1/game_version.proto
    external/v1/generic_resource.proto
    external/v1/leaderboard.proto
    external/v1/limited_time_event.proto
    external/v1/limited_time_event_common.proto
    external/v1/terror_zones.proto)

add_library(${PROJECT_NAME} ${PROTO_SRCS} ${PROTO_HDRS})
add_library(bgs::bgs-sdk ALIAS ${PROJECT_NAME})

add_dependencies(${PROJECT_NAME} protobuf::protoc-gen-bgs)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    bgs::bgs

    protobuf::protobuf
    protobuf::proto-extensions)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR})
