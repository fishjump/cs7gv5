#version 330 core
layout(location = 0) in vec3 pos;

uniform mat4 model_uni;
uniform mat4 view_uni;
uniform mat4 projection_uni;

void main() {
	gl_Position = projection_uni * view_uni * model_uni * vec4(pos, 1.0);
}