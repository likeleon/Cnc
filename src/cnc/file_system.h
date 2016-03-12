#pragma once

#include "cnc/package_entry.h"
#include "cnc/stream_ptr.h"

namespace cnc {

class Manifest;

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

class CNC_API FileSystem {
public:
  static HMODULE ResolveLibrary(const std::string& filename);
  
  IFolderPtr OpenPackage(const std::string& filename, const std::string& annotation, int32_t order);
  
  void Mount(const std::string& name, const std::string& annotation = "");
  void UnmountAll();
  void LoadFromManifest(const Manifest& manifest);

  bool Exists(const std::string& filename);

  StreamPtr Open(const std::string& filename);
  bool TryOpen(const std::string& filename, StreamPtr& s);

private:
  using HashIndex = std::unordered_map<uint32_t, std::vector<const IFolder*>>;

  struct LibraryDeleter {
    typedef HMODULE pointer;
    void operator() (HMODULE h) {
      FreeLibrary(h);
    }
  };
  using LibraryPtr = std::unique_ptr<HMODULE, LibraryDeleter>;

  void MountInner(IFolderPtr folder);
  bool GetFromCache(PackageHashType type, const std::string& filename, StreamPtr& s);

  static std::map<std::string, LibraryPtr> library_cache_;
  std::vector<IFolderPtr> mounted_folders_;
  HashIndex classic_hash_index_;
  HashIndex crc_hash_index_;
  int32_t order_ = 0;
};

}
