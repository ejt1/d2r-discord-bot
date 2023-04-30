#include "compiler/service.h"

#include "bgs/low/pb/client/global_extensions/method_options.pb.h"
#include "bgs/low/pb/client/global_extensions/service_options.pb.h"

#include <google/protobuf/compiler/cpp/cpp_helpers.h>
#include <google/protobuf/compiler/cpp/cpp_service.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/stubs/strutil.h>

namespace bgs {
namespace compiler {

ServiceGenerator::ServiceGenerator(const ServiceDescriptor* descriptor,
                                   const cpp::Options& options)
    : descriptor_(descriptor) {
  vars_["classname"] = descriptor_->name();
  vars_["full_name"] = descriptor_->full_name();
  if (options.dllexport_decl.empty()) {
    vars_["dllexport"] = "";
  } else {
    vars_["dllexport"] = options.dllexport_decl + " ";
  }

  auto& service_options = descriptor_->options();
  if (service_options.HasExtension(bgs::protocol::service_options)) {
    auto& ext = service_options.GetExtension(bgs::protocol::service_options);
    if (ext.has_descriptor_name()) {
      vars_["hash"] = ToUpper(ToHex(HashServiceName(ext.descriptor_name())));
    } else if (ext.has_descriptor_hash()) {
      vars_["hash"] = ToUpper(ToHex(ext.descriptor_hash()));
    }
  }
}

ServiceGenerator::~ServiceGenerator() {}

void ServiceGenerator::GenerateDeclarations(io::Printer* printer) {
  // Forward-declare the stub type.
  printer->Print(vars_, "class $classname$_Stub;\n\n");

  GenerateInterface(printer);
  GenerateStubDefinition(printer);
}

void ServiceGenerator::GenerateInterface(io::Printer* printer) {
  printer->Print(vars_,
                 "class $dllexport$$classname$ : public ::bgs::rpc::Service {\n"
                 " protected:\n"
                 "  // This class should be treated as an abstract interface.\n"
                 "  inline $classname$() {};\n"
                 " public:\n"
                 "  virtual ~$classname$();\n"
                 "  \n"
                 "  typedef std::integral_constant<uint32_t, 0x$hash$u> "
                 "DescriptorHash;\n");
  printer->Indent();

  printer->Print(
      vars_,
      "\n"
      "typedef $classname$_Stub Stub;\n"
      "\n"
      "static const ::google::protobuf::ServiceDescriptor* descriptor();\n"
      "\n");

  GenerateMethodSignatures(VIRTUAL, printer);

  printer->Print(
      "\n"
      "// implements Service ----------------------------------------------\n"
      "\n"
      "const ::google::protobuf::ServiceDescriptor* GetDescriptor();\n"
      "boost::asio::awaitable<::bgs::rpc::Response> "
      "CallMethod(std::shared_ptr<::bgs::rpc::Channel> "
      "channel,\n"
      "                                            ::bgs::rpc::Request& "
      "request) "
      "override;\n"
      "const ::google::protobuf::Message& GetRequestPrototype(\n"
      "  const ::google::protobuf::MethodDescriptor* method) const;\n"
      "const ::google::protobuf::Message& GetResponsePrototype(\n"
      "  const ::google::protobuf::MethodDescriptor* method) const;\n"
      "const uint32_t Hash() const override;\n");

  printer->Outdent();
  printer->Print(vars_,
                 "\n"
                 " private:\n"
                 "  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS($classname$);\n"
                 "};\n"
                 "\n");
}

void ServiceGenerator::GenerateStubDefinition(io::Printer* printer) {
  printer->Print(vars_,
                 "class $dllexport$$classname$_Stub : public $classname$ {\n"
                 " public:\n");

  printer->Indent();

  printer->Print(
      vars_,
      "$classname$_Stub(std::shared_ptr<::bgs::rpc::Channel> channel);\n"
      "~$classname$_Stub();\n"
      "\n"
      "inline std::shared_ptr<::bgs::rpc::Channel> channel() { return "
      "channel_; }\n"
      "\n"
      "// implements $classname$ ------------------------------------------\n"
      "\n");

  GenerateMethodSignatures(NON_VIRTUAL, printer);

  printer->Outdent();
  printer->Print(vars_,
                 " private:\n"
                 "  std::shared_ptr<::bgs::rpc::Channel> channel_;\n"
                 "  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS($classname$_Stub);\n"
                 "};\n"
                 "\n");
}

void ServiceGenerator::GenerateMethodSignatures(VirtualOrNon virtual_or_non,
                                                io::Printer* printer) {
  for (int i = 0; i < descriptor_->method_count(); i++) {
    const MethodDescriptor* method = descriptor_->method(i);
    map<string, string> sub_vars;
    sub_vars["name"] = method->name();
    sub_vars["method_id"] = SimpleItoa(
        method->options().GetExtension(bgs::protocol::method_options).id());
    sub_vars["input_type"] = cpp::ClassName(method->input_type(), true);
    sub_vars["output_type"] = cpp::ClassName(method->output_type(), true);
    sub_vars["virtual"] = virtual_or_non == VIRTUAL ? "virtual " : "";

    if (virtual_or_non == VIRTUAL) {
      printer->Print(
          sub_vars,
          "$virtual$boost::asio::awaitable<::bgs::rpc::Response> "
          "$name$(std::shared_ptr<::bgs::rpc::Channel> channel,\n"
          "                     const bgs::protocol::Header& header,\n"
          "                     const $input_type$& request);\n");
    } else {
      if (method->output_type()->name() != "NO_RESPONSE") {
        printer->Print(
            sub_vars,
            "boost::asio::awaitable<::bgs::rpc::Response> $name$(const "
            "$input_type$& request);\n");
      } else {
        printer->Print(sub_vars,
                       "boost::asio::awaitable<void> $name$(const "
                       "$input_type$& request);\n");
      }
    }
  }
}

// ===================================================================

void ServiceGenerator::GenerateDescriptorInitializer(io::Printer* printer,
                                                     int index) {
  map<string, string> vars;
  vars["classname"] = descriptor_->name();
  vars["index"] = SimpleItoa(index);

  printer->Print(vars, "$classname$_descriptor_ = file->service($index$);\n");
}

// ===================================================================

void ServiceGenerator::GenerateImplementation(io::Printer* printer) {
  printer->Print(vars_,
                 "$classname$::~$classname$() {}\n"
                 "\n"
                 "const ::google::protobuf::ServiceDescriptor* "
                 "$classname$::descriptor() {\n"
                 "  protobuf_AssignDescriptorsOnce();\n"
                 "  return $classname$_descriptor_;\n"
                 "}\n"
                 "\n"
                 "const ::google::protobuf::ServiceDescriptor* "
                 "$classname$::GetDescriptor() {\n"
                 "  protobuf_AssignDescriptorsOnce();\n"
                 "  return $classname$_descriptor_;\n"
                 "}\n"
                 "\n");

  // Generate methods of the interface.
  GenerateNotImplementedMethods(printer);
  GenerateCallMethod(printer);
  GenerateGetPrototype(REQUEST, printer);
  GenerateGetPrototype(RESPONSE, printer);

  printer->Print(
      vars_,
      "const uint32_t $classname$::Hash() const { return 0x$hash$; }\n\n");

  // Generate stub implementation.
  printer->Print(vars_,
                 "$classname$_Stub::$classname$_Stub(std::shared_ptr<::bgs::"
                 "rpc::Channel> channel)\n"
                 "  : channel_(channel) {}\n"
                 "$classname$_Stub::~$classname$_Stub() {}\n"
                 "\n");

  GenerateStubMethods(printer);
}

void ServiceGenerator::GenerateNotImplementedMethods(io::Printer* printer) {
  for (int i = 0; i < descriptor_->method_count(); i++) {
    const MethodDescriptor* method = descriptor_->method(i);
    map<string, string> sub_vars;
    sub_vars["classname"] = descriptor_->name();
    sub_vars["name"] = method->name();
    sub_vars["method_id"] = SimpleItoa(
        method->options().GetExtension(bgs::protocol::method_options).id());
    sub_vars["index"] = SimpleItoa(i);
    sub_vars["input_type"] = cpp::ClassName(method->input_type(), true);
    sub_vars["output_type"] = cpp::ClassName(method->output_type(), true);

    printer->Print(
        sub_vars,
        "boost::asio::awaitable<::bgs::rpc::Response> "
        "$classname$::$name$(std::shared_ptr<::bgs::rpc::Channel> channel,\n"
        "                     const bgs::protocol::Header& header,\n"
        "                     const $input_type$& request) {\n"
        "  co_return ::bgs::rpc::ErrorCode::NOT_IMPLEMENTED;\n"
        "}\n"
        "\n");
  }
}

void ServiceGenerator::GenerateCallMethod(io::Printer* printer) {
  printer->Print(
      vars_,
      "boost::asio::awaitable<::bgs::rpc::Response> "
      "$classname$::CallMethod(std::shared_ptr<::bgs::rpc::Channel> channel,\n"
      "                ::bgs::rpc::Request& request) {\n"
      "  auto& header = request.Header();\n"
      "  auto& payload = request.Payload();\n"
      "\n"
      "  switch (header.method_id()) {\n");

  for (int i = 0; i < descriptor_->method_count(); i++) {
    const MethodDescriptor* method = descriptor_->method(i);
    map<string, string> sub_vars;
    sub_vars["classname"] = descriptor_->name();
    sub_vars["name"] = method->name();
    sub_vars["method_id"] = SimpleItoa(
        method->options().GetExtension(bgs::protocol::method_options).id());
    sub_vars["index"] = SimpleItoa(i);
    sub_vars["input_type"] =
        compiler::cpp::ClassName(method->input_type(), true);
    sub_vars["output_type"] =
        compiler::cpp::ClassName(method->output_type(), true);

    // Note:  down_cast does not work here because it only works on pointers,
    //   not references.
    printer->Print(sub_vars,
                   "    case $method_id$: {\n"
                   "      auto&& req = request.Deserialize<$input_type$>();\n"
                   "      spdlog::debug(\"$classname$::$name$ request : {}\", "
                   "req.ShortDebugString());\n"
                   "\n"
                   "      AWAIT_RETURN $name$(channel, header, req);\n"
                   "    } break;\n");
  }

  printer->Print(vars_,
                 "    default:\n"
                 "      co_return ::bgs::rpc::ErrorCode::RPC_INVALID_METHOD;\n"
                 "      break;\n"
                 "  }\n"
                 "}\n"
                 "\n");
}

void ServiceGenerator::GenerateGetPrototype(RequestOrResponse which,
                                            io::Printer* printer) {
  if (which == REQUEST) {
    printer->Print(vars_,
                   "const ::google::protobuf::Message& "
                   "$classname$::GetRequestPrototype(\n");
  } else {
    printer->Print(vars_,
                   "const ::google::protobuf::Message& "
                   "$classname$::GetResponsePrototype(\n");
  }

  printer->Print(
      vars_,
      "    const ::google::protobuf::MethodDescriptor* method) const {\n"
      "  GOOGLE_DCHECK_EQ(method->service(), descriptor());\n"
      "  switch(method->index()) {\n");

  for (int i = 0; i < descriptor_->method_count(); i++) {
    const MethodDescriptor* method = descriptor_->method(i);
    const Descriptor* type =
        (which == REQUEST) ? method->input_type() : method->output_type();

    map<string, string> sub_vars;
    sub_vars["index"] = SimpleItoa(i);
    sub_vars["type"] = cpp::ClassName(type, true);

    printer->Print(sub_vars,
                   "    case $index$:\n"
                   "      return $type$::default_instance();\n");
  }

  printer->Print(
      vars_,
      "    default:\n"
      "      GOOGLE_LOG(FATAL) << \"Bad method index; this should never "
      "happen.\";\n"
      "      return *reinterpret_cast< ::google::protobuf::Message*>(NULL);\n"
      "  }\n"
      "}\n"
      "\n");
}

void ServiceGenerator::GenerateStubMethods(io::Printer* printer) {
  for (int i = 0; i < descriptor_->method_count(); i++) {
    const MethodDescriptor* method = descriptor_->method(i);
    map<string, string> sub_vars;
    sub_vars["classname"] = descriptor_->name();
    sub_vars["name"] = method->name();
    sub_vars["method_id"] = SimpleItoa(
        method->options().GetExtension(bgs::protocol::method_options).id());
    sub_vars["index"] = SimpleItoa(i);
    sub_vars["input_type"] = cpp::ClassName(method->input_type(), true);
    sub_vars["output_type"] = cpp::ClassName(method->output_type(), true);

    if (method->output_type()->name() != "NO_RESPONSE") {
      printer->Print(
          sub_vars,
          "boost::asio::awaitable<::bgs::rpc::Response> "
          "$classname$_Stub::$name$(const "
          "$input_type$& request) {\n"
          "  spdlog::debug(\"$classname$::$name$ call : {}\", "
          "request.ShortDebugString());\n"
          "\n"
          "  auto&& response = co_await "
          "channel_->CallMethod<$output_type$>(Hash(), $method_id$, request);\n"
          "\n"
          "  spdlog::debug(\"$classname$::$name$ call response : {}\", "
          "response.Deserialize<$output_type$>().ShortDebugString());\n"
          "\n"
          "  co_return ::bgs::rpc::Response(response);\n"
          "}\n");
    } else {
      printer->Print(
          sub_vars,
          "boost::asio::awaitable<void> $classname$_Stub::$name$(const "
          "$input_type$& request) {\n"
          "  spdlog::debug(\"$classname$::$name$ call without response : {}\", "
          "request.ShortDebugString());"
          "  co_await channel_->CallMethod(Hash(), \n"
          "                                $method_id$,\n"
          "                                request);\n"
          "}\n");
    }
  }
}

std::uint32_t ServiceGenerator::HashServiceName(const std::string& name) {
  std::uint32_t hash = 0x811C9DC5;
  for (std::size_t i = 0; i < name.length(); ++i) {
    hash ^= name[i];
    hash *= 0x1000193;
  }

  return hash;
}

}  // namespace compiler
}  // namespace bgs
