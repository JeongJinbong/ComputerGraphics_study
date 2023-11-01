#version 330 core

//--- in_Position: attribute index 0
//--- in_Color: attribute index 1

layout (location = 0) in vec3 vPos; //--- 위치 변수: attribute position 0
layout (location = 1 ) in vec3 vColor;

uniform mat4 Transform;
uniform mat4 projection;
uniform mat4 viewTransform;

out vec3 color;

void main()
{
	gl_Position  = projection * viewTransform * Transform * vec4(vPos, 1.0);

	color= vColor;
}
