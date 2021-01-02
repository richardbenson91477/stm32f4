#!/usr/bin/env python3
import sys

def main ():
    f_in = open('rgb.txt', 'r')
    if not f_in.readline ():
        return 1

    lines = f_in.readlines ()
    if not lines:
        return 2
    del f_in

    f_h = open('colors565.h', 'w')
    if not f_h:
        return 4
    
    f_h.write ('#ifndef COLORS565_INCLUDED\n')
    f_h.write ('#define COLORS565_INCLUDED\n')
    f_h.write ('\n')

    lines_n = len(lines)
    for line_c in range(lines_n):
        line_split = lines[line_c].split (maxsplit = 3)
        if len(line_split) != 4:
            print("WARNING: ignoring: " + line_split)
            continue

        name_s = line_split[3].strip().replace(' ', '_')

        color_r = int(line_split [0])
        color_g = int(line_split [1])
        color_b = int(line_split [2])
        color_s = hex (((color_r >> 3) << 11) +\
                ((color_g >> 2) << 5) +\
                (color_b >> 3))
        f_h.write ('#define COLOR565_' + name_s + ' ' + color_s + '\n')

    f_h.write ('\n')
    f_h.write ('#endif\n\n')
    
    return 0

if __name__ == '__main__':
    sys.exit (main ())

