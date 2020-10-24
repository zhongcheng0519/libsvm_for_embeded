#!/usr/bin/env python3
import libsvm.svmutil as svm
import sys


if __name__ == "__main__":
    model_path = "test.model"
    if len(sys.argv) == 2:
        model_path = sys.argv[1]

    model = svm.svm_load_model(model_path)
    for line in sys.stdin:
        line = line.strip()
        x = line.split(' ')
        try:
            vx = [{i+1: float(x[i]) for i in range(len(x))}]
        except:
            continue
        print('vx = ', vx)
        p_label, p_acc, p_val = svm.svm_predict([], vx, model, '-q')
        print('p_val = ', p_val)
