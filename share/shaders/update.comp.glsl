#version 450 core
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

#include "include/AppInfo-ubo.inc.glsl"
#include "include/GameState-ssbo.inc.glsl"

uniform float speed = 1.0;

void main() {
    float speedModifier = 1.0 / (0.5 + game.cameraZoom);
    float actualSpeed = speed * speedModifier;

    if (game.mouse.x < 100) {
        game.cameraPosition.x -= actualSpeed;
    } else if (game.mouse.x > iResolution.x - 100) {
        game.cameraPosition.x += actualSpeed;
    }

    if (game.mouse.y < 100) {
        game.cameraPosition.y -= actualSpeed;
    } else if (game.mouse.y > iResolution.y - 100) {
        game.cameraPosition.y += actualSpeed;
    }

    game.cameraZoom = clamp(game.cameraZoom + game.mouse.w * 0.1, 0.3, 2.0);
    game.mouse.zw = vec2(0.0);
}
