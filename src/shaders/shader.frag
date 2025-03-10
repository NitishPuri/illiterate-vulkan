# Fragment shader code in GLSL for Vulkan
# This shader processes pixel data and outputs color values.

#version 450

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(1.0, 0.0, 0.0, 1.0); // Output red color
}