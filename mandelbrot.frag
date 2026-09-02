#version 460 core
out vec4 frag_color;

in vec4 TexCoord;

uniform float x_offset = 0.0; // Offset on X coordinates -
uniform float y_offset = 0.0; // Offset on Y coordinates |
uniform int iterations = 250; // Iterations for mandelbrot set
uniform float scale = 2.0; // Scaling
uniform float color_multiply = 1.0; // Var for changing colors (just multiplying values with this var)


float mandelbrotIter(vec2 c) // Mandelbrot iterations calculation
{
    vec2 z = vec2(0.0); // Empty vec2 var for main formula 
    float n = 0.0; // We need this variable to count iterations and calculate speed of entering into set

    for (int i = 0; i < iterations; i++) // Main loop
    {
        z = ((vec2(z.x*z.x-z.y*z.y, 2.0*z.x*z.y) + c) + vec2(x_offset, y_offset)); // Main formula https://en.wikipedia.org/wiki/Mandelbrot_set plus my vector with offsets

        if (dot(z, z) > (iterations * iterations))
        {
         break;
        }

        if (n > (iterations - 2)) // If n is > then iterations then it is in the set so color it with black color in main function
        {
            return 0.0;
        }

        n += 1.0; // iteration count
    }

    // Code for smooth iterations you can read more about it here: https://linas.org/art-gallery/escape/escape.html, https://iquilezles.org/articles/msetsmooth/, https://www.shadertoy.com/view/4df3Rn

    float sn = n - log2(log2(dot(z,z))) + 4.0;

    float al = smoothstep(-0.1, 0.0, sin(3.1415927));

    return mix(n, sn, al);
}

void main()
{
    vec2 c = vec2(TexCoord.x * scale, TexCoord.y * scale); // Calculate scale with our coordinates of pixel
    
    float iter = mandelbrotIter(c); // call function to calculate iterations

    vec3 col = vec3(0.0);

    if( iter>0.0 )
    {
      col += 0.5+0.5*cos(0.2 * iter + vec3(2.7 * color_multiply,3.2 * color_multiply,3.7 * color_multiply)); // Color palletes https://iquilezles.org/articles/palettes/, or you can see it here too https://www.shadertoy.com/view/4df3Rn
    }

    frag_color = vec4( col, 1.0 ); // Coloring our pixel
}