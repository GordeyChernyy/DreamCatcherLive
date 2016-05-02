#version 120
varying vec3  normal;
uniform sampler2DRect u_tex_unit0;
void main( void )
{
    vec4 color = gl_Color + vec4(0, 0, 0, normalize(normal)/4-0.4);
    // voila, we can use our flat normal! This will colour our fragment with the current normal for debug purposes.
    gl_FragColor = color;
}