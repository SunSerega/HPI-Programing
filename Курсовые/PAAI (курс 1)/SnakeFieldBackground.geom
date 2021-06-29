#version 460 core

layout(location = 0) uniform dvec2 field_pos;
layout(location = 1) uniform dvec2 field_size;

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
out vec3 color;

/**
layout(binding = 1) buffer temp_otp {
	vec4 data[4];
} temp;
int i;
/**/

void SendVertex(double coord1, double coord2) {
	gl_Position = vec4(coord1, coord2, 0, 1);
	//temp.data[i++] = gl_Position;
	EmitVertex();
}

void main() {
	//i = 0;
	color = vec3(0.6, 0.6, 0.6);
	SendVertex(field_pos.x + field_size.x*0, field_pos.y + field_size.y*0);
	SendVertex(field_pos.x + field_size.x*0, field_pos.y + field_size.y*1);
	SendVertex(field_pos.x + field_size.x*1, field_pos.y + field_size.y*0);
	SendVertex(field_pos.x + field_size.x*1, field_pos.y + field_size.y*1);
	EndPrimitive();
}
