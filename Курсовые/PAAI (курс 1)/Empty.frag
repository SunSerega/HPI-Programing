#version 460 core

//ToDo check out the binary
noperspective in vec3 color;
out vec3 out_color;

/**
layout(binding = 1) buffer temp_otp {
	vec3 color;
} temp;
/**/

void main() {
	out_color = color;
}