#pragma once

#include "cnc/package_entry.h"
#include "cnc/stream_ptr.h"
#include "cnc/ifolder.h"

namespace cnc {

class Manifest;

class CNC_API FileSystem {
public:
  static HMODULE ResolveLibrary(const std::string& filename);
  
  IFolderPtr OpenPackage(const std::string& filename, const std::string& annotation, int32_t order);
  
  void Mount(IFolderPtr mount);
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
