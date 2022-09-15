#include <cassert>
#include <ctime>
#include <fmt/core.h>
#include <iostream>

#include "chip8.h"

chip8::chip8(const uint8_t* program, size_t size)
    : quirks(quirks_profile::CHIP_8)
{
    vram_dirty = true;
    V.fill(0);
    ram.fill(0);
    vram.fill(0);
    stack.fill(0);
    keypad.fill(false);
    I = 0;
    ip = entry_point;
    sp = 0;
    delay_timer = 0;
    sound_timer = 0;
    wait_key = false;
    wait_key_val = std::nullopt;
    wait_draw = false;
    
    std::srand(std::time(nullptr));

    // Load user program
    assert(size < ram_size);
    std::copy(program, program + size, ram.begin() + entry_point);

    // Load fontset
    std::copy(font.begin(), font.end(), ram.begin());

}

[[noreturn]] void chip8::panic(std::string msg)
{
    // FIXME: there should be a panic handler function
    std::cerr << fmt::format("CPU Panic:\n\t{}\n", msg);
    std::cerr << "Register state:\n";
    dump_regs(std::cerr);
    std::cerr << "Stack trace:\n";
    print_stack_trace(std::cerr);
    std::cerr << "Keypad status:\n";
    print_keypad(std::cerr);
    std::abort();
}

void chip8::dump_regs(std::ostream& stream)
{
    stream << fmt::format("ip = {:#06x}\n", ip);
    stream << fmt::format("I = {:#06x}\n", I);
    for (std::size_t i = 0; i < register_number; i += 4)
    {
        stream << fmt::format("V{:02} = {:03}  V{:02} = {:03}  V{:02} = {:03}  V{:02} = {:03}\n",
            i, V[i],
            i + 1, V[i + 1],
            i + 2, V[i + 2],
            i + 3, V[i + 3]);
    }
}

void chip8::print_stack_trace(std::ostream& stream)
{
    for (int i = sp; i >= 0; i--)
        stream << fmt::format("{:02} -> {:#06x}\n", i, stack[i]);
}

void chip8::print_keypad(std::ostream& stream)
{
    for (std::size_t i = 0; i < keypad_size; i += 4)
    {
        stream << fmt::format("{:#04x} {} {:#04x} {} {:#04x} {} {:#04x} {}\n",
            i, keypad[i] ? "down" : "  up",
            i + 1, keypad[i + 1] ? "down" : "  up",
            i + 2, keypad[i + 2] ? "down" : "  up",
            i + 3, keypad[i + 3] ? "down" : "  up");
    }
}

void chip8::tick(uint8_t instructions_per_frame)
{
    wait_draw = false;

    for (std::size_t i = 0; i < instructions_per_frame; i++)
    {
        uint16_t raw_opcode = fetch_opcode();
        opcode decoded_opcode = opcode::decode(raw_opcode);
        execute(decoded_opcode);
    }

    if (delay_timer > 0)
        delay_timer--;
    if (sound_timer > 0)
        sound_timer--;
}

uint16_t chip8::fetch_opcode()
{
    uint16_t raw_opcode = ram[ip] << 8 | ram[ip + 1];
    ip += 2; // Instructions are 16 bits wide
    
    return raw_opcode;
}

