use chip_8;
use chip_8::Chip8;
use egui::Key;
use std::collections::HashMap;

#[derive(serde::Deserialize, serde::Serialize)]
#[serde(default)]
pub struct Chip8Egui {
    #[serde(skip)]
    emulator: Chip8,
    #[serde(skip)]
    key_bindings: HashMap<Key, usize>,
}

impl Default for Chip8Egui {
    fn default() -> Self {
        let mut key_bindings: HashMap<Key, usize> = HashMap::new();
        key_bindings.insert(Key::Num1, 1);
        key_bindings.insert(Key::Num2, 2);
        key_bindings.insert(Key::Num3, 3);
        key_bindings.insert(Key::Num4, 12);

        key_bindings.insert(Key::Q, 4);
        key_bindings.insert(Key::W, 5);
        key_bindings.insert(Key::E, 6);
        key_bindings.insert(Key::R, 13);

        key_bindings.insert(Key::A, 7);
        key_bindings.insert(Key::S, 8);
        key_bindings.insert(Key::D, 9);
        key_bindings.insert(Key::F, 14);

        key_bindings.insert(Key::Z, 10);
        key_bindings.insert(Key::X, 0);
        key_bindings.insert(Key::C, 11);
        key_bindings.insert(Key::V, 15);

        Self {
            emulator: Chip8::new(),
            key_bindings: key_bindings,
        }
    }
}

impl Chip8Egui {
    pub fn new(cc: &eframe::CreationContext<'_>) -> Self {
        if let Some(storage) = cc.storage {
            return eframe::get_value(storage, eframe::APP_KEY).unwrap_or_default();
        }

        Default::default()
    }
}

impl eframe::App for Chip8Egui {
    fn save(&mut self, storage: &mut dyn eframe::Storage) {
        eframe::set_value(storage, eframe::APP_KEY, self);
    }

    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        ctx.input(|i| {
            for (&key, &keypad_index) in &self.key_bindings {
                if i.key_pressed(key) || i.key_down(key) {
                    self.emulator.set_key_down(keypad_index);
                } else {
                    self.emulator.set_key_up(keypad_index);
                }
            }
        });

        self.emulator.tick(15);

        let tile_size = (ctx.screen_rect().width() / 64.0)
            .ceil()
            .min((ctx.screen_rect().height() / 32.0).ceil());

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
                            (col as f32 * tile_size).round(),
                            (row as f32 * tile_size).round(),
                        ),
                        egui::vec2(tile_size, tile_size),
                    );

                    ui.painter().rect_filled(rect, 0.0, color);
                }
            }
        });
        ctx.request_repaint();
    }
}
