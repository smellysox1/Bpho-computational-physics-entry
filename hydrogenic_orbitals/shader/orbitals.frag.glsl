#version 330 core
out vec4 colour;

in vec3 fragPos;

uniform int n, l, m, A, Z;
uniform float an, maxPD;

const float pi = 3.1415926535898;

vec3 PDToColour(float probabilityDensity) {
    float hue = 6.0 - 6.0 * max(probabilityDensity, 0.0);

    vec3 xt = vec3(
        hue - 4.0,
        0.0,
        6.0 - hue
    );

    if (hue < 4.0) {
        xt.r = 0.0;
        xt.g = 4.0 - hue;
        xt.b = hue - 2.0;
    }

    if (hue < 2.0) {
        xt.r = 2.0 - hue;
        xt.g = hue;
        xt.b = 0.0;
    }

    xt = min(xt, 1.0);

    return xt * 2.0;
}

int factorial(int n) {
    int product = 1;

    for (int i = 2; i <= n; i++)
        product *= i;

    return product;
}

// optimised for gpu, as factorials are very slow
float laguerre(float x) {
    int p = n - l - 1;

    if (p <= 0)
        return 1.0;

    int q = 2 * l;

    if (p == 1)
        return float(q + 2) - x;

    float total2 = 1.0;
    float total1 = float(q + 2) - x;
    float total = 0.0;

    for (int k = 2; k <= p; k++) {
        total = (total1 * float(2 * k + q) - x - total2 * float(k + q)) / float(k);
        total2 = total1;
        total1 = total;
    }

    return total;
}

float poly_legendre_p(float x) {
    if (x == 1.0)
        return 1.0;

    if (x == -1.0)
        return ((l % 2 == 1) ? -1.0 : 1.0);

    float p_lm2 = 1.0;

    if (l == 0)
        return p_lm2;
    if (l == 1)
        return x;

    float p_lm1 = x;

    float p_l = 0.0;

    for (int ll = 2; ll <= l; ll++) {
        p_l = 2.0 * x * p_lm1 - p_lm2 - (x * p_lm1 - p_lm2) / float(ll);
        p_lm2 = p_lm1;
        p_lm1 = p_l;
    }

    return p_l;
}

float P(int M, float x) {
    if (M > l)
        return 0.0;

    if (M == 0)
        return poly_legendre_p(x);

    float p_mm = 1.0;

    if (M > 0) {
        float root = sqrt(1.0 - x) * sqrt(1.0 + x);
        float fact = 1.0;

        for (int i = 1; i <= M; i++) {
            p_mm *= fact * root;
            fact += 2.0;
        }
    }

    if (l == M)
        return p_mm;

    float p_mp1m = float(2 * M + 1) * x * p_mm;

    if (l == M + 1)
        return p_mp1m;

    float p_lm2m = p_mm;
    float P_lm1m = p_mp1m;
    float p_lm = 0.0;

    for (int j = M + 2; j <= l; j++) {
        p_lm = (float(2 * j - 1) * x * P_lm1m - float(j + M - 1) * p_lm2m) / float(j - M);
        p_lm2m = P_lm1m;
        P_lm1m = p_lm;
    }

    return p_lm;
}

vec2 Y(int M, float azimuth, float zenith) {
    int y1 = ((M % 2 == 1) ? -1 : 1);
    int y2 = 2 * l + 1;
    int y3 = factorial(l - M);
    int y4 = factorial(l + M);
    float y5 = P(abs(M), cos(azimuth));

    float y = float(y1) * sqrt(float(y2 * y3) / (float(y4) * 4.0 * pi)) * y5;

    float arg = zenith * float(M);

    return y * vec2(cos(arg), sin(arg));
}

void main() {
    float r = length(fragPos);
    float azimuth = atan(fragPos.y, fragPos.x);
    float zenith = acos(fragPos.z / r);

    float x = 2.0 * r / an;

    float r1 = sqrt(float(factorial(n - l - 1)) / float(2 * n * factorial(n + l)));
    float r2 = pow(2.0 / an, 1.5);
    float r3 = pow(x, l);
    float r4 = exp(x * -0.5);
    float r5 = laguerre(x);

    float R = r1 * r2 * r3 * r4 * r5;

    vec2 omega;

    if (m < 0)
        omega = Y(-m, azimuth, zenith) - Y(m, azimuth, zenith);
    else if (m == 0)
        omega = Y(0, azimuth, zenith);
    else
        omega = Y(m, azimuth, zenith) + Y(-m, azimuth, zenith);

    vec2 psi = R * omega;
    float abspsi = length(psi);

    float pd = abspsi * abspsi / maxPD;

    colour = vec4(PDToColour(pd), 0.05 * pd);
}
