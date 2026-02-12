#include "pch.h"
#include "LoadFont.h"
#include <fstream>
#include "Font.h"

void LoadFont::Load(Font& _font, WString _textPath, WString _dataPath)
{
    std::wifstream file(_dataPath);
    if (!file.is_open())
    {
        DebugMsg(L"Failed to open font data file: " + _dataPath, DebugFlag::ERROR_);
        return;
    }

    float32 texWidth = 256.0f;
    float32 texHeight = 256.0f;

    WString line;
    while (std::getline(file, line))
    {
        if (line.substr(0, 5) == L"char ")
        {
            int id, x, y, width, height, xadvance;
            if (swscanf_s(line.c_str(), L"char id=%d x=%d y=%d width=%d height=%d xoffset=%*d yoffset=%*d xadvance=%d",
                &id, &x, &y, &width, &height, &xadvance) == 6)
            {
                float u = (float)x / texWidth;
                float v = (float)y / texHeight;
                float wUV = (float)width / texWidth;
                float hUV = (float)height / texHeight;
                _font.AddCharacter((char)id, u, v, wUV, hUV, xadvance);
            }
        }
        else if (line.substr(0, 7) == L"common ")
        {
            swscanf_s(line.c_str(), L"common lineHeight=%*d base=%*d scaleW=%f scaleH=%f", &texWidth, &texHeight);
        }
    }
    file.close();
}
