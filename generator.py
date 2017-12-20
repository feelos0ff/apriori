import argparse
import random

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-m", type=int,
                    help="transactions count")
    parser.add_argument("-n", type=int,
                    help="items count")
    parser.add_argument("-f", type=str,
                    help="filename")
    parser.add_argument("-k", type=float,
                    help="line k")
    
    args = parser.parse_args()
     
    with open(args.f, "w") as text_file:
        for l in range(args.m):
            
            rand_line = [str(int(random.random() < args.k / args.n)) for i in range(args.n)] + ['\n']
            text_file.write("".join(rand_line))


