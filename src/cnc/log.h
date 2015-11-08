#pragma once

namespace cnc {

class Log {
public:
  Log() = delete;

  static void AddChannel(const std::string& channel_name, const std::string& base_filename);
  static void Write(const std::string& channel, const std::string& value);

private:
  struct ChannelInfo {
    std::tr2::sys::path filename;
    std::unique_ptr<std::ostream> writer;
  };

  static std::unordered_map<std::tr2::sys::path, ChannelInfo> channels_;
};

}
