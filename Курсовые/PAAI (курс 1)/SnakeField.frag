#version 460 core

in CellVertex {
	//ToDo check out the binary
	noperspective vec3 color;
} inp;

out vec3 out_color;

void main() {
	out_color = inp.color;
}
