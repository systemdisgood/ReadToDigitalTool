import array as arr
import sys
import math
import wave

if __name__ == "__main__":

    TONE = 641
    SAMPLING_FREQUENCY = 44100
    PHASE_STEP = 2*math.pi*(TONE/SAMPLING_FREQUENCY) 
    phase = 0
    need_space = False
    need_enter = False

    args_quantity = len(sys.argv) - 1
    #print(args_quantity)
    args = sys.argv[1::]
    #print(args)
    if args_quantity < 1:
        print("TOO FEW ARGUMENTS!")
        sys.exit()
    with wave.open(args[0], 'wb') as outfile:
        samples_count = 67000
        outfile.setnchannels(1)
        outfile.setsampwidth(1)
        outfile.setframerate(44100)
        outfile.writeframes(bytes([5 for i in range(samples_count)]))
        #print(int.to_bytes(64, length=1, byteorder='big'))
    with open(args[0], 'rb') as infile:
        rsc = 0 #reconstructed_samples_count
        rscn = 0 #reconstructed_samples_count_byte_num
        bytes_counter = 0
        while True: 
            infile_byte = infile.read(1)
            if infile_byte:
                bytes_counter += 1
                #print(infile_byte)
                infile_byte_int = int.from_bytes(infile_byte)
                not_is_first = True
                if bytes_counter < 41:
                    print(infile_byte_int, end=" ")
                else:
                    print(infile_byte, end=" ")
                    if infile_byte_int != 0:
                        rsc += infile_byte_int * (2**(rscn*8))
                    rscn += 1
                if bytes_counter == 44:
                    print("")
                    break
            else:
                print("")
                print(bytes_counter)
                break
        print(rsc)
            
