use std::{collections::HashMap, str::FromStr};

use async_trait::async_trait;
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
    platform: String,
}

impl Game {
    fn to_rom_info(&self, name: &String) -> RomMetadata {
        let url = Url::from_str(&format!("{BASE_URL}/roms/{}.ch8", name)).unwrap();
        RomMetadata {
            title: self.title.clone(),
            rom_url: url,
        }
    }
}

#[derive(Clone)]
pub struct Chip8Archive {
    name: String,
}

impl Chip8Archive {
    pub fn new() -> Self {
        Chip8Archive {
            name: "Chip 8 Community Archive".to_string(),
        }
    }

    async fn fetch_rom_list(&self) -> Result<HashMap<String, Game>, reqwest::Error> {
        let res = reqwest::get(ROM_LIST_URL).await?;
        let payload: HashMap<String, Game> = res.json().await?;

        Ok(payload)
    }
}

#[cfg(not(target_arch = "wasm32"))]
#[async_trait]
impl Repository for Chip8Archive {
    fn name(&self) -> &str {
        &self.name
    }

    fn permissions(&self) -> RepositoryPermission {
        RepositoryPermission::ReadOnly
    }

    async fn list(&self) -> Result<RomList, reqwest::Error> {
        Ok(self
            .fetch_rom_list()
            .await?
            .iter()
            .filter_map(|(name, game)| {
                if game.platform == "chip8" {
                    Some(game.to_rom_info(name))
                } else {
                    None
                }
            })
            .collect())
    }
}

#[cfg(target_arch = "wasm32")]
#[async_trait(?Send)]
impl Repository for Chip8Archive {
    fn name(&self) -> &str {
        &self.name
    }

    fn permissions(&self) -> RepositoryPermission {
        RepositoryPermission::ReadOnly
    }

    async fn list(&self) -> Result<RomList, reqwest::Error> {
        Ok(self
            .fetch_rom_list()
            .await?
            .iter()
            .map(|(name, game)| game.to_rom_info(name))
            .collect())
    }
}

#[cfg(test)]
mod tests {
    // use super::*;

    #[tokio::test]
    async fn test_fetch_rom_list() {
        // let repo = Chip8Archive::new("Chip 8 Archive".to_string());
        // let _ = repo.sync().await;
        // for (_, metadata) in repo.list() {
        // println!("{}", metadata.title);
        // }
        // assert_eq!(repo.fetch_rom_list().await.unwrap(), ());
    }
}
