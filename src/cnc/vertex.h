#pragma once

namespace cnc {

struct Float2;

struct Vertex {
  Vertex(const Float2& xy, float u, float v, float p, float c);
  Vertex(float x, float y, float z, float u, float v, float p, float c);

  float x;
  float y;
  float z;
  float u;
  float v;
  float p;
  float c;
};

}
