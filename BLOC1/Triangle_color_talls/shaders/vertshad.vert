#version 330 core

in vec3 vertex;
in vec3 vertex_2;

void main()  {
    gl_Position = vec4 (vertex, 1.0);
	//gl_Position = vec4 (vertex_2, 1.0);
}
