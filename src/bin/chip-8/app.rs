use ::chip_8::core::{Chip8, Error, chip_8};
use ::chip_8::rom_library::{Chip8Archive, Repository, RomList, TimendusTestSuite};
use ::chip_8::{Rom, version};
use egui::{Align, Button, CollapsingHeader, Id, Layout, Modal, ScrollArea};
use egui_extras::{Column, TableBuilder};
use egui_notify::{Anchor, Toasts};
use std::collections::HashMap;
use std::fs;
use std::sync::Arc;
use std::sync::mpsc::{Receiver, Sender, channel};
use std::time::Duration;

use crate::emulator_state::EmulatorState;
use crate::task::{Message, execute_task};

const DEFAULT_TOAST_DURATION: Option<Duration> = Some(Duration::from_secs(3));

pub struct Chip8Egui {
    emulator: Chip8,
    emulator_state: EmulatorState,
    repositories: [Arc<dyn Repository>; 2],
    rom_catalog: HashMap<String, RomList>,
    repository_view: bool,
    toasts: Toasts,
    message_sender: Sender<Message>,
    message_receiver: Receiver<Message>,
}

impl Default for Chip8Egui {
    fn default() -> Self {
        let (sender, receiver) = channel();
        Self {
            emulator: Chip8::new(),
            emulator_state: Default::default(),
            repositories: [Arc::new(Chip8Archive()), Arc::new(TimendusTestSuite())],
            rom_catalog: HashMap::new(),
            repository_view: false,
            toasts: Toasts::default().with_anchor(Anchor::BottomRight),
            message_sender: sender,
            message_receiver: receiver,
        }
    }
}

impl Chip8Egui {
    pub fn new(_cc: &eframe::CreationContext<'_>) -> Self {
        Default::default()
    }

    fn poll_async_tasks(&mut self) {
        if let Ok(msg) = self.message_receiver.try_recv() {
            match msg {
                Message::LoadNewRom { rom } => {
                    self.emulator.reset();
                    match self.emulator.load_rom(&rom) {
                        Ok(_) => {
                            self.emulator_state.has_rom_loaded = true;
                            self.toasts
                                .info("ROM loaded")
                                .duration(DEFAULT_TOAST_DURATION);
                        }
                        Err(_) => {
                            self.emulator_state.has_rom_loaded = false;
                            // TODO: Better error message
                            self.toasts
                                .error("Failed to load ROM")
                                .duration(DEFAULT_TOAST_DURATION);
                        }
                    }
                }
                Message::UpdateRepository {
                    repository_name,
                    roms,
                } => {
                    self.rom_catalog.insert(repository_name, roms);
                }
            }
        }
    }

    fn handle_keypad_events(&mut self, ctx: &egui::Context) {
        ctx.input(|i| {
            for (&key, &keypad_index) in &self.emulator_state.keymap {
                if i.key_pressed(key) || i.key_down(key) {
                    self.emulator.set_key_down(keypad_index);
                } else {
                    self.emulator.set_key_up(keypad_index);
                }
            }
        });
    }

    fn draw_emulator_screen(&mut self, ctx: &egui::Context) {
        egui::CentralPanel::default().show(ctx, |ui| {
            ui.input(|i| {
                if !i.raw.dropped_files.is_empty() {
                    if i.raw.dropped_files.len() > 1 {
                        self.toasts.warning(format!(
                            "Only one ROM can be loaded at a time\nThe following files will be ignored:\n{}",
                            &i.raw.dropped_files[1..]
                                .iter()
                                .map(|f| format!(
                                    "\t- {}\n",
                                    if let Some(path) = &f.path {
                                        path.to_string_lossy().into_owned()
                                    } else {
                                        f.name.clone()
                                    }
                                ))
                                .collect::<String>()
                        ));
                    }

                    if let Some(path) = &i.raw.dropped_files[0].path {
                        match fs::read(path) {
                            Ok(rom) => {
                                let _ = self.message_sender.send(Message::LoadNewRom {rom});
                            },
                            Err(err) =>{
                                 self.toasts.error(format!("{err}"));
                            }
                        }
                    } else if let Some(bytes) = &i.raw.dropped_files[0].bytes {
                        let _ = self.message_sender.send(Message::LoadNewRom { rom: bytes.to_vec()});
                    };
                }
            });

            let tile_size = (ui.available_width() / 64.0)
                .ceil()
                .min((ui.available_height() / 32.0).ceil());

            for row in 0..chip_8::SCREEN_HEIGHT {
                for col in 0..chip_8::SCREEN_WIDTH {
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
    }

    fn draw_top_panel(&mut self, ctx: &egui::Context) {
        egui::TopBottomPanel::top("top").show(ctx, |ui| {
            ui.with_layout(Layout::left_to_right(Align::LEFT), |ui| {
                if ui
                    .add_enabled(
                        self.emulator_state.has_rom_loaded,
                        Button::new(if self.emulator_state.is_paused {
                            "⏵"
                        } else {
                            "⏸"
                        }),
                    )
                    .clicked()
                {
                    self.emulator_state.is_paused = !self.emulator_state.is_paused;
                }

                #[cfg(debug_assertions)]
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
                                let rom: Rom = rom_file.read().await;
                                let _ = sender.send(Message::LoadNewRom { rom });
                            }
                        });
                    }

                    if ui.button("Load from repository").clicked() {
                        self.repository_view = true;
                    }
                });
            });
        });
    }

    fn draw_bottom_panel(&mut self, ctx: &egui::Context) {
        egui::TopBottomPanel::bottom("bottom").show(ctx, |ui| {
            ui.with_layout(Layout::right_to_left(Align::RIGHT), |ui| {
                ui.hyperlink_to(
                    format!("{} Github", egui::special_emojis::GITHUB),
                    env!("CARGO_PKG_REPOSITORY"),
                );
                ui.separator();
                ui.label(format!("Version: {}", version()));
            });
        });
    }
}

