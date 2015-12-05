#pragma once

#include "cnc/global_file_system.h"

namespace cnc {

class Folder : public IFolder {
public:
  Folder(const std::string& path, int32_t priority);
  
  std::vector<uint32_t> ClassicHashes() const override;
  std::vector<uint32_t> CrcHashes() const override;

private:
  std::string path_;
  int32_t priority_;
};

}
