#pragma once

#include "Client.h"

#include <external/v1/diablo_clone.pb.h>

#include <chrono>

namespace d2rc {

struct DiabloCloneStatus {
  std::chrono::system_clock::time_point last_update;
  bool initialized = false;
  std::array<uint32_t, 8> progression;
};

class DiabloClone {
  DiabloClone() {}
  ~DiabloClone() {}

 public:
  static DiabloClone* GetInstance() {
    static DiabloClone instance;
    return &instance;
  }

  std::string GetStatus(const Region& region);
  void SetStatus(const Region& region, classic::protocol::external::v1::diablo_clone::GameType gametype,
                 uint32_t progress);

 private:
  std::mutex uniq_mtx_;
  std::array<DiabloCloneStatus, 3> status_;
};

}  // namespace d2rc
