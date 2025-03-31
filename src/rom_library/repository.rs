use super::RomMetadata;

pub enum RepositoryPermission {
    ReadOnly,
    ReadWrite,
}

pub type RomList = Vec<RomMetadata>;

#[macro_export]
macro_rules! async_trait_compat {
    ($($item:item)*) => {
        $(
            use async_trait::async_trait;

            #[cfg(not(target_arch = "wasm32"))]
            #[async_trait]
            $item

            #[cfg(target_arch = "wasm32")]
            #[async_trait(?Send)]
            $item
        )*
    };
}

async_trait_compat! {
pub trait Repository: Send + Sync {
    fn name(&self) -> &str;
    fn permissions(&self) -> RepositoryPermission;
    async fn list(&self) -> Result<RomList, reqwest::Error>;
}
}
