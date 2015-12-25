#pragma once

namespace cnc {

class IBatchRenderer {
public:
  virtual ~IBatchRenderer() {}
  
  virtual void Flush() = 0;
};

}
