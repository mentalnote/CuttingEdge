#version 150 core
uniform mat4 MVP;
in vec3 position;
in vec3 normal;
in vec3 color;
in vec2 uv;
out vec3 onormal;
out vec3 ocolor;
out vec2 texcoord;
void main() {
   gl_Position = MVP * vec4(position.xyz, 1.0);
   onormal = normal;
   ocolor = color;
   texcoord = uv;
};