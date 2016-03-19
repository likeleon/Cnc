#pragma once

/*
sha1.hpp - header of
============
SHA-1 in C++
============
100% Public Domain.
Original C Code
-- Steve Reid <steve@edmweb.com>
Small changes to fit into bglibs
-- Bruce Guenter <bruce@untroubled.org>
Translation to simpler C++ Code
-- Volker Grabsch <vog@notjusthosting.com>
Safety fixes
-- Eugene Hopkinson <slowriot at voxelstorm dot com>
*/

#include <cstdint>
#include <iostream>
#include <string>

class SHA1 {
public:
  SHA1();
  
  static std::string FromFile(const std::string &filename);
  
  void Update(const std::string &s);
  void Update(std::istream &is);
  std::string Final();

private:
  uint32_t digest[5];
  std::string buffer;
  uint64_t transforms;
};