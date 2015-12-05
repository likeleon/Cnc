#pragma once

namespace cnc {

class IFolder {
public:
  virtual ~IFolder() {}
  virtual std::vector<uint32_t> ClassicHashes() const = 0;
  virtual std::vector<uint32_t> CrcHashes() const = 0;
};

using IFolderPtr = std::unique_ptr<IFolder>;

class GlobalFileSystem {
public:
  static void Mount(const std::string& name, const std::string& annotation = "");
  static void UnmountAll();

private:
  static IFolderPtr OpenPackage(const std::string& filename,
                                const std::string& annotation,
                                int32_t order);
  static void MountInner(IFolderPtr folder);

  static std::vector<IFolderPtr> mounted_folders_;
  static std::unordered_map<uint32_t, std::vector<const IFolder*>> classic_hash_index_;
  static std::unordered_map<uint32_t, std::vector<const IFolder*>> crc_hash_index_;
  static int32_t order_;
};

}
