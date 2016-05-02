varying vec2 texCoordVarying;
varying vec3  normal;
uniform float vol;
uniform float s1;
uniform float s2;
uniform float s3;
uniform float s4;
uniform float s5;
uniform float s6;
uniform float s7;
uniform float s8;
uniform float s9;
uniform float s10;
uniform float time;
uniform vec2 m;
float sine(float a, float b, float c){
    return sin(a*0.2*-time)*0.2;
}
float noise(vec3 p) //Thx to Las^Mercury
{
        vec3 i = floor(p);
        vec4 a = dot(i, vec3(1., 57., 21.)) + vec4(0., 57., 21., 78.);
        vec3 f = cos((p-i)*acos(-1.))*(-.5)+.5;
        a = mix(sin(cos(a)*a),sin(cos(1.+a)*(1.+a)), f.x);
        a.xy = mix(a.xz, a.yw, f.y);
        return mix(a.x, a.y, f.z);
}

void main(void)
{
    
    float x = gl_Vertex.x;
    float y = gl_Vertex.y;
    float z = gl_Vertex.z;
    gl_FrontColor = gl_Color;
    vec4 a = vec4(x+noise(vec3(y*0.02+time, x*0.02, z*0.02-time))*900.0*s2*vol,
                  y+noise(vec3(z*0.02-time, z*0.02-time, y*0.02-time))*900.0*s3*vol,
                  z,
                  1.0
                  );
    vec4 b = vec4(x,
                  y,
                  z,
                  1.0
    
                );
    
    normal = gl_NormalMatrix * vec3( noise(vec3(x*1.5*s4+time, y*0.8*s5, time))*480.0*s6,
                                    cos(x*0.02*s7-y*0.6*s8-time),
                                    sin(x*0.2*s7-y*1.6*s8-time)*200.02*s9)*s10*0.02;
//    gl_Position =  a*0.2+ftransform();
    gl_Position = a*0.02+ftransform();

}
