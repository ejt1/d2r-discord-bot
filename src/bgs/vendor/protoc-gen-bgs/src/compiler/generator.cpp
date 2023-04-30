#include "compiler/generator.h"

#include "compiler/file.h"

#include <google/protobuf/compiler/cpp/cpp_helpers.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>

#include <memory>
#include <utility>
#include <vector>

namespace bgs {
namespace compiler {

BgsGenerator::BgsGenerator() {}
BgsGenerator::~BgsGenerator() {}

bool BgsGenerator::Generate(const FileDescriptor* file, const string& parameter,
                            GeneratorContext* generator_context,
                            string* error) const {
  vector<pair<string, string>> options;
  ParseGeneratorParameter(parameter, &options);

  cpp::Options file_options;

  for (int i = 0; i < options.size(); i++) {
    if (options[i].first == "dllexport_decl") {
      file_options.dllexport_decl = options[i].second;
    } else if (options[i].first == "safe_boundary_check") {
      file_options.safe_boundary_check = true;
    } else {
      *error = "Unknown generator option: " + options[i].first;
      return false;
    }
  }

  //-----------------------------------------------------------------

  string basename = cpp::StripProto(file->name());
  basename.append(".pb");

  FileGenerator file_generator(file, file_options);

  // Generate header.
  {
    scoped_ptr<io::ZeroCopyOutputStream> output(
        generator_context->Open(basename + ".h"));
    io::Printer printer(output.get(), '$');
    file_generator.GenerateHeader(&printer);
  }

  // Generate cc file.
  {
    scoped_ptr<io::ZeroCopyOutputStream> output(
        generator_context->Open(basename + ".cc"));
    io::Printer printer(output.get(), '$');
    file_generator.GenerateSource(&printer);
  }

  return true;
}

}  // namespace compiler
}  // namespace bgs
