#include "cnc/stdafx.h"
#include "cnc/content_installer.h"

namespace cnc {

const std::vector<FieldLoadInfo> ContentInstaller::load_info = {
  { "TestFiles", StringVectorFieldInfo(&ContentInstaller::test_files) },
  { "PackageMirrorList", TypeFieldInfo(&ContentInstaller::package_mirror_list) },
  { "MusicPackageMirrorList", TypeFieldInfo(&ContentInstaller::music_package_mirror_list) },
  { "ShippedSoundtracks", TypeFieldInfo(&ContentInstaller::shipped_soundtracks) },
};

}