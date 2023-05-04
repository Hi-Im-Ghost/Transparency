#version 330 core
out vec4 FragColor;

//in vec3 ourColor;
//in vec2 TexCoord;
in vec3 ourNormal;
in vec3 ourFragPos;

//uniform sampler2D texture1;
//uniform sampler2D texture2;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;  
uniform vec3 viewPos;

void main()
{
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.3);
	
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //wektor kierunku miedzy zrodlem swiatla a pozycja fragmentu
    //normalizacja w celu osiagniecia wektora jednostkowego
    vec3 norm = normalize(ourNormal);
    vec3 lightDir = normalize(lightPos - ourFragPos);  
    //obliczenie wplywu swiatla na biezacy fragment
    //iloczyn skalarny miedzy wektorami (max by nigdy nie bylo ujemne)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;


    //Intensywnosc odblyskow
    float specularStrength = 0.5;
    //Obliczenie wektora kierunku widoku i wektor odbicia wzdloz normalnej
    vec3 viewDir = normalize(viewPos - ourFragPos);
    //negujemy poniewaz oczekujemy ze wektor bedzie wskazywal od zrodla swiatla 
    vec3 reflectDir = reflect(-lightDir, norm);  
    //Obliczanie skladowej zwierciadlanej
    //mnozenie skalara miedzy kierunkiem patrzenia i odbicia i podnoszenie do potegi 32(liczba to oznacza wartosc polysku)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  


    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}