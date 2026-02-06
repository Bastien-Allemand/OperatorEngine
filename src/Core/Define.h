#pragma once
#include <DirectXMath.h>
#include <string>

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint32_t = __int32;

using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;
using uint32_t = unsigned __int32;

using float32 = float;
using float64 = double;

using Vector2f = DirectX::XMFLOAT2;
using Vector3f = DirectX::XMFLOAT3;
using Vector4f = DirectX::XMFLOAT4;
using Quaternion = DirectX::XMFLOAT4;
using Matrix4x4f = DirectX::XMFLOAT4X4;

using String = std::string;
using WString = std::wstring;
