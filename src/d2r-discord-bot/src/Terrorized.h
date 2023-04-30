#pragma once

namespace d2rc {

class Terrorized {
  Terrorized() {}
  ~Terrorized() {}

 public:
  static Terrorized* GetInstance() {
    static Terrorized instance;
    return &instance;
  }

  std::vector<std::string> GetCurrent() {
    std::unique_lock<std::mutex> lock(list_mtx_);
    return current_;
  }

  void SetCurrent(std::vector<std::string> current) {
    std::unique_lock<std::mutex> lock(list_mtx_);
    current_ = current;
  }

  std::vector<std::string> GetNext() {
    std::unique_lock<std::mutex> lock(list_mtx_);
    return next_;
  }

  void SetNext(std::vector<std::string> next) {
    std::unique_lock<std::mutex> lock(list_mtx_);
    next_ = next;
  }

 private:
  std::mutex list_mtx_;
  std::vector<std::string> current_;
  std::vector<std::string> next_;
};

}  // namespace d2rc
