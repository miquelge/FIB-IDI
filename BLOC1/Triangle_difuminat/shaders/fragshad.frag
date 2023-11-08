#version 330 core

out vec4 FragColor;
in vec3 frcolor;

void main() {
    FragColor = vec4(frcolor, 1);
}

