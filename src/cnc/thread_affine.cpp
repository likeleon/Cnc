#include "cnc/stdafx.h"
#include "cnc/thread_affine.h"
#include "cnc/error.h"

namespace cnc {

ThreadAffine::ThreadAffine()
  : managed_thread_id_(std::this_thread::get_id()) {
}

void ThreadAffine::VerifyThreadAffinity() const {
  if (managed_thread_id_ != std::this_thread::get_id()) {
    throw Error(MSG("Cross-thread operation not valid: This method must be called from the same thread that created this object."));
  }
}

}