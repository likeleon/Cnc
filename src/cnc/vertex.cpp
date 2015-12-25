#include "cnc/stdafx.h"
#include "cnc/vertex.h"
#include "cnc/float2.h"

namespace cnc {

Vertex::Vertex(const Float2& xy, float u, float v, float p, float c)
  : Vertex(xy.x, xy.y, 0, u, v, p, c) {
}

Vertex::Vertex(float x, float y, float z, float u, float v, float p, float c)
  : x(x), y(y), z(z), u(u), v(v), p(p), c(c) {
}

}