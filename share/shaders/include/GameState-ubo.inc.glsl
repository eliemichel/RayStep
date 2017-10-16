
layout(std140) uniform GameState {
    vec4 mouse;
    vec4 cameraPosition;
    float cameraZoom;
    float _pad[3];
} game;
