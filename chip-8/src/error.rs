#[derive(Debug)]
pub enum Error {
    FetchError,
    DecodeError(DecodeError),
    RuntimeError(RuntimeError),
}

#[derive(Debug)]
pub struct RuntimeError {
    pub opcode: Opcode,
    pub message: String,
}

#[derive(Debug, PartialEq)]
pub struct DecodeError(pub u16);

impl RuntimeError {
    fn new(opcode: Opcode, message: String) -> RuntimeError {
        RuntimeError { opcode, message }
    }
}
