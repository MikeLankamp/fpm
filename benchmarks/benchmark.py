#!/usr/bin/python
"""
Translates google benchmark JSON output to a format expected by gnuplot.
"""
import argparse
import json
import os
import re
import sys

def sanitize_type(type):
    if type == "Fix16":
        return "fix16"
    if type == "CnlFixed16":
        return "cnl::fixed_16_16"
    if type.startswith("fpm::fixed"):
        return type
    return type

def parse_name(name):
    # Expected format:
    #   test-class "<" float-type ">" "/" operation
    result = re.match(r'^(.*)<(.*)>/([a-z0-9_]*)$', name)
    if result:
        return sanitize_type(result.group(2)), result.group(3)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('INFILE', help="the JSON file to process")
    parser.add_argument('OUTFILE', help="the file to write GnuPlot data to")
    args = parser.parse_args()

    operation_data = {}
    operations = []
    types = []
    with open(args.INFILE) as infile:
        data = json.load(infile)
        for entry in data['benchmarks']:
            if entry['time_unit'] != 'ns':
                raise ValueError("time_unit not in nanoseconds")

            result = parse_name(entry['name'])
            if result:
                float_type, operation_name = result
                if not operation_name in operations:
                    operations.append(operation_name)
                if not float_type in types:
                    types.append(float_type)

                d = operation_data.get(operation_name, {})
                d[float_type] = entry['cpu_time']
                operation_data[operation_name] = d

    with open(args.OUTFILE, 'w') as outfile:
        outfile.write("operation,{}\n".format(",".join(types)))
        for operation_name in operations:
            d = operation_data[operation_name]
            data = []
            for type in types:
                data.append(str(d.get(type, "-")))
            outfile.write("{},{}\n".format(operation_name, ",".join(data)))

    return 0

if __name__== "__main__":
    sys.exit(main())