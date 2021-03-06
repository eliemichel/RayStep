#version 450 core

layout (location = 0) out vec4 color;

#include "include/AppInfo-ubo.inc.glsl"
#include "include/GameState-ubo.inc.glsl"
#include "include/Camera-ubo.inc.glsl"

#include "sys:raystep-uniforms"

vec2 iMouse = game.cameraPosition.xy;
uniform sampler2D iChannel1;
#include "include/templated-ray-marcher.inc.glsl"

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution.xy;

    mainImage(color, gl_FragCoord.xy);
}
