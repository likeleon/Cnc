#pragma once

#include "cnc/package_entry.h"

namespace cnc {

class Manifest;

class CNC_API IFolder {
public:
  virtual ~IFolder() {}
  virtual std::vector<char> GetContent(const std::string& filename) const = 0;
  virtual bool Exists(const std::string& filename) const = 0;
  virtual std::vector<uint32_t> ClassicHashes() const = 0;
  virtual std::vector<uint32_t> CrcHashes() const = 0;
  virtual int32_t priority() const = 0;
  virtual const std::string& name() const = 0;
};

using IFolderPtr = std::shared_ptr<IFolder>;

class CNC_API FileSystem {
public:
  void Mount(const std::string& name, const std::string& annotation = "");
  void UnmountAll();
  void LoadFromManifest(const Manifest& manifest);

  std::vector<char> Open(const std::string& filename);
  bool TryOpen(const std::string& filename, std::vector<char>& s);

private:
  using HashIndex = std::unordered_map<uint32_t, std::vector<const IFolder*>>;

  IFolderPtr OpenPackage(const std::string& filename, const std::string& annotation, int32_t order);
  void MountInner(IFolderPtr folder);
  bool GetFromCache(PackageHashType type, const std::string& filename, std::vector<char>& s);

  std::vector<IFolderPtr> mounted_folders_;
  HashIndex classic_hash_index_;
  HashIndex crc_hash_index_;
  int32_t order_ = 0;
};

}
