#version 450 core

// "offset" is an input vertex attribute
// layout (location = 0) tells glVertexAttribfv to put the "0" specified attribute int offset.
layout (location = 0) in vec4 offset;
layout (location = 1) in vec4 color;

out VS_OUT {
	vec4 color;
} vs_out;

void main(void) {
	const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25, 0.5, 1.0),
									 vec4(-0.25, -0.25, 0.5, 1.0),
									 vec4( 0.25,  0.25, 0.5, 1.0));

	// tell the renderer to postion these vectors with the "dynamic" offset
	gl_Position = vertices[gl_VertexID] + offset;

	vs_out.color = color;
}