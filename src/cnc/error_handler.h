#pragma once

namespace cnc {

class ErrorHandler {
public:
  static void CheckGlVersion();
  static void CheckGlError();
  static void WriteGraphicsLog(const std::string& message);
};

}
