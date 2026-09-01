#version 460 core
out vec4 frag_color;

in vec4 TexCoord;

uniform float x_offset = 0.0;
uniform float y_offset = 0.0;
uniform int iterations = 250;
uniform float scale = 2.0;
uniform float color_multiply = 1.0;
 
vec2 ds_set(float a)
{
    vec2 z;
    z.x = a;
    z.y = 0.0;
    return z;
}

vec2 ds_add(vec2 dsa, vec2 dsb)
{
    vec2 dsc;
    float t1, t2, e;

    t1 = dsa.x + dsb.x;
    e = t1 - dsa.x;
    t2 = ((dsb.x - e) + (dsa.x - (t1 - e))) + dsa.y + dsb.y;

    dsc.x = t1+t2;
    dsc.y = t2 - (dsc.x - t1);
    return dsc;
}

vec2 ds_mul(vec2 dsa, vec2 dsb)
{
    vec2 dsc;
    float c11, c21, c2, e, t1, t2;
    float a1, a2, b1, b2, cona, conb, split = 8193.;

    cona = dsa.x * split;
    conb = dsb.x * split;
    a1 = cona - (cona - dsa.x);
    b1 = conb - (conb - dsb.x);
    a2 = dsa.x - a1;
    b2 = dsb.x - b1;

    c11 = dsa.x * dsb.x;
    c21 = a2 * b2 + (a2 * b1 + (a1 * b2 + (a1 * b1 - c11)));

    c2 = dsa.x * dsb.y + dsa.y * dsb.x;

    t1 = c11 + c2;
    e = t1 - c11;

    t2 = dsa.y * dsb.y + ((c2 - e) + (c11 - (t1 - e))) + c21;

    dsc.x = t1 + t2;
    dsc.y = t2 - (dsc.x - t1);
    return dsc;
}

void main()
{
    vec2 z = vec2(0.0);

    vec2 c = vec2(TexCoord.x * scale, TexCoord.y * scale);

    float n = 0.0;

    for (int i = 0; i < iterations; i++)
    {
        z = ((vec2(z.x*z.x-z.y*z.y, 2.0*z.x*z.y) + c) + vec2(x_offset, y_offset));

        if (dot(z, z) > (iterations * iterations)) 
        {
         break;
        }

        n += 1.0;
    }

    float sn;

    sn = n - log2(log2(dot(z,z))) + 4.0;

    float al = smoothstep(-0.1, 0.0, sin(3.1415927));

    float new_sn =  mix( n, sn, al );

    if(n > ((iterations)-1))
    {
      new_sn = 0.0;
    }

    vec3 col = vec3(0.0);

    if( new_sn>0.0 )
    {
      col += 0.5+0.5*cos(0.2 * new_sn + vec3(2.7 * color_multiply,3.2 * color_multiply,3.7 * color_multiply));
    }

    frag_color = vec4( col, 1.0 );
}