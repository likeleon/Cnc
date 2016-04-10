#pragma once

namespace cnc {

class CNC_API IFolder {
public:
  virtual ~IFolder() {}
  virtual StreamPtr GetContent(const std::string& filename) const = 0;
  virtual bool Exists(const std::string& filename) const = 0;
  virtual std::vector<uint32_t> ClassicHashes() const = 0;
  virtual std::vector<uint32_t> CrcHashes() const = 0;
  virtual int32_t priority() const = 0;
  virtual const std::string& name() const = 0;
};

using IFolderPtr = std::shared_ptr<IFolder>;

}
