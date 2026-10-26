#version 460 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D textTexture;
uniform vec4 textColor;

void main() {
    vec4 texColor = texture(textTexture, TexCoords);
    FragColor = vec4(textColor.rgb, texColor.r);
}