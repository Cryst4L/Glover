#pragma once

static int bowl(int x, int i, int g_i) {
	return (x - i) * (x - i) + g_i * g_i;
}
static int sep(int i, int u, int g_i, int g_u)
{
	double r = floor((u * u - i * i + g_u * g_u - g_i * g_i) / (2 * (u - i)));
	return (int) r;
}
