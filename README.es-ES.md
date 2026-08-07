

# libsvm para sistemas embebidos (solo inferencia)

[libsvm](https://www.csie.ntu.edu.tw/~cjlin/libsvm/) es la conocida biblioteca SVM, que proporciona interfaces para casi todos los lenguajes de programación. Sin embargo, parece no ser amigable para sistemas embebidos. En mi aplicación, entrené datos en mi máquina Linux para obtener un archivo de modelo, y lo uso en una placa STM32, que no cuenta con un sistema de archivos. Por lo tanto, la forma más sencilla podría ser convertir el archivo de modelo generado en archivos C como `svm_model.h` y `svm_model.c`, los cuales pueden compilarse para cualquier sistema embebido.

## Uso

`convert_model_to_c.py` lee un archivo de modelo y sustituye las variables en `STM32_files/svm_model.c.in` para generar `STM32_files/svm_model.c` como salida.

```
./convert_model_to_c.py [model_path] [prefix]
```

`[prefix]svm_model.h` y `[prefix]svm_model.c` pueden luego ser utilizados en un proyecto STM32.

`prefix` se utiliza para distinguir los modelos, ya que en mi aplicación uso dos modelos SVM. También puede dejarse sin especificar, por lo que no se añadirá ningún prefijo.

`STM32_files/main.c` prueba los dos archivos y también proporciona una demostración de cómo invocar las funciones en `svm_model.c`


## Pruebas

Para asegurarse de que `svm_model.c` es correcto, se pueden realizar pruebas. A continuación se muestran los pasos:

En primer lugar, ejecuta `gen_test_model.py` para obtener el archivo `test.model`.

```
./gen_test_model.py
```

En segundo lugar, ejecuta `convert_model_to_c.py` para generar el archivo `svm_model.c` más reciente.

```
./convert_model_to_c.py
```

Finalmente, prueba los mismos datos tanto con libsvm estándar como con `svm_model.c` convertido.

prueba con libsvm estándar:

```
cat data.txt | ./test_model.py
```

prueba con `svm_model.c` convertido:

```
cd STM32_files
make
cat ../data.txt | ./svm_test
```

la salida indicará si el programa convertido es correcto o no.

## Referencia de implementación

> [LIBSVM: A Library for Support Vector Machines](https://www.csie.ntu.edu.tw/~cjlin/papers/libsvm.pdf)
