#version 450 core

layout (location = 0) out vec4 o_Color;

in vec2 textureCoords;

uniform sampler2D msdf_atlas;
uniform vec4 text_color = vec4(1.0, 1.0, 1.0, 1.0);
uniform float pxRange = 1.5;

float median(float a, float b, float c) {
	return max(min(a, b), min(max(a, b), c));
}

float screenPxRange() {
	vec2 unitRange = vec2(pxRange) / vec2(textureSize(msdf_atlas, 0));
	vec2 screenTexSize = vec2(1.0) / fwidth(textureCoords);
	return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

void main()
{
	vec3 msd = texture(msdf_atlas, textureCoords).rgb;
	float sd = median(msd.r, msd.g, msd.b);
	float screenPxDistance = screenPxRange()*(sd - 0.5);
	float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	o_Color = mix(vec4(text_color.rgb, 0.0), text_color, opacity);

	// float smoothGapSize = fwidth(textureCoords.x) * 320;
	// float halfSmoothGapSize = smoothGapSize * 0.5;
	// vec3 msd = texture(msdf_atlas, textureCoords).rgb;
	// float sd = median(msd.r, msd.g, msd.b);
	// // float opacity = smoothstep(0.5 - halfSmoothGapSize, 0.5 + halfSmoothGapSize, sd);
	// float opacity = smoothstep(0.5, 0.5 + smoothGapSize, sd);
	// o_Color = mix(vec4(text_color.rgb, 0.0), text_color, opacity);
}