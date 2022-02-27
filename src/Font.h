#pragma once

#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <vector>
#include <string>

using namespace msdf_atlas;
using MtsdfDynamicAtlas = DynamicAtlas<ImmediateAtlasGenerator<float, 4, &mtsdfGenerator, BitmapAtlasStorage<byte, 4>>>;
const double pixelRange = 2.0;
const double glyphScale = 32.0;
const double miterLimit = 1.0;
const double maxCornerAngle = 3.0;

class Font
{
private:
	msdfgen::FreetypeHandle* freetypeHandle = nullptr;
	msdfgen::FontHandle* fontHandle = nullptr;
	MtsdfDynamicAtlas atlas;
	Charset currentlyLoadedCharacters;
	std::vector<GlyphGeometry> glyphs;

public:
	FontGeometry fontGeometry;
	bool LoadFromFile(const std::string& filePath);
	void AddCharacter(unicode_t character);

	void GetAtlasSize(int& width, int& height);
	const void* GetAtlasPointer();
	unsigned int CharsetSize();

	void MeasureText(const std::string& text, float& width, float scale);

	void DebugWriteAtlas(const std::string& fileName = "DebugAtlas.png");
};