impl eframe::App for Chip8Egui {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        self.poll_async_tasks();

        self.handle_keypad_events(ctx);

        if self.repository_view {
            Modal::new(Id::new("Modal A")).show(ctx, |ui| {
                ui.set_width(ui.available_width());
                ui.heading("Repository");
                ui.separator();

                for repo in &self.repositories {
                    CollapsingHeader::new(repo.name()).show(ui, |ui| {
                        if ui.button("Update").clicked() {
                            let sender = self.message_sender.clone();
                            let repo_clone = Arc::clone(repo);
                            execute_task(async move {
                                let res = repo_clone.list().await;
                                match res {
                                    Ok(roms) => {
                                        let repository_name = repo_clone.name().to_string();
                                        let _ = sender.send(Message::UpdateRepository {
                                            repository_name,
                                            roms,
                                        });
                                    }
                                    Err(_) => todo!(),
                                }
                            });
                        }

                        ScrollArea::vertical().show(ui, |ui| {
                            let table = TableBuilder::new(ui)
                                .column(Column::remainder())
                                .min_scrolled_height(0.0)
                                .max_scroll_height(100.0);

                            table.body(|mut body| {
                                if let Some(roms) = self.rom_catalog.get(repo.name()) {
                                    for rom in roms {
                                        body.row(20.0, |mut row| {
                                            row.col(|col| {
                                                col.with_layout(
                                                    Layout::left_to_right(Align::Center),
                                                    |col| {
                                                        col.label(rom.title.clone());
                                                        if col.button("Download").clicked() {
                                                            let sender =
                                                                self.message_sender.clone();
                                                            let url_copy = rom.rom_url.clone();
                                                            execute_task(async move {
                                                                match reqwest::get(url_copy).await {
                                                                    Ok(res) => {
                                                                        let rom = res
                                                                            .bytes()
                                                                            .await
                                                                            // FIXME
                                                                            .unwrap()
                                                                            .to_vec();
                                                                        let _ = sender.send(
                                                                            Message::LoadNewRom {
                                                                                rom,
                                                                            },
                                                                        );
                                                                    }
                                                                    Err(_) => todo!(),
                                                                }
                                                            });
                                                        };
                                                    },
                                                );
                                            });
                                        });
                                    }
                                }
                            });
                        });
                    });
                    ui.separator();
                }

                if ui.button("Close").clicked() {
                    self.repository_view = false;
                    ui.close_menu();
                }
            });
        }

        self.draw_top_panel(ctx);

        if !self.emulator_state.is_paused {
            if let Err(err) = self
                .emulator
                .tick(self.emulator_state.instruction_per_frame)
            {
                let message = match err {
                    Error::FetchError => "Invalid instruction pointer",
                    Error::DecodeError(decode_error) => {
                        &format!("Found invalide opcode 0x{:x}", decode_error.0)
                    }
                    Error::RuntimeError(runtime_error) => &runtime_error.message.clone(),
                    _ => panic!("Never reached"),
                };
                self.toasts.error(message).duration(DEFAULT_TOAST_DURATION);
                self.emulator_state.is_paused = true
            }
        }

        self.draw_emulator_screen(ctx);

        self.draw_bottom_panel(ctx);

        self.toasts.show(ctx);

        ctx.request_repaint();
    }
}
