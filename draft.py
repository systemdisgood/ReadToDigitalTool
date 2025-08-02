import array as arr
import sys
import math
import wave

def calc_nco_incr(f, sf, denominator_pow):
    denominator = 2**denominator_pow - 1
    hz_incr = denominator / sf
    return int(hz_incr * f)


def calc_cordic_coefs(steps):
	coefs = []
	for i in range(steps):
		coefs.append([])
		coefs[-1].append(math.sin((math.pi/2)/(i+1)))
		coefs[-1].append(math.cos((math.pi/2)/(i+1)))
		print(coefs[-1][-2], end=" ")
		print(coefs[-1][-1])

if __name__ == "__main__" and True:

    coefs = calc_cordic_coefs(30)
    
    sampling_freq = 44100
    #zero_freq = 440
	#zerone_freq = 659.26
    #one_freq =  997.76
    
    zero_freq = 493.88
    zerone_freq = 587.32
    one_freq = 698.46
    pause_freq = 830.60

    zero_nco_incr = calc_nco_incr(zero_freq, sampling_freq, 32)
    print("zero_nco_incr = " + str(zero_nco_incr))
    zerone_nco_incr = calc_nco_incr(zerone_freq, sampling_freq, 32)
    print("zerone_nco_incr = " + str(zerone_nco_incr))
    one_nco_incr = calc_nco_incr(one_freq, sampling_freq, 32)
    print("one_nco_incr = " + str(one_nco_incr))
    pause_nco_incr = calc_nco_incr(pause_freq, sampling_freq, 32)
    print("pause_nco_incr = " + str(pause_nco_incr))


if __name__ == "__main__" and False:

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
            
