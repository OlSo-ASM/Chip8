#include<stdio.h>
#include"chip8.h"
#define _CRT_SECURE_NO_WARNINGS
int main(){
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 1;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
    Chip8 chip8;
    init(&chip8);
    loadFromROM(&chip8, "ProjectName.ch8");
    int msec = 0,
        trigger1 = 16,
        trigger2 = 1;
    clock_t before1 = clock();
    clock_t before2 = clock();
    clock_t difference = clock() - before1;
    while (1) {
        difference = clock() - before2;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        if (msec >= trigger2)
        {
            before2 = clock();
            readKey(&chip8);
            fetch(&chip8);
            execute(&chip8);
            render(&chip8);
            difference = clock() - before1;
            msec = difference * 1000 / CLOCKS_PER_SEC;
            if (msec >= trigger1)
            {

                before1 = clock();
                if (chip8.dTimer > 0)
                    --chip8.dTimer;
                if (chip8.sTimer > 0)
                    -
                    -chip8.sTimer;
            }
        }
    }

    return 0;
}