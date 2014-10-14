#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

float volumecone(float radius, float height)
{
    float volume;
    volume = (1.0 / 3) * M_PI * (radius * radius) * height;
    return volume;
}

void rawprint(char *string)
{
    printf("%s", string);
}
