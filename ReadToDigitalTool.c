#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef unsigned large_unsigned;

typedef unsigned phase_t;
phase_t  max_phase_t = -1;

phase_t beep_phase = 0;

// A - zero
// E - zero or one (zerone)
// B - one
double zero_frequency = 641;
double zerone = 0;
double one_frequency = 440;
unsigned sampling_frequency = 44100;

void phase_step(phase_t* phase, phase_t step)
{
	*phase += step;
}

unsigned infile_name_len = 0;
FILE* infile = NULL;

unsigned outfile_name_len = 0;
FILE* outfile = NULL;



large_unsigned get_ascii_quantity(FILE* infile)
{
	large_unsigned quantity = 0;
	int character;
	fseek(infile, 0, SEEK_SET);
	while((character = fgetc(infile)) != EOF)
	{
		if(character < 128)
		{
			++quantity;
		}
	}
	return quantity;
}




int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("Must be 2 arguments\n");
		return 0;
	}

	infile_name_len = strlen(argv[1]);
	infile = fopen(argv[1], "r");
	if(!infile)
	{
		printf("Can't open the input file\n");
		return 0;
	}
	
	outfile_name_len = strlen(argv[2]);
	outfile = fopen(argv[2], "w");
	if(!outfile)
     {
		 printf("Can't open the outlut file\n");
		 return 0;
	 }
	
	large_unsigned ascii_quantity =  get_ascii_quantity(infile);
	printf("%u\n", ascii_quantity);

	fclose(infile);
	fclose(outfile);
	return 0;
}
