
layout(std140) buffer GameState {
	vec4 mouse;
    vec2 cameraPosition;
    float cameraZoom;
    float _pad;
} game;
