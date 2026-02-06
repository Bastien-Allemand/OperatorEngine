#pragma once


// Library

//#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "ws2_32.lib")
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

#define SAFE_RELEASE(p) if (p) { (p)->Release(); (p) = nullptr; }

// CATH INITIALISATION OR MEMORY GPU ERROR
inline void ThrowIfFailed(HRESULT hr) {
	if (FAILED(hr)) {
		// Dans un vrai moteur, on loggerait l'erreur ici
	#ifdef _DEBUG
			OutputDebugStringA("DirectX Error Detected!\n");
			DebugBreak();
	#endif
			throw std::runtime_error("DirectX Error");
	}
}

// ALIGNEMENT DES CONSTANT BUFFER
#define D3D12_CONSTANT_BUFFER_ALIGN(size) ((size + 255) & ~255)


#define ALIGN_UP(size, alignment) (((size) + (alignment) - 1) & ~((alignment) - 1))



// DEBUG CONFIGURATIONa
#ifdef _DEBUG
#define ENGINE_DEBUG 1
#else
#define ENGINE_DEBUG 0
#endif
