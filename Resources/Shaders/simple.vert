#version 150 core
uniform mat4 MVP;
in vec3 position;
out vec4 colour;
void main() {
   gl_Position = MVP * vec4(position.xyz * 0.01, 1.0);
   colour = vec4(position.xyz, 1.0);
};