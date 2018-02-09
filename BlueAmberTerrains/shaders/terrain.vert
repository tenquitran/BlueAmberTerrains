#version 440 core
#pragma debug(on)
#pragma optimize(off)

layout(location = 0) in vec4 vPosition;
layout(location = 1) in float vColor;     // height-based color

uniform mat4 mvp;

smooth out vec4 heightColor;    // height-based color converted to RGBA

void main()
{
	gl_Position = mvp * vPosition;

	heightColor = vec4(vColor, vColor, vColor, 1.0);
}
