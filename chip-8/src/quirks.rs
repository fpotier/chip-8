pub struct Quirks {
    pub vf_reset: bool,
    pub memory: bool,
    pub display_wait: bool,
    pub clipping: bool,
    pub shifting: bool,
    pub jumping: bool,
}

pub const CHIP8_QUIRKS: Quirks = Quirks {
    vf_reset: true,
    memory: true,
    display_wait: true,
    clipping: false,
    shifting: false,
    jumping: false,
};
