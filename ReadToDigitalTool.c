#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef unsigned large_unsigned;

typedef unsigned nco_counter_t;
nco_counter_t  max_beep_nco_counter = -1; // unsigned underflow 

nco_counter_t beep_nco_counter = 0; // beep phase is beep_nco_counter / max_beep_nco_counter

// A - zero
// E - zero or one (stuffing)
// B - one

const double zero_frequency = 440;
const double stuffing_frequency = 659.26;
const double one_frequency = 997.76;
const unsigned sampling_frequency = 44100;

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
