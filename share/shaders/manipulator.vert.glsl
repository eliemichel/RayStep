#version 450 core

layout (location = 0) in vec3 position;
layout (location = 0) in vec3 color;

out vec3 vColor;

#include "include/AppInfo-ubo.inc.glsl"
#include "include/GameState-ubo.inc.glsl"
#include "include/Camera-ubo.inc.glsl"


void main() {
	gl_Position = camera.perspectiveMatrix * camera.viewMatrix * vec4(position, 1.0);
	vColor = color;
}
