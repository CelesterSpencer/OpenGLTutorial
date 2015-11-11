#version 330

layout (location = 0) in vec3 position;
uniform mat4 gWorld;

out vec4 color;

void main() {
    gl_Position = gWorld * vec4(position, 1.0);
    color = vec4(clamp(position, 0.0, 1.0), 1.0);
}
