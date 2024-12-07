varying vec2 v_UV;

uniform sampler2D u_texture;

void main()
{
	vec4 color = texture2D( u_texture, v_UV );
	gl_FragColor = color;
}
