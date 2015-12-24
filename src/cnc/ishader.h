#pragma once

namespace cnc {

class IShader {
public:
};

using IShaderPtr = std::unique_ptr<IShader>;

}