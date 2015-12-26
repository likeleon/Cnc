#pragma once

#include "package_entry.h"

namespace cnc {

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
  static void Mount(const std::string& name, const std::string& annotation = "");
  static void UnmountAll();

  static std::vector<char> Open(const std::string& filename);
  static bool TryOpen(const std::string& filename, std::vector<char>& s);

private:
  using HashIndex = std::unordered_map<uint32_t, std::vector<const IFolder*>>;

  static IFolderPtr OpenPackage(const std::string& filename,
                                const std::string& annotation,
                                int32_t order);
  static void MountInner(IFolderPtr folder);
  static bool GetFromCache(PackageHashType type, const std::string& filename, std::vector<char>& s);

  static std::vector<IFolderPtr> mounted_folders_;
  static HashIndex classic_hash_index_;
  static HashIndex crc_hash_index_;
  static int32_t order_;
};

}
