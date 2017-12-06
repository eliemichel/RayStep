#version 450 core

in vec3 vColor;

layout (location = 0) out vec4 color;

#include "include/AppInfo-ubo.inc.glsl"
#include "include/GameState-ubo.inc.glsl"
#include "include/Camera-ubo.inc.glsl"

void main() {
	color = vec4(vColor, 1.0);
}
