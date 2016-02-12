#include "cnc/stdafx.h"
#include "cnc/file_system.h"
#include "cnc/platform.h"
#include "cnc/string_utils.h"
#include "cnc/file.h"
#include "cnc/path.h"
#include "cnc/folder.h"
#include "cnc/error.h"
#include "cnc/package_entry.h"
#include "cnc/manifest.h"
#include "cnc/game.h"
#include "cnc/mod_data.h"
#include "cnc/directory.h"
#include "cnc/mix_file.h"
#include "cnc/field_loader.h"

namespace cnc {

std::map<std::string, FileSystem::LibraryPtr> FileSystem::library_cache_;

HMODULE FileSystem::ResolveLibrary(const std::string& filename) {
  auto iter = library_cache_.find(filename);
  if (iter != library_cache_.end()) {
    return iter->second.get();
  }

  LibraryPtr library(LoadLibraryA(filename.c_str()));
  if (!library) {
    throw Error(MSG("Failed to load library: " + filename));
  }

  iter = library_cache_.emplace(filename, std::move(library)).first;
  return iter->second.get();
}

void FileSystem::Mount(const std::string& name, const std::string& annotation) {
  std::string final_name = name;
  bool optional = StringUtils::StartsWith(name, "~");
  if (optional) {
    final_name = name.substr(1);
  }

  final_name = Platform::ResolvePath(final_name);

  auto action = [this, name, annotation]() {
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

IFolderPtr FileSystem::OpenPackage(const std::string& filename, const std::string& annotation, int32_t order) {
  if (StringUtils::EndsWith(filename, ".mix")) {
    auto type = annotation.empty() ? PackageHashType::Classic : FieldLoader::GetEnumValue<PackageHashTypeTraits>(annotation);
    return std::make_unique<MixFile>(*this, filename, type, order);
  }
  return std::make_unique<Folder>(filename, order);
}

void FileSystem::MountInner(IFolderPtr folder_ptr) {
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

void FileSystem::UnmountAll() {
  mounted_folders_.clear();
  classic_hash_index_.clear();
  crc_hash_index_.clear();
}

void FileSystem::LoadFromManifest(const Manifest& manifest) {
  UnmountAll();
  for (const auto& dir : manifest.folders()) {
    Mount(dir);
  }
  for (const auto& pkg : manifest.packages()) {
    Mount(pkg.first, pkg.second);
  }
}

bool FileSystem::Exists(const std::string& name) {
  auto explicit_folder = StringUtils::Contains(name, ":") && !File::Exists(Path::GetDirectoryName(name));
  if (explicit_folder) {
    auto divide = StringUtils::Split(name, ':');
    auto foldername = divide.front();
    auto filename = divide.back();
    
    std::vector<IFolderPtr> folders;
    std::copy_if(
      mounted_folders_.begin(),
      mounted_folders_.end(),
      std::back_inserter(folders),
      [foldername](const auto& f) { return f->name() == foldername; });
    return std::any_of(folders.begin(), folders.end(),
                       [name](const auto& f) { return f->Exists(name); });
  } else {
    return std::any_of(mounted_folders_.begin(), mounted_folders_.end(),
                       [name](const auto& f) { return f->Exists(name); });
  }
}

std::vector<char> FileSystem::Open(const std::string& filename) {
  std::vector<char> s;
  if (!TryOpen(filename, s)) {
    throw Error(MSG("File not found: " + filename));
  }
  return s;
}

bool FileSystem::TryOpen(const std::string& name, std::vector<char>& s) {
  std::string filename = name;
  std::string foldername = "";

  bool explicit_folder = StringUtils::Contains(name, ":") && !File::Exists(Path::GetDirectoryName(name));
  if (explicit_folder) {
    auto divide = StringUtils::Split(name, ':');
    foldername = divide.front();
    filename = divide.back();
  }

  if (filename.find_first_of("/\\") != std::string::npos && !explicit_folder) {
    if (GetFromCache(PackageHashType::Classic, filename, s)) {
      return true;
    }
    //if (GetFromCache(PackageHashType::CRC32, filename, s)) {
    //  return true;
    //}
  }

  std::vector<IFolderPtr> folders;
  if (explicit_folder && !foldername.empty()) {
    std::copy_if(
      mounted_folders_.begin(), 
      mounted_folders_.end(), 
      std::back_inserter(folders),
      [foldername](const auto& f) { return f->name() == foldername; });
  } else {
    std::copy_if(
      mounted_folders_.begin(),
      mounted_folders_.end(),
      std::back_inserter(folders),
      [filename](const auto& f) { return f->Exists(filename); });
  }
  auto max_elem = std::max_element(
    folders.begin(),
    folders.end(),
    [](const auto& a, const auto& b) { return a->priority() < b->priority(); });
  if (max_elem == folders.end()) {
    return false;
  }

  IFolderPtr folder = *max_elem;
  s = folder->GetContent(filename);
  return true;
}

bool FileSystem::GetFromCache(PackageHashType type, const std::string& filename, std::vector<char>& s) {
  auto* index = (type == PackageHashType::CRC32) ? &crc_hash_index_ : &classic_hash_index_;
  auto iter = index->find(PackageEntry::HashFilename(filename, type));
  if (iter == index->end()) {
    return false;
  }
  std::vector<const IFolder*> folders;
  std::copy_if(
    iter->second.begin(),
    iter->second.end(),
    std::back_inserter(folders),
    [filename](const IFolder* f) { return f->Exists(filename); });
  auto min_elem = std::min_element(
    folders.begin(),
    folders.end(),
    [](const auto& a, const auto& b) { return a->priority() < b->priority(); });
  if (min_elem == folders.end()) {
    return false;
  }

  const IFolder* folder = *min_elem;
  s = folder->GetContent(filename);
  return true;
}

}