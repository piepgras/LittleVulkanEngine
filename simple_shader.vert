#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

//layout(location = 0) out vec3 fragColor;

//Max 1 push constant per shader
layout(push_constant) uniform Push {
	mat2 transform;
	vec2 offset;
	vec3 color;
} push;

void main() {

	// Note that matrix multiplication is NOT commutative, MATRIX * POSITION != POSITION * MATRIX
	gl_Position = vec4(push.transform * position + push.offset, 0.0, 1.0);

}