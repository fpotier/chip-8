use url::Url;

#[derive(PartialEq, Clone)]
pub struct RomMetadata {
    pub title: String,
    pub rom_url: Url,
}
