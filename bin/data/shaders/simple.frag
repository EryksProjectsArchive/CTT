#version 330 core

in vec3 fragNormal;

// Simple fragment shader
void main(void)
{
	gl_FragColor = vec4(fragNormal, 1.0);
}