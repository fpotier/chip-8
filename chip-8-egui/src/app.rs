use chip_8::core::Chip8;
use chip_8::rom_library::{Chip8Archive, Repository};
use egui::{CollapsingHeader, Id, Key, Modal, ScrollArea};
use egui_extras::{Column, TableBuilder};
use std::sync::mpsc::{channel, Receiver, Sender};
use std::{collections::HashMap, fs};

use crate::task::execute_task;
use crate::Message;

pub struct Chip8Egui {
    emulator: Chip8,
    key_bindings: HashMap<Key, usize>,
    paused: bool,
    repository: [Chip8Archive; 2],
    repository_view: bool,
    message_sender: Sender<Message>,
    message_receiver: Receiver<Message>,
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

        let (sender, receiver) = channel();
        Self {
            emulator: Chip8::new(),
            key_bindings: key_bindings,
            paused: true,
            repository: [
                Chip8Archive::new("Test 1".to_string()),
                Chip8Archive::new("Test 2".to_string()),
            ],
            repository_view: false,
            message_sender: sender,
            message_receiver: receiver,
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
        if let Ok(msg) = self.message_receiver.try_recv() {
            match msg {
                Message::LoadNewRom { rom } => {
                    self.emulator.reset();
                    self.emulator.load_rom(&rom);
                }
                Message::UpdateRepository { index, roms } => {
                    self.repository[index].update(roms);
                }
            }
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
                if ui.button("📁 Open file").clicked() {
                    let sender = self.message_sender.clone();
                    let async_task = rfd::AsyncFileDialog::new().pick_file();
                    execute_task(async move {
                        let rom_file = async_task.await;
                        if let Some(rom_file) = rom_file {
                            let rom: Vec<u8> = rom_file.read().await;
                            let _ = sender.send(Message::LoadNewRom { rom: rom });
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
                ui.set_width(ui.available_width());
                ui.heading("Repository");
                ui.separator();

                let mut index = 0;
                for repo in &self.repository {
                    CollapsingHeader::new(&repo.name).show(ui, |ui| {
                        if ui.button("Update").clicked() {
                            let sender = self.message_sender.clone();
                            let repo = repo.clone();
                            execute_task(async move {
                                let roms = repo.fetch().await.unwrap();
                                let _ = sender.send(Message::UpdateRepository {
                                    index: index,
                                    roms: roms,
                                });
                            });
                        }

                        ScrollArea::vertical().show(ui, |ui| {
                            let table = TableBuilder::new(ui)
                                .column(Column::remainder())
                                .min_scrolled_height(0.0)
                                .max_scroll_height(100.0);

                            table.body(|mut body| {
                                for (title, _metadata) in repo.list() {
                                    // body.label(title);
                                    body.row(20.0, |mut row| {
                                        row.col(|col| {
                                            col.label(title);
                                        });
                                    });
                                }
                            });
                        });
                    });
                    ui.separator();
                    index += 1;
                }

                if ui.button("Close").clicked() {
                    self.repository_view = false;
                    ui.close_menu();
                }
            });
        }

        // egui::SidePanel::right("side_panel").show(ctx, |ui| {
        //     let table = TableBuilder::new(ui)
        //         .columns(Column::auto(), 2)
        //         .striped(true)
        //         .resizable(true)
        //         .cell_layout(egui::Layout::left_to_right(egui::Align::Center));
        //     table
        //         .header(20.0, |mut header| {
        //             header.col(|ui| {
        //                 ui.strong("Register");
        //             });
        //             header.col(|ui| {
        //                 ui.strong("Value");
        //             });
        //         })
        //         .body(|mut body| {
        //             for (index, value) in self.emulator.registers.iter().enumerate() {
        //                 body.row(10.0, |mut row| {
        //                     row.col(|col| {
        //                         col.label(format!("V{index}"));
        //                     });
        //                     row.col(|col| {
        //                         col.label(format!("0x{value:x}"));
        //                     });
        //                 });
        //             }
        //         })
        // });

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
