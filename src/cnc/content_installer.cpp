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

std::deque<std::type_index> ContentInstaller::Types() const {
  return{ typeid(ContentInstaller), typeid(GlobalModData) };
}

ContentInstaller::ContentInstaller() {
}

ContentInstaller::ContentInstaller(const MiniYaml& yaml) {
  FieldLoader::Load<ContentInstaller>(*this, yaml);
}

}