use egui::Key;
use std::collections::HashMap;

pub type KeyMap = HashMap<Key, usize>;
pub struct EmulatorState {
    pub has_rom_loaded: bool,
    pub is_paused: bool,
    pub instruction_per_frame: u8,
    pub keymap: KeyMap,
}

impl Default for EmulatorState {
    fn default() -> Self {
        let keymap: KeyMap = [
            (Key::Num1, 1),
            (Key::Num2, 2),
            (Key::Num3, 3),
            (Key::Num4, 12),
            (Key::Q, 4),
            (Key::W, 5),
            (Key::E, 6),
            (Key::R, 13),
            (Key::A, 7),
            (Key::S, 8),
            (Key::D, 9),
            (Key::F, 14),
            (Key::Z, 10),
            (Key::X, 0),
            (Key::C, 11),
            (Key::V, 15),
        ]
        .iter()
        .cloned()
        .collect();

        Self {
            has_rom_loaded: false,
            is_paused: true,
            instruction_per_frame: 10,
            keymap,
        }
    }
}
