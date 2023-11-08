#version 330 core

in vec3 vertex;
in vec3 color;
out vec3 frcolor;

void main()  {
	frcolor = color;
    gl_Position = vec4 (vertex.x*0.5, vertex.y*0.5, vertex.z*0.5, 1.0);
}
