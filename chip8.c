#include"chip8.h"
#define _CRT_SECURE_NO_WARNINGS
void init(Chip8* chip8) {
    memset(chip8, 0, sizeof(Chip8));
    chip8->PC = 0x200;
    uint8_t  fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xF0, 0x10, 0xF0, 0x80, 0xF0,
        0xF0, 0x10, 0xF0, 0x10, 0xF0,
        0x90, 0x90, 0xF0, 0x10, 0x10,
        0xF0, 0x80, 0xF0, 0x10, 0xF0,
        0xF0, 0x80, 0xF0, 0x90, 0xF0,
        0xF0, 0x10, 0x20, 0x40, 0x40,
        0xF0, 0x90, 0xF0, 0x90, 0xF0,
        0xF0, 0x90, 0xF0, 0x10, 0xF0,
        0xF0, 0x90, 0xF0, 0x90, 0x90,
        0xE0, 0x90, 0xE0, 0x90, 0xE0,
        0xF0, 0x80, 0x80, 0x80, 0xF0,
        0xE0, 0x90, 0x90, 0x90, 0xE0,
        0xF0, 0x80, 0x80, 0x80, 0xF0,
        0xF0, 0x80, 0x80, 0x80, 0x80
    };
    memcpy(chip8->RAM + 0x50, fontset, 80 * sizeof(fontset[0]));
}

void loadFromROM(Chip8* chip8, const char* fileName)
{
    FILE* file;
    file = fopen(fileName, "rb");

    if (file == NULL) {
        perror("File opening error");
        exit(1);
    }

    fread(chip8->RAM + 0x200u, 1, sizeof(chip8->RAM) - sizeof(chip8->RAM[0]) * 0x200u, file);
    fclose(file);
}

void fetch(Chip8* chip8) {
    chip8->opcode = chip8->RAM[chip8->PC] << 8 | chip8->RAM[chip8->PC + 1];
}

void execute(Chip8* chip8) {
    switch (chip8->opcode & 0xF000u) {
    case 0x0000u:
        switch (chip8->opcode) {
        case 0x00E0u:
            CLS(chip8);
            break;
        case 0x00EE:
            RET(chip8);
            break;
        }
        break;
    case 0x1000u:
        JP_addr(chip8);
        break;
    case 0x2000u:
        CALL_addr(chip8);
        break;
    case 0x3000u:
        SE_Vx_byte(chip8);
        break;
    case 0x4000u:
        SNE_Vx_byte(chip8);
        break;
    case 0x5000u:
        SE_Vx_Vy(chip8);
        break;
    case 0x6000u:
        LD_Vx_byte(chip8);
        break;
    case 0x7000u:
        ADD_Vx_byte(chip8);
        break;
    case 0x8000u:
        switch (chip8->opcode & 0x000F) {
        case 0x0u:
            LD_Vx_Vy(chip8);
            break;
        case 0x1u:
            OR_Vx_Vy(chip8);
            break;
        case 0x2u:
            AND_Vx_Vy(chip8);
            break;
        case 0x3u:
            XOR_Vx_Vy(chip8);
            break;
        case 0x4u:
            ADD_Vx_Vy(chip8);
            break;
        case 0x5u:
            SUB_Vx_Vy(chip8);
            break;
        case 0x6u:
            SHR_Vx(chip8);
            break;
        case 0x7u:
            SUBN_Vx_Vy(chip8);
            break;
        case 0xEu:
            SHL_Vx_Vy(chip8);
            break;
        }
        break;
    case 0x9000u:
        SNE_Vx_Vy(chip8);
        break;
    case 0xA000u:
        LD_I_addr(chip8);
        break;
    case 0xB000u:
        JP_V0_addr(chip8);
        break;
    case 0xC000u:
        RND_Vx_byte(chip8);
        break;
    case 0xD000u:
        DRW_Vx_Vy_n(chip8);
        break;
    case 0xE000u:
        switch (chip8->opcode & 0x00FFu) {
        case 0x9Eu:
            SKP_Vx(chip8);
            break;
        case 0xA1u:
            SKNP_Vx(chip8);
            break;
        }
        break;
    case 0xF000u:
        switch (chip8->opcode & 0x00FFu) {
        case 0x07:
            LD_Vx_DT(chip8);
            break;
        case 0x0A:
            LD_Vx_K(chip8);
            break;
        case 0x15:
            LD_DT_Vx(chip8);
            break;
        case 0x18:
            LD_ST_Vx(chip8);
            break;
        case 0x1E:
            ADD_I_Vx(chip8);
            break;
        case 0x29:
            LD_F_Vx(chip8);
            break;
        case 0x33:
            LD_B_Vx(chip8);
            break;
        case 0x55:
            LD_I_Vx(chip8);
            break;
        case 0x65:
            LD_Vx_I(chip8);
            break;
        }
        break;
    }

}

