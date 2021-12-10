#pragma once

static int bowl(int x, int i, int g_i) {
	return (x - i) * (x - i) + g_i * g_i;
}
static int sep(int i, int u, int g_i, int g_u)
{
	double r = floor((u * u - i * i + g_u * g_u - g_i * g_i) / (2 * (u - i)));
	return (int) r;
}
static void firstPass(int** input, int width, int height)
{
	int m = height, n = width;
	
	int **output = (int **) malloc(height * sizeof(int *));
		for (int r = 0; r < height; r++)
			output[r] = (int *) malloc(width * sizeof(int));
	for (int x = 0; x < m; x++)
		for (int y = 0; y < n; y++)
			output[x][y] = 0;
	////////////////////////////////////////////////////////
	int upper_bound = m + n;
	for (int x = 0; x < m; x++) {
		if (input[x][0] == 0) {
			output[x][0] = upper_bound;
		}
		// forward scan
		for (int y = 1; y < n; y++) {
			output[x][y] = input[x][y] ? 0 : (1 + output[x][y - 1]);
		}
		// backward scan
		for (int y = n - 1; y > 0; y--) {
			if (output[x][y] < output[x][y - 1])
				output[x][y - 1] = 1 + output[x][y];
		}
	}
	////////////////////////////////////////////////////////
	for (int x = 0; x < m; x++)
		for (int y = 0; y < n; y++)
			input[x][y] = output[x][y];
	for (int i = 0; i < height; i++)
		free(output[i]);
	free(output);
}
