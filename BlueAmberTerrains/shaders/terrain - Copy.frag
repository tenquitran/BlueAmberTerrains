#version 440 core
#pragma debug(on)
#pragma optimize(off)

smooth in vec4 heightColor;    // height-based color

out vec4 fragColor;

void main()
{
    // Using height-based color.
	fragColor = heightColor;
}
