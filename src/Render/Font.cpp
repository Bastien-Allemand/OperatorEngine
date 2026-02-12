#include "pch.h"
#include "Font.h"
#include "DDSTextureLoader12.h"

bool Font::Init(ID3D12Device* _device, WString _texturePath, int32 _rows, int32 _cols)
{
	m_rows = _rows;
	m_cols = _cols;
	m_charWidth = 1.0f / static_cast<float32>(_cols);
	std::unique_ptr<uint8_t[]> ddsData;
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	HRESULT hr = DirectX::LoadDDSTextureFromFile(
		_device,
		_texturePath.c_str(),
		&m_texture,
		ddsData,
		subresources
	);
	if (FAILED(hr))
	{
		DEBUG_MESSAGE(L"Failed to load font texture: " + _texturePath, DebugFlag::Error);
		return false;
	}
}

void Font::AddCharacter(char _character, float32 _u, float32 _v, float32 _width, float32 _height, int _advanceX)
{
	CharDefinition def;
	def.u = _u;
	def.v = _v;
	def.width = _width;
	def.height = _height;
	def.aspectRatio = _width / _height;
	def.advanceX = _advanceX;
	m_alphabet[_character] = def;
}

void Font::DrawString(ID3D12GraphicsCommandList* _cmdList, String _text, float32 _x, float32 _y, float32 _size)
{
	for (char c : _text)
	{
		if (m_alphabet.find(c) == m_alphabet.end()) continue;
		CharDefinition& def = m_alphabet[c];
		float32 charWidth = m_charWidth * def.aspectRatio * _size;

		_x += charWidth + (def.advanceX * _size);
	}
}