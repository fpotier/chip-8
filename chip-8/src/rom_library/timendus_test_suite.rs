use super::{Repository, RepositoryPermission};

pub struct TimendusTestSuite {}

impl Repository for TimendusTestSuite {
    fn permissions(&self) -> RepositoryPermission {
        todo!()
    }

    async fn update(&mut self) -> Result<(), reqwest::Error> {
        todo!()
    }

    fn list(&self) -> &std::collections::HashMap<String, super::RomMetadata> {
        todo!()
    }
}
