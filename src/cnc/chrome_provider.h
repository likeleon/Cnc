#pragma once

#include "cnc/mapped_image.h"

namespace cnc {

class ChromeProvider {
public:
  static void Initialize(const std::vector<std::string>& chrome_files);
  static void Deinitialize();

  ChromeProvider() = delete;

private:
  struct Collection {
    std::string src;
    std::map<std::string, MappedImage> regions;
  };

  static void LoadCollection(const std::string& name, const MiniYaml& yaml);

  static std::map<std::string, Collection> collections_;
};

}