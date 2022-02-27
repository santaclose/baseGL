#include "Renderer.h"
#include <vector>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <Shader.h>
#include <Font.h>
#include <iostream>

#undef DrawText // stupid windows thing
#undef CreateFont// stupid windows thing

float DEFAULT_MATRIX[16]{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

float SQUARE_VERTEX_POSITIONS[18] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
};

float CUBE_VERTEX_POSITIONS[18 * 6] = {
	-0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5f, 0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f,-0.5f,
	0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
	0.5f, 0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f,-0.5f,
	0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f,-0.5f, 0.5f,
	0.5f,-0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f,-0.5f,-0.5f,
	0.5f, 0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f,-0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f,-0.5f,
	-0.5f, 0.5f,-0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5f,-0.5f, 0.5f
};

struct glUnindexedDrawData {
	unsigned int VBO = -1;
	unsigned int VAO = -1;
};


struct glIndexedDrawData {
	unsigned int VBO = -1;
	unsigned int VAO = -1;
	unsigned int EBO = -1;
};

std::unordered_map<const void*, glUnindexedDrawData> unindexedDrawMap;
std::unordered_map<const void*, glIndexedDrawData> indexedDrawMap;

Shader msdfTextShader;
Shader defaultShader;
const float* matrix = DEFAULT_MATRIX;

std::vector<float> text_vertices;
std::vector<unsigned int> text_indices;

struct fontData {
	Font* font;
	unsigned int glTextureId;
};

std::vector<fontData> fontVector;
glIndexedDrawData textDrawData;

glm::mat4 toScreenSpaceMatrix;

void InitializeDefaultShaderIfNeeded()
{
	if (!defaultShader.IsInitialized())
		defaultShader.CreateFromFiles("assets/shaders/vert.glsl", "assets/shaders/frag.glsl");
}

void Renderer::UpdateWindowSize(const glm::vec2& newSize)
{
	std::cout << "panel 2d resized\n";
	std::cout << newSize.x << ", " << newSize.y << std::endl;
	toScreenSpaceMatrix = glm::ortho(0.0f, newSize.x, 0.0f, newSize.y);
}

void Renderer::SetMatrix(const float* mat)
{
	matrix = mat;
}

void Renderer::DrawCube(const float* color)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	InitializeDefaultShaderIfNeeded();

	if (unindexedDrawMap.find(CUBE_VERTEX_POSITIONS) == unindexedDrawMap.end())
	{
		glUnindexedDrawData drawData;
		// 1. bind Vertex Array Object
		glGenVertexArrays(1, &drawData.VAO);
		glGenBuffers(1, &drawData.VBO);
		glBindVertexArray(drawData.VAO);
		// 2. copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, drawData.VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_VERTEX_POSITIONS), CUBE_VERTEX_POSITIONS, GL_STATIC_DRAW);
		// 3. then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		unindexedDrawMap[CUBE_VERTEX_POSITIONS] = drawData;
	}

	defaultShader.Bind();
	defaultShader.SetUniformMatrix4fv("u_Mat", matrix);
	defaultShader.SetUniform4fv("u_Color", color);
	glBindVertexArray(unindexedDrawMap[CUBE_VERTEX_POSITIONS].VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6);
}

