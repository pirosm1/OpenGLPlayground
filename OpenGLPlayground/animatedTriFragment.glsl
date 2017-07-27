#version 450 core

// input from vector shader
in VS_OUT {
	vec4 color;
} fs_in;

// output to the program
out vec4 color;

void main(void) {
	color = fs_in.color;
}