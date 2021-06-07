#version 460 core

layout(location = 0) uniform dvec2 field_pos;
layout(location = 1) uniform double cell_size;

layout(location = 0) in ivec2 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in ivec2 dir[2];

/**
layout(binding = 1) buffer temp_otp {
	int temp0;
	int temp1;
	int temp2;
	int temp3;
	int temp4;
};
/**/

out CellData {
	dvec2 pos;
	int dir[2];
	vec3 color;
} otp;

int ConvertDir(ivec2 dir) {
	//ToDo check out the binary
	return
		(dir.y==-1) || (dir.y > +1) ? 1 :
		(dir.x==+1) || (dir.x < -1) ? 2 :
		(dir.y==+1) || (dir.y < -1) ? 3 :
		(dir.x==-1) || (dir.x > +1) ? 4 :
	0;
}

void main() {

	otp.pos = pos*cell_size + field_pos;
	
	otp.dir[0] = ConvertDir(dir[0]);
	otp.dir[1] = ConvertDir(dir[1]);

	otp.color = color;

}
