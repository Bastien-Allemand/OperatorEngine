#pragma once


// Library

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32.lib")
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <wrl.h>        // Pour Microsoft::WRL::ComPtr
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cstdint>
#include <string>
#include <stdexcept>


using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;

using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using float32 = float;
using float64 = double;

//DirectX

#include <DirectXMath.h>
using namespace DirectX;


XMFLOAT3 d;


// Maths
namespace ope {

	template <typename T> union Vector2;
	union Vector3f32;

	using Vector2i8 = Vector2<int8>;
	using Vector2i16 = Vector2<int16>;
	using Vector2i32 = Vector2<int32>;
	using Vector2i64 = Vector2<int64>;
	using Vector2ui8 = Vector2<uint8>;
	using Vector2ui16 = Vector2<uint16>;
	using Vector2ui32 = Vector2<uint32>;
	using Vector2ui64 = Vector2<uint64>;
	using Vector2f32 = Vector2<float32>;
	using Vector2f64 = Vector2<float64>;
}


// CATH INITIALISATION OR MEMORY GPU ERROR
inline void ThrowIfFailed(HRESULT hr) {
	if (FAILED(hr)) {
		// Dans un vrai moteur, on loggerait l'erreur ici
		throw std::runtime_error("DirectX 12 Error: HRESULT " + std::to_string(hr));
	}
}


#define ALIGN_UP(size, alignment) (((size) + (alignment) - 1) & ~((alignment) - 1))

// DEBUG CONFIGURATION
#ifdef _DEBUG
#define ENGINE_DEBUG 1
#else
#define ENGINE_DEBUG 0
#endif
