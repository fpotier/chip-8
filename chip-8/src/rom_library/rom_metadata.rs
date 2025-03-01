use url::Url;

#[derive(PartialEq)]
pub struct RomMetadata {
    pub title: String,
    pub rom_url: Url,
}
