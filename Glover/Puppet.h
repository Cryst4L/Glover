#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Algebra.h"
#include "Types.h"


typedef struct {
	Vector3D axis;
	double angle;
	int pivot_index;
	int nb_vertices;
	int * vertices_indexes;
} Hinge;

typedef struct {
	Vertice  * vertices;
	Edge     * edges;
	Triangle * triangles;
	Hinge    * hinges;

	int nb_vertices;
	int nb_edges;
 	int nb_triangles;
	int nb_hinges;
} Puppet;

int loadPuppet(Puppet* puppet, const char * path)
{
	puppet->nb_vertices   = 0;
	puppet->nb_edges      = 0;
	puppet->nb_triangles  = 0;
	puppet->nb_hinges     = 0;

	////////////////////////////////////////////////////////

	puppet->vertices  = NULL;
	puppet->edges     = NULL;
	puppet->triangles = NULL;
	puppet->hinges    = NULL;

	////////////////////////////////////////////////////////

	FILE * fp = fopen(path, "r");
	char * line = NULL;
	size_t length = 0;

	if (fp == NULL) {
		printf("Could not open the config file '%s'.\n", path); 
		return 1;
	}

	////////////////////////////////////////////////////////

	char * c;

	while (getline(&line, &length, fp) != -1)
	{
		// Take of the part before the comments ////////////		
		if ((c = strchr(line, '#')) != NULL) 
			*c = 0;

		// Check if the resulting line is empty ////////////
		c = line;
		int is_empty = 1;
		while (*c != '\0') { 
			if ((*c != ' ') && (*c != '\t') && (*c != '\n')) {
				is_empty = 0;
				break;
			}
			c++;
		}

		// Else, process the line //////////////////////////
		if (!is_empty)
		{
			// Get the type
			char * type = strtok(line, " \t\n");

			////////////////////////////////////////////////

			if (strcmp(type, "v") == 0) {

				puppet->nb_vertices++;

				int nb = puppet->nb_vertices;

				puppet->vertices = 
					realloc(puppet->vertices, nb * sizeof(Vertice));

				char * end;
				for (int i = 0; i < 3; i++) {

					char * token = strtok(NULL, " \t\n");
					double value = strtod(token, &end);

					if (i == 0) puppet->vertices[nb - 1].x = value;
					if (i == 1) puppet->vertices[nb - 1].y = value;
					if (i == 2) puppet->vertices[nb - 1].z = value;
				}

			////////////////////////////////////////////////



			} else if (strcmp(type, "e") == 0) {

				puppet->nb_edges++;

				int nb = puppet->nb_edges;

				puppet->edges = 
					realloc(puppet->edges, nb * sizeof(Edge));

				for (int i = 0; i < 2; i++) {

					char * token = strtok(NULL, " \t\n");
					int index = atoi(token);

					if (i == 0) puppet->edges[nb - 1].a = index;
					if (i == 1) puppet->edges[nb - 1].b = index;
				}

			////////////////////////////////////////////////

			} else if (strcmp(type, "vt") == 0) {

				puppet->nb_triangles++;

				int nb = puppet->nb_triangles;

				puppet->triangles = 
					realloc(puppet->triangles, nb * sizeof(Triangle));

				for (int i = 0; i < 3; i++) {

					char * token = strtok(NULL, " \t\n");
					int index = atoi(token);

					if (i == 0) puppet->triangles[nb - 1].a = index;
					if (i == 1) puppet->triangles[nb - 1].b = index;
					if (i == 2) puppet->triangles[nb - 1].c = index;
				}

			////////////////////////////////////////////////

			} else if (strcmp(type, "h") == 0) {

				puppet->nb_hinges++;

				int nb = puppet->nb_hinges;

				puppet->hinges = 
					realloc(puppet->hinges, nb * sizeof(Hinge));

				Hinge hinge;

				char * token = strtok(NULL, " \t\n");
				hinge.pivot_index = atoi(token);

				char * end;
				for (int i = 0; i < 3; i++) {
					char * token = strtok(NULL, " \t\n");
					double value = strtod(token, &end);

					if (i == 0) hinge.axis.d[0] = value;
					if (i == 1) hinge.axis.d[1] = value;
					if (i == 2) hinge.axis.d[2] = value;
				}

				hinge.angle = 0;

				hinge.nb_vertices = 0;
				hinge.vertices_indexes = NULL;

				while ((token = strtok(NULL, " \t\n")) != NULL) {
					hinge.nb_vertices++;
					int nv = hinge.nb_vertices;
					hinge.vertices_indexes = 
						realloc(hinge.vertices_indexes, nv * sizeof(int));
					hinge.vertices_indexes[nv - 1] = atoi(token);
				}
				
				puppet->hinges[nb - 1] = hinge;

			}

		}
	}

	free(line);
	fclose(fp);

	return 0;	
}

