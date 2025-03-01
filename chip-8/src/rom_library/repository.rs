use std::collections::HashMap;

use super::RomMetadata;

pub enum RepositoryPermission {
    ReadOnly,
    ReadWrite,
}

pub trait Repository {
    #[allow(async_fn_in_trait)]
    async fn update(&mut self) -> Result<(), reqwest::Error>;
    fn permissions(&self) -> RepositoryPermission;
    fn list(&self) -> &HashMap<String, RomMetadata>;
}
