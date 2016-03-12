#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/install_utils.h"
#include "cnc/file.h"
#include "cnc/directory.h"
#include "cnc/path.h"
#include "cnc/error.h"
#include "cnc/stream_utils.h"
#include <ZipFile.h>

namespace cnc {
namespace mods {
namespace common {

static void ExtractZipArchive(ZipArchive::Ptr z, const std::string& dest_path, std::vector<std::string>& extracted, Action<std::string> on_progress) {
  for (size_t i = 0; i < z->GetEntriesCount(); ++i) {
    auto entry = z->GetEntry(i);
    if (entry->IsDirectory()) {
      continue;
    }

    on_progress(entry->GetFullName());

    auto dir = Path::Combine({ dest_path, Path::GetDirectoryName(entry->GetFullName()) });
    Directory::CreateDir(dir);
    auto path = Path::Combine({ dest_path, entry->GetFullName() });
    extracted.emplace_back(path);

    std::ofstream f(path, std::ios::binary | std::ios::trunc);
    if (!f.is_open()) {
      throw Error(MSG("Cannot create destination file: " + path));
    }

    auto* stream = entry->GetDecompressionStream();
    if (stream == nullptr) {
      throw Error(MSG("Failed to get decompressed stream: " + entry->GetFullName()));
    }

    StreamUtils::Copy(*stream, f);
    
    f.flush();
    f.close();
  }
}

bool InstallUtils::ExtractZip(const std::string& zip_file, const std::string& dest, Action<std::string> on_progress, Action<std::string> on_error) {
  if (!File::Exists(zip_file)) {
    on_error("Invalid path: " + zip_file);
    return false;
  }

  std::vector<std::string> extracted;
  auto archive = ZipFile::Open(zip_file);
  ExtractZipArchive(archive, dest, extracted, [=](const auto& s) { on_progress("Extracting " + s); });
  return true;
}

}
}
}
