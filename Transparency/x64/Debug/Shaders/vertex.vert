#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
//layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

//out vec3 ourColor;
out vec2 TexCoord;
out vec3 ourNormal;
out vec3 ourFragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform mat4 transform;

void main()
{
	//ourColor = aColor;
	TexCoord = aTexCoord;
	//Pozycja fragmentu (wspolrzedne w przestrzeni swiata)
	ourFragPos = vec3(model * vec4(aPos, 1.0));
	//ourNormal = aNormal;
	//Macierz normalna 
	ourNormal = mat3(transpose(inverse(model))) * aNormal;  

	gl_Position =  projection * view * model * vec4(aPos, 1.0);
}