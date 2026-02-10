#include "pch.h"

#include <iostream>

#include "RenderDevice.h"
#include "Factory.h"



bool RenderDevice::Init(Factory* _factory)
{
	if (_factory == nullptr)	
	{
		std::cout << "Factory Required" << std::endl;
		return 1;
	}
	HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
	if (FAILED(hr))
	{
		//find adapter for device
		if (_factory->InitAdapter())
		{
			if (hr = D3D12CreateDevice(_factory->GAdapter(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));//pray this works ig
			{
				std::cout << "Device creation failed" << std::endl;
				return 1;
			}
		}
		else
		{
			std::cout << "Failed to find compatible adapter" << std::endl;
			return 1;
		}
	}
	return 0; 
}



