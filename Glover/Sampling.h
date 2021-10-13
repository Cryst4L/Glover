#pragma once
#include "Types.h"

void sampleAlongX(BYTE ** screen_p, int width, int height)
{
	BYTE * screen = (* screen_p);

	int dest_width = floor(0.5 * width);

	BYTE * sampled = malloc(dest_width * height);

	for (int y = 0; y < height; y++)
		for(int x = 0; x < dest_width; x++)
		{
			int a = screen[y * width + (2 * x)];
			int b = screen[y * width + (2 * x + 1)]; 
			sampled[y * dest_width + x] = 0.5 * (a + b);
		}

	(* screen_p) = realloc((* screen_p), dest_width * height);

	for (int n = 0; n < dest_width * height; n++)
		(* screen_p)[n] = sampled[n];

	free(sampled);
}

void sampleAlongY(BYTE ** screen_p, int width, int height)
{
	BYTE * screen = (* screen_p);

	int dest_height = floor(0.5 * height);

	BYTE * sampled = malloc(width * dest_height);

	for(int x = 0; x < width; x++)
		for (int y = 0; y < dest_height; y++)
		{
			int a = screen[(2 * y) * width + x];
			int b = screen[(2 * y + 1) * width + x]; 
			sampled[y * width + x] = 0.5 * (a + b);
		}

	(* screen_p) = realloc((* screen_p), width * dest_height);

	for (int n = 0; n < width * dest_height; n++)
		(* screen_p)[n] = sampled[n];

	free(sampled);
}

void sample2X(BYTE ** screen_p, int width, int height)
{
	sampleAlongX(screen_p, width, height);
	sampleAlongY(screen_p, 0.5 * width, height);
}




/*
void sampleAlongX(float ratio, BYTE ** screen_p, int * width_p, int * height_p)
{
	int width  = (* width_p);
	int height = (* height_p);
	BYTE * screen = (* screen_p);

	int dest_width = floor(ratio * width);

	BYTE * sampled = malloc(dest_width * height);

	for (int x = 0; x < dest_width; x++) 
		for (int y = 0; y < height; y++)
		{
			float x_a = x / ratio;
			float x_b = (x + 1) / ratio;

			float value = 0;

			for (int dx = ceil(x_a); dx < floor(x_b); dx++)
				value += screen[y * width + dx];

			value += (ceil(x_a) - x_a)  * screen[(int) (y * width + floor(x_a))];
			value += (x_b - floor(x_b)) * screen[(int) (y * width + floor(x_b))];

			value /= (x_b - x_a);

			sampled[y * dest_width + x] = (int) value;
		}

	(* width_p) = dest_width;

	(* screen_p) = realloc((* screen_p), dest_width * height); 
	for (int n = 0; n < dest_width * height; n++)
		(*screen_p)[n] = sampled[n];

	// BYTE * temp = (* screen_p);
	// (* screen_p) = sampled;
	// free(temp);
}

void sampleAlongY(float ratio, BYTE ** screen_p, int * width_p, int * height_p)
{
	int width  = (* width_p);
	int height = (* height_p);
	BYTE * screen = (* screen_p);

	int dest_height = floor(ratio * height);

	BYTE * sampled = malloc(dest_height * width);

	for (int x = 0; x < width; x++) 
		for (int y = 0; y < dest_height; y++)
		{
			float y_a = y / ratio;
			float y_b = (y + 1) / ratio;

			float value = 0;

			for (int dy = ceil(y_a); dy < floor(y_b); dy++)
				value += screen[dy * width + x];

			value += (ceil(y_a) - y_a) *  screen[(int) (floor(y_a) * width + x)];
			value += (y_b - floor(y_b)) * screen[(int) (floor(y_b) * width + x)];

			value /= (y_b - y_a);

			sampled[y * width + x] = (int) value;
		}

	(* height_p) = dest_height;


	(* screen_p) = realloc((* screen_p), width * dest_height); 
	for (int n = 0; n < width * dest_height; n++)
		(*screen_p)[n] = sampled[n];

	// BYTE * temp = (* screen_p);
	// (* screen_p) = sampled;
	// free(temp);
}

void subSample(float ratio, BYTE ** screen_p, int * width_p, int * height_p)
{
	sampleAlongX(ratio, screen_p, width_p, height_p);
	sampleAlongY(ratio, screen_p, width_p, height_p);
}
*/
