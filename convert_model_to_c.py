#!/usr/bin/env python3
import sys
import os
from libsvm_model import Model


def convert_model(model_path, prefix: str):
    model = Model(prefix)
    model.load(model_path)
    if model.fprint_c('STM32_files/svm_model.c.in'):
        print(f'Converting succeeded! "{prefix}svm_model.c"')
    else:
        print(f'Converting failed! "{prefix}svm_model.c"')
    if model.fprint_c('STM32_files/svm_model.h.in'):
        print(f'Converting succeeded! "{prefix}svm_model.h"')
    else:
        print(f'Converting failed! "{prefix}svm_model.h"')


def print_help():
    print("Usage: ./convert_model_to_c.py [model_path] [prefix]")
    print('\tmodel_path: path of model file, "fall.model" by default.')


if __name__ == '__main__':
    model_path = 'test.model'
    prefix = ""
    if len(sys.argv) == 1:
        pass
    elif len(sys.argv) == 2:
        model_path = sys.argv[1]
    elif len(sys.argv) == 3:
        model_path = sys.argv[1]
        prefix = sys.argv[2]
    else:
        print_help()
        sys.exit(-1)

    if os.path.splitext(model_path)[-1] != '.model':
        print("model file's extension should be .model")
        sys.exit(-1)

    if not os.path.exists(model_path):
        print('model path does not exist, please recheck your params')
        sys.exit(-1)

    convert_model(model_path, prefix)
