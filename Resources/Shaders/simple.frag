#version 150 core
uniform sampler2D mainTex;
in vec3 onormal;
in vec3 ocolor;
in vec2 texcoord;
out vec4 outColor;
void main() {
   outColor = texture(mainTex, texcoord);
};