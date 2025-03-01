use std::{collections::HashMap, str::FromStr};

use serde::Deserialize;
use url::Url;

use super::{Repository, RepositoryPermission, RomMetadata};

const BASE_URL: &str =
    "https://raw.githubusercontent.com/JohnEarnest/chip8Archive/refs/heads/master/";
const ROM_LIST_URL: &str =
    "https://raw.githubusercontent.com/JohnEarnest/chip8Archive/refs/heads/master/programs.json";

#[derive(Debug, Deserialize)]
struct Game {
    title: String,
}

impl Game {
    fn to_rom_info(&self) -> RomMetadata {
        let url = Url::from_str(&format!("{BASE_URL}/roms/{}.ch8", self.title.clone())).unwrap();
        RomMetadata {
            title: self.title.clone(),
            rom_url: url,
        }
    }
}
pub struct Chip8Archive {
    client: reqwest::Client,
    roms: HashMap<String, RomMetadata>,
}

impl Chip8Archive {
    pub fn new() -> Self {
        Chip8Archive {
            client: reqwest::Client::new(),
            roms: HashMap::new(),
        }
    }

    async fn fetch_rom_list(&self) -> Result<HashMap<String, Game>, reqwest::Error> {
        let res = self.client.get(ROM_LIST_URL).send().await?;
        let payload: HashMap<String, Game> = res.json().await?;

        Ok(payload)
    }
}

impl Repository for Chip8Archive {
    async fn update(&mut self) -> Result<(), reqwest::Error> {
        let game_list = self.fetch_rom_list().await?;
        for (game_name, metadata) in game_list {
            self.roms.insert(game_name, metadata.to_rom_info());
        }

        Ok(())
    }

    fn permissions(&self) -> RepositoryPermission {
        RepositoryPermission::ReadOnly
    }

    fn list(&self) -> &HashMap<String, RomMetadata> {
        &self.roms
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[tokio::test]
    async fn test_fetch_rom_list() {
        let mut repo = Chip8Archive::new();
        let _ = repo.update().await;
        for (_, metadata) in repo.list() {
            println!("{}", metadata.title);
        }
        // assert_eq!(repo.fetch_rom_list().await.unwrap(), ());
    }
}
