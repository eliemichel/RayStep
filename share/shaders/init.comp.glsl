#version 450 core
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

#include "include/AppInfo-ubo.inc.glsl"
#include "include/GameState-ssbo.inc.glsl"

void main() {
    game.cameraZoom = 1.8;
    game.cameraPosition = vec2(350.0, 2050.0);
}
