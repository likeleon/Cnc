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
};

struct DownloadDataCompleted : public AsyncCompleted {
  std::vector<char> result;
};


class Download : public std::enable_shared_from_this<Download> {
public:
  using Ptr = std::shared_ptr<Download>;

  static Ptr ToFile(const std::string& url,
                    const std::string& path,
                    Action<DownloadProgressChanged> on_progress,
                    Action<AsyncCompleted, bool> on_complete);

  static Ptr ToData(const std::string& url,
                    Action<DownloadProgressChanged> on_progress,
                    Action<DownloadDataCompleted, bool> on_complete);

  ~Download();

  void Cancel();

private:
  Download();

  std::unique_ptr<WebClient> wc_;
  bool cancelled_ = false;
};

}
