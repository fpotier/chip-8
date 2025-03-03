use std::collections::HashMap;

use super::RomMetadata;

pub enum RepositoryPermission {
    ReadOnly,
    ReadWrite,
}

pub type RomList = HashMap<String, RomMetadata>;

pub trait Repository {
    #[allow(async_fn_in_trait)]
    async fn fetch(&self) -> Result<RomList, reqwest::Error>;
    fn permissions(&self) -> RepositoryPermission;
    fn list(&self) -> &RomList;
    fn update(&mut self, roms: RomList);
}
