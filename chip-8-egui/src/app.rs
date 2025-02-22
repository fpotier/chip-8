use std::collections::HashMap;

use chip_8;
use chip_8::Chip8;
use egui::Key;

#[derive(serde::Deserialize, serde::Serialize)]
#[serde(default)]
pub struct TemplateApp {
    #[serde(skip)]
    emulator: Chip8,
    #[serde(skip)]
    key_bindings: HashMap<Key, usize>,
}

impl Default for TemplateApp {
    fn default() -> Self {
        let mut key_bindings: HashMap<Key, usize> = HashMap::new();
        key_bindings.insert(Key::Num1, 0);
        key_bindings.insert(Key::Num2, 1);
        key_bindings.insert(Key::Num3, 2);
        key_bindings.insert(Key::Num4, 12);

        key_bindings.insert(Key::Q, 3);
        key_bindings.insert(Key::W, 4);
        key_bindings.insert(Key::E, 5);
        key_bindings.insert(Key::R, 13);

        key_bindings.insert(Key::A, 6);
        key_bindings.insert(Key::S, 7);
        key_bindings.insert(Key::D, 8);
        key_bindings.insert(Key::F, 14);

        key_bindings.insert(Key::Z, 9);
        key_bindings.insert(Key::X, 7);
        key_bindings.insert(Key::C, 8);
        key_bindings.insert(Key::V, 15);

        Self {
            emulator: Chip8::new(),
            key_bindings: key_bindings,
        }
    }
}

impl TemplateApp {
    pub fn new(cc: &eframe::CreationContext<'_>) -> Self {
        if let Some(storage) = cc.storage {
            return eframe::get_value(storage, eframe::APP_KEY).unwrap_or_default();
        }

        Default::default()
    }
}

impl eframe::App for TemplateApp {
    fn save(&mut self, storage: &mut dyn eframe::Storage) {
        eframe::set_value(storage, eframe::APP_KEY, self);
    }

    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        ctx.input(|i| {
            for (&key, &keypad_index) in &self.key_bindings {
                if i.key_pressed(key) {
                    self.emulator.set_key(keypad_index, true);
                } else if i.key_released(key) {
                    self.emulator.set_key(keypad_index, false);
                }
            }
        });

        self.emulator.tick();

        if ctx.input(|i| i.key_pressed(Key::Q)) {
            self.emulator.set_key(0, true);
        }

        let tile_width = 10;
        let tile_height = 10;

        egui::CentralPanel::default().show(ctx, |ui| {
            for row in 0..chip_8::SCREEN_HEIGHT {
                for col in 0..chip_8::SCREEN_WIDTH {
                    let color = if self.emulator.vram[row][col] {
                        egui::Color32::WHITE
                    } else {
                        egui::Color32::BLACK
                    };

                    let rect = egui::Rect::from_min_size(
                        egui::Pos2::new(
                            col as f32 * tile_width as f32,
                            row as f32 * tile_height as f32,
                        ),
                        egui::vec2(tile_width as f32, tile_height as f32),
                    );

                    ui.painter().rect_filled(rect, 0.0, color);
                }
            }
        });
    }
}
