#pragma once

namespace cnc {

class ChromeMetrics {
public:
  static void Initialize(const std::vector<std::string>& yaml);
  
  ChromeMetrics() = delete;

private:
  static std::map<std::string, std::string> data_;
};

}
