#include "cnc/stdafx.h"
#include "cnc/content_installer.h"
#include "cnc/field_loader.h"

namespace cnc {

const std::vector<std::string> ContentInstaller::CtorArgNames = {
  "yaml"
};

std::vector<FieldLoadInfo> ContentInstaller::GetLoadInfo() const {
  return{
    { "TestFiles", StringVectorFieldInfo(&ContentInstaller::test_files) },
    { "PackageMirrorList", TypeFieldInfo(&ContentInstaller::package_mirror_list) },
    { "MusicPackageMirrorList", TypeFieldInfo(&ContentInstaller::music_package_mirror_list) },
    { "ShippedSoundtracks", TypeFieldInfo(&ContentInstaller::shipped_soundtracks) },
  };
};

std::vector<std::type_index> ContentInstaller::Interfaces() const {
  return{ typeid(GlobalModData) };
}

std::vector<std::type_index> ContentInstaller::BaseTypes() const {
  return{ typeid(ContentInstaller) };
}

ContentInstaller::ContentInstaller() {
}

ContentInstaller::ContentInstaller(const MiniYaml& yaml) {
  FieldLoader::Load<ContentInstaller>(*this, yaml);
}

}