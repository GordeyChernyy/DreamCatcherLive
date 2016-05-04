uniform sampler2DRect u_tex_unit0;
uniform float time;
uniform float iteration;
uniform float blursize;
uniform vec2 windowSize;
uniform vec2 mouse;

#define ITER 5
#define SIZE 6.0

void srand(vec2 a, out float r)
{
    r=sin(dot(a,vec2(1233.224,1743.335)));
}

float rand(inout float r)
{
    r=fract(3712.65*tan(r)+sin(r-time/2.)+0.61432);
    return (r-0.5)*2.0;
}

void main( void )
{
    vec2 uv = vec2(gl_FragCoord.x, gl_FragCoord.y) ;
    float p=windowSize.y*(sin(time/2.0)+1.0);
    vec4 c=vec4(0.0);
    float r;
    srand(uv, r);
    vec2 rv;

    for(int i=0;i<int(iteration);i++)
    {
        rv.x=rand(r)/10.;
        rv.y=rand(r);
        c+=texture2DRect(u_tex_unit0, uv-rv*r*blursize)/iteration;
    }
    gl_FragColor = c*1.9;
}
//void srand(vec2 a, out float r)
//{
//    r=sin(dot(a,vec2(1233.224,1743.335)));
//}
//
//float rand(inout float r)
//{
//    r=fract(3712.65*r+0.61432);
//    return (r-0.5)*2.0;
//}
//
//void main( void )
//{
//    
//    vec2 uv = gl_FragCoord.xy;
//    float p=SIZE/windowSize.y*(sin(time/2.0)+1.0);
//    vec4 c=vec4(0.0);
//    float r;
//    srand(uv, r);
//    vec2 rv;
//    
//    for(int i=0;i<ITER;i++)
//    {
//        rv.x=rand(r);
//        rv.y=rand(r);
//        c+=texture2DRect(u_tex_unit0,uv-rv*r*10.)/float(ITER);
//    }
//    gl_FragColor = c;
//}
