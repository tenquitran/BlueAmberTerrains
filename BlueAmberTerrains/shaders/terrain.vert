#version 440 core
#pragma debug(on)
#pragma optimize(off)

layout(location = 0) in vec4 vPosition;
layout(location = 1) in float vColor;     // height-based color
layout(location = 2) in vec2 texCoord;

// Presence of the textures (in percent).
layout(location = 3) in float firstTexturePresence;
layout(location = 4) in float secondTexturePresence;
layout(location = 5) in float thirdTexturePresence;

uniform mat4 mvp;

smooth out vec4 heightColor;    // height-based color converted to RGBA

out vec2 texCoord2;

// Presence of the textures (in percent).
out float firstTexturePresence2;
out float secondTexturePresence2;
out float thirdTexturePresence2;

void main()
{
	gl_Position = mvp * vPosition;

	heightColor = vec4(vColor, vColor, vColor, 1.0);

	texCoord2 = texCoord;

	// 0.5, 0.5, 0.0
	/*
	firstTexturePresence2  = 0.02;// firstTexturePresence;
	secondTexturePresence2 = 0.0;//secondTexturePresence;
	thirdTexturePresence2  = 0.98;//thirdTexturePresence;
	*/

	firstTexturePresence2  = firstTexturePresence;
	secondTexturePresence2 = secondTexturePresence;
	thirdTexturePresence2  = thirdTexturePresence;
}
