#version 460 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	/*
		Mix both textures, the 0.2 says that the resulting
		texture will have 80% (0.8) of the first texture
		and 20% (0.2) of the second texture
	*/

	FragColor = mix(texture(texture1, TexCoord),
					texture(texture2, vec2(1-TexCoord.x, TexCoord.y)), 0.2);
}

