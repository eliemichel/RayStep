#version 450 core

layout (location = 0) out vec4 color;

#include "include/AppInfo-ubo.inc.glsl"
#include "include/GameState-ubo.inc.glsl"

vec2 iMouse = game.cameraPosition;
uniform sampler2D iChannel1;
#include "include/map-generator.inc.glsl"

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution.xy;

    color = vec4(uv, 0.0, 1.0);

    if (length(gl_FragCoord.xy - game.cameraPosition.xy) < 5.0) {
        color = vec4(0.0, 0.0, 1.0, 1.0);
    }

    mainImage(color, gl_FragCoord.xy);
}
