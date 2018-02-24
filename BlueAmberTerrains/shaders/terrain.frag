#version 440 core
#pragma debug(on)
#pragma optimize(off)

smooth in vec4 heightColor;    // height-based color
in vec2 texCoord2;             // texture coordinates

// Presence of the textures (in percent).
in float firstTexturePresence2;
in float secondTexturePresence2;
in float thirdTexturePresence2;

out vec4 fragColor;

//uniform sampler2D uSampler;

layout(binding=0) uniform sampler2D firstTextureSampler;
layout(binding=1) uniform sampler2D secondTextureSampler;
layout(binding=2) uniform sampler2D thirdTextureSampler;

void main()
{
	// Multitexturing (using fixed texture weights).

	vec4 firstTextureColor  = texture(firstTextureSampler, texCoord2);
	vec4 secondTextureColor = texture(secondTextureSampler, texCoord2);
	vec4 thirdTextureColor  = texture(thirdTextureSampler, texCoord2);
	
	// TODO: original code
	//vec4 texColor = firstTextureColor * firstTexturePresence2 + secondTextureColor * secondTexturePresence2 + thirdTextureColor * thirdTexturePresence2;

	vec4 texColor;

	if (firstTexturePresence2 <= 0.0)
	{
		texColor = mix(thirdTextureColor, secondTextureColor, secondTexturePresence2);
	}
	else if (secondTexturePresence2 <= 0.0)
	{
		texColor = mix(firstTextureColor, thirdTextureColor, thirdTexturePresence2);
	}
	else if (thirdTexturePresence2 <= 0.0)
	{
		texColor = mix(firstTextureColor, secondTextureColor, secondTexturePresence2);
	}

	/*
	if (firstTexturePresence2 + secondTexturePresence2 + thirdTexturePresence2 < 1.0)
	{
		if (firstTexturePresence2 <= 0.0)
		{
			texColor = vec4(1.0, 0.0, 0.0, 1.0);
			//texColor = mix(secondTextureColor, thirdTextureColor, thirdTexturePresence2);
		}
		else if (secondTexturePresence2 <= 0.0)
		{
			texColor = vec4(0.0, 1.0, 0.0, 1.0);
			//texColor = mix(firstTextureColor, thirdTextureColor, thirdTexturePresence2);
		}
		else if (thirdTexturePresence2 <= 0.0)
		{
			texColor = vec4(0.0, 0.0, 1.0, 1.0);
			//texColor = mix(firstTextureColor, secondTextureColor, secondTexturePresence2);
		}
	}
	*/
	
	fragColor = texColor;

	//fragColor = clamp(texColor, 0.0, 1.0);
	//fragColor = heightColor * texColor;    // add height-based component

	// One texture.
	//fragColor = texture2D(uSampler, texCoord1.xy);
}
