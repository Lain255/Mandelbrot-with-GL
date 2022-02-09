#version 330 core

uniform View {
    vec2 view_scale;
    vec2 view_offset;
    int resolution;
};

in vec2 coords;

out vec4 out_color;

int mandelBrot(float a, float b, int n)
{
    float x = 0.0f;
    float y = 0.0f;
    float temp;

    for (int i = 1; i < n; i++) {
        temp = x;
        x = x*x - y*y + a;
        y = 2.0f * temp * y + b;
        if (4.0f < x*x + y*y)
            return i;
    }
    return n;
}

void main() {
    int maxIter = resolution;
    int numIter = mandelBrot(coords.x, coords.y, maxIter);
    int inSet = (numIter == maxIter) ? 1 : 0;
    float value = float(numIter)/float(maxIter);

    float r = value*value*value;
    float g = float(inSet);
    float b = value;

    out_color = vec4(r, g, b, 1.0f);
}