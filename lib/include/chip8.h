#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "chip8.h"
#include "cpu_quirks.h"
#include "opcode.h"

class chip8
{
    friend class chip8_test;

public:
    static constexpr int ram_size = 4096;
    static constexpr int screen_width = 64;
    static constexpr int screen_height = 32;
    static constexpr int vram_size = screen_width * screen_height;
    static constexpr int register_number = 16;
    static constexpr int stack_size = 16;
    static constexpr int keypad_size = 16;
    static constexpr int no_key_pressed = -1;
    static constexpr int entry_point = 512;
    static constexpr std::array<uint8_t, 80> font
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
    chip8(const uint8_t* program, size_t size);

    [[noreturn]] void panic(std::string msg);
    void dump_regs(std::ostream& stream);
    void print_stack_trace(std::ostream& stream);
    void print_keypad(std::ostream& stream);
    void tick();
    uint16_t fetch_opcode();
    void execute(opcode_info opcode);
    uint8_t generate_random() { return std::rand() % UINT8_MAX; }
    void key_pressed(std::size_t key_code)  { keypad[key_code] = true; }
    void key_released(std::size_t key_code) { keypad[key_code] = false; }

    std::array<uint8_t, register_number> const& get_registers() const { return V; }
    std::array<uint8_t, ram_size> const& get_ram() const { return ram; }
    std::array<uint8_t, vram_size> const& get_vram() const { return vram; }
    std::array<uint16_t, stack_size> const& get_stack() const { return stack; }
    uint16_t get_I() const { return I; }
    uint16_t get_ip() const { return ip; }
    uint8_t get_sp() const { return sp; }
    std::array<bool, keypad_size> get_keypad() const { return keypad; }
    uint8_t get_sound_timer() const { return sound_timer; }
    uint8_t get_delay_timer() const { return delay_timer; }

private:
    std::array<uint8_t, register_number> V;
    std::array<uint8_t, ram_size> ram;
    std::array<uint8_t, vram_size> vram;
    std::array<uint16_t, stack_size> stack;
    std::array<bool, keypad_size> keypad;
    uint16_t I;  // Index register
    uint16_t ip; // Instruction pointer
    uint8_t sp; // Stack pointer
    uint8_t delay_timer;
    uint8_t sound_timer;
    cpu_quirks quirks;

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
