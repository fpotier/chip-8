#![warn(clippy::all, rust_2018_idioms)]

mod app;
pub use app::Chip8Egui;
mod version;
pub use version::*;
pub mod task;
pub use task::*;
pub mod emulator_state;
pub use emulator_state::*;
