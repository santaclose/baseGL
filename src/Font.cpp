#include "Font.h"

template <typename T>
void addGlyphsToAtlas(GlyphGeometry* glyphs, int count, T& atlas)
{
	for (int i = 0; i < count; ++i)
	{
		glyphs[i].edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
		glyphs[i].wrapBox(glyphScale, pixelRange / glyphScale, miterLimit);
	}

	T::ChangeFlags change = atlas.add(glyphs, count);
}

bool Font::LoadFromFile(const std::string& filePath)
{
    bool success = false;
    if (freetypeHandle = msdfgen::initializeFreetype())
	{
		if (fontHandle = msdfgen::loadFont(freetypeHandle, filePath.c_str()))
		{
			fontGeometry = FontGeometry(&glyphs);
			fontGeometry.loadCharset(fontHandle, 1.6, Charset::ASCII);
			currentlyLoadedCharacters = Charset::ASCII;
			addGlyphsToAtlas(&(glyphs[0]), glyphs.size(), atlas);

			success = true;
		}
    }
    return success;
}

void Font::AddCharacter(unicode_t character)
{
	if (!currentlyLoadedCharacters.contains(character))
	{
		GlyphGeometry newGlyph;
		newGlyph.load(fontHandle, fontGeometry.getGeometryScale(), character);
		
		addGlyphsToAtlas(&newGlyph, 1, atlas);
		fontGeometry.addGlyph(newGlyph);

		currentlyLoadedCharacters.add(character);
	}
}

void Font::GetAtlasSize(int& width, int& height)
{
	auto atlasStorage = atlas.atlasGenerator().atlasStorage();
	width = atlasStorage.getWidth();
	height = atlasStorage.getHeight();
}

const void* Font::GetAtlasPointer()
{
	const BitmapAtlasStorage<byte, 4>& atlasStorage = atlas.atlasGenerator().atlasStorage();
	msdfgen::BitmapConstRef<byte, 4> reference = static_cast<msdfgen::BitmapConstRef<byte, 4>>(atlasStorage);
	return reference.pixels;
}

unsigned int Font::CharsetSize()
{
	return currentlyLoadedCharacters.size();
}

void Font::MeasureText(const std::string& text, float& width, float scale)
{
	if (text.length() == 0)
	{
		width = 0.0f;
		return;
	}

	float l, r;

	float x = 0.0f;
	float y = 0.0f;
	for (int i = 0; i < text.length(); i++)
	{
		double pl, pb, pr, pt;
		fontGeometry.getGlyph(text[i])->getQuadPlaneBounds(pl, pb, pr, pt);

		if (i == 0)
		{
			l = x + pl;
		}
		if (i == text.length() - 1)
		{
			r = x + pr;
		}

		if (i < text.length() - 1)
		{
			double advance;
			fontGeometry.getAdvance(advance, text[i], text[i + 1]);
			x += advance * scale;
		}
	}
	width = r - l;
}

void Font::DebugWriteAtlas(const std::string& fileName)
{
	auto bitmap = atlas.atlasGenerator().atlasStorage();
	msdfgen::savePng(bitmap, fileName.c_str());
}