void readKey(Chip8* chip8) {
    if (GetAsyncKeyState('0'))
        chip8->keypad[0] = 1;
    else
        chip8->keypad[0] = 0;
    if (GetAsyncKeyState('1'))
        chip8->keypad[1] = 1;
    else
        chip8->keypad[1] = 0;
    if (GetAsyncKeyState('2'))
        chip8->keypad[2] = 1;
    else
        chip8->keypad[2] = 0;
    if (GetAsyncKeyState('3'))
        chip8->keypad[3] = 1;
    else
        chip8->keypad[3] = 0;
    if (GetAsyncKeyState('Q'))
        chip8->keypad[4] = 1;
    else
        chip8->keypad[4] = 0;
    if (GetAsyncKeyState('W'))
        chip8->keypad[5] = 1;
    else
        chip8->keypad[5] = 0;
    if (GetAsyncKeyState('E'))
        chip8->keypad[6] = 1;
    else
        chip8->keypad[6] = 0;
    if (GetAsyncKeyState('A'))
        chip8->keypad[7] = 1;
    else
        chip8->keypad[7] = 0;
    if (GetAsyncKeyState('S'))
        chip8->keypad[8] = 1;
    else
        chip8->keypad[8] = 0;
    if (GetAsyncKeyState('D'))
        chip8->keypad[9] = 1;
    else
        chip8->keypad[9] = 0;
    if (GetAsyncKeyState('Z'))
        chip8->keypad[10] = 1;
    else
        chip8->keypad[10] = 0;
    if (GetAsyncKeyState('C'))
        chip8->keypad[11] = 1;
    else
        chip8->keypad[11] = 0;
    if (GetAsyncKeyState('4'))
        chip8->keypad[12] = 1;
    else
        chip8->keypad[12] = 0;
    if (GetAsyncKeyState('R'))
        chip8->keypad[13] = 1;
    else
        chip8->keypad[13] = 0;
    if (GetAsyncKeyState('F'))
        chip8->keypad[14] = 1;
    else
        chip8->keypad[14] = 0;
    if (GetAsyncKeyState('V'))
        chip8->keypad[15] = 1;
    else
        chip8->keypad[15] = 0;
    if (GetAsyncKeyState(VK_ESCAPE))
        exit(0);
}

void render(Chip8* chip8) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { 0, 0 };
    SetConsoleCursorPosition(hOut, pos);

    char buffer[32 * (64 + 1) + 1];
    size_t idx = 0;

    for (size_t i = 0; i < 32; ++i) {
        for (size_t j = 0; j < 64; ++j) {
            buffer[idx++] = chip8->video[i][j] ? L'█' : ' ';
        }
        buffer[idx++] = '\n';
    }
    buffer[idx] = '\0';

    DWORD written;
    WriteConsoleA(hOut, buffer, (DWORD)idx, &written, NULL);
}

