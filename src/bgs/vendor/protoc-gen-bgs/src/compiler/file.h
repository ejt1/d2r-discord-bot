#pragma once

#include "compiler/service.h"

#include <google/protobuf/compiler/cpp/cpp_field.h>
#include <google/protobuf/compiler/cpp/cpp_options.h>
#include <google/protobuf/stubs/common.h>

#include <memory>
#include <string>
#include <vector>

namespace google {
namespace protobuf {
class FileDescriptor;  // descriptor.h
namespace compiler {
namespace cpp {
class EnumGenerator;       // enum.h
class MessageGenerator;    // message.h
class ExtensionGenerator;  // extension.h
}  // namespace cpp
}  // namespace compiler
namespace io {
class Printer;  // printer.h
}  // namespace io
}  // namespace protobuf
}  // namespace google

namespace bgs {
namespace compiler {

using namespace google::protobuf;
using namespace google::protobuf::compiler;

class ServiceGenerator;  // bnet_service.h

class FileGenerator {
 public:
  // See generator.cc for the meaning of dllexport_decl.
  explicit FileGenerator(const FileDescriptor* file,
                         const cpp::Options& options);
  ~FileGenerator();

  void GenerateHeader(io::Printer* printer);
  void GenerateSource(io::Printer* printer);

 private:
  // Generate the BuildDescriptors() procedure, which builds all descriptors
  // for types defined in the file.
  void GenerateBuildDescriptors(io::Printer* printer);

  void GenerateNamespaceOpeners(io::Printer* printer);
  void GenerateNamespaceClosers(io::Printer* printer);

  const FileDescriptor* file_;

  scoped_array<scoped_ptr<cpp::MessageGenerator> > message_generators_;
  scoped_array<scoped_ptr<cpp::EnumGenerator> > enum_generators_;
  scoped_array<scoped_ptr<ServiceGenerator> > service_generators_;
  scoped_array<scoped_ptr<cpp::ExtensionGenerator> > extension_generators_;

  // E.g. if the package is foo.bar, package_parts_ is {"foo", "bar"}.
  vector<string> package_parts_;
  const cpp::Options options_;

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(FileGenerator);
};

}  // namespace compiler
}  // namespace bgs
