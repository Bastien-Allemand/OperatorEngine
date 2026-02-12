#include "pch.h"
#include "Geometry.h"
#include <DirectXColors.h>

void Geometry::BuildBox()
{
	vertices =
	{
		vertex({ Vector3f(-1.f, -1.f, -1.f), Vector4f(DirectX::Colors::White) }),
		vertex({ Vector3f(-1.f, +1.f, -1.f), Vector4f(DirectX::Colors::Black) }),
		vertex({ Vector3f(+1.f, +1.f, -1.f), Vector4f(DirectX::Colors::Red) }),
		vertex({ Vector3f(+1.f, -1.f, -1.f), Vector4f(DirectX::Colors::Green) }),
		vertex({ Vector3f(-1.f, -1.f, +1.f), Vector4f(DirectX::Colors::Blue) }),
		vertex({ Vector3f(-1.f, +1.f, +1.f), Vector4f(DirectX::Colors::Yellow) }),
		vertex({ Vector3f(+1.f, +1.f, +1.f), Vector4f(DirectX::Colors::Cyan) }),
		vertex({ Vector3f(+1.f, -1.f, +1.f), Vector4f(DirectX::Colors::Magenta) })
	};

	indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};
}