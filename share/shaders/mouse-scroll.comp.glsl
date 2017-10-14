#version 450 core
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

uniform vec2 scroll;

#include "include/AppInfo-ubo.inc.glsl"
#include "include/GameState-ssbo.inc.glsl"

void main() {
    game.mouse.zw = scroll.xy;
}
