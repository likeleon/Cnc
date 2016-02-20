#pragma once

#include "cnc/action.h"

namespace cnc {

class WebClient;

struct DownloadProgressChanged {
  int64_t bytes_received = 0;
  int64_t total_bytes_to_receive = 0;
};

struct AsyncCompleted {
  bool cancelled = false;
  std::string error;
  void* user_state = nullptr;
};

class Download {
public:
  Download(const std::string& url, const std::string& path, Action<DownloadProgressChanged> on_progress, Action<AsyncCompleted, bool> on_complete);
  Download(const std::string& url, Action<DownloadProgressChanged> on_progress, Action<AsyncCompleted, bool> on_complete);
  ~Download();

  void Cancel();

private:
  Download(Action<DownloadProgressChanged> on_progress, Action<AsyncCompleted, bool> on_complete);

  std::unique_ptr<WebClient> wc_;
  bool cancelled_ = false;
};

}
