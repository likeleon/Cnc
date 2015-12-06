#pragma once

namespace cnc {

class ThreadAffine {
public:
  virtual ~ThreadAffine() {}

protected:
  ThreadAffine();

  void VerifyThreadAffinity() const;

private:
  std::thread::id managed_thread_id_;
};

}