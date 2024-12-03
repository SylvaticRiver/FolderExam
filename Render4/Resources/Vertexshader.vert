#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;
layout (location = 4) in float aFriction;

out vec3 crntPos;
out vec3 Normal;
out vec3 ourColor;
out vec2 texCoord;
out float friction;

uniform mat4 camMatrix;
uniform mat4 model;


void main()
{
	crntPos = vec3(model * vec4(aPos, 1.0f));
	Normal = aNormal;
	ourColor = aColor;
	texCoord = aTex;
	friction = aFriction;
	
	gl_Position = camMatrix * model * vec4(aPos, 1.0f);
}