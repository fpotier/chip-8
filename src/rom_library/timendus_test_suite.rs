use std::str::FromStr;

use url::Url;

use crate::async_trait_compat;

use super::{Repository, RepositoryPermission, RomList, RomMetadata};

pub struct TimendusTestSuite();

async_trait_compat! {
impl Repository for TimendusTestSuite {
    fn name(&self) -> &str { "CHIP-8 test suite"}

    fn permissions(&self) -> RepositoryPermission { RepositoryPermission::ReadOnly }

    async fn list(&self) -> Result<RomList, reqwest::Error> {
        Ok(vec![
            RomMetadata{title: String::from("1 - Chip 8 Logo"), rom_url: Url::from_str("https://raw.githubusercontent.com/Timendus/chip8-test-suite/refs/heads/main/bin/1-chip8-logo.ch8").unwrap()},
            RomMetadata{title: String::from("2 - IBM logo"), rom_url: Url::from_str("https://raw.githubusercontent.com/Timendus/chip8-test-suite/refs/heads/main/bin/2-ibm-logo.ch8").unwrap()},
            RomMetadata{title: String::from("3 - Corax+"), rom_url: Url::from_str("https://raw.githubusercontent.com/Timendus/chip8-test-suite/refs/heads/main/bin/3-corax+.ch8").unwrap()},
            RomMetadata{title: String::from("4 - Flags"), rom_url: Url::from_str("https://raw.githubusercontent.com/Timendus/chip8-test-suite/refs/heads/main/bin/4-flags.ch8").unwrap()},
            RomMetadata{title: String::from("5 - Quirks"), rom_url: Url::from_str("https://raw.githubusercontent.com/Timendus/chip8-test-suite/refs/heads/main/bin/5-quirks.ch8").unwrap()},
            RomMetadata{title: String::from("6 - Keypad"), rom_url: Url::from_str("https://raw.githubusercontent.com/Timendus/chip8-test-suite/refs/heads/main/bin/6-keypad.ch8").unwrap()},
            RomMetadata{title: String::from("7 - Beep"), rom_url: Url::from_str("https://raw.githubusercontent.com/Timendus/chip8-test-suite/refs/heads/main/bin/7-beep.ch8").unwrap()},
            RomMetadata{title: String::from("8 - Scrolling"), rom_url: Url::from_str("https://raw.githubusercontent.com/Timendus/chip8-test-suite/refs/heads/main/bin/8-scrolling.ch8").unwrap()},
        ])
    }
}
}
