#version 150 core
uniform mat4 MVP;
in vec3 position;
in vec3 normal;
in vec3 color;
in vec2 uv;
out vec2 frag_uv;
void main() {
   gl_Position = MVP * vec4(position.xyz * 0.01, 1.0);
   frag_uv = uv;
};