#pragma once
class Font
{
public :
		Font();
		~Font();

		bool Init(ID3D12Device* _device,WString _texturePath, int32 _rows, int32 _cols);

		void AddCharacter(char _character, float32 _u, float32 _v, float32 _width, float32 _height, int _advanceX);

		void DrawString(ID3D12GraphicsCommandList* _cmdList, String _text, float32 _x, float32 _y, float32 _size);
private:
	ID3D12Resource* m_texture;
	ID3D12Resource* m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	int32 m_rows;
	int32 m_cols;
	float32 m_charWidth;

	struct FontVertex
	{
		Vector3f pos;
		Vector2f uv;
	};

	struct CharDefinition
	{
		float32 u;
		float32 v;
		float32 width;
		float32 height;
		float32 aspectRatio;
		int advanceX;
	};

	CharMap<CharDefinition> m_alphabet;
};

