#version 440 core
#pragma debug(on)
#pragma optimize(off)

layout(location = 0) in vec3 vPosition;
layout(location = 1) in float vColor;     // height-based color
//layout(location = 2) in vec3 vNormal;

uniform mat4 mvp;

smooth out vec4 heightColor;    // height-based color converted to RGBA

void main()
{
	gl_Position = mvp * vec4(vPosition, 1.0);

	heightColor = vec4(vColor, vColor, vColor, 1.0);
}
