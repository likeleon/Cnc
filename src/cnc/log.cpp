#include "cnc/stdafx.h"
#include "cnc/log.h"
#include "cnc/platform.h"
#include "cnc/fatal_exception.h"

namespace cnc {

std::unordered_map<std::string, Log::ChannelInfo> Log::channels_;

void Log::AddChannel(const std::string& channel_name, const std::string& base_filename) {
  if (channels_.find(channel_name) != channels_.end()) {
    return;
  }

  if (base_filename.empty()) {
    channels_.emplace(channel_name, ChannelInfo());
    return;
  }

  auto log_dir = Platform::ResolvePath({ Platform::GetSupportDir(), "Logs" });
  Platform::CreateDir(log_dir);

  for (int i = 0; ; ++i) {
    std::string filename = (i > 0) ? base_filename + std::to_string(i) : base_filename;
    filename = Platform::ResolvePath({ log_dir, filename });
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

void Log::Write(const std::string& channel_name, const std::string& value) {
  auto iter = channels_.find(channel_name);
  if (iter == channels_.end()) {
    throw FatalException("Tried logging to non-existant channel " + channel_name);
  }

  auto& channel = iter->second;
  if (channel.writer == nullptr) {
    return;
  }

  *channel.writer << value << std::endl;
}

}