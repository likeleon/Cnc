#pragma once

namespace cnc {

struct FieldLoadInfo;

class TileSet {
public:
  TileSet(const std::string& filepath);

  const std::string& name() const { return name_; }
  const std::string& id() const { return id_; }

  std::vector<FieldLoadInfo> GetLoadInfo() const;

  TileSet(const TileSet& other) = delete;
  TileSet& operator=(const TileSet& other) = delete;
  TileSet& operator=(TileSet&& other) = delete;

private:
  std::string name_;
  std::string id_;
  int32_t sheet_size_ = 512;
  std::string palette_;
  std::string player_palette_;
  bool ignore_tile_sprite_offsets_ = false;
  bool enable_depth_ = false;
};

}