void freePuppet(Puppet * puppet)
{
	free(puppet->vertices);

	free(puppet->edges);

	free(puppet->triangles);

	for (int i = 0; i < puppet->nb_hinges; i++)
		free(puppet->hinges[i].vertices_indexes);
	
	free(puppet->hinges);
}

void setPuppetHinge(Puppet* puppet, int hinge_index, double angle)
{
	Hinge hinge = puppet->hinges[hinge_index];

	double delta = angle - hinge.angle;

	Matrix3D R = axisAngleMatrix(hinge.axis, delta);

	puppet->hinges[hinge_index].angle = angle;

	for (int i = 0; i < hinge.nb_vertices; i++)
	{
		// Rotate the leaf vertice /////////////////////////

		int leaf_index = hinge.vertices_indexes[i];

		Vector3D O = V3DFromVertice(puppet->vertices[hinge.pivot_index]);
		Vector3D M = V3DFromVertice(puppet->vertices[leaf_index]);
		
		M = V3DAdd(MV3DMul(R, V3DSub(M, O)), O);

		puppet->vertices[leaf_index] = VerticeFromV3D(M);

		// If the leaf is a pivot, rotate the axis too /////

		for (int j = 0; j < puppet->nb_hinges; j++)
		{
			if (puppet->hinges[j].pivot_index == leaf_index)
			{
				Vector3D A = puppet->hinges[j].axis; 
				puppet->hinges[j].axis = MV3DMul(R, A);
				break; // TODO: try without breaking
			}
		}
	}
}

void printPuppet(Puppet* puppet) 
{
	printf("\n---------- Vertices ----------\n");

	for (int i = 0; i < puppet->nb_vertices; i++) {
		Vertice v = puppet->vertices[i];
		printf("n :%3i - [% 5.1lf; % 5.1lf; % 5.1lf]\n", i, v.x, v.y, v.z);
	} 

	printf("\n----------- Edges ------------\n");

	for (int i = 0; i < puppet->nb_edges; i++) {
		Edge e = puppet->edges[i];
		printf("n :%3i - (% 3d; % 3d)\n", i, e.a, e.b);
	} 

	printf("\n--------- Triangles ----------\n");

	for (int i = 0; i < puppet->nb_triangles; i++) {
		Triangle t = puppet->triangles[i];
		printf("n :%3i - (% 3d; % 3d; % 3d)\n", i, t.a, t.b, t.c);
	} 

	printf("\n---------- Hinges -------------\n");

	for (int i = 0; i < puppet->nb_hinges; i++) {
		Hinge h = puppet->hinges[i];
		printf("n :%3i - i : %3i - ", i, h.pivot_index);
		printf("a[% 5.1lf; % 5.1lf; % 5.1lf] -", 
			h.axis.d[0], h.axis.d[1], h.axis.d[3]);
		for (int j = 0; j < h.nb_vertices; j++)
			printf("% 2d", h.vertices_indexes[j]);
		printf("\n"); 
	} 

}


static double scale(double x, double a, double b)
{
	return x * (b - a) + a;
}

void setDOF(Puppet * puppet, int index, double value)
{
	switch (index) {
		case 0 : 
		{
			setPuppetHinge(puppet,  0, scale(value, -0.1, 0.4));
			break;
		}
		case 1 : 
		{
			setPuppetHinge(puppet,  1, scale(value, -0.6, 0.7));
			setPuppetHinge(puppet,  2, scale(value, -0.3, 1.2));
			break;
		}
		case 2 : 
		{
			setPuppetHinge(puppet,  3, scale(value, -0.1, 1.3));
			setPuppetHinge(puppet,  4, scale(value,  0.0, 1.9));
			setPuppetHinge(puppet,  5, scale(value,  0.0, 1.6));
			break;
		}
		case 3 : 
		{
			setPuppetHinge(puppet,  6, scale(value, -0.1, 1.2));
			setPuppetHinge(puppet,  7, scale(value,  0.0, 1.9));
			setPuppetHinge(puppet,  8, scale(value,  0.0, 1.6));
			break;
		}
		case 4 : 
		{
			setPuppetHinge(puppet,  9, scale(value, -0.1, 1.3));
			setPuppetHinge(puppet, 10, scale(value,  0.0, 1.9));
			setPuppetHinge(puppet, 11, scale(value,  0.0, 1.6));
			break;
		}
		case 5 : 
		{
			setPuppetHinge(puppet, 12, scale(value, -0.1, 1.3));
			setPuppetHinge(puppet, 13, scale(value,  0.0, 1.9));
			setPuppetHinge(puppet, 14, scale(value,  0.0, 1.6));
			break;
		}
		case 6 : 
		{
			setPuppetHinge(puppet, 15, scale(value, -0.1, 0.2));
			setPuppetHinge(puppet, 16, scale(value,  0.0, 0.0)); 
			setPuppetHinge(puppet, 17, scale(value, -0.1, 0.2));
			setPuppetHinge(puppet, 18, scale(value, -0.2, 0.4));
			break;
		}
	}

}















