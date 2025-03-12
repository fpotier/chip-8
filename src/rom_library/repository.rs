use super::RomMetadata;

use async_trait::async_trait;

pub enum RepositoryPermission {
    ReadOnly,
    ReadWrite,
}

pub type RomList = Vec<RomMetadata>;

#[cfg(not(target_arch = "wasm32"))]
#[async_trait]
pub trait Repository: Send + Sync {
    fn name(&self) -> &str;
    fn permissions(&self) -> RepositoryPermission;
    async fn list(&self) -> Result<RomList, reqwest::Error>;
}

#[cfg(target_arch = "wasm32")]
#[async_trait(?Send)]
pub trait Repository {
    fn name(&self) -> &str;
    fn permissions(&self) -> RepositoryPermission;
    async fn list(&self) -> Result<RomList, reqwest::Error>;
}
