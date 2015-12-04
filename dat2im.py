#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Usage: ./dat2im.py <img> [<out>]

Options:
   -h --help   Show this message
"""

import struct


def read(src):
    bmp = []
    with open(src, 'rb') as f:
        H, W = struct.unpack('2i', f.read(8))
        for i in range(H):
            row = []
            for j in range(W):
                row.append(struct.unpack('1B', f.read(1))[0])
            bmp.append(row)
    return bmp

if __name__ == '__main__':
    import cv2
    import numpy as np
    from docopt import docopt
    import pylab as plt
    args = docopt(__doc__)
    img = read(args['<img>'])
    if args['<out>']:
        img = np.array(img, np.uint8)
        img *= 255
        img = ~img
        out = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)
        cv2.imwrite(args['<out>'], out)
    else:
        plt.imshow(img, interpolation='nearest', cmap='Greys')
        plt.axis('off')
        plt.show()
