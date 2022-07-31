#pragma once

// https://chip-8.github.io/extensions/

enum class quirks_profile
{
    CHIP_8,
    SCHIP,
    XO_CHIP,
};

struct cpu_quirks
{
    /* The AND, OR and XOR opcodes (8XY1, 8XY2 and 8XY3) reset the flags 
     * register to zero */
    bool vf_reset;
    // https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#fx55-and-fx65-store-and-load-memory
    bool mem;
    // https://laurencescotford.com/chip-8-on-the-cosmac-vip-drawing-sprites/
    bool display_wait;
    bool clipping;
    // https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#8xy6-and-8xye-shift
    bool shifting;
    // https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#bnnn-jump-with-offset
    bool jumping;

    cpu_quirks(quirks_profile profile)
    {
        switch (profile)
        {
            // Consider that original implementation has no quirk
            case quirks_profile::CHIP_8:
                vf_reset = false;
                mem = false;
                display_wait = false;
                clipping = false;
                shifting = false;
                jumping = false;
                break;
            case quirks_profile::SCHIP:
                vf_reset = true;
                mem = true;
                display_wait = true;
                clipping = true;
                shifting = true;
                jumping = true;
                break;
            case quirks_profile::XO_CHIP:
                vf_reset = true;
                mem = true;
                display_wait = true;
                clipping = true;
                shifting = true;
                jumping = true;
                break;
        }
    }
};