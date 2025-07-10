use std::future::Future;

use chip_8::{Rom, rom_library::RomList};

#[cfg(not(target_arch = "wasm32"))]
pub fn execute_task<F: Future<Output = ()> + Send + 'static>(f: F) {
    tokio::spawn(f);
}

#[cfg(target_arch = "wasm32")]
pub fn execute_task<F: Future<Output = ()> + 'static>(f: F) {
    wasm_bindgen_futures::spawn_local(f);
}

pub enum Message {
    LoadNewRom {
        rom: Rom,
    },
    UpdateRepository {
        repository_name: String,
        roms: RomList,
    },
}