void CLS(Chip8* chip8) {
    for (size_t i = 0; i < 32; ++i)
        for (size_t j = 0; j < 64; ++j)
            chip8->video[i][j] = 0;
    chip8->PC += 2;
}
void RET(Chip8* chip8) {
    --chip8->sp;
    chip8->PC = chip8->stack[chip8->sp];
    chip8->PC += 2;
}
void JP_addr(Chip8* chip8) {
    chip8->PC = (chip8->opcode & 0x0FFFu);

}
void CALL_addr(Chip8* chip8) {
    chip8->stack[chip8->sp] = chip8->PC;
    ++chip8->sp;
    chip8->PC = (chip8->opcode & 0x0FFFu);

}
void SE_Vx_byte(Chip8* chip8) {
    uint8_t byte = chip8->opcode & 0x00FFu;
    uint8_t numOfReg = (chip8->opcode & 0x0F00u) >> 8u;
    if (byte == chip8->regs[numOfReg])
        chip8->PC += 2;
    chip8->PC += 2;
}
void SNE_Vx_byte(Chip8* chip8) {
    uint8_t byte = chip8->opcode & 0x00FFu;
    uint8_t numOfReg = (chip8->opcode & 0x0F00u) >> 8u;
    if (byte != chip8->regs[numOfReg])
        chip8->PC += 2;
    chip8->PC += 2;
}
void SE_Vx_Vy(Chip8* chip8) {
    uint8_t numOfRegX = (chip8->opcode & 0x0F00u) >> 8u;
    uint8_t numOfRegY = (chip8->opcode & 0x00F0u) >> 4u;
    if (chip8->regs[numOfRegX] == chip8->regs[numOfRegY])
        chip8->PC += 2;
    chip8->PC += 2;
}
void LD_Vx_byte(Chip8* chip8) {
    uint8_t byte = chip8->opcode & 0x00FFu;
    uint8_t numOfReg = (chip8->opcode & 0x0F00u) >> 8u;
    chip8->regs[numOfReg] = byte;
    chip8->PC += 2;
}
void ADD_Vx_byte(Chip8* chip8) {
    uint8_t byte = chip8->opcode & 0x00FFu;
    uint8_t numOfReg = (chip8->opcode & 0x0F00u) >> 8u;
    chip8->regs[numOfReg] += byte;
    chip8->PC += 2;
}
void LD_Vx_Vy(Chip8* chip8) {
    uint8_t numOfRegX = (chip8->opcode & 0x0F00u) >> 8u;
    uint8_t numOfRegY = (chip8->opcode & 0x00F0u) >> 4u;
    chip8->regs[numOfRegX] = chip8->regs[numOfRegY];
    chip8->PC += 2;
}
void OR_Vx_Vy(Chip8* chip8) {
    uint8_t numOfRegX = (chip8->opcode & 0x0F00u) >> 8u;
    uint8_t numOfRegY = (chip8->opcode & 0x00F0u) >> 4u;
    chip8->regs[numOfRegX] |= chip8->regs[numOfRegY];
    chip8->PC += 2;
}
void AND_Vx_Vy(Chip8* chip8) {
    uint8_t numOfRegX = (chip8->opcode & 0x0F00u) >> 8u;
    uint8_t numOfRegY = (chip8->opcode & 0x00F0u) >> 4u;
    chip8->regs[numOfRegX] &= chip8->regs[numOfRegY];
    chip8->PC += 2;
}
void XOR_Vx_Vy(Chip8* chip8) {
    uint8_t numOfRegX = (chip8->opcode & 0x0F00u) >> 8u;
    uint8_t numOfRegY = (chip8->opcode & 0x00F0u) >> 4u;
    chip8->regs[numOfRegX] ^= chip8->regs[numOfRegY];
    chip8->PC += 2;
}
void ADD_Vx_Vy(Chip8* chip8) {
    uint8_t numOfRegX = (chip8->opcode & 0x0F00u) >> 8u;
    uint8_t numOfRegY = (chip8->opcode & 0x00F0u) >> 4u;
    uint16_t summ = chip8->regs[numOfRegX] + chip8->regs[numOfRegY];
    if (summ > 255U)
        chip8->regs[0xF] = 1;
    else
        chip8->regs[0xF] = 0;
    chip8->regs[numOfRegX] = summ & 0xFF;
    chip8->PC += 2;
}
void SUB_Vx_Vy(Chip8* chip8) {
    uint8_t numOfRegX = (chip8->opcode & 0x0F00u) >> 8u;
    uint8_t numOfRegY = (chip8->opcode & 0x00F0u) >> 4u;
    if (chip8->regs[numOfRegX] > chip8->regs[numOfRegY])
        chip8->regs[0xF] = 1;
    else
        chip8->regs[0xF] = 0;
    chip8->regs[numOfRegX] -= chip8->regs[numOfRegY];
    chip8->PC += 2;
}
void SHR_Vx(Chip8* chip8) {
    uint8_t numOfReg = (chip8->opcode & 0x0F00u) >> 8u;
    chip8->regs[0xF] = chip8->regs[numOfReg] & 0x1u;
    chip8->regs[numOfReg] >>= 1;
    chip8->PC += 2;
}
void SUBN_Vx_Vy(Chip8* chip8) {
    uint8_t numOfRegX = (chip8->opcode & 0x0F00u) >> 8u;
    uint8_t numOfRegY = (chip8->opcode & 0x00F0u) >> 4u;
    if (chip8->regs[numOfRegY] > chip8->regs[numOfRegX])
        chip8->regs[0xF] = 1;
    else
        chip8->regs[0xF] = 0;
    chip8->regs[numOfRegX] = chip8->regs[numOfRegY] - chip8->regs[numOfRegX];
    chip8->PC += 2;
}
void SHL_Vx_Vy(Chip8* chip8) {
    uint8_t numOfReg = (chip8->opcode & 0x0F00u) >> 8u;
    chip8->regs[0xF] = (chip8->regs[numOfReg] & 0x80u) >> 7u;
    chip8->regs[numOfReg] <<= 1;
    chip8->PC += 2;
}
void SNE_Vx_Vy(Chip8* chip8) {
    uint8_t numOfRegX = (chip8->opcode & 0x0F00u) >> 8u;
    uint8_t numOfRegY = (chip8->opcode & 0x00F0u) >> 4u;
    if (chip8->regs[numOfRegX] != chip8->regs[numOfRegY])
        chip8->PC += 2;
    chip8->PC += 2;
}
void LD_I_addr(Chip8* chip8) {
    chip8->IR = (chip8->opcode & 0x0FFFu);
    chip8->PC += 2;
}
void JP_V0_addr(Chip8* chip8) {
    chip8->PC = chip8->regs[0] + (chip8->opcode & 0x0FFFu);
}
void RND_Vx_byte(Chip8* chip8) {
    srand(time(NULL));
    uint8_t rnd = rand() % 256;
    uint8_t numOfReg = (chip8->opcode & 0x0F00u) >> 8u;
    uint8_t byte = chip8->opcode & 0x00FFu;
    chip8->regs[numOfReg] = rnd & byte;
    chip8->PC += 2;
}
void DRW_Vx_Vy_n(Chip8* chip8) {
    uint8_t n = chip8->opcode & 0x000Fu;
    uint8_t numOfRegX = (chip8->opcode & 0x0F00u) >> 8u;
    uint8_t numOfRegY = (chip8->opcode & 0x00F0u) >> 4u;
    uint8_t Vx = chip8->regs[numOfRegX];
    uint8_t Vy = chip8->regs[numOfRegY];
    chip8->regs[0xF] = 0;
    for (size_t i = 0;i < n;++i) {
        uint8_t sprite = chip8->RAM[chip8->IR + i];
        for (size_t j = 0; j < 8; ++j) {
            uint8_t pixel = (sprite >> (7 - j)) & 1;
            if (pixel) {
                size_t x = (Vx + j) % 64;
                size_t y = (Vy + i) % 32;

                if (chip8->video[y][x])
                    chip8->regs[0xF] = 1;

                chip8->video[y][x] ^= 1;
            }
        }
    }
    chip8->PC += 2;
}
void SKP_Vx(Chip8* chip8) {
    uint8_t numOfReg = (chip8->opcode & 0x0F00) >> 8u;
    uint8_t Vx = chip8->regs[numOfReg];
    if (chip8->keypad[Vx])
        chip8->PC += 2;
    chip8->PC += 2;
}
void SKNP_Vx(Chip8* chip8) {
    uint8_t numOfReg = (chip8->opcode & 0x0F00) >> 8u;
    uint8_t Vx = chip8->regs[numOfReg];
    if (!chip8->keypad[Vx])
        chip8->PC += 2;
    chip8->PC += 2;
}
void LD_Vx_DT(Chip8* chip8) {
    uint8_t numOfReg = (chip8->opcode & 0x0F00) >> 8u;
    chip8->regs[numOfReg] = chip8->dTimer;
    chip8->PC += 2;
}
void LD_Vx_K(Chip8* chip8) {
    uint8_t numOfReg = (chip8->opcode & 0x0F00) >> 8u;
    for (size_t i = 0; i < 16; ++i) {
        if (chip8->keypad[i]) {
            chip8->regs[numOfReg] = i;
            chip8->PC += 2;
            break;
        }
    }
}
void LD_DT_Vx(Chip8* chip8) {
    uint8_t numOfReg = (chip8->opcode & 0x0F00) >> 8u;
    chip8->dTimer = chip8->regs[numOfReg];
    chip8->PC += 2;
}
void LD_ST_Vx(Chip8* chip8) {
    uint8_t numOfReg = (chip8->opcode & 0x0F00) >> 8u;
    chip8->sTimer = chip8->regs[numOfReg];
    chip8->PC += 2;
}
void ADD_I_Vx(Chip8* chip8) {
    uint8_t numOfReg = (chip8->opcode & 0x0F00) >> 8u;
    chip8->IR += chip8->regs[numOfReg];
    chip8->PC += 2;
}
void LD_F_Vx(Chip8* chip8) {
    uint8_t numOfReg = (chip8->opcode & 0x0F00) >> 8u;
    chip8->IR = 0x50 + (5 * chip8->regs[numOfReg]);
    chip8->PC += 2;
}
void LD_B_Vx(Chip8* chip8) {
    uint8_t numOfReg = (chip8->opcode & 0x0F00) >> 8u;
    uint8_t Vx = chip8->regs[numOfReg];
    chip8->RAM[chip8->IR + 2] = Vx % 10;
    Vx /= 10;
    chip8->RAM[chip8->IR + 1] = Vx % 10;
    Vx /= 10;
    chip8->RAM[chip8->IR] = Vx % 10;
    chip8->PC += 2;
}
void LD_I_Vx(Chip8* chip8) {
    uint8_t numOfReg = (chip8->opcode & 0x0F00) >> 8u;
    for (size_t i = 0; i <= numOfReg; ++i)
        chip8->RAM[chip8->IR + i] = chip8->regs[i];
    chip8->PC += 2;
}
void LD_Vx_I(Chip8* chip8) {
    uint8_t numOfReg = (chip8->opcode & 0x0F00) >> 8u;
    for (size_t i = 0; i <= numOfReg; ++i)
        chip8->regs[i] = chip8->RAM[chip8->IR + i];
    chip8->PC += 2;
}