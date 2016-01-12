#pragma once

namespace cnc {

class ModData;

class CursorProvider {
public:
  CursorProvider(const ModData& mod_data);

private:
  const ModData& mod_data_;
};

}
