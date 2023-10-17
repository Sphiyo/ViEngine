// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;
out vec3 fragPosition;													// test

void main()
{
	//fragPosition = vec3(u_Transform * vec4(a_Position, 1.0));					// test
    //gl_Position = u_ViewProjection * vec4(fragPosition, 1.0);			// test
	
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position,1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
uniform vec4 u_Color; 												    

uniform float u_TilingFactor;
uniform sampler2D u_Texture;

//in vec3 fragPosition;													// test

//uniform float maxDistance;					     						// test




void main()
{

    
	//float distance = length(fragPosition - cameraPos);
    //float brightness = 1.0 - clamp(distance / maxDistance, 0.0, 1.0);
    //color = vec4(brightness, brightness, brightness, 1.0);


	 color = texture(u_Texture, v_TexCoord * u_TilingFactor) * u_Color;
}