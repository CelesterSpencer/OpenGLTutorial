#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

struct PointLight {
    vec3 position;
    vec3 color;
};

struct BasicMaterial {
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
    float specularPower;
};

uniform sampler2D gSampler;
uniform PointLight gPointLight;
uniform BasicMaterial gBasicMaterial;
uniform vec3 gEye;

out vec4 fragColor;

vec4 calcLightInternal(PointLight light, vec3 lightDirection, BasicMaterial material, vec3 normal) {
    vec4 ambientColor = vec4(light.color, 1.0f) * material.ambientIntensity;
    float diffuseFactor = dot(normal, -lightDirection);
    vec4 diffuseColor = vec4(0,0,0,0);
    vec4 specularColor = vec4(0,0,0,0);

    if (diffuseFactor > 0) {
        diffuseColor = vec4(light.color * material.diffuseIntensity * diffuseFactor, 1.0f);
        vec3 vertexToEye = normalize(gEye - worldPos0);
        vec3 lightReflect = normalize(reflect(lightDirection, normal));
        float specularFactor = dot(vertexToEye, lightReflect);
        if (specularFactor > 0) {
            specularFactor = pow(specularFactor, material.specularPower);
            specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    return (ambientColor + diffuseColor + specularColor);
}

vec4 calcPointLight(vec3 normal) {
    vec3 lightDirection = worldPos0 - gPointLight.position;
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    vec4 color = calcLightInternal(gPointLight, lightDirection, gBasicMaterial, normal);

    vec3 invEyeDir = normalize(gEye - worldPos0);
    float rimFactor = dot(invEyeDir, normal);
    rimFactor = rimFactor * 1.5f;
    rimFactor = 1 - rimFactor;
    vec4 rimColor = vec4(1,1,1,1) * rimFactor;

    // hard code attenuation of the light
    float attenuation = 0 + 0.1 * distance + 0.3 * distance * distance;

    return (color / attenuation) + clamp(rimColor, 0.0f, 1.0f);
}

void main() {
    vec4 lightColor = calcPointLight(normal0);
    //vec4 textureColor = texture2D(gSampler, texCoord0.xy);
    vec4 textureColor = vec4(0,0,1,0.5);
    fragColor = textureColor * lightColor;
}
