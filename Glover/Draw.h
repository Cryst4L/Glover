#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Types.h"

void draw(BYTE * data, int width, int height)
{
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			data[width * i + j] = 255 * (0.5 * sin(0.1 * (i + j)) + 0.5);
}

void drawFlatLine(int x1, int x2, int y, BYTE * data, int width, int height)
{
	if (x1 == x2)
		return;

	if (x1 > x2) {
		int temp = x1;
		x1 = x2;
		x2 = temp;
	}	

	if (y > 0 && y < height)
	{
		for(int x = x1; x <= x2 ; x++)
		{
			if (x > 0 && x < width)
			{
				data[y * width + x] = 255;
			}
		}
	}
}

void drawTriangle(Point a, Point b, Point c, BYTE * data, int width, int height)
{
	Point t;

	if (a.y > b.y) {
		t = a; a = b; b = t;
	}

	if (a.y > c.y) {
		t = a; a = c; c = t;
	}

	if (b.y > c.y) {
		t = b; b = c; c = t;
	}

	double dx1 = (b.x - a.x) / (b.y - a.y);
	double dx2 = (c.x - a.x) / (c.y - a.y);
	double dx3 = (c.x - b.x) / (c.y - b.y);

	Point s = a, e = a;

	if (dx1 > dx2) {

		for (; s.y < b.y; 
			   s.y++, e.y++, 
			   s.x += dx2, e.x += dx1)
		{	
			drawFlatLine(s.x, e.x, s.y, data, width, height);
		}

		e = b;

		for (; s.y < c.y; 
		       s.y++, e.y++, 
		       s.x += dx2, e.x += dx3)
		{
			drawFlatLine(s.x, e.x, s.y, data, width, height);
		}

	} else {
	
		for (; s.y < b.y; 
		       s.y++, e.y++, 
		       s.x += dx1, e.x += dx2)
		{
			drawFlatLine(s.x, e.x, s.y, data, width, height);
		}
					
		s = b;

		for (; s.y < c.y; 
		       s.y++, e.y++, 
		       s.x += dx3, e.x += dx2)
		{
			drawFlatLine(s.x, e.x, s.y, data, width, height);
		}
	}
}

void drawLine(Point a, Point b, BYTE * data, int width, int height)
{
	int x0 = a.x, y0 = a.y;
	int x1 = b.x, y1 = b.y;

	int dx =  abs(x1 - x0);
	int dy = -abs(y1 - y0);

	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;

	int err = dx + dy, e2;

	int x = x0, y = y0;

	while (x != x1 || y != y1) 
	{
		data[y * width + x] = 255;

		e2 = 2 * err;

		if (e2 >= dy) {
			err += dy;
			x += sx;
		}

		if (e2 <= dx) {
			err += dx;
			y += sy;
		}
	}
}
