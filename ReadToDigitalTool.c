#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// WAV_HEADING_WITHOUT_QUANTITY_BYTES_QUANTITY
#define WHWOQBQ 40

//[82, 73, 70, 70, 36, 1, 0, 0, 87, 65, 86, 69, 102, 109, 116, 32, 16, 0, 0, 0, 1, 0, 1, 0, 68, 172, 0, 0, 68, 172, 0, 0, 1, 0, 8, 0, 100, 97, 116, 97, b'\x00', b'\x01', b'\x00', b'\x00']

const uint8_t wav_heading_without_quantity[WHWOQBQ] = {82, 73, 70, 70, 36, 1, 0, 0, 87, 65, 86, 69, 102, 109, 116, 32, 16, 0, 0, 0, 1, 0, 1, 0, 68, 172, 0, 0, 68, 172, 0, 0, 1, 0, 8, 0, 100, 97, 116, 97};

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
const unsigned sample_duration_ms = 500;

unsigned infile_name_len = 0;
FILE* infile = NULL;

unsigned outfile_name_len = 0;
FILE* outfile = NULL;

large_unsigned count_file_bytes(FILE* infile)
{
	long old_file_position = ftell(infile);
	large_unsigned quantity = 0;
	int character;
	fseek(infile, 0, SEEK_SET);
	while((character = fgetc(infile)) != EOF) ++quantity;
	fseek(infile, old_file_position, SEEK_SET);
	return quantity;
}

void generate_wav_file(FILE* infile, FILE* outfile)
{
	long old_file_position = ftell(infile);
	
	// writing heading without quantity (last 4 bytes)
	if(!(fwrite(wav_heading_without_quantity, 1, WHWOQBQ, outfile)))
	{
		printf("CAN NOT WRITE BYTES TO FILE\n");
	}

	// counting bytes of the converting file
	large_unsigned infile_bytes_quantity = count_file_bytes(infile);
	// counting samples to write
	large_unsigned samples_quantity = infile_bytes_quantity * sampling_frequency * sample_duration_ms / 1000; 
	printf("%lld\n", (long long)samples_quantity);

	// converting samples quantity to 4 bytes number in the end of wav file heading
	large_unsigned accumulator = samples_quantity;
	large_unsigned last_accumulator = accumulator;
	uint64_t powered_two = 1;
	uint8_t heading_quantity_bytes[4];
	for(uint8_t counter = 0; counter < 4; ++counter)
	{
		for(uint8_t power = 0; power < (counter + 2) * 8 - 8; ++power)
		{
			powered_two = powered_two * 2;
		}
		//printf("%lld\n", (long long)powered_two);
		//heading_quantity_bytes[counter] = accumulator %
		accumulator = accumulator / 8;
		heading_quantity_bytes[counter] = last_accumulator - accumulator;
		printf("%lld\n", (long long)heading_quantity_bytes[counter]);
		last_accumulator = accumulator;
		powered_two = 1;
	}
	int character;
	uint8_t byte_to_write = 0;
	while((character = fgetc(infile)) != EOF)
	{
		if(!(fwrite(&byte_to_write, 1, 1, outfile)))
		{
			printf("CAN NOT WRITE BYTE TO FILE\n");
		}
	}

	fseek(infile, old_file_position, SEEK_SET);
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
	
	large_unsigned infile_bytes_quantity =  count_file_bytes(infile);
	printf("%u\n", (unsigned)infile_bytes_quantity);
	
	generate_wav_file(infile, outfile);

	fclose(infile);
	fclose(outfile);
	return 0;
}
