# Записки по Informatics Rescue

В помощь лекторам и студентам.

## Компиляция

Чтобы получить .pdf файл с лекционными записями, необходимо иметь:

* TeX Live
* GNU make
* CMake

Для компиляции необходимо выполнить следующие команды из корневой директории проекта:

    mkdir _build && cd _build && cmake .. && make

После чего в директории `_build` будет `notes.pdf`
