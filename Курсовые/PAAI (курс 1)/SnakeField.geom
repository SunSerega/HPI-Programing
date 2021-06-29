#version 460 core

uniform dvec2 cell_size;

layout(points) in;
in CellData {
	dvec2 pos;
	int dir[2];
	vec3 color;
} inp[];

layout(triangle_strip, max_vertices = 5) out;
out vec3 color;

/**
layout(binding = 1) buffer temp_otp {
	int temp[3];
};
/**/



const float border_len1 = 0.3;
const float border_len2 = 1f-border_len1;

void SendVertex(double coord1, double coord2) {
	gl_Position = vec4(coord1, coord2, 0, 1);
	EmitVertex();
}
void SendVertex(dvec2 coord) {
	SendVertex(coord.x, coord.y);
}

vec2 RotatePoint(vec2 p, int dir0) {
	if (bool(dir0 & 1)) p = vec2(1-p.y, 0+p.x);
	if (!bool(dir0 & 2)) p = 1-p;
	return p;
}



void SingleCell(dvec2 pos) {
	SendVertex( pos + dvec2(0, 0) * cell_size );
	SendVertex( pos + dvec2(0, 1) * cell_size );
	SendVertex( pos + dvec2(1, 0) * cell_size );
	SendVertex( pos + dvec2(1, 1) * cell_size );
	EndPrimitive();
}

void SnakeEnd(dvec2 pos, int dir0) {
	vec2 pts[4] = {
		{ border_len1, border_len1 },
		{ border_len1, 1 },
		{ border_len2, border_len1 },
		{ border_len2, 1 },
	};

	for (int i = 0; i<4; ++i)
		SendVertex(pos + cell_size*RotatePoint(pts[i], dir0));
	EndPrimitive();
}

void SnakeBody(dvec2 pos, int dir0, int dir_change) {
	vec2 pts[5];
	pts[0] = vec2( border_len1, 0 );
	pts[1] = vec2( border_len2, 0 );
	
	int pts_count = 4 + int(dir_change != 0);
	switch (dir_change) {
	case 0:
		pts[2] = vec2( border_len1, 1 );
		pts[3] = vec2( border_len2, 1 );
		break;
	case 1:
		pts[2] = vec2( 0,			border_len1 );
		pts[3] = vec2( border_len2,	border_len2 );
		pts[4] = vec2( 0,			border_len2 );
		break;
	case 3:
		pts[2] = vec2( border_len1, border_len2 );
		pts[3] = vec2( 1,			border_len1 );
		pts[4] = vec2( 1,			border_len2 );
		break;
	default:
		return;
	}

	for (int i = 0; i<pts_count; ++i)
		SendVertex(pos + cell_size*RotatePoint(pts[i], dir0));
	EndPrimitive();
}

void main() {
	color = inp[0].color;
	dvec2 pos = inp[0].pos;
	int dir[2] = inp[0].dir;
	
	if (dir[1] == 0) {
		
		if (dir[0] == 0)
			SingleCell(pos);
		else
			SnakeEnd(pos, dir[0]+1);
		
	} else {
		
		if (dir[0] == 0)
			SnakeEnd(pos, dir[1]-1);
		else
			SnakeBody(pos, dir[0]-1, (dir[1]-dir[0]) & 3);
		
	}

}
