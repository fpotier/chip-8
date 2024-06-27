#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <vector>

#include "chip8.h"
#include "cpu_quirks.h"
#include "opcode.h"

class Chip8
{
    friend class chip8_test;

public:
    static constexpr int RAM_SIZE = 4096;
    static constexpr int SCREEN_WIDTH = 64;
    static constexpr int SCREEN_HEIGHT = 32;
    static constexpr int VRAM_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;
    static constexpr int NB_REGISTER = 16;
    static constexpr int STACK_SIZE = 16;
    static constexpr int KEYPAD_SIZE = 16;
    static constexpr int NO_KEY_PRESSED = -1;
    static constexpr int ENTRYPOINT = 0x200;
    static constexpr std::array<uint8_t, 80> DEFAULT_FONT
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0x70, 0x80, 0x80, 0x80, 0x70, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xE0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xE0, 0x80, 0x80  // F
    };

    bool vram_dirty;
    Chip8(std::vector<uint8_t> const& rom);

    [[noreturn]] void panic(std::string msg);
    void dump_regs(std::ostream& stream);
    void print_stack_trace(std::ostream& stream);
    void print_keypad(std::ostream& stream);
    void tick(uint8_t instructions_per_frame);
    uint16_t fetch_opcode();
    void execute(Opcode opcode);
    uint8_t generate_random() { return std::rand() % UINT8_MAX; }
    void key_pressed(std::size_t key_code);
    void key_released(std::size_t key_code);

    std::array<uint8_t, NB_REGISTER> const& get_registers() const { return V; }
    std::array<uint8_t, RAM_SIZE> const& get_ram() const { return ram; }
    std::array<uint8_t, VRAM_SIZE> const& get_vram() const { return vram; }
    std::array<uint16_t, STACK_SIZE> const& get_stack() const { return stack; }
    uint16_t get_I() const { return I; }
    uint16_t get_ip() const { return ip; }
    uint8_t get_sp() const { return sp; }
    std::array<bool, KEYPAD_SIZE> get_keypad() const { return keypad; }
    uint8_t get_sound_timer() const { return sound_timer; }
    uint8_t get_delay_timer() const { return delay_timer; }

private:
    std::array<uint8_t, NB_REGISTER> V;
    std::array<uint8_t, RAM_SIZE> ram;
    std::array<uint8_t, VRAM_SIZE> vram;
    std::array<uint16_t, STACK_SIZE> stack;
    std::array<bool, KEYPAD_SIZE> keypad;
    uint16_t I;  // Index register
    uint16_t ip; // Instruction pointer
    uint8_t sp; // Stack pointer
    uint8_t delay_timer;
    uint8_t sound_timer;
    CpuQuirks quirks;
    bool wait_key;
    std::optional<uint8_t> wait_key_val;
    bool wait_draw;

    void clear();
    void ret();
    void jmp(uint16_t NNN);
    void call(uint16_t NNN);
    void skieq(uint8_t X, uint8_t NN);
    void skine(uint8_t X, uint8_t NN);
    void skreq(uint8_t X, uint8_t Y);
    void movi(uint8_t X, uint8_t NN);
    void addi(uint8_t X, uint8_t NN);
    void mov(uint8_t X, uint8_t Y);
    void _or(uint8_t X, uint8_t Y);
    void _and(uint8_t X, uint8_t Y);
    void _xor(uint8_t X, uint8_t Y);
    void add(uint8_t X, uint8_t Y);
    void sub(uint8_t X, uint8_t Y);
    void srl(uint8_t X, uint8_t Y);
    void lsub(uint8_t X, uint8_t Y);
    void sll(uint8_t X, uint8_t Y);
    void skrne(uint8_t X, uint8_t Y);
    void movri(uint16_t NNN);
    void jmpv0(uint16_t NNN, uint8_t X);
    void rand(uint8_t X, uint8_t NN);
    void draw(uint8_t X, uint8_t Y, uint8_t N);
    void skkp(uint8_t X);
    void skknp(uint8_t X);
    void loadd(uint8_t X);
    void wkey(uint8_t X);
    void movd(uint8_t X);
    void movs(uint8_t X);
    void addri(uint8_t X);
    void spri(uint8_t X);
    void bcd(uint8_t X);
    void store(uint8_t X);
    void load(uint8_t X);

    void check_register(uint8_t index, std::string instruction_name);
};
