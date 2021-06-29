#version 460 core

layout(location = 0) uniform dvec2 field_pos;
layout(location = 1) uniform dvec2 cell_size;

layout(location = 0) in ivec2 prev_pos;
layout(location = 1) in ivec2 curr_pos;
layout(location = 2) in ivec2 next_pos;
layout(location = 3) in vec3 color;
layout(location = 4) in int dir_bits;

#define DIR_HAS_PREV_BIT 1
#define DIR_HAS_NEXT_BIT 2

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
	
	otp.pos = curr_pos*cell_size + field_pos;
	
	otp.dir[0] = bool(dir_bits&DIR_HAS_PREV_BIT) ? ConvertDir(curr_pos-prev_pos) : 0;
	otp.dir[1] = bool(dir_bits&DIR_HAS_NEXT_BIT) ? ConvertDir(next_pos-curr_pos) : 0;
	
	otp.color = color;
	
}
