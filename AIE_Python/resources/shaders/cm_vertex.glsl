#version 330

// very simple vertex shader that just transforms an object-space
// position into clip space and passes along a texture coordinate

in vec4 Position;
in vec4 Normal;
in vec2 UV;
in vec4 Color;

out vec4 vPosition;
out vec4 vNormal;
out vec2 vUV;
out vec4 vColor;


uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

uniform vec4 CamPos;
uniform float Time;
out vec4 vCameraPosition;

void main()
{
	vPosition	= Position;
	vNormal		= Normal;
	vUV			= UV;
	vColor		= Color;

	vCameraPosition = CamPos;

	gl_Position = Projection * View * Model * (Position + (Normal * sin(Time) ));
}
