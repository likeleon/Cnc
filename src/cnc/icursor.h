#pragma once

namespace cnc {

class Renderer;

class ICursor {
public:
  virtual ~ICursor() {};

  virtual void Render(Renderer& renderer) = 0;
  virtual void SetCursor(const std::string& cursor) = 0;
  virtual void Tick() = 0;
};

}