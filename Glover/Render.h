#pragma once

#include "Draw.h"
#include "Algebra.h"
#include "Distance.h"
#include "Sampling.h"

static const double FOV       = 60.;
static const int DRAFT_SIZE   = 48;
static const double FILLING   = 0.12;
static const double THICKNESS = 0.8;

void renderWireFrame(Puppet * puppet, Vector3D angles, 
                     Vector3D offset, BYTE* screen, 
                     int screen_width, int screen_height)
{
	Matrix3D Rx = eulerMatrixAlpha(angles.d[0]);
	Matrix3D Ry = eulerMatrixBeta( angles.d[1]);
	Matrix3D Rz = eulerMatrixGamma(angles.d[2]);

	Matrix3D R = M3DMul(M3DMul(Rz, Ry), Rx);

	int nb_vertices = puppet->nb_vertices;
	Vertice * vertices = puppet->vertices;

	Point * points = (Point *) malloc(nb_vertices * sizeof(Point));

	// Vertices projection /////////////////////////////////
/*
	printf("[[% 2.2f, % 2.2f, % 2.2f],\n", R.d[0][0], R.d[0][1], R.d[0][2]);
	printf(" [% 2.2f, % 2.2f, % 2.2f],\n", R.d[1][0], R.d[1][1], R.d[1][2]);
	printf(" [% 2.2f, % 2.2f, % 2.2f]]\n", R.d[2][0], R.d[2][1], R.d[2][2]);
*/	
	for (int i = 0; i < nb_vertices; i++)
	{
		Vector3D position = V3DFromVertice(vertices[i]);
		position = V3DAdd(MV3DMul(R, position), offset);
		
		points[i].x = position.d[0] / position.d[2]; 
		points[i].y = position.d[1] / position.d[2];
//		printf("add point : (% 2.2f; % 2.2f)\n", points[i].x, points[i].y);
	}

	double scale = (0.5 / tan(FOV * M_PI / 360)) * screen_height;

	// Edges Rendering /////////////////////////////////////

	int nb_edges = puppet->nb_edges;
	Edge * edges = puppet->edges;

	for (int i = 0; i < nb_edges; i++)
	{
		Edge edge = edges[i];

		Point O = {0.5 * screen_width, 0.5 * screen_height};
		Point A = {O.x + scale * points[edge.a].x, O.y + scale * points[edge.a].y};
		Point B = {O.x + scale * points[edge.b].x, O.y + scale * points[edge.b].y};

		drawLine(A, B, screen, screen_width, screen_height);
//		printf("drawline : (% 2.1f; % 2.1f) -> (% 2.1f; % 2.1f)\n", A.x, A.y, B.x, B.y);
	}

	// Triangles Rendering /////////////////////////////////

	int nb_triangles = puppet->nb_triangles;
	Triangle * triangles = puppet->triangles;

	for (int i = 0; i < nb_triangles; i++)
	{
		Triangle triangle = triangles[i];

		Point O = {0.5 * screen_width, 0.5 * screen_height};
		Point A = {O.x + scale * points[triangle.a].x, O.y + scale * points[triangle.a].y}; 	
		Point B = {O.x + scale * points[triangle.b].x, O.y + scale * points[triangle.b].y};
		Point C = {O.x + scale * points[triangle.c].x, O.y + scale * points[triangle.c].y};

		drawTriangle(A, B, C, screen, screen_width, screen_height);	
	}
	
	free(points);
}

void renderFleshHull(double distance, BYTE * screen, int width, int height)
{
	// TODO: fix this

	double scale = 2 * tan(FOV / 360 * M_PI);
	double thickness = THICKNESS * height / distance / scale;
	double threshold = thickness * thickness; 

	// convert the screen to a 2D int array ////////////////

	int **array = (int **) malloc(height * sizeof(int *));
		for (int n = 0; n < height; n++)
			array[n] = (int *) malloc(width * sizeof(int));

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			array[i][j] = screen[i * width + j];

	// Perform the distance transform //////////////////////

	distanceTransform(array, width, height);

	// Treshold ////////////////////////////////////////////

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			array[i][j] = (array[i][j] < threshold) ? 255 : 0;

	// Convert the 2D array back to the screen /////////////

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			screen[i * width + j] = array[i][j];

	// Free the 2D array ///////////////////////////////////

	for (int n = 0; n < height; n++)
		free(array[n]);

	free(array);

}

void renderRawSample(Puppet * puppet, Vector3D angles,
                     Vector3D offset, BYTE * screen, 
                     int sample_size, int skeleton)
{
	for (int i = 0; i < sample_size * sample_size; i++)
		screen[i] = 0;

	renderWireFrame(puppet, angles, offset, screen, sample_size, sample_size);

	if (skeleton)
	{
		BYTE * temp = malloc(sample_size * sample_size);
		memcpy(temp, screen, sample_size * sample_size);
		renderFleshHull(offset.d[2], temp, sample_size, sample_size);

		for (int n = 0; n < sample_size * sample_size; n++)
			screen[n] = (screen[n] + temp[n]) / 2;

		free(temp);
	} else {
		renderFleshHull(offset.d[2], screen, sample_size, sample_size);
	}
}

void renderAdjustedSample(Puppet * puppet, Vector3D angles,
                          Vector3D offset, BYTE * screen, 
                          int sample_size, int skeleton)
{
	// Evaluate the geometry on a draft sample /////////////

	BYTE * draft = malloc(DRAFT_SIZE * DRAFT_SIZE);

	renderRawSample(puppet, angles, offset, draft, DRAFT_SIZE, 0);
	
	double sum = 0;

	Point center = {0, 0};

	for (int x = 0; x < DRAFT_SIZE; x++)
	{
		for (int y = 0; y < DRAFT_SIZE; y++)
		{
			BYTE value = draft[y * DRAFT_SIZE + x];
			center.x += value * x;
			center.y += value * y;
			sum += value;
		}
	}

	center.x /= (float) sum;
	center.y /= (float) sum;

	double filling = sum / (double) (DRAFT_SIZE * DRAFT_SIZE) / 256.;

	double scaling = sqrt(FILLING / filling);

	free(draft);

	// Render the sample ///////////////////////////////////

	int upscaled_size = sample_size * scaling * 2;

	//printf("%d\n", upscaled_size);

	BYTE * upscaled = malloc(upscaled_size * upscaled_size);

	renderRawSample(puppet, angles, offset, upscaled, 
	                upscaled_size, skeleton);

	center.x *= upscaled_size / (double) DRAFT_SIZE;
	center.y *= upscaled_size / (double) DRAFT_SIZE; 

	// Crop and Resize the sample //////////////////////////

	int corner_x = center.x - (0.5 * sample_size) * 2;
	int corner_y = center.y - (0.5 * sample_size) * 2;

	BYTE * sample = malloc(4 * sample_size * sample_size);

	for (int x = 0; x < (2 * sample_size); x++)
		for (int y = 0; y < (2 * sample_size); y++)
			sample[y * (2 * sample_size) + x] = 
				upscaled[(corner_y + y) * upscaled_size + (corner_x + x)];	

	sample2X(&sample, 2 * sample_size, 2 * sample_size);

	for (int n = 0; n < sample_size * sample_size; n++)
		screen[n] = sample[n];

	free(upscaled);
	free(sample);
}
