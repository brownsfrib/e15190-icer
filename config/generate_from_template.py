#!/usr/bin/env python3
import argparse
from pathlib import Path

def main(args):
    calib_dir = str(args.dir)
    if calib_dir[-1] == '/': calib_dir = calib_dir[:-1]

    output_dir = str(args.output)
    if output_dir[-1] == '/': output_dir = output_dir[:-1]

    lines = []
    with open('HiRAEVT.conf.template', 'r') as infile, open('HiRAEVT.conf', 'w') as outfile:
        for line in infile:
            line = line.replace(r'${CALIBRATIONS_DIR}', calib_dir)
            line = line.replace(r'${OUTPUT_DIR}', output_dir)
            lines.append(line)
        outfile.writelines(lines)

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--dir", type=str, help='calibrations/ directory')
    parser.add_argument("-o", "--output", type=str, help='output directory')
    args = parser.parse_args()
    args.dir = Path(args.dir).resolve()
    args.output = Path(args.output).resolve()
    return args

if __name__ == '__main__':
    args = get_args()
    main(args)