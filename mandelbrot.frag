#version 460 core
out vec4 frag_color;

in vec4 TexCoord;

uniform float x_offset = 0.0;
uniform float y_offset = 0.0;
uniform int iterations = 250;
uniform double scale = 2.000;
uniform float color_multiply = 1.0;
 
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