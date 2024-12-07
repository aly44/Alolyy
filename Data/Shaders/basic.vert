attribute vec2 a_position;
attribute vec2 a_UV;

uniform vec2 u_objectTranslation;

varying vec2 v_UV;

void main()
{
    // Tilemap will be 20x20 units. We show a bit more vertically.
    // If the window is resized to be less wide than a 1.5 aspect ratio, parts will be cut off.
    // But, atm the resolution is hard-coded to 1280x720.
    vec2 objectScale = vec2( 2.0, 2.0 );
    vec2 cameraPosition = vec2( 10.0, 10.0 );
    float aspectRatio = 1280.0/720.0;
    vec2 zoom = vec2( 1.0/(12.0*(aspectRatio)), 1.0/12.0 );

    vec2 objectSpacePos = a_position;
    vec2 worldSpacePos = objectSpacePos * objectScale + u_objectTranslation;
    vec2 viewSpacePos = worldSpacePos - cameraPosition;
    vec2 clipSpacePos = viewSpacePos * zoom;

    gl_Position = vec4( clipSpacePos, 0, 1 );

    v_UV = a_UV;
}
