#version 460 core

in vec2 uvOut;
in vec3 fragPos;
in vec3 normal;
flat in int materialId;

out vec4 fragColor;

uniform sampler2D textures[16];
uniform vec3 lightPos;       // World-space light position
uniform vec3 viewPos;        // World-space camera position
uniform vec3 lightColor;     // RGB light color
uniform vec3 ambientColor;   // Base ambient color

vec2 flip(vec2 uv) {
	return vec2(uv.x, 1.0 - uv.y);
}

void main() {
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	vec3 ambient = ambientColor * lightColor;
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32) * 3;
	vec3 specular = spec * lightColor;

	vec3 lighting = ambient + diffuse + specular;

	vec4 textureColor = texture(textures[materialId], flip(uvOut));
	fragColor = vec4(textureColor.rgb * lighting, textureColor.a) * 1.5;
}



