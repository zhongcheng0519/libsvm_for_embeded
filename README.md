# libsvm for embeded (forward only)

[libsvm](https://www.csie.ntu.edu.tw/~cjlin/libsvm/) is the famous SVM library, which provides interfaces for almost all programming languages. However, it seems not friendly for embeded system. In my application, I trained data on my Linux machine for a model file, and use it on STM32 board, which do not have a file system. So the easiest way maybe is converting the generated model file to c files like `svm_model.h` and `svm_model.c` which can be compiled to any embeded system.

## Usage

`convert_model_to_c.py` reads an model file and replace variables to `STM32_files/svm_model.c.in` and generate `STM32_files/svm_model.c` as output.

```
./convert_model_to_c.py [model_path] [prefix]
```

`[prefix]svm_model.h` and `[prefix]svm_model.c` can then be utilized in STM32 project.

`prefix` is used to distinguish models as I used two SVM models in my application. It can also be leave alone, thus no prefix will be added.

`STM32_files/main.c` tests the two files and also gives the demonstration of invoking functions in `svm_model.c`


## Test

To make sure the `svm_model.c` is correct, tests could be conducted. Following are the steps:

Firstly, run `gen_test_model.py` to get `test.model` file.

```
./gen_test_model.py
```

Secondly, execute `convert_model_to_c.py` to generate newest `svm_model.c` file

```
./convert_model_to_c.py
```

Finally, test same data both by standard libsvm and converted `svm_model.c`

test standard libsvm:

```
cat data.txt | ./test_model.py
```

test converted `svm_model.c`:

```
cd STM32_files
make
cat ../data.txt | ./svm_test
```

the output will tell whether the converted program is right or not.

## Implementation reference

> [LIBSVM: A Library for Support Vector Machines](https://www.csie.ntu.edu.tw/~cjlin/papers/libsvm.pdf)

