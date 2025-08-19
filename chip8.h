#ifndef CHIP8_H
#define CHIP8_H
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdint.h>
#include<string.h>
#include<windows.h>

#endif

typedef struct {
    uint8_t regs[16];
    uint8_t RAM[4096];
    uint16_t IR;
    uint16_t PC;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t dTimer;
    uint8_t sTimer;
    uint8_t keypad[16];
    uint8_t video[32][64];
    uint16_t opcode;
} Chip8;

void init(Chip8* chip8);
void loadFromROM(Chip8* chip8, const char* fileName);
void fetch(Chip8* chip8);
void execute(Chip8* chip8);
void readKey(Chip8* chip8);
void render(Chip8* chip8);
void CLS(Chip8* chip8);
void RET(Chip8* chip8);
void JP_addr(Chip8* chip8);
void CALL_addr(Chip8* chip8);
void SE_Vx_byte(Chip8* chip8);
void SNE_Vx_byte(Chip8* chip8);
void SE_Vx_Vy(Chip8* chip8);
void LD_Vx_byte(Chip8* chip8);
void ADD_Vx_byte(Chip8* chip8);
void LD_Vx_Vy(Chip8* chip8);
void OR_Vx_Vy(Chip8* chip8);
void AND_Vx_Vy(Chip8* chip8);
void XOR_Vx_Vy(Chip8* chip8);
void ADD_Vx_Vy(Chip8* chip8);
void SUB_Vx_Vy(Chip8* chip8);
void SHR_Vx(Chip8* chip8);
void SUBN_Vx_Vy(Chip8* chip8);
void SHL_Vx_Vy(Chip8* chip8);
void SNE_Vx_Vy(Chip8* chip8);
void LD_I_addr(Chip8* chip8);
void JP_V0_addr(Chip8* chip8);
void RND_Vx_byte(Chip8* chip8);
void DRW_Vx_Vy_n(Chip8* chip8);
void SKP_Vx(Chip8* chip8);
void SKNP_Vx(Chip8* chip8);
void LD_Vx_DT(Chip8* chip8);
void LD_Vx_K(Chip8* chip8);
void LD_DT_Vx(Chip8* chip8);
void LD_ST_Vx(Chip8* chip8);
void ADD_I_Vx(Chip8* chip8);
void LD_F_Vx(Chip8* chip8);
void LD_B_Vx(Chip8* chip8);
void LD_I_Vx(Chip8* chip8);
void LD_Vx_I(Chip8* chip8);



