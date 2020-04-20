#shader Vertex
#version 440

layout(location = 0) in vec4 position; //Use IN in case of Input

uniform vec4 u_Position;

void main()
{
	//position = u_Position;
	gl_Position = position;
}

#shader Fragment
#version 440

layout(location = 0) out vec4 color; //Use OUT in case of Output

uniform vec4 u_Color;

void main()
{
	color = u_Color;
}