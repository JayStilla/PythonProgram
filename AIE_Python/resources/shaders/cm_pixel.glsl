#version 330

// very simple pixel shader that just samples a texture

in vec4 vPosition;
in vec4 vNormal;
in vec2 vUV;
in vec4 vColor;
in vec4 vCameraPosition;

out vec4 outColour;

uniform sampler2D diffuseTexture;
uniform sampler2D additiveBlend;
uniform samplerCube cubeMap;

// direction of brightest point in environment map
const vec3 v3LightDir = vec3( -0.13, -0.434, -0.453 );

// lighting parameters -- could pass in as uniform
const float r0 = .201;		// Fresnel reflectance at zero angle
const vec4 ka = vec4( 0.2, 0.2, 0.2, 1.0 ); // ambient Light colour
const vec4 kd = vec4( 0.1, 0.1, 0.1, 0.0 ); //diffuse light colour
const vec4 ks = vec4( 0.2, 0.2, 0.2, 0.0 ); //specular light colour
const float es = 16.0;

void main()
{
	vec4 additive = texture2D( additiveBlend, vUV );
	
	// lighting vectors
	// direction of brightest point in environment map
	vec3 v3LightDir = vec3( -0.13, -0.434, -0.453 );

    vec3 viewDir = normalize(vPosition.xyz - vCameraPosition.xyz);// -view
    vec3 midPointViewToLight = normalize(v3LightDir-vCameraPosition.xyz);	// half way between view & light

	 // color
    float diffuseColor	= max(0.0,dot(vNormal.xyz,v3LightDir));
    float specularColor = pow(max(0.0,dot(vNormal.xyz,midPointViewToLight)),es);
    vec4 col			= ka * (kd*diffuseColor + ks*specularColor);// * additive;

    vec3 v3Reflection = reflect(viewDir,vNormal.xyz);
    vec3 RH = normalize(v3Reflection-viewDir);
    float fresnel = r0 + (1.0-r0) * pow(1.0 + dot(viewDir, RH),5.0);
    vec4 env = texture(cubeMap, 0.5 + 0.5 * normalize( v3Reflection ).xyz );

	
	//additive.a = 0.0;

    outColour = mix(col ,env ,fresnel);
	//outColour.a = 1.0;
    //outColour = vec4( 1.0, 1.0, 0.0, 1.0) * texture2D(diffuseTexture, vUV);
}
