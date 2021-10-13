#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double parseOption(const char * option, 
	               double default_value, 
	               int argc, char ** argv)
{
	int l = strlen(option) + 2;

	char * key = malloc(l + 1);
	strcpy(key + 1, option);
	key[0] = '-'; key[l - 1] = '='; key[l]  = '\0';

	double value = default_value;
	for (int i = 1; i < argc; i++)
		if (strncmp(key, argv[i], l) == 0) {
			char * end;
			value = strtod(argv[i] + l, &end);
		}

	free(key);			

	return value;
}

char * parseFilename(const char * default_name, 
                     int argc, char ** argv)
{
	char * name;

	if (argc == 1) {
		name = malloc(strlen(default_name) + 1); 
		strcpy(name, default_name);
		return name;
	}

	int * flags = calloc(argc, sizeof(int));

	for (int i = 1; i < argc; i++)
		if (argv[i][0] != '-') 
			if (strchr(argv[i], '=') == NULL)
				flags[i] = 1;
	
	int count = 0, index = 0;
	for (int i = 1; i < argc; i++) {
		count += flags[i];
		if ((count == 1) && flags[i])
			index = i;
	}	

	free(flags);

	if (count > 0) {

		int length = strlen(argv[index]);
		name = malloc(length);
		strcpy(name, argv[index]);

		if (count > 1)
			printf(" Invalid usage. Only one filename is required.\n");

	} else {
		name = malloc(strlen(default_name) + 1); 
		strcpy(name, default_name);
	}

	return name;
}


