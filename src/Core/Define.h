#pragma once
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using int32_t = __int32;

using byte = unsigned char;
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

using Matrix = DirectX::XMMATRIX;
using XVector = DirectX::XMVECTOR;




inline Vector3f OPE_VEC3_RIGHT = { 1.0f, 0.0f, 0.0f };
inline Vector3f OPE_VEC3_UP = { 0.0f, 1.0f, 0.0f };
inline Vector3f OPE_VEC3_DIR = { 0.0f, 0.0f, 1.0f };
inline Vector3f OPE_VEC3_ZERO = { 0.0f, 0.0f, 0.0f };
inline Vector3f OPE_VEC3_ONE = { 1.0f, 1.0f, 1.0f };


inline Quaternion OPE_QUAT_ZERO = { 0.0f, 0.0f, 0.0f, 1.0f };





using String = std::string;
using WString = std::wstring;

template<typename T>
using Vector = std::vector<T>;


enum class DebugFlag {
    Info,
    Warning,
    Error
};


#define DEBUG_MESSAGE(msg, flag) \
    do { \
        if (flag == DebugFlag::Error) { \
            std::wcerr << L"[CRITICAL ERROR] " << msg << std::endl; \
            std::abort(); \
        } else { \
            std::wcout << L"[LOG] " << msg << std::endl; \
        } \
    } while(0)
