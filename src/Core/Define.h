#pragma once

//#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "ws2_32.lib")
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <stdexcept>

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

//DirectX
#include <DirectXMath.h>

#define Vector2 DirectX::XMFLOAT2 
#define Vector3 DirectX::XMFLOAT3 
#define Quaternion DirectX::XMFLOAT4
#define Matrix DirectX::XMMATRIX