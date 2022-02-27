#version 450 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TextureCoords;

uniform mat4 u_Mat;

out vec2 textureCoords;

void main()
{
	textureCoords = a_TextureCoords;
	gl_Position = u_Mat * vec4(a_Position, 0.0, 1.0);
}