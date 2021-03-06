#version 330 core

uniform sampler2D texture;

in vec4 outColor;
in vec2 outUV;

// Simple fragment shader
void main(void)
{
	gl_FragColor = vec4(1,1,1,texture2D(texture, outUV).r) * outColor;
}