void chip8::execute(opcode opcode)
{
    #ifndef NDEBUG
        fmt::print("opcode:{:#06x} ", opcode.raw_opcode);
        for (std::size_t i = 0; i < register_number; i++)
            fmt::print("V{}={} ", i, V[i]);
        fmt::print("ip={:#06x} ", ip - 2);
        fmt::print("I={:#06x} ", I);
        fmt::print("sp={} ", sp);
        fmt::print("dt={} ", delay_timer);
        fmt::print("st={} \n", sound_timer);
    #endif

    switch (opcode.id)
    {
        case opcode_id::CLEAR: clear();                            break;
        case opcode_id::RET:   ret();                              break;
        case opcode_id::JMP:   jmp(opcode.NNN);                    break;
        case opcode_id::CALL:  call(opcode.NNN);                   break;
        case opcode_id::SKIEQ: skieq(opcode.X, opcode.NN);         break;
        case opcode_id::SKINE: skine(opcode.X, opcode.NN);         break;
        case opcode_id::SKREQ: skreq(opcode.X, opcode.Y);          break;
        case opcode_id::MOVI:  movi(opcode.X, opcode.NN);          break;
        case opcode_id::ADDI:  addi(opcode.X, opcode.NN);          break;
        case opcode_id::MOV:   mov(opcode.X, opcode.Y);            break;
        case opcode_id::OR:    _or(opcode.X, opcode.Y);            break;
        case opcode_id::AND:   _and(opcode.X, opcode.Y);           break;
        case opcode_id::XOR:   _xor(opcode.X, opcode.Y);           break;
        case opcode_id::ADD:   add(opcode.X, opcode.Y);            break;
        case opcode_id::SUB:   sub(opcode.X, opcode.Y);            break;
        case opcode_id::SRL:   srl(opcode.X, opcode.Y);            break;
        case opcode_id::LSUB:  lsub(opcode.X, opcode.Y);           break;
        case opcode_id::SLL:   sll(opcode.X, opcode.Y);            break;
        case opcode_id::SKRNE: skrne(opcode.X, opcode.Y);          break;
        case opcode_id::MOVRI: movri(opcode.NNN);                  break;
        case opcode_id::JMPV0: jmpv0(opcode.NNN, opcode.X);        break;
        case opcode_id::RAND:  rand(opcode.X, opcode.NN);          break;
        case opcode_id::DRAW:  draw(opcode.X, opcode.Y, opcode.N); break;
        case opcode_id::SKKP:  skkp(opcode.X);                     break;
        case opcode_id::SKKNP: skknp(opcode.X);                    break;
        case opcode_id::LOADD: loadd(opcode.X);                    break;
        case opcode_id::WKEY:  wkey(opcode.X);                     break;
        case opcode_id::MOVD:  movd(opcode.X);                     break;
        case opcode_id::MOVS:  movs(opcode.X);                     break;
        case opcode_id::ADDRI: addri(opcode.X);                    break;
        case opcode_id::SPRI:  spri(opcode.X);                     break;
        case opcode_id::BCD:   bcd(opcode.X);                      break;
        case opcode_id::STORE: store(opcode.X);                    break;
        case opcode_id::LOAD:  load(opcode.X);                     break;
        case opcode_id::UNKNOWN_OPCODE:
            panic(fmt::format("Unsupported opcode {:#06x}", opcode.raw_opcode));
            break;
    }
}

void chip8::key_pressed(std::size_t key_code)
{
    keypad[key_code] = true;
}

void chip8::key_released(std::size_t key_code)
{
    if (wait_key)
        wait_key_val = key_code;
    keypad[key_code] = false;
}

void chip8::check_register(uint8_t index, std::string instruction_name)
{
    if (index >= register_number)
        panic(fmt::format("'{}' Invalid register index {:#04x}", instruction_name, index));
}

/* 00E0 -> Clears the screen */
void chip8::clear()
{
    vram.fill(0);
    vram_dirty = true;
}

/* 00EE -> Returns from a subroutine */
void chip8::ret()
{
    if (sp == 0)
        panic("Use of 'ret' instruction but callstack is empty");

    ip = stack[--sp];
}

/* 1NNN -> Jump to address NNN */
void chip8::jmp(uint16_t NNN)
{
    if (NNN > ram_size)
        panic(fmt::format("Can not 'jmp' to invalid address {:#06x}", NNN));

    ip = NNN;
}

/* 2NNN -> Call subroutine at NNN */
void chip8::call(uint16_t NNN)
{
    if (NNN > ram_size)
        panic(fmt::format("Cannot 'call' invalid address {:#06x}", NNN));
    if (sp + 1 >= stack_size)
        panic(fmt::format("Callstack overflow in 'call {:#06x}'", NNN));

    stack[sp++] = ip;
    ip = NNN;
}

/* 3XNN -> Skip next instruction if VX equals NN */
void chip8::skieq(uint8_t X, uint8_t NN)
{
    check_register(X, "skieq");

    if (V[X] == NN)
        ip += 2;
}

/* 4XNN -> Skip next instruction if VX does not equal NN */
void chip8::skine(uint8_t X, uint8_t NN)
{
    check_register(X, "skine");

    if (V[X] != NN)
        ip += 2;
}

/* 5XY0 -> Skip next instruction if VX equals VY */
void chip8::skreq(uint8_t X, uint8_t Y)
{
    check_register(X, "skreq");
    check_register(Y, "skreq");

    if (V[X] == V[Y])
        ip += 2;
}

/* 6XNN -> Sets VX to NN */
void chip8::movi(uint8_t X, uint8_t NN)
{
    check_register(X, "movi");

    V[X] = NN;
}

/* 7XNN -> Adds NN to VX (carry flag is not changed) */
void chip8::addi(uint8_t X, uint8_t NN)
{
    check_register(X, "addi");

    V[X] += NN;
}

