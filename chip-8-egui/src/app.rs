use chip_8::core::Chip8;
use egui::{Id, Key, Modal};
use egui_extras::{Column, TableBuilder};
use std::future::Future;
use std::sync::mpsc::{channel, Receiver, Sender};
use std::{collections::HashMap, fs};

pub struct Chip8Egui {
    emulator: Chip8,
    key_bindings: HashMap<Key, usize>,
    paused: bool,
    repository_view: bool,
    file_dialog_channel: (Sender<Vec<u8>>, Receiver<Vec<u8>>),
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
            paused: true,
            repository_view: false,
            file_dialog_channel: channel(),
        }
    }
}

impl Chip8Egui {
    pub fn new(_cc: &eframe::CreationContext<'_>) -> Self {
        Default::default()
    }
}

impl eframe::App for Chip8Egui {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        if let Ok(rom) = self.file_dialog_channel.1.try_recv() {
            self.emulator.reset();
            self.emulator.load_rom(&rom);
        }

        ctx.input(|i| {
            for (&key, &keypad_index) in &self.key_bindings {
                if i.key_pressed(key) || i.key_down(key) {
                    self.emulator.set_key_down(keypad_index);
                } else {
                    self.emulator.set_key_up(keypad_index);
                }
            }
        });

        egui::TopBottomPanel::top("top").show(ctx, |ui| {
            if ui.button(if self.paused { "⏵" } else { "⏸" }).clicked() {
                self.paused = !self.paused;
            }
            if ui.button("Dump VRAM").clicked() {
                self.emulator.dump_vram();
            }

            ui.menu_button("Load ROM", |ui| {
                if ui.button("📂 Open file").clicked() {
                    let sender = self.file_dialog_channel.0.clone();
                    let async_task = rfd::AsyncFileDialog::new().pick_file();
                    execute(async move {
                        let rom_file = async_task.await;
                        if let Some(rom_file) = rom_file {
                            let rom: Vec<u8> = rom_file.read().await;
                            let _ = sender.send(rom);
                        }
                    });
                }

                if ui.button("Load from repository").clicked() {
                    self.repository_view = true;
                }
            });
        });

        if self.repository_view {
            Modal::new(Id::new("Modal A")).show(ctx, |ui| {
                ui.heading("Repository");
                if ui.button("Close").clicked() {
                    self.repository_view = false;
                    ui.close_menu();
                }
            });
        }

        egui::SidePanel::right("side_panel").show(ctx, |ui| {
            let table = TableBuilder::new(ui)
                .columns(Column::auto(), 2)
                .striped(true)
                .resizable(true)
                .cell_layout(egui::Layout::left_to_right(egui::Align::Center));
            table
                .header(20.0, |mut header| {
                    header.col(|ui| {
                        ui.strong("Register");
                    });
                    header.col(|ui| {
                        ui.strong("Value");
                    });
                })
                .body(|mut body| {
                    for (index, value) in self.emulator.registers.iter().enumerate() {
                        body.row(10.0, |mut row| {
                            row.col(|col| {
                                col.label(format!("V{index}"));
                            });
                            row.col(|col| {
                                col.label(format!("0x{value:x}"));
                            });
                        });
                    }
                })
        });

        if !self.paused {
            self.emulator.tick(15);
        }

        egui::CentralPanel::default().show(ctx, |ui| {
            ui.input(|i| {
                if i.raw.dropped_files.len() == 1 {
                    // TODO: what if multiple files are dropped?
                    if let Some(path) = &i.raw.dropped_files[0].path {
                        let rom: Vec<u8> = fs::read(path).unwrap();
                        self.emulator.reset();
                        self.emulator.load_rom(&rom);
                    }
                }
            });

            let tile_size = (ui.available_width() / 64.0)
                .ceil()
                .min((ui.available_height() / 32.0).ceil());

            for row in 0..chip_8::core::SCREEN_HEIGHT {
                for col in 0..chip_8::core::SCREEN_WIDTH {
                    let color = if self.emulator.vram[row][col] {
                        egui::Color32::WHITE
                    } else {
                        egui::Color32::BLACK
                    };

                    let rect = egui::Rect::from_min_size(
                        ui.clip_rect().min
                            + egui::vec2(
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

#[cfg(not(target_arch = "wasm32"))]
fn execute<F: Future<Output = ()> + Send + 'static>(f: F) {
    tokio::spawn(f);
}

#[cfg(target_arch = "wasm32")]
fn execute<F: Future<Output = ()> + 'static>(f: F) {
    wasm_bindgen_futures::spawn_local(f);
}
