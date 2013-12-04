#version 420 core

in vec2 tc;

out vec4 color;

uniform float time;

float rand_float(float seed)
{
    uint useed = floatBitsToUint(seed);

    uint t = useed;

    t = t * 7342643;
    t = (t << 6) ^ (t >> 4);
    t = t * 7823143;
    t = (t << 5) ^ (t >> 3);

    useed = 0x3F800000 + (t & 0x7FFFFF);

    return uintBitsToFloat(useed) - 1.0;
}

float curve(float f)
{
    float t2 = f * f;
    float t3 = t2 * f;

    return t3 * t2 * 6.0 - 15.0 * t2 * t2 + 10.0 * t3;
}

float snoise2(vec2 p)
{
    vec2 pi = floor(p);
    vec2 pf = fract(p);

    vec2 g00 = normalize(vec2(rand_float(pi.x), rand_float(pi.y)));
    vec2 g10 = normalize(vec2(rand_float(pi.x + 1.0), rand_float(pi.y)));
    vec2 g01 = normalize(vec2(rand_float(pi.x), rand_float(pi.y + 1.0)));
    vec2 g11 = normalize(vec2(rand_float(pi.x + 1.0), rand_float(pi.y + 1.0)));

    float n00 = dot(g00, pf);
    float n10 = dot(g10, pf - vec2(1.0, 0.0));
    float n01 = dot(g01, pf - vec2(0.0, 1.0));
    float n11 = dot(g11, pf - vec2(1.0, 1.0));

    float fu = curve(pf.x);
    float fv = curve(pf.y);

    float nx0 = n00 * (1.0 - fu) + n10 * fu;
    float nx1 = n01 * (1.0 - fu) + n11 * fu;
    float nxy = nx0 * (1.0 - fv) + nx1 * fv;

    return nxy;
}

vec3 mod289(vec3 x)
{
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x)
{
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x)
{
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
{
    const vec4 C = vec4(0.211324865405187, // (3.0-sqrt(3.0))/6.0
                        0.366025403784439, // 0.5*(sqrt(3.0)-1.0)
                       -0.577350269189626, // -1.0 + 2.0 * C.x
                        0.024390243902439); // 1.0 / 41.0
    // First corner
    vec2 i = floor(v + dot(v, C.yy) );
    vec2 x0 = v - i + dot(i, C.xx);

    // Other corners
    vec2 i1;
    //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
    //i1.y = 1.0 - i1.x;
    i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    // x0 = x0 - 0.0 + 0.0 * C.xx ;
    // x1 = x0 - i1 + 1.0 * C.xx ;
    // x2 = x0 - 1.0 + 2.0 * C.xx ;
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;

    // Permutations
    i = mod289(i); // Avoid truncation effects in permutation
    vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 )) +
                      i.x + vec3(0.0, i1.x, 1.0 ));

    vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
    m = m*m ;
    m = m*m ;

    // Gradients: 41 points uniformly over a line, mapped onto a diamond.
    // The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;

    // Normalise gradients implicitly by scaling m
    // Approximation of: m *= inversesqrt( a0*a0 + h*h );
    m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

    // Compute final noise value at P
    vec3 g;
    g.x = a0.x * x0.x + h.x * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0 * dot(m, g);
}

float fbm(vec2 p, int octaves)
{
    float f = 0.0;

    const mat2 m = mat2(0.8, 0.6, -0.6, 0.8);
    float a = 0.5;
    float t = 1.0;

    int i;

    for (i = 0; i < octaves; i++)
    {
        f += a * snoise(p);
        p *= 2.02;
        t *= a;
        a *= 0.5;
    }

    return f / (1.0 - t);
}

void main(void)
{
    vec2 p = tc; // 8.0 * tc * (1.2 + sin(time * 1000.0));

    float r = length(p);
    float a = atan(p.y, p.x);

    vec3 rgb;

    // float f = rand_float(p.x + p.y * 290 + time * 100.0);
    float f = fbm(vec2(r / (a + 1.0), r * a), 8); // 1.0 * a * 3.14159267), 4);

    rgb = vec3(f);

    color = vec4(rgb * 0.5 + vec3(0.5), 1.0);
}
