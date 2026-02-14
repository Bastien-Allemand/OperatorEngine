#pragma once
#include <DirectXMath.h>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <array>
#include <unordered_map>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define BOLD    "\033[1m"

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using int32_t = __int32;

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

using String = std::string;
using WString = std::wstring;

template<typename T>
using Vector = std::vector<T>;

template<typename Key, typename Value>
using UnOrderedMap = std::unordered_map<Key, Value>;

inline Vector3f VEC3_RIGHT = { 1.0f, 0.0f, 0.0f };
inline Vector3f VEC3_UP = { 0.0f, 1.0f, 0.0f };
inline Vector3f VEC3_FOWARD = { 0.0f, 0.0f, 1.0f };
inline Vector3f VEC3_ZERO = { 0.0f, 0.0f, 0.0f };
inline Vector3f VEC3_ONE = { 1.0f, 1.0f, 1.0f };
inline Quaternion QUAT_ZERO = { 0.0f, 0.0f, 0.0f, 0.0f };
inline Quaternion QUAT_IDENTITY = { 0.0f, 0.0f, 0.0f, 1.0f };
inline Matrix4x4f MATRIX_IDENTITY = 
{ 1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f };

enum class DebugFlag {
    LOG,  
    WARNING, 
    ERROR_   
};

#define DebugMsg(msg, flag) \
    do { \
        if (flag == DebugFlag::ERROR_) { \
            std::wcerr << L"[CRITICAL ERROR] " << msg << std::endl; \
            std::abort(); \
        } else { \
            std::wcout << L"[LOG] " << msg << std::endl; \
        } \
    } while(0)

#define Log(condition, message)                                                                          \
    do {                                                                                                 \
        if (condition) {                                                                                 \
            std::cerr << RED << BOLD << "[ERROR] " << RESET << RED << message << RESET << std::endl;    \
        } else {                                                                                         \
            std::cout << GREEN << BOLD << "[Valid] " << RESET << GREEN << message << RESET << std::endl; \
        }                                                                                                \
    } while (0);

#define _Log(message) \
    do { \
        std::cout << BLUE << BOLD << "[Log] " << RESET << BLUE << message << RESET << std::endl; \
    } while (0);
