#version 460 core
out vec4 frag_color;

uniform float scale = 1.0;
uniform vec2 center = vec2(0.1, 0.7);

in vec4 TexCoord;
 
float IterateMandelbrot()
{
    vec2 floatPosition = vec2(TexCoord.x, TexCoord.y);
 
    float iterations = 0.0;

    vec2 z;
    vec2 result = vec2(0.0);
    vec2 c = vec2((floatPosition.x * 2.0 * scale) + (center.x), (floatPosition.y * 2.0 * scale) + (center.y));
    z = c;

    for (int i = 0; i < 256; i++)
    {         
        result = vec2((z.x * z.x) - (z.y * z.y), (2 * z.x * z.y)) + c;
         
        if (dot(result, result) > (256 * 256))
        break;

        iterations += 1.0;
        z = result;
    }

    if( iterations>511.0 ) return 0.0;

    float sn = iterations - log2(log2(dot(z,z))) + 4.0;
    float al = smoothstep(-0.1, 0.0, sin(3.1415927));
    return mix(iterations, sn, al);
}
 
vec4 return_color()
{
    float iter = IterateMandelbrot();
    vec3 col = vec3(0.0);

    if (iter > 0.0) 
    {
        col += 0.5+0.5*cos(0.2*iter+vec3(2.7,3.2,3.7));
    }

    return vec4(col, 1.0f );

}
 
void main()
{
    frag_color = return_color();
}