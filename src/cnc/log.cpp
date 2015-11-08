#include "cnc/stdafx.h"
#include "cnc/log.h"
#include "cnc/platform.h"

namespace cnc {

std::unordered_map<std::tr2::sys::path, Log::ChannelInfo> Log::channels_;

void Log::AddChannel(const std::string& channel_name, const std::string& base_filename) {
  if (channels_.find(channel_name) != channels_.end()) {
    return;
  }

  if (base_filename.empty()) {
    channels_.emplace(channel_name, ChannelInfo());
    return;
  }

  std::wstring log_dir = Platform::GetSupportDir() + L"Logs";
  std::tr2::sys::create_directory(log_dir);

  for (int i = 0; ; ++i) {
    std::string filename = (i > 0) ? base_filename + std::to_string(i) : base_filename;
    auto writer = std::make_unique<std::ofstream>(filename, std::ios_base::trunc);
    if (writer->fail()) {
      continue;
    }

    ChannelInfo channel_info;
    channel_info.filename = filename;
    channel_info.writer.reset(writer.release());
    channels_.emplace(channel_name, std::move(channel_info));
    return;
  }
}

void Log::Write(const std::string& /*channel*/, const std::string& /*value*/) {
}

}