#version 330


in vec3 fragNormal;
in vec2 fragTexCoord;
in vec3 fragPos;
in vec3 fragViewPos;

uniform sampler2D texSampler;

out vec4 outColor;

struct DirLight {
    vec3 direction;
	vec3 color;

    float ambient;
    float diffuse;
    float specular;
};

#define SHINYNESS 8

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
     float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), SHINYNESS);
    // combine results
    float ambient  = light.ambient; //* vec3(texture(texture1, TexCoords));
    float diffuse  = light.diffuse  * diff; //* vec3(texture(texture1, TexCoords));
    float specular = light.specular * spec; //* vec3(texture(texture1, TexCoords));
    return (ambient + diffuse + specular) * light.color;
}

void main() {
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(fragViewPos - fragPos);

    DirLight light;
    light.direction = vec3(0.2, -0.5, 0.5);
    light.color = vec3(1, 1, 1);
    light.ambient = 0.2;
    light.diffuse = 0.5;
    light.specular = 0.7;

    vec3 result = CalcDirLight(light, norm, viewDir);

    outColor =  vec4(result, 1.0);
}