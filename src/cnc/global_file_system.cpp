#include "cnc/stdafx.h"
#include "cnc/global_file_system.h"
#include "cnc/platform.h"
#include "cnc/string.h"
#include "cnc/folder.h"

namespace cnc {

std::vector<IFolderPtr> GlobalFileSystem::mounted_folders_;
std::unordered_map<uint32_t, std::vector<const IFolder*>> GlobalFileSystem::classic_hash_index_;
std::unordered_map<uint32_t, std::vector<const IFolder*>> GlobalFileSystem::crc_hash_index_;
int32_t GlobalFileSystem::order_ = 0;

void GlobalFileSystem::Mount(const std::string& name, const std::string& annotation) {
  std::string final_name = name;
  bool optional = String::StartsWith(name, "~");
  if (optional) {
    final_name = name.substr(1);
  }
  
  final_name = Platform::ResolvePath(final_name);

  auto action = [name, annotation]() {
    MountInner(OpenPackage(name, annotation, order_++));
  };

  if (optional) {
    try {
      action();
    } catch (...) {
    }
  } else {
    action();
  }
}

IFolderPtr GlobalFileSystem::OpenPackage(const std::string& filename, 
                                         const std::string& /*annotation*/, 
                                         int32_t order) {
  return std::make_unique<Folder>(filename, order);
}

void GlobalFileSystem::MountInner(IFolderPtr folder_ptr) {
  auto* folder = folder_ptr.get();
  mounted_folders_.emplace_back(std::move(folder_ptr));

  for (uint32_t hash : folder->ClassicHashes()) {
    auto& l = classic_hash_index_[hash];
    if (std::find(l.begin(), l.end(), folder) == l.end()) {
      l.emplace_back(folder);
    }
  }

  for (uint32_t hash : folder->CrcHashes()) {
    auto& l = crc_hash_index_[hash];
    if (std::find(l.begin(), l.end(), folder) == l.end()) {
      l.emplace_back(folder);
    }
  }
}

void GlobalFileSystem::UnmountAll() {
  for (auto& folder : mounted_folders_) {
    folder.release();
  }
  mounted_folders_.clear();
  classic_hash_index_.clear();
  crc_hash_index_.clear();
}

}