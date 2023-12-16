#version 330 core
in vec3 Normal; // Normal in world space
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};

uniform Material material;

struct DirLight {
    vec3 direction; // light source to scene
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewPos) {
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightDir = -normalize(light.direction);
    vec3 halfVec = normalize(viewDir + lightDir);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoords))
        * max(0.0, dot(lightDir, normal));
    vec3 specular = light.specular * vec3(texture(material.specular, TexCoords))
        * pow(max(0.0, dot(normal, halfVec)), material.shininess);
    return ambient + diffuse + specular;
};

struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 lightDir;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
};
uniform SpotLight spotLight;

vec3 CalcSpotLight(SpotLight light) {
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightDir = -normalize(light.lightDir);
    vec3 halfVec = normalize(viewDir + lightDir);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    float theta = dot(lightDir, viewDir);
    vec3 distance = viewPos - FragPos;
    float attenuation = 1.0 / (light.constant + light.linear * length(distance) +
        light.quadratic * dot(distance, distance));
    if (theta < light.cutOff) {
        return attenuation * ambient;
    }
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoords))
        * max(0.0, dot(lightDir, Normal));
    vec3 specular = light.specular * vec3(texture(material.specular, TexCoords))
        * pow(max(0.0, dot(Normal, halfVec)), material.shininess);
    return attenuation * (ambient + diffuse + specular);
}

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewPos) {
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 distance = light.position - fragPos;
    vec3 lightDir = normalize(distance);
    vec3 halfVec = normalize(viewDir + lightDir);
    float attenuation = 1.0 / (light.constant + light.linear * length(distance) +
        light.quadratic * dot(distance, distance));
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoords))
        * max(0.0, dot(lightDir, normal));
    vec3 specular = light.specular * vec3(texture(material.specular, TexCoords))
        * pow(max(0.0, dot(normal, halfVec)), material.shininess);
    return attenuation * (ambient + diffuse + specular);
}


void main() {
    vec3 result = CalcDirLight(dirLight, Normal, viewPos);
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], Normal, FragPos, viewPos);
    result += CalcSpotLight(spotLight); 
    FragColor = vec4(result, 1.0);
}