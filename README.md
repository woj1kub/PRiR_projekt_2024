# Projekt PRiR 2024
Celem projektu jest wykonanie aplikacji z interfejsem GUI, która będzie wykorzystywała wielowątkowość oraz połączenie sieciowe.

## Konfiguracja projektu
1. Instalacja kompilatora
    1. Pobierz środowisko MSYS2
    https://github.com/msys2/msys2-installer/releases/download/2024-01-13/msys2-x86_64-20240113.exe
    2. W konsoli MSYS wykonaj polecenie

        `pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain`

    3. Dodaj ścieżke do zmiennych środowiskowych

        `C:\msys64\ucrt64\bin\`

2. Pobierz repozytorjum wybrane przez siebie miejsce
3. Otwórz repozytorium w Visual Studio Code
4. Dodatkowo w celu łatwiejszj pracy z projektem możesz zainstalować w VS Code dodatki:
    ```
    ms-vscode.cpptools
    jeff-hykin.better-cpp-syntax    
    donjayamanne.githistory
    ```
5. W konsoli VS Code wykonaj polecenie
    
    `mingw32-make setup`
    
6. Po wykonaniu jesteś gotowy do kompilacji i debgowania projektu za pomocą VS Code

## Zasady nazewnictwa commitów
W celu skompitowania zmian format nazewnictwa powinien wyglądać następująco:

    "nazwa pliku/plików": "opis dokonanych zmian"

## Twórcy
    Wojciech Kubowicz
    Maciej Wielgosz
    Damian Barczak