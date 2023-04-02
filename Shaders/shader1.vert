#version 330
layout (location = 0) in vec3 pos;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
out vec4 vCol;
void main()
{
	gl_Position = projection * view * model * vec4(pos.x, pos.y, pos.z, 1.0);
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	// vCol = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}