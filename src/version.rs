pub fn version() -> &'static str {
    env!("CARGO_PKG_VERSION")
}

pub fn version_major() -> &'static str {
    env!("CARGO_PKG_VERSION_MAJOR")
}
pub fn version_minor() -> &'static str {
    env!("CARGO_PKG_VERSION_MINOR")
}
pub fn version_patch() -> &'static str {
    env!("CARGO_PKG_VERSION_PATCH")
}
pub fn version_pre() -> &'static str {
    env!("CARGO_PKG_VERSION_PRE")
}
