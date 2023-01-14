#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform bool is_plan;

varying vec2 v_texcoord;

//! [0]
void main()
{
    // Set fragment color from texture
	if (is_plan)
    gl_FragColor = texture2D(texture, v_texcoord);
	else
	{
		float c = 0.5;
		gl_FragColor =vec4(c, c, c, 0.);
	}
}
//! [0]

