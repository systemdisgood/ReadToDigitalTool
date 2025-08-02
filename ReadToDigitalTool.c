#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
//#define _USE_MATH_DEFINES
#include <math.h>
#define M_PI 3.14159265358979323846

// WAV_HEADING_WITHOUT_QUANTITY_BYTES_QUANTITY
#define WHWOQBQ 40

// WAV_HEADING_WITHOUT_QUANTITY_BYTES
//[82, 73, 70, 70, 36, 1, 0, 0, 87, 65, 86, 69, 102, 109, 116, 32, 16, 0, 0, 0, 1, 0, 1, 0, 68, 172, 0, 0, 68, 172, 0, 0, 1, 0, 8, 0, 100, 97, 116, 97, b'\x00', b'\x01', b'\x00', b'\x00']

const uint8_t wav_heading_without_quantity[WHWOQBQ] = {82, 73, 70, 70, 36, 1, 0, 0, 87, 65, 86, 69, 102, 109, 116, 32, 16, 0, 0, 0, 1, 0, 1, 0, 68, 172, 0, 0, 68, 172, 0, 0, 1, 0, 8, 0, 100, 97, 116, 97};

typedef unsigned large_unsigned;

typedef unsigned nco_counter_t;
nco_counter_t  max_beep_nco_counter = -1; // unsigned underflow 

nco_counter_t beep_nco_counter = 0; // beep phase is beep_nco_counter / max_beep_nco_counter

const uint32_t PAUSES_QUANTITY = 2;

const uint64_t MAX_FILE_SAMPLES_QUANTITY = 0x4F;

//const uint32_t zero_nco_incr = 42852281;
//const uint32_t zerone_nco_incr = 64206352;
//const uint32_t one_nco_incr = 97173391;

const uint32_t zero_nco_incr = 48099738;
const uint32_t zerone_nco_incr = 57200004;
const uint32_t one_nco_incr = 68024101;
const uint32_t pause_nco_incr = 80893420;

const double sampling_frequency = 44100;
const uint32_t symbol_duration_ms = 500;

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
	large_unsigned symbol_duration_samples = sampling_frequency * symbol_duration_ms / 1000;
	large_unsigned samples_quantity = (8+PAUSES_QUANTITY) * infile_bytes_quantity * symbol_duration_samples; 
	//printf("%lld\n", (long long)samples_quantity);
	
	

	// converting samples quantity to 4 bytes number in the end of wav file heading
	large_unsigned accumulator = samples_quantity;
	uint8_t heading_quantity_bytes[4];
	for(uint8_t counter = 0; counter < 4; ++counter)
	{
		heading_quantity_bytes[counter] = accumulator % 0x100;
		accumulator = accumulator / 0x100;
		//printf("%lld\n", (long long)heading_quantity_bytes[counter]);
	}
	
	// writing samples quantity bytes to file
	if(!(fwrite(heading_quantity_bytes, 1, 4, outfile)))
	{
		printf("CAN NOT WRITE BYTES TO FILE\n");
	}

	enum Symbol
	{
		ZERO,
		ONE,
		ZERONE,
        PAUSE
	};
	int character;
	uint8_t byte_to_write = 0;
	uint8_t symbol = 0;
	bool bit_is_one = false;
	bool bit_was_one = false;
	bool zerone_needless = true;
	uint8_t char_accumulator = 0;
	uint32_t nco_counter = 0;
	double phase = 0;
	uint64_t samples_counter = 0;
	while((character = fgetc(infile)) != EOF)
	{
		char_accumulator = character;
		for(uint8_t bits_counter = 0; bits_counter < 8; ++bits_counter)
		{
			bit_is_one = (char_accumulator % 2) ? true : false;
			char_accumulator /= 2;
			if(((bit_is_one && bit_was_one) || (!bit_is_one && !bit_was_one)) && !zerone_needless)
			{
				symbol = ZERONE;
				zerone_needless = true;
			}
			else
			{
				if(bit_is_one)
				{
					symbol = ONE;
				}
				else
				{
					symbol = ZERO;
				}
				zerone_needless = false;
			}
			bit_was_one = bit_is_one ? true : false;

			switch(symbol)
			{
				case ONE:
					//printf("1");
					for(uint32_t samples_counter = 0; samples_counter < symbol_duration_samples; ++ samples_counter)
					{
						phase = (M_PI * 2 * ((double)nco_counter / 0xFFFFFFFF));
						//byte_to_write = samples_counter;
						byte_to_write = 127 * (sin(phase) + 1);
						if(!(fwrite(&byte_to_write, 1, 1, outfile)))
						{
							printf("CAN NOT WRITE BYTES TO FILE\n");
						}
						nco_counter += one_nco_incr;
					}
					break;
				case ZERO:
					//printf("0");
                    for(uint32_t samples_counter = 0; samples_counter < symbol_duration_samples; ++samples_counter)
                    {
                    	phase = (M_PI * 2 * ((double)nco_counter / 0xFFFFFFFF));
                        //byte_to_write = samples_counter;
                        byte_to_write = 127 * (sin(phase) + 1);
                        if(!(fwrite(&byte_to_write, 1, 1, outfile)))
                        {
                            printf("CAN NOT WRITE BYTES TO FILE\n");
                        }
                        nco_counter += zero_nco_incr;
                    }
					break;
				case ZERONE:
					//printf("_");
                    for(uint32_t samples_counter = 0; samples_counter < symbol_duration_samples; ++samples_counter)
                    {
						phase = (M_PI * 2 * ((double)nco_counter / 0xFFFFFFFF));
                        //byte_to_write = samples_counter;
                        byte_to_write = 127 * (sin(phase) + 1);
                        if(!(fwrite(&byte_to_write, 1, 1, outfile)))
                        {
                        	printf("CAN NOT WRITE BYTES TO FILE\n");
                        }
                        nco_counter += zerone_nco_incr;
                    }

			}
		}
		zerone_needless = true;
		for(uint32_t samples_counter = 0; samples_counter < symbol_duration_samples * PAUSES_QUANTITY; ++samples_counter)
		{
            phase = (M_PI * 2 * ((double)nco_counter / 0xFFFFFFFF));
			byte_to_write = 127 * (sin(phase) + 1);
            //byte_to_write = 127;
			if(!(fwrite(&byte_to_write, 1, 1, outfile)))
			{
				 printf("CAN NOT WRITE BYTES TO FILE\n");
			}
            nco_counter += pause_nco_incr;
		}
		//printf("\n");
		
		//getchar();

		if(!(fwrite(&byte_to_write, 1, 1, outfile)))
		{
			printf("CAN NOT WRITE BYTE TO FILE\n");
		}
		++samples_counter;
		if(samples_counter >= MAX_FILE_SAMPLES_QUANTITY) break;
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
	
	//large_unsigned infile_bytes_quantity =  count_file_bytes(infile);
	//printf("%u\n", (unsigned)infile_bytes_quantity);
	
	generate_wav_file(infile, outfile);

	fclose(infile);
	fclose(outfile);
	return 0;
}
