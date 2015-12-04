#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

out vec4 fragColor;

struct Attenuation{
    float constant;
    float linear;
    float exponential;
};

struct PointLight{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
    vec3 position;
    Attenuation attenuation;
};

uniform PointLight gPointLight;
uniform sampler2D gSampler;
uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;

vec4 calcLightInternal(PointLight light, vec3 lightDirection, vec3 normal) {
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
    float diffuseFactor = dot(normal, -lightDirection);
    vec4 diffuseColor = vec4(0,0,0,0);
    vec4 specularColor = vec4(0,0,0,0);

    if (diffuseFactor > 0) {
        diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);
        vec3 vertexToEye = normalize(gEyeWorldPos - worldPos0);
        vec3 lightReflect = normalize(reflect(lightDirection, normal));
        float specularFactor = dot(vertexToEye, -lightReflect);
        if (specularFactor > 0) {
            specularFactor = pow(specularFactor, gSpecularPower);
            specularColor = vec4(light.color * gMatSpecularIntensity * specularFactor, 1.0f);
        }
    }

    return (ambientColor + diffuseColor + specularColor);
}

vec4 calcPointLight(vec3 normal) {
    vec3 lightDirection = worldPos0 - gPointLight.position;
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    vec4 color = calcLightInternal(gPointLight, lightDirection, normal);
    float attenuation = gPointLight.attenuation.constant +
                        gPointLight.attenuation.linear * distance +
                        gPointLight.attenuation.exponential * distance * distance;

    return color / attenuation;
}

void main() {

    vec3 normal = normalize(normal0);
    vec4 totalLight = calcPointLight(normal);

	fragColor = texture2D(gSampler, texCoord0.xy) * totalLight;

}
