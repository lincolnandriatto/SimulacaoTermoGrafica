#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 Normal;
in vec3 FragPos;
uniform vec3 lightPos;
uniform vec3 lightPos1;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;
uniform bool Wiremode;

void main()
{
    //ambient
    float ambientStrength = 0.1;
    float diffuseStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

// diffuse
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);
float diff = max(dot(norm, lightDir), 0.0);
vec3 lightDir1 = normalize(lightPos1 - FragPos);
float diff1 = max(dot(norm, lightDir1), 0.0);
vec3 diffuse = diffuseStrength*(diff+diff1) * lightColor;

// specular
float specularStrength = 0.2;
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
vec3 reflectDir1 = reflect(-lightDir1, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
float spec1 = pow(max(dot(viewDir, reflectDir1), 0.0), 16);
vec3 specular = specularStrength * (spec+spec1) * lightColor;

vec3 color;
if(Wiremode==false)
  color =ourColor;
  else
  color = objectColor;

vec3 result = (ambient + diffuse + specular) * color;
FragColor = vec4(result, 1.0);
}
