#version 150 core
uniform mat4 MVP;
uniform sampler2D someTex;
in vec3 position;
out vec4 colour;
void main() {
   gl_Position = MVP * vec4(position.xyz * 0.01, 1.0);
   colour = texture2D(someTex, position.xy);//vec4(position.xyz, 1.0);
};