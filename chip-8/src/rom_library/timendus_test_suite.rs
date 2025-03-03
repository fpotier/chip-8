use super::{Repository, RepositoryPermission, RomList};

pub struct TimendusTestSuite {}

impl Repository for TimendusTestSuite {
    fn permissions(&self) -> RepositoryPermission {
        todo!()
    }

    async fn fetch(&self) -> Result<RomList, reqwest::Error> {
        todo!()
    }

    fn list(&self) -> &std::collections::HashMap<String, super::RomMetadata> {
        todo!()
    }

    fn update(&mut self, _roms: RomList) {
        todo!()
    }
}
