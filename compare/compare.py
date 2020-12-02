"""
Compare a two PGM or PNM images (with an epsilon).
"""

import argparse

from PIL import Image

def compare(image_1, image_2, eps):
    """
    Compares two images.
    """
    assert image_1.mode == image_2.mode, 'Images are of different type.'
    assert image_1.size == image_2.size, 'Images are of different size.'

    pairs = zip(image_1.getdata(), image_2.getdata())

    if len(image_1.getbands()) == 1:
        # for gray-scale image
        dif = list(abs(p1 - p2) for p1, p2 in pairs)
    else:
        # multi-channel image
        dif = list(abs(c1 - c2) for p1, p2 in pairs for c1, c2 in zip(p1, p2))

    assert len(list(filter(lambda x: x > eps, dif))) == 0, \
           'At least one pixel is different.'

    print('Images are equal.')

def main():
    """
    Main function - parses arguments and calls the comparing function.
    """
    parser = argparse.ArgumentParser()
    parser.add_argument('first_file')
    parser.add_argument('second_file')
    parser.add_argument('--eps', type=int, default=2, help='Diferenta maxima.')
    args = parser.parse_args()

    image_1 = Image.open(args.first_file)
    image_2 = Image.open(args.second_file)

    compare(image_1, image_2, args.eps)

if __name__ == '__main__':
    main()
