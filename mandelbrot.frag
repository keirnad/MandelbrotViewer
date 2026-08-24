#version 460 core
in vec4 gl_FragCoord;
 
out vec4 frag_color;
 
float IterateMandelbrot()
{
    float real = (gl_FragCoord.x / 800.0 - 0.6) * 4.0;
    float imag = (gl_FragCoord.y / 800.0 - 0.5) * 4.0;
 
    float const_real = real;
    float const_imag = imag;

    float iterations = 0.0;
    vec2 z  = vec2(0.0);

    for (int i = 0; i < 256; i++)
    {
        float tmp_real = real;
        real = (real * real - imag * imag) + const_real;
        imag = (2.0 * tmp_real * imag) + const_imag;
         
        vec2 z = vec2(real, imag);
         
        if (dot(z,z) > 4.0)
        break;

        iterations += 1.0;
    }

    float sn = iterations - log2(log2(dot(z,z))) + 4.0;
    return sn;
}
 
vec4 return_color()
{
    float iter = IterateMandelbrot();
    vec3 col = vec3(0.0);

    if (iter > 0.0) 
    {
        col += 0.5+0.5*cos(0.2*(iter * 0.5)+vec3(2.7,3.2,3.7));
    }

    return vec4(col, 1.0f );
}
 
void main()
{
    frag_color = return_color();
}