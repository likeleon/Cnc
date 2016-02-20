#include "cnc/stdafx.h"
#include "cnc/download.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include "cnc/game.h"

namespace cnc {

using namespace utility;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

class WebClient {
public:
  pplx::task<size_t> DownloadFileAsync(const std::string& url, const std::string& path);
  void DownloadDataAsync(const std::string& url);

  Action<DownloadProgressChanged> download_progress_changed_;
  Action<AsyncCompleted> download_file_completed_;
};

pplx::task<size_t> WebClient::DownloadFileAsync(const std::string& url, const std::string& path) {
  return file_stream<uint8_t>::open_ostream(conversions::to_string_t(path))
  .then([=](ostream stream) {
    http_client client(conversions::to_string_t(url));
    return client.request(methods::GET)
      .then([stream](http_response response) -> pplx::task<size_t> {
        return response.body().read_to_end(stream.streambuf());
      })
      .then([stream](pplx::task<size_t> ret_task) {
        return stream.flush().then([=]() {
          return stream.close();
        }).then([ret_task]() {
          return ret_task;
        });
      });
  });
}

void WebClient::DownloadDataAsync(const std::string& /*url*/) {
}

Download::Download(const std::string& url, const std::string& path, Action<DownloadProgressChanged> on_progress, Action<AsyncCompleted, bool> on_complete)
  : Download(on_progress, on_complete) {
  wc_->DownloadFileAsync(url, path);
}

Download::Download(const std::string& url, Action<DownloadProgressChanged> on_progress, Action<AsyncCompleted, bool> on_complete)
  : Download(on_progress, on_complete) {
  wc_->DownloadDataAsync(url);
}

Download::~Download() = default;

Download::Download(Action<DownloadProgressChanged> on_progress, Action<AsyncCompleted, bool> on_complete)
  : wc_(std::make_unique<WebClient>()) {
  wc_->download_progress_changed_ = on_progress;
  wc_->download_file_completed_ = [this, on_complete](auto a) { on_complete(a, cancelled_); };
}

}