/* 8XY0 -> Sets VX to the value of VY */
void chip8::mov(uint8_t X, uint8_t Y)
{
    check_register(X, "mov");
    check_register(Y, "mov");

    V[X] = V[Y];
}

/* 8XY1 -> Sets VX to VX | VY */
void chip8::_or(uint8_t X, uint8_t Y)
{
    check_register(X, "or");
    check_register(Y, "or");

    V[X] |= V[Y];
    if (!quirks.vf_reset)
        V[0xF] = 0;
}

/* 8XY2 -> Sets VX to VX & VY */
void chip8::_and(uint8_t X, uint8_t Y)
{
    check_register(X, "and");
    check_register(Y, "and");

    V[X] &= V[Y];
    if (!quirks.vf_reset)
        V[0xF] = 0;
}

/* 8XY3 -> Sets VX to VX ^ VY */
void chip8::_xor(uint8_t X, uint8_t Y)
{
    check_register(X, "xor");
    check_register(Y, "xor");

    V[X] ^= V[Y];
    if (!quirks.vf_reset)
        V[0xF] = 0;
}

/* 8XY4 -> Adds VY to VX. VF is set to 1 when there's a carry,
 * and to 0 when there is not */
void chip8::add(uint8_t X, uint8_t Y)
{
    check_register(X, "add");
    check_register(Y, "add");

    uint8_t flag = V[Y] > (UINT8_MAX - V[X]);
    V[X] += V[Y];
    V[0xF] = flag;
}

/* 8XY5 -> VY is subtracted from VX. VF is set to 0 when there's
 * a borrow, and 1 when there is not */
void chip8::sub(uint8_t X, uint8_t Y)
{
    check_register(X, "sub");
    check_register(Y, "sub");

    uint8_t flag = V[Y] > V[X] ? 0 : 1;
    V[X] -= V[Y];
    V[0xF] = flag;
}

/* 8XY6 -> Stores the least significant bit of VX in VF and then
 * shifts VX to the right by 1 */
void chip8::srl(uint8_t X, uint8_t Y)
{
    check_register(X, "srl");
    check_register(Y, "srl");

    if (quirks.shifting)
        Y = X;
    uint8_t flag = V[Y] & 0x01;
    V[X] = V[Y] >> 1;
    V[0xF] = flag;
}

/* 8XY7 -> Sets VX to VY minus VX. VF is set to 0 when there's a
 * borrow, and 1 when there is not */
void chip8::lsub(uint8_t X, uint8_t Y)
{
    check_register(X, "lsub");
    check_register(Y, "lsub");

    uint8_t flag = V[X] > V[Y] ? 0 : 1;
    V[X] = V[Y] - V[X];
    V[0xF] = flag;
}

/* 8XYE -> Stores the most significant bit of VX in VF and then
 * shifts VX to the left by 1 */
void chip8::sll(uint8_t X, uint8_t Y)
{
    check_register(X, "sll");
    check_register(Y, "sll");

    if (quirks.shifting)
        Y = X;
    uint8_t flag = (V[Y] & 0x80) ? 1 : 0;
    V[X] = V[Y] << 1;
    V[0xF] = flag;
}
/* 9XY0 -> Skips the next instruction if VX does not equal VY */
void chip8::skrne(uint8_t X, uint8_t Y)
{
    check_register(X, "skrne");
    check_register(Y, "skrne");

    if (V[X] != V[Y])
        ip += 2;
}

/* ANNN -> Sets I to the address NNN */
void chip8::movri(uint16_t NNN)
{
    I = NNN;
}

/* BNNN -> Jumps to the address NNN + VO */
void chip8::jmpv0(uint16_t NNN, uint8_t X)
{
    uint8_t reg = 0;
    if (quirks.jumping)
    {
        check_register(X, "jmpv0");
        reg = X;
    }
    ip = V[reg] + NNN;
}

/* CXNN -> Sets VX to the result of a bitwise and operation  on a random
 * number and NN */
void chip8::rand(uint8_t X, uint8_t NN)
{
    check_register(X, "rand");

    V[X] = generate_random() & NN;
}

/* DXYN -> Draws a sprite at coordinate (VX, VY) that has a width of 8
 * pixels and a height of N pixels. Each row of 8 pixels is read as
 * bit-coded starting from memory location I; I value does not change after
 * the execution of this instruction. As described above, VF is set to 1 if
 * any screen pixels are flipped from set to unset when the sprite is drawn,
 * and to 0 if that does not happen */
