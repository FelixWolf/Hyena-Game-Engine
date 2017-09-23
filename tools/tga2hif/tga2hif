#!/usr/bin/env python3
import sys
import argparse
from PIL import Image
import struct
import gzip

def isPow2(i):
    return (i != 0) and ((i & (i - 1)) == 0)

version = (1,0)

parser = argparse.ArgumentParser()
parser.add_argument("-c", "--compress", help="Compress image with zlib", action='store_true')
parser.add_argument("-m", "--mips", help="Include mips at specific value", nargs="?", type=int)
parser.add_argument("input", help="Source image")
parser.add_argument("output", help="Source image")
args = parser.parse_args()

colorModes = {
    "pal": 0,
    "g8": 1,
    "ga88": 2,
    "ga81": 3,
    "ga44": 4,
    "a8": 5,
    "a1": 6,
    "rgb888": 7,
    "rgba8888": 8,
    "rgba8881": 9,
    "rgb565": 10,
    "rgba5651": 11,
    "rgba4444": 12
}

flags = {
    "nearest": 0x1, #Default is linear
    "mips": 0x2,
    "fullbright": 0x4, #Should ignore lighting
    "ignore_z": 0x8, #ALWAYS render on top of every other polygon.
    "compressed": 0x10 #If set, each mipmap is compressed with glib.
}

im = None
try:
    im = Image.open(args.input)
except FileNotFoundError:
    print("ERROR: {} does not exist!".format(args.input))
    exit(1)

flag = 0
if args.compress:
    flag = flag | flags["compressed"]
if args.mips:
    flag = flag | flags["mips"]
    if not isPow2(im.width) or not isPow2(im.height):
        print("ERROR: Image must be power of 2 when using mipmaps.")
        exit(1)
        
    if args.mips < 1 or args.mips > 256:
        print("ERROR: Mips value must be greater than 0 and less than 256.")
        exit(1)

mode = None
if im.mode == "RGBA":
    mode = colorModes["rgba8888"]
elif im.mode == "RGB":
    mode = colorModes["rgb888"]
else:
    print("ERROR: Unrecognized colour mode from input: {}".format(im.mode))
    exit(1)

def getColorBytes(mode, im):
    result = b""
    if mode == colorModes["rgb888"]:
        tmp = im.getdata()
        cols = []
        for col in tmp:
            cols.append(col[0])
            cols.append(col[1])
            cols.append(col[2])
        
        return struct.pack(">%iB"%len(cols), *cols)
    elif mode == colorModes["rgba8888"]:
        tmp = im.getdata()
        cols = []
        for col in tmp:
            cols.append(col[0])
            cols.append(col[1])
            cols.append(col[2])
            cols.append(col[3])
        
        return struct.pack(">%iB"%len(cols), *cols)
    
result = b"HIF" + struct.pack(">BBHHIB",
    version[0],
    version[1],
    im.width,
    im.height,
    flag,
    mode
)

if args.mips:
    result = result + struct.pack(">B", args.mips-1)
    
    mipCount = args.mips - 1
    result = result + struct.pack(">HH", im.width, im.height)
    if args.compress:
        data = gzip.compress(getColorBytes(mode, im))
        result = result + struct.pack(">I", len(data)) + data
    else:
        data = getColorBytes(mode, im)
        result = result + struct.pack(">I", len(data)) + data
    
    for i in reversed(range(mipCount)):
        newSize = (int(im.height / 2), int(im.width / 2))
        if isPow2(newSize[0]) and isPow2(newSize[1]):
            im = im.resize((newSize[0], newSize[1]), Image.ANTIALIAS)
            result = result + struct.pack(">HH", im.width, im.height)
            if args.compress:
                data = gzip.compress(getColorBytes(mode, im))
                result = result + struct.pack(">I", len(data)) + data
            else:
                data = getColorBytes(mode, im)
                result = result + struct.pack(">I", len(data)) + data
        else:
            print("ERROR: Mipmap generation error! Probably too many mips! Size = ({}, {})".format(im.width / 2, im.height / 2))
    
else:
    result = result + struct.pack(">HH", im.width, im.height)
    
    if args.compress:
        data = gzip.compress(getColorBytes(mode, im))
        result = result + struct.pack(">I", len(data)) + data
    else:
        data = getColorBytes(mode, im)
        result = result + struct.pack(">I", len(data)) + data

with open(args.output, "wb") as f:
    f.write(result)

print("Wrote {} with a total of {} bytes.".format(args.output, len(result)))
    
