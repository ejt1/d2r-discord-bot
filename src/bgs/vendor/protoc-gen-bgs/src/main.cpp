#include "compiler/generator.h"

#include <google/protobuf/compiler/plugin.h>

int main(int argc, char* argv[]) {
  bgs::compiler::BgsGenerator generator;
  return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}
