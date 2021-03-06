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
  void DownloadFileAsync(const std::string& url, const std::string& path);
  void DownloadDataAsync(const std::string& url);

  void CancelAsync();

  Action<DownloadProgressChanged> download_progress_changed_;
  Action<AsyncCompleted> download_file_completed_;
  Action<DownloadDataCompleted> download_data_completed_;

private:
  pplx::task<size_t> DownloadToStream(const std::string& url, streambuf<char> buffer);
  pplx::task<size_t> Repeat(streambuf<char> buffer,istream stream, size_t chunk_size, size_t read_length, size_t total_length);

  pplx::cancellation_token_source cancel_source_;
};

static void StoreError(AsyncCompleted& c, pplx::task<size_t> t) {
  try {
    t.get();
  } catch (const http_exception& e) {
    c.cancelled = e.error_code() == std::errc::operation_canceled;
    c.error = e.what();
  }
}

void WebClient::DownloadFileAsync(const std::string& url, const std::string& path) {
  file_stream<uint8_t>::open_ostream(conversions::to_string_t(path)).then([=](ostream stream) {
    DownloadToStream(url, stream.streambuf()).then([=](pplx::task<size_t> t) {
      stream.flush().then([=]() { return stream.close(); });
      // FIX: 호출 쓰레드 문맥에서 호출되어야 함
      if (download_file_completed_ != nullptr) {
        AsyncCompleted c;
        StoreError(c, t);
        download_file_completed_(c);
      }
    });
  });
}

pplx::task<size_t> WebClient::DownloadToStream(const std::string& url, streambuf<char> buffer) {
  http_client client(conversions::to_string_t(url));
  return client.request(methods::GET, cancel_source_.get_token()).then([=](http_response response) -> pplx::task<size_t> {
    auto total_length = static_cast<size_t>(response.headers().content_length());
    return Repeat(buffer, response.body(), 8192, 0, total_length);
  });
}

pplx::task<size_t> WebClient::Repeat(streambuf<char> buffer, istream stream, size_t chunk_size, size_t read_length, size_t total_length) {
  return concurrency::create_task([=] { 
    return stream.read(buffer, chunk_size).get();
  }).then([=](int32_t bytes_read) {
    if (bytes_read > 0) {
      if (download_progress_changed_ != nullptr) {
        DownloadProgressChanged c;
        c.bytes_received = read_length + bytes_read;
        c.total_bytes_to_receive = total_length;
        if (c.total_bytes_to_receive > 0) {
          c.percentage_ = static_cast<int32_t>(c.bytes_received * 100 / c.total_bytes_to_receive);
        }
        download_progress_changed_(c);
      }
      return Repeat(buffer, stream, chunk_size, read_length + bytes_read, total_length);
    }
    return concurrency::create_task([=] {
      return read_length;
    });
  });
}

void WebClient::DownloadDataAsync(const std::string& url) {
  container_buffer<std::string> buffer;
  DownloadToStream(url, buffer).then([=](pplx::task<size_t> t) {
    // FIX: 호출 쓰레드 문맥에서 호출되어야 함
    if (download_data_completed_ != nullptr) {
      DownloadDataCompleted c;
      StoreError(c, t);
      c.result = buffer.collection();
      download_data_completed_(c);
    }
  });
}

void WebClient::CancelAsync() {
  cancel_source_.cancel();
}

Download::Ptr Download::ToFile(const std::string& url,
                               const std::string& path,
                               Action<DownloadProgressChanged> on_progress,
                               Action<AsyncCompleted, bool> on_complete) {
  Download::Ptr d(new Download());
  d->wc_->download_progress_changed_ = on_progress;
  d->wc_->download_file_completed_ = [d, on_complete](auto a) { 
    if (on_complete != nullptr) {
      on_complete(a, d->cancelled_);
    }
  };
  d->wc_->DownloadFileAsync(url, path);
  return d;
}

Download::Ptr Download::ToData(const std::string& url, Action<DownloadProgressChanged> on_progress, Action<DownloadDataCompleted, bool> on_complete) {
  Download::Ptr d(new Download());
  d->wc_->download_progress_changed_ = on_progress;
  d->wc_->download_data_completed_ = [d, on_complete](auto a) { 
    if (on_complete != nullptr) {
      on_complete(a, d->cancelled_);
    }
  };
  d->wc_->DownloadDataAsync(url);
  return d;
}

Download::Download()
  : wc_(std::make_unique<WebClient>()) {
}

Download::~Download() = default;

void Download::Cancel() {
  wc_->CancelAsync();
  cancelled_ = true;
}

}