#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Draw.h"
#include "Utils.h"
#include "Puppet.h"
#include "Render.h"
#include "Parse.h"

static const int DISTANCE = 60;

int main(int argc, char * argv[])
{
	// Parse the configurations ////////////////////////////

	char * filename = parseFilename("glover-dataset.bytes", argc, argv);

	int sample_size = parseOption("size"  ,  48, argc, argv);
	int nb_samples  = parseOption("number", 1e4, argc, argv);
	int random_seed = parseOption("seed"  ,   0, argc, argv);

	double alpha    = parseOption("alpha" ,   0, argc, argv);
	double beta     = parseOption("beta"  ,   0, argc, argv);
	double gamma    = parseOption("gamma" ,   0, argc, argv);

	// Open the dest file //////////////////////////////////

	FILE* fp = fopen(filename, "wb");

	if (!fp) {
		printf("Could not open '%s'\n", filename);
		return EXIT_FAILURE;
	}

	// Write the header bytes //////////////////////////////

	fputc(sample_size     , fp);
	fputc(nb_samples >> 16, fp);
	fputc(nb_samples >>  8, fp);
	fputc(nb_samples      , fp);

	// Load the puppet /////////////////////////////////////

	Puppet puppet;
	loadPuppet(&puppet, "hand.config");

	// Init the rendering //////////////////////////////////

	BYTE * screen = malloc(sample_size * sample_size * sizeof(BYTE));

	Vector3D angles = {{0, 0, 0}};
	Vector3D position = {{0, 0, DISTANCE}};

	srand(random_seed);

	printf(" Generating '%s' ...\n", filename);

	///////////////////////////////////////////////////////

	for (int n = 0; n < nb_samples; n++)
	{
		// Draw the parameters /////////////////////////////

		double parameters[10];

		for (int i = 0; i < 10; i++)
			parameters[i] = (rand() % 256) / (float) 256;

		// Prevent unrealistic poses ///////////////////////

		double c = 0.5 * (parameters[3] + parameters[4]);
		parameters[2] *= c;
		parameters[5] *= c;

		double p = parameters[5];
		parameters[4] = (1 - p) * parameters[4] + p * parameters[5];

		p = parameters[3];
		parameters[4] = (1 - p) * parameters[4] + p * parameters[3];

		// Increase the average opening ////////////////////

		parameters[2] = pow(parameters[2], 1.2);
		parameters[3] = pow(parameters[3], 1.8);
		parameters[4] = pow(parameters[4], 1.8);
		parameters[5] = pow(parameters[5], 1.2);

		// Set the DOFs ////////////////////////////////////

		for (int d = 0; d < 7; d++)
			setDOF(&puppet, d, parameters[d]);

		// Setup the angles ////////////////////////////////

		angles.d[0] = scale(parameters[7], 0.2, -1.0) + alpha;
		angles.d[1] = scale(parameters[8], 0.2, -1.0) + beta;
		angles.d[2] = scale(parameters[9], 0.4, -1.0) + gamma;

		// Render the sample ///////////////////////////////

		renderAdjustedSample(&puppet, angles, position, screen, sample_size, 0);

		// Save the sample /////////////////////////////////

		fwrite(screen, sizeof(BYTE), sample_size * sample_size, fp);

		for (int i = 0; i < 10; i++)
			fputc(256 * parameters[i], fp);

		printProgress(n / (double) nb_samples);
	}
	
	printf("\n ... Done !\n");

	// Free the allocated memory ///////////////////////////

	fclose(fp);
	free(filename);
	freePuppet(&puppet);
	free(screen);

	return EXIT_SUCCESS;
}
