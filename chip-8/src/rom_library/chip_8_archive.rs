use std::{collections::HashMap, str::FromStr};

use serde::Deserialize;
use url::Url;

use super::{Repository, RepositoryPermission, RomList, RomMetadata};

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

#[derive(Clone)]
pub struct Chip8Archive {
    pub name: String,
    client: reqwest::Client,
    roms: HashMap<String, RomMetadata>,
}

impl Chip8Archive {
    pub fn new() -> Self {
        Chip8Archive {
            name: "Chip 8 Archive".to_string(),
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
    async fn fetch(&self) -> Result<RomList, reqwest::Error> {
        let game_list = self.fetch_rom_list().await?;
        let mut roms = HashMap::new();
        for (game_name, metadata) in game_list {
            roms.insert(game_name, metadata.to_rom_info());
        }

        Ok(roms)
    }

    fn permissions(&self) -> RepositoryPermission {
        RepositoryPermission::ReadOnly
    }

    fn list(&self) -> &HashMap<String, RomMetadata> {
        &self.roms
    }

    fn update(&mut self, roms: RomList) {
        self.roms = roms;
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[tokio::test]
    async fn test_fetch_rom_list() {
        let repo = Chip8Archive::new();
        let _ = repo.fetch().await;
        for (_, metadata) in repo.list() {
            println!("{}", metadata.title);
        }
        // assert_eq!(repo.fetch_rom_list().await.unwrap(), ());
    }
}
