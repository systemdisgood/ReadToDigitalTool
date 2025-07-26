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
        outfile.setnchannels(1)
        outfile.setsampwidth(1)
        outfile.setframerate(44100)
        outfile.writeframes(bytes([5 for i in range(0xFFFFF7)]))
        #print(int.to_bytes(64, length=1, byteorder='big'))
    with open(args[0], 'rb') as infile:
        print("wav_heading = [", end="")
        not_is_first = False
        bytes_counter = 0
        while True: 
            infile_byte = infile.read(1)
            if infile_byte:
                bytes_counter += 1
                #print(infile_byte)
                infile_byte_int = int.from_bytes(infile_byte)
                if not_is_first:
                    print(", ", end="")
                not_is_first = True
                print(infile_byte_int, end="")
                if bytes_counter == 44:
                    print("]")
                    break
            else:
                print("]")
                print(bytes_counter)
                break
            
