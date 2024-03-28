# Написание С++ расширений для Python с помощью pybind11 (работает только с с++ исходниками)

Сделан на основе: https://smyt.ru/blog/sozdaem-s-python-rasshireniya-s-pomshyu-pybind11/?ysclid=ltsg5sohmf143600276
distutils заменен на setuptools на примере https://www.youtube.com/watch?v=l_Rpk6CRJYI&list=PLDV2CyUo4q-LKuiNltBqCKdO9GH4SS_ec

1. Создать виртуальное окружение

        python3.10 -m venv venv
        source venv/bin/activate
        python -m pip install --upgrade pip

2. Установить компилятор

        sudo apt install clang

3. Установить зависимости

        python -m pip install -r requirements.txt

4.  Обновить инструменты сборки модулей

        python -m pip install wheel setuptools pip --upgrade 

5.  Скомпилировать и собрать python модуль ("... install ." потому что setup.py находится в текущей директории)
   
        python -m pip install .

6.  Проверить, что модуль добавился в python

        python -m pip list | grep mylibrary

7.  Запустить тест
   
        python test.py


## Добить пример https://smyt.ru/blog/sozdaem-s-python-rasshireniya-s-pomshyu-pybind11/
## Переписать сишное расширение на с++ и сравнить скорость

Установить последнюю версию g++

        sudo apt install -y g++-13
        g++-13 --version
        which g++-13

Стандартная библиотека будет лежать

        /usr/include/c++/13

# ATTENTION

Параметр типа pybind11::array_t<std::array<char, 32>> передается в функцию по значению.