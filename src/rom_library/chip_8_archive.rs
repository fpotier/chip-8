use std::{collections::HashMap, str::FromStr};

use serde::Deserialize;
use url::Url;

use crate::async_trait_compat;

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
        // FIXME
        let url = Url::from_str(&format!("{BASE_URL}/roms/{}.ch8", name)).unwrap();
        RomMetadata {
            title: self.title.clone(),
            rom_url: url,
        }
    }
}

#[derive(Clone)]
pub struct Chip8Archive();

impl Chip8Archive {
    async fn fetch_rom_list(&self) -> Result<HashMap<String, Game>, reqwest::Error> {
        let res = reqwest::get(ROM_LIST_URL).await?;
        let payload: HashMap<String, Game> = res.json().await?;

        Ok(payload)
    }
}

async_trait_compat! {
impl Repository for Chip8Archive {
    fn name(&self) -> &str {
        "Chip 8 Community Archive"
    }

    fn permissions(&self) -> RepositoryPermission {
        RepositoryPermission::ReadOnly
    }

    async fn list(&self) -> Result<RomList, reqwest::Error> {
        let mut rom_list: RomList = self
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
            .collect();

            rom_list.sort_by(|a, b| a.title.cmp(&b.title));
            Ok(rom_list)
    }
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
