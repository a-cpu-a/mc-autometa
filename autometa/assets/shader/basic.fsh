#version 330 core
layout(location=0) out vec4 color;

uniform sampler2D textureId;

in vec2 pos;


void main()
{
	vec2 uv = (pos.xy+1.0) / 2.0;
	color = texture(textureId,vec2(uv.x,1.0-uv.y));
}