void chip8::draw(uint8_t X, uint8_t Y, uint8_t N)
{
    check_register(X, "draw");
    check_register(Y, "draw");

    if (!quirks.display_wait && wait_draw)
    {
        ip -= 2;
        return;
    }

    if (!quirks.clipping)
    {
        V[X] %= screen_width;
        V[Y] %= screen_height;
    }

    V[0xF] = 0;
    for (std::size_t d_y = 0; d_y < N; d_y++)
    {
        // FIXME: there is probably a better way implement this quirk
        if (!quirks.clipping && V[Y] + d_y >= screen_height)
            break;
        std::size_t line_start = (V[Y] + d_y) * chip8::screen_width + V[X];
        for (std::size_t d_x = 0; d_x < 8; d_x++)
        {
            // FIXME: there is probably a better way implement this quirk
            if (!quirks.clipping && V[X] + d_x >= screen_width)
                break;
            assert(line_start + d_x < vram.size());

            uint8_t old_pixel = vram[line_start + d_x];
            vram[line_start + d_x] ^= bool(ram[I + d_y] & (0x80 >> d_x));
            V[0xF] |= old_pixel && !vram[line_start + d_x];
        }
    }
    vram_dirty = true;
    wait_draw = true;
}

/* EX9E -> Skips the next instruction if the key stored in VX is
 * pressed */
void chip8::skkp(uint8_t X)
{
    check_register(X, "skkp");

    if (keypad[V[X]])
        ip += 2;
}

/* EXA1 - >Skips the next instruction if the key stored in VX is
 * not pressed */
void chip8::skknp(uint8_t X)
{
    check_register(X, "skknp");

    if (!keypad[V[X]])
        ip += 2;
}

/* FX07 -> Sets VX to the value of the delay timer */
void chip8::loadd(uint8_t X)
{
    check_register(X, "loadd");

    V[X] = delay_timer;
}

/* FX0A -> A key press is awaited, and then stored in VX.
 * (Blocking Operation. All instruction halted until next key event) */
void chip8::wkey(uint8_t X)
{
    check_register(X, "wkey");

    if (!wait_key || (wait_key && !wait_key_val))
    {
        wait_key = true;
        ip -= 2;
    }
    else
    {
        wait_key = false;
        V[X] = *wait_key_val;
        wait_key_val.reset();
    }
}

/* FX15 -> Sets the delay timer to VX */
void chip8::movd(uint8_t X)
{
    check_register(X, "movd");

    delay_timer = V[X];
}

/* FX18 -> Sets the sound timer to VX */
void chip8::movs(uint8_t X)
{
    check_register(X, "movs");

    sound_timer = V[X];
}

/* FX1E -> Adds VX to I. VF is not affected */
void chip8::addri(uint8_t X)
{
    check_register(X, "addri");

    I += V[X];
}

/* FX29 -> Sets I to the location of the sprite for the character in VX.
 * Characters 0-F (in hexadecimal) are represented by a 4x5 font */
void chip8::spri(uint8_t X)
{
    check_register(X, "spri");

    I = V[X] * 0x5;
}

 /* FX33 -> Stores the binary-coded decimal representation of VX, with
 * the most significant of three digits at the address in I, the middle
 * digit at I plus 1, and the least significant digit at I plus 2.
 * (In other words, take the decimal representation of VX, place the
 * hundreds digit in memory at location in I, the tens digit at location
 * I+1, and the ones digit at location I+2.) */
void chip8::bcd(uint8_t X)
{
    check_register(X, "bcd");

    ram[I] = V[X] / 100;
    ram[I + 1] = (V[X] / 10) % 10;
    ram[I + 2] = V[X] % 10;
}

/* FX55 -> Stores from V0 to VX (including VX) in memory, starting at
 * address I. The offset from I is increased by 1 for each value written,
 * but I itself is left unmodified */
void chip8::store(uint8_t X)
{
    check_register(X, "store");

    for (std::size_t i = 0; i <= X; i++)
        ram[I + i] = V[i];
    if (!quirks.mem)
        I += X + 1;
}

/* FX65 -> Fills from V0 to VX (including VX) with values from memory,
 * starting at address I. The offset from I is increased by 1 for each
 * value written, but I itself is left unmodified */
void chip8::load(uint8_t X)
{
    check_register(X, "load");

    for (std::size_t i = 0; i <= X; i++)
        V[i] = ram[I + i];
    if (!quirks.mem)
        I += X + 1;
}
