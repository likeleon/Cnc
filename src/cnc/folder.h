#pragma once

#include "cnc/file_system.h"

namespace cnc {

class Folder : public IFolder {
public:
  Folder(const std::string& path, int32_t priority);
  
  std::string GetContent(const std::string& filename) const override;
  bool Exists(const std::string& filename) const override;
  std::vector<uint32_t> ClassicHashes() const override;
  std::vector<uint32_t> CrcHashes() const override;
  int32_t priority() const override;
  const std::string& name() const override;

private:
  std::string path_;
  int32_t priority_;
};

}
