#pragma once

#include <google/protobuf/compiler/code_generator.h>

#include <string>

namespace bgs {
namespace compiler {

using namespace google::protobuf;
using namespace google::protobuf::compiler;

// CodeGenerator implementation which generates a C++ source file and
// header.  If you create your own protocol compiler binary and you want
// it to support C++ output, you can do so by registering an instance of this
// CodeGenerator with the CommandLineInterface in your main() function.
class LIBPROTOC_EXPORT BgsGenerator : public CodeGenerator {
 public:
  BgsGenerator();
  ~BgsGenerator();

  // implements CodeGenerator ----------------------------------------
  bool Generate(const FileDescriptor* file, const string& parameter,
                GeneratorContext* generator_context, string* error) const;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(BgsGenerator);
};

}  // namespace compiler
}  // namespace bgs
