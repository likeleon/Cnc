#include "cnc/stdafx.h"
#include "cnc/thread_affine.h"
#include "cnc/debug.h"

namespace cnc {

ThreadAffine::ThreadAffine()
  : managed_thread_id_(std::this_thread::get_id()) {
}

void ThreadAffine::VerifyThreadAffinity() const {
  Debug::CheckAssertion(
    managed_thread_id_ == std::this_thread::get_id(),
    "Cross-thread operation not valid: This method must be called from the same thread that created this object.");
}

}