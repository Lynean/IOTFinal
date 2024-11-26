/*
 * fire.c
 *
 *  Created on: Nov 12, 2024
 *      Author: Admin
 */
#include "ws2812.h"
#include <math.h>
//The variation in yellow color to create the fire effect, define the interval where the color can change.
#define MIN_VARIATION 0
#define MAX_VARIATION 100

//Value must be between 0 & 1.
//If you never want a LED to be completly off, put 0.1 to min
#define MIN_INTENSITY 0.1
#define MAX_INTENSITY 1.0

//Speed for variations, higher is slower
#define NOISE_SPEED_COLOR 5
#define NOISE_SPEED_INTENSITY 3

typedef enum{
	Manual,
	Rainbow,
	Storm,
	Fire,
	Aurora,
	Party,
	Warning,
	Off
}ColorMode_t;

double n;
double ni;

const uint8_t RED = 255;
unsigned int lastTime = 0;

extern WSColor_t currentStruct_Indexes[60];
extern uint8_t currentAnim;
extern uint8_t isControllingAll;

typedef struct {
    float x, y;
} vector2;

float QuadraticEaseInOut(float p);
float CubicEaseInOut(float p);
float SineEaseOut(float p);
vector2 randomGradient(int ix, int iy);
float perlin(float x, float y);
float interpolate(float a0, float a1, float w);
float dotGridGradient(int ix, int iy, float x, float y);
float perlin(float x, float y);

void SetColor_Fire() {
	static uint32_t time = 10000;
	isControllingAll = 0;
	currentAnim = Fire;
	time += 41;

	for (int i = 0; i < LED_NUM; i++) {
		//adjust the mult and divide to change the global effect
		// will be added to advanced settings later
		n = (perlin((float)i*100 , (time+i)/(float)NOISE_SPEED_COLOR)/2 + 0.5);

		ni = (perlin((float)i*100, (time+i)/(float)NOISE_SPEED_INTENSITY)/2 + 0.5);

		//You can change the easing function here
		//Used to avoid a linear effect and give a more natural curve.
//		float v = QuadraticEaseInOut(n/255);
//		float vi = QuadraticEaseInOut(ni/255);
//
		n = (MAX_INTENSITY - MIN_INTENSITY) * n + MIN_INTENSITY;
		float red = (RED*n);
		float yellow = ni *((MAX_VARIATION - MIN_VARIATION)*n + MIN_VARIATION);
		currentStruct_Indexes[i].red.byte = (uint8_t)red;
		currentStruct_Indexes[i].green.byte = (uint8_t)yellow;
		currentStruct_Indexes[i].blue.byte = (uint8_t)0;
		WS2812SetColor_Index(i, currentStruct_Indexes[i]);
  }
}

/* Function to linearly interpolate between a0 and a1
 * Weight w should be in the range [0.0, 1.0]
 */
float interpolate(float a0, float a1, float w) {
    /* // You may want clamping by inserting:
     * if (0.0 > w) return a0;
     * if (1.0 < w) return a1;
     */
    return (a1 - a0) * w + a0;
	//return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
    /* // Use this cubic interpolation [[Smoothstep]] instead, for a smooth appearance:
     * return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
     *
     * // Use [[Smootherstep]] for an even smoother result with a second derivative equal to zero on boundaries:
     * return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
     */
}

/* Create pseudorandom direction vector
 */
vector2 randomGradient(int ix, int iy) {
    // No precomputed gradients mean this works for any number of grid coordinates
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2; // rotation width
    unsigned a = ix, b = iy;
    a *= 3284157443;
    b ^= a << s | a >> (w-s);
    b *= 1911520717;
    a ^= b << s | b >> (w-s);
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
    vector2 v;
    v.x = cos(random);
    v.y = sin(random);
    return v;
}

// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y) {
    // Get gradient from integer coordinates
    vector2 gradient = randomGradient(ix, iy);

    // Compute the distance vector
    float dx = x - (float)ix;
    float dy = y - (float)iy;

    // Compute the dot-product
    return (dx*gradient.x + dy*gradient.y);
}

// Compute Perlin noise at coordinates x, y
float perlin(float x, float y) {
    // Determine grid cell coordinates
    int x0 = (int)floor(x);
    int x1 = x0 + 1;
    int y0 = (int)floor(y);
    int y1 = y0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    // Interpolate between grid point gradients
    float n0, n1, ix0, ix1, value;

    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = interpolate(n0, n1, sx);

    value = interpolate(ix0, ix1, sy);
    return value; // Will return in range -1 to 1. To make it in range 0 to 1, multiply by 0.5 and add 0.5
}

float CubicEaseInOut(float p)
{
	if (p < 0.5)
	{
		return 4 * p * p * p;
	}
	else
	{
		float f = ((2 * p) - 2);
		return 0.5 * f * f * f + 1;
	}
}

float QuadraticEaseInOut(float p)
{
	if (p < 0.5)
	{
		return 2 * p * p;
	}
	else
	{
		return (-2 * p * p) + (4 * p) - 1;
	}
}

float SineEaseOut(float p)
{
	return sin(p * (float)M_PI / 2);
}
