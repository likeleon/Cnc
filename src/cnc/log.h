#pragma once

namespace cnc {

struct ChannelInfo {
  std::string filename;
  std::ostream writer;
};

class Log {
public:
  Log() = delete;

  static void AddChannel(const std::string& channel_name, const std::string& base_filename);
  static void Write(const std::string& channel, const std::string& value);
};

}
