#version 150 core
uniform sampler2D mainTex;
in vec2 frag_uv;
out vec4 outColor;
void main() {
   outColor = texture(mainTex, frag_uv);//vec4(frag_uv, 0, 1);
};