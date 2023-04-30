###############################################################################
# Protobuf core library
###############################################################################
project(protobuf)

if (WIN32)
  set(ATOMIC_OPS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/stubs/atomicops_internals_x86_msvc.cc")
elseif (UNIX)
  set(ATOMIC_OPS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/stubs/atomicops_internals_x86_gcc.cc")
endif ()

set(PROTOBUF_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/descriptor.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/descriptor.pb.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/descriptor_database.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/dynamic_message.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/extension_set.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/extension_set_heavy.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/generated_message_reflection.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/generated_message_util.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/message.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/message_lite.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/reflection_ops.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/repeated_field.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/service.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/text_format.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/unknown_field_set.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/wire_format.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/wire_format_lite.cc"

  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/code_generator.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/command_line_interface.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/importer.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/parser.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/plugin.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/plugin.pb.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/subprocess.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/zip_writer.cc"
    
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/cpp/cpp_enum.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/cpp/cpp_enum_field.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/cpp/cpp_extension.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/cpp/cpp_field.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/cpp/cpp_file.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/cpp/cpp_generator.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/cpp/cpp_helpers.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/cpp/cpp_message.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/cpp/cpp_message_field.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/cpp/cpp_primitive_field.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/cpp/cpp_service.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/cpp/cpp_string_field.cc"
    
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_context.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_doc_comment.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_enum.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_enum_field.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_extension.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_field.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_file.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_generator.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_generator_factory.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_helpers.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_lazy_message_field.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_message.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_message_field.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_name_resolver.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_primitive_field.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_service.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_shared_code_generator.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/java/java_string_field.cc"
  
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/python/python_generator.cc"

  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/io/coded_stream.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/io/gzip_stream.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/io/printer.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/io/strtod.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/io/tokenizer.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/io/zero_copy_stream.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/io/zero_copy_stream_impl.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/io/zero_copy_stream_impl_lite.cc"
    
  ${ATOMIC_OPS}
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/stubs/common.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/stubs/once.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/stubs/stringprintf.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/stubs/structurally_valid.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/stubs/strutil.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/stubs/substitute.cc")

add_library(${PROJECT_NAME} ${PROTOBUF_SOURCES})
add_library(protobuf::protobuf ALIAS ${PROJECT_NAME})

if (UNIX)
  target_compile_definitions(${PROJECT_NAME} PUBLIC
    HAVE_PTHREAD=1)
endif ()

target_include_directories(${PROJECT_NAME}
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src")
    
###############################################################################
# Protobuf compiler
###############################################################################
project(protoc)

set(PROTOC_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/compiler/main.cc")

add_executable(${PROJECT_NAME} ${PROTOC_SOURCES})
add_executable(protobuf::protoc ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE
  protobuf::protobuf)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src")