void Renderer::DrawSquare(const float* color)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	InitializeDefaultShaderIfNeeded();

	if (unindexedDrawMap.find(SQUARE_VERTEX_POSITIONS) == unindexedDrawMap.end())
	{
		glUnindexedDrawData drawData;
		// 1. bind Vertex Array Object
		glGenVertexArrays(1, &drawData.VAO);
		glGenBuffers(1, &drawData.VBO);
		glBindVertexArray(drawData.VAO);
		// 2. copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, drawData.VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SQUARE_VERTEX_POSITIONS), SQUARE_VERTEX_POSITIONS, GL_STATIC_DRAW);
		// 3. then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		unindexedDrawMap[SQUARE_VERTEX_POSITIONS] = drawData;
	}

	defaultShader.Bind();
	defaultShader.SetUniformMatrix4fv("u_Mat", matrix);
	defaultShader.SetUniform4fv("u_Color", color);
	glBindVertexArray(unindexedDrawMap[SQUARE_VERTEX_POSITIONS].VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawText(unsigned int font, const std::string& text, const float* color, bool screenSpace, const glm::vec2& pos, float scale, bool center)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	fontData fd = fontVector[font];
	unsigned int charsetSizeBefore = fd.font->CharsetSize();
	for (int i = 0; i < text.length(); i++)
		fd.font->AddCharacter(text[i]);
	unsigned int charsetSizeAfter = fd.font->CharsetSize();
	int atlasWidth, atlasHeight;
	fd.font->GetAtlasSize(atlasWidth, atlasHeight);
	// update texture in gpu if new chars in atlas
	if (charsetSizeAfter > charsetSizeBefore)
	{
		glBindTexture(GL_TEXTURE_2D, fd.glTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, fd.font->GetAtlasPointer());
	}


	float x = pos.x;
	float y = pos.y;
	if (center)
	{
		float width;
		fd.font->MeasureText(text, width, scale);
		x -= width * 0.5f;
	}
	text_vertices.resize(text.length() * 4 * 4);
	text_indices.resize(text.length() * 6);
	for (int i = 0; i < text.length(); i++)
	{
		double pl, pb, pr, pt;
		double il, ib, ir, it;
		float tl, tb, tr, tt;
		fd.font->fontGeometry.getGlyph(text[i])->getQuadPlaneBounds(pl, pb, pr, pt);
		fd.font->fontGeometry.getGlyph(text[i])->getQuadAtlasBounds(il, ib, ir, it);
		tl = (float)il / (float)atlasWidth;
		tr = (float)ir / (float)atlasWidth;
		tt = (float)it / (float)atlasHeight;
		tb = (float)ib / (float)atlasHeight;

		text_vertices[i * 4 * 4 + 0] = x + pl * scale;
		text_vertices[i * 4 * 4 + 1] = y + pb * scale;
		text_vertices[i * 4 * 4 + 2] = tl;
		text_vertices[i * 4 * 4 + 3] = tb;

		text_vertices[i * 4 * 4 + 4] = x + pr * scale;
		text_vertices[i * 4 * 4 + 5] = y + pb * scale;
		text_vertices[i * 4 * 4 + 6] = tr;
		text_vertices[i * 4 * 4 + 7] = tb;

		text_vertices[i * 4 * 4 + 8] = x + pr * scale;
		text_vertices[i * 4 * 4 + 9] = y + pt * scale;
		text_vertices[i * 4 * 4 + 10] = tr;
		text_vertices[i * 4 * 4 + 11] = tt;

		text_vertices[i * 4 * 4 + 12] = x + pl * scale;
		text_vertices[i * 4 * 4 + 13] = y + pt * scale;
		text_vertices[i * 4 * 4 + 14] = tl;
		text_vertices[i * 4 * 4 + 15] = tt;

		text_indices[i * 6 + 0] = i * 4 + 0;
		text_indices[i * 6 + 1] = i * 4 + 1;
		text_indices[i * 6 + 2] = i * 4 + 3;
		text_indices[i * 6 + 3] = i * 4 + 3;
		text_indices[i * 6 + 4] = i * 4 + 1;
		text_indices[i * 6 + 5] = i * 4 + 2;

		if (i < text.length() - 1)
		{
			double advance;
			fd.font->fontGeometry.getAdvance(advance, text[i], text[i + 1]);
			x += advance * scale;
		}
	}

	if (textDrawData.VAO == -1)
	{
		glGenVertexArrays(1, &textDrawData.VAO);
		glGenBuffers(1, &textDrawData.VBO);
		glGenBuffers(1, &textDrawData.EBO);

		glBindVertexArray(textDrawData.VAO);
		// 2. copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, textDrawData.VBO);
		glBufferData(GL_ARRAY_BUFFER, text_vertices.size() * sizeof(float), &(text_vertices[0]), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textDrawData.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, text_indices.size() * sizeof(unsigned int), &(text_indices[0]), GL_DYNAMIC_DRAW);
		// 3. then set our vertex attributes pointers
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
		glEnableVertexAttribArray(1);
	}
	else if (text_vertices.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, textDrawData.VBO);
		glBufferData(GL_ARRAY_BUFFER, text_vertices.size() * sizeof(float), &(text_vertices[0]), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textDrawData.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, text_indices.size() * sizeof(unsigned int), &(text_indices[0]), GL_DYNAMIC_DRAW);
	}

	if (!msdfTextShader.IsInitialized())
		msdfTextShader.CreateFromFiles("assets/shaders/msdf_vert.glsl", "assets/shaders/msdf_frag.glsl");
	msdfTextShader.Bind();
	msdfTextShader.SetUniformMatrix4fv("u_Mat", screenSpace ? &toScreenSpaceMatrix[0][0] : matrix);
	msdfTextShader.SetUniform4fv("text_color", color);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fd.glTextureId);
	glBindVertexArray(textDrawData.VAO);
	glDrawElements(GL_TRIANGLES, text_indices.size(), GL_UNSIGNED_INT, 0);
}

unsigned int Renderer::CreateFont(const std::string& fontFilePath)
{
	// font and atlas
	fontData newFontData;
	newFontData.font = new Font();
	newFontData.font->LoadFromFile(fontFilePath);

	int atlasWidth, atlasHeight;
	newFontData.font->GetAtlasSize(atlasWidth, atlasHeight);
	glGenTextures(1, &newFontData.glTextureId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, newFontData.glTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, newFontData.font->GetAtlasPointer());
	fontVector.push_back(newFontData);

	return fontVector.size() - 1;
}

unsigned int Renderer::CreateSVG(const std::string& svgFilePath)
{
	return 0;
}
