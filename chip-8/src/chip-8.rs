pub const NB_REGISTER: usize = 16;
pub const RAM_SIZE: usize = 4096;
pub const STACK_SIZE: usize = 16;
pub const KEYPAD_SIZE: usize = 16;

pub const SCREEN_WIDTH: usize = 64;
pub const SCREEN_HEIGHT: usize = 32;

pub const ENTRYPOINT_ADDRESS: usize = 0x200;

const DEFAULT_FONT: [u8; 80] = [
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0x70, 0x80, 0x80, 0x80, 0x70, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xE0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xE0, 0x80, 0x80, // F
];

pub struct Chip8 {
    registers: [u8; NB_REGISTER],
    stack: [usize; STACK_SIZE],
    ram: [u8; RAM_SIZE],
    pub vram: [[bool; SCREEN_WIDTH]; SCREEN_HEIGHT],
    keypad: [bool; KEYPAD_SIZE],
    index_register: usize,
    instruction_pointer: usize,
    stack_pointer: usize,
    delay_timer: u8,
    sound_timer: u8,
}

impl Chip8 {
    pub fn new() -> Chip8 {
        let mut chip8 = Chip8 {
            registers: [0; NB_REGISTER],
            stack: [0; STACK_SIZE],
            ram: [0; RAM_SIZE],
            vram: [[false; SCREEN_WIDTH]; SCREEN_HEIGHT],
            keypad: [false; KEYPAD_SIZE],
            index_register: 0,
            instruction_pointer: ENTRYPOINT_ADDRESS,
            stack_pointer: 0,
            delay_timer: 0,
            sound_timer: 0,
        };
        chip8.ram[..DEFAULT_FONT.len()].copy_from_slice(&DEFAULT_FONT);
        // TODO: remove
        chip8.load_rom();

        chip8
    }

    pub fn load_rom(&mut self) {
        // TODO: check size
        // let ibm_logo = include_bytes!("../../roms/2-ibm-logo.ch8");
        let ibm_logo = include_bytes!("../../roms/3-corax+.ch8");
        self.ram[ENTRYPOINT_ADDRESS..(ENTRYPOINT_ADDRESS + ibm_logo.len())]
            .copy_from_slice(ibm_logo);
    }

    pub fn tick(&mut self) {
        let opcode = self.fetch().unwrap();
        // TODO: put in fetch
        self.instruction_pointer += 2;
        println!("{:?}", opcode);
        self.execute(opcode).unwrap();

        if self.delay_timer > 0 {
            self.delay_timer -= 1;
        }
        if self.sound_timer > 0 {
            self.sound_timer -= 1;
        }
    }

    pub fn fetch(&mut self) -> Result<Opcode, Error> {
        let msb = self.ram.get(self.instruction_pointer);
        let lsb = self.ram.get(self.instruction_pointer + 1);

        match (msb, lsb) {
            (Some(msb), Some(lsb)) => {
                decode_opcode(((*msb as u16) << 8) | *lsb as u16).map_err(|e| Error::DecodeError(e))
            }
            (_, _) => Err(Error::FetchError),
        }
    }

    pub fn execute(&mut self, opcode: Opcode) -> Result<(), RuntimeError> {
        match opcode {
            Opcode::ClearDisplay => {
                self.vram.iter_mut().for_each(|row| row.fill(false));
                Ok(())
            }
            Opcode::Return => {
                if self.stack_pointer == 0 {
                    return Err(RuntimeError::new(
                        opcode,
                        "Use of 'return' instruction but call stack is empty".into(),
                    ));
                }

                self.stack_pointer -= 1;
                self.instruction_pointer = self.stack[self.stack_pointer];
                Ok(())
            }
            Opcode::Jump { address } => {
                self.validate_address(opcode, address)?;

                self.instruction_pointer = address;
                Ok(())
            }
            Opcode::Call { address } => {
                self.validate_address(opcode.clone(), address)?;
                if self.stack_pointer + 1 >= STACK_SIZE {
                    return Err(RuntimeError::new(
                        opcode,
                        format!("Call stack overflow in call to: {:x}", address),
                    ));
                }

                self.stack[self.stack_pointer] = self.instruction_pointer;
                self.stack_pointer += 1;
                self.instruction_pointer = address;
                Ok(())
            }
            Opcode::SkipEqualImmediate {
                register_index,
                immediate,
            } => {
                self.validate_register(opcode, register_index)?;

                if self.registers[register_index] == immediate {
                    self.instruction_pointer += 2;
                }
                Ok(())
            }
            Opcode::SkipNotEqualImmediate {
                register_index,
                immediate,
            } => {
                self.validate_register(opcode, register_index)?;

                if self.registers[register_index] != immediate {
                    self.instruction_pointer += 2;
                }
                Ok(())
            }
            Opcode::SkipEqualRegister {
                left_register_index,
                right_register_index,
            } => {
                self.validate_register(opcode.clone(), left_register_index)?;
                self.validate_register(opcode.clone(), right_register_index)?;

                if self.registers[left_register_index] == self.registers[right_register_index] {
                    self.instruction_pointer += 2;
                }
                Ok(())
            }
            Opcode::MoveImmediate {
                register_index,
                immediate,
            } => {
                self.validate_register(opcode, register_index)?;

                self.registers[register_index] = immediate;
                Ok(())
            }
            Opcode::AddImmediate {
                register_index,
                immediate,
            } => {
                self.validate_register(opcode, register_index)?;

                self.registers[register_index] += immediate;
                Ok(())
            }
            Opcode::MoveRegister {
                destination_register_index,
                source_register_index,
            } => {
                self.validate_register(opcode.clone(), source_register_index)?;
                self.validate_register(opcode.clone(), destination_register_index)?;

                self.registers[destination_register_index] = self.registers[source_register_index];
                Ok(())
            }
            Opcode::Or {
                left_register_index,
                right_register_index,
            } => {
                self.validate_register(opcode.clone(), left_register_index)?;
                self.validate_register(opcode.clone(), right_register_index)?;

                self.registers[left_register_index] |= self.registers[right_register_index];
                Ok(())
            }
            Opcode::And {
                left_register_index,
                right_register_index,
            } => {
                self.validate_register(opcode.clone(), left_register_index)?;
                self.validate_register(opcode.clone(), right_register_index)?;

                self.registers[left_register_index] &= self.registers[right_register_index];
                Ok(())
            }
            Opcode::Xor {
                left_register_index,
                right_register_index,
            } => {
                self.validate_register(opcode.clone(), left_register_index)?;
                self.validate_register(opcode.clone(), right_register_index)?;

                self.registers[left_register_index] ^= self.registers[right_register_index];
                Ok(())
            }
            Opcode::Add {
                left_register_index,
                right_register_index,
            } => {
                self.validate_register(opcode.clone(), left_register_index)?;
                self.validate_register(opcode.clone(), right_register_index)?;

                let flag = self.registers[right_register_index]
                    > (u8::MAX - self.registers[left_register_index]);
                self.registers[left_register_index] += self.registers[right_register_index];
                self.registers[0xF] = flag as u8;
                Ok(())
            }
            Opcode::Substract {
                left_register_index,
                right_register_index,
            } => {
                self.validate_register(opcode.clone(), left_register_index)?;
                self.validate_register(opcode.clone(), right_register_index)?;

                let flag =
                    self.registers[right_register_index] > self.registers[left_register_index];
                self.registers[left_register_index] -= self.registers[right_register_index];
                self.registers[0xF] = flag as u8;
                Ok(())
            }
            Opcode::ShiftRight {
                left_register_index,
                right_register_index,
            } => {
                self.validate_register(opcode.clone(), left_register_index)?;
                self.validate_register(opcode.clone(), right_register_index)?;

                let flag = self.registers[right_register_index] & 0x01;
                self.registers[left_register_index] = self.registers[right_register_index] >> 1;
                self.registers[0xF] = flag as u8;
                Ok(())
            }
            Opcode::LSubstract {
                left_register_index,
                right_register_index,
            } => {
                self.validate_register(opcode.clone(), left_register_index)?;
                self.validate_register(opcode.clone(), right_register_index)?;

                let flag =
                    self.registers[left_register_index] > self.registers[right_register_index];
                self.registers[left_register_index] =
                    self.registers[right_register_index] - self.registers[left_register_index];
                self.registers[0xF] = flag as u8;
                Ok(())
            }
            Opcode::ShiftLeft {
                left_register_index,
                right_register_index,
            } => {
                self.validate_register(opcode.clone(), left_register_index)?;
                self.validate_register(opcode.clone(), right_register_index)?;

                let flag = self.registers[right_register_index] & 0x80;
                self.registers[left_register_index] = self.registers[right_register_index] << 1;
                self.registers[0xF] = flag as u8;
                Ok(())
            }
            Opcode::SkipNotEqualRegister {
                left_register_index,
                right_register_index,
            } => {
                self.validate_register(opcode.clone(), left_register_index)?;
                self.validate_register(opcode.clone(), right_register_index)?;

                if self.registers[left_register_index] != self.registers[right_register_index] {
                    self.instruction_pointer += 2;
                }
                Ok(())
            }
            Opcode::MoveIndexRegister { address } => {
                self.validate_address(opcode, address)?;

                self.index_register = address;
                Ok(())
            }
            Opcode::OffsetJump { address } => {
                let computed_address = address + (self.registers[0] as usize);
                self.validate_address(opcode, computed_address)?;
                self.instruction_pointer = computed_address;
                Ok(())
            }
            Opcode::Rand {
                register_index,
                immediate,
            } => {
                self.validate_register(opcode, register_index)?;

                self.registers[register_index] = fastrand::u8(u8::MIN..=u8::MAX) & immediate;
                Ok(())
            }
            Opcode::Draw {
                x_register_index,
                y_register_index,
                sprite_height,
            } => {
                self.validate_register(opcode.clone(), x_register_index)?;
                self.validate_register(opcode.clone(), y_register_index)?;

                self.registers[0xF] = 0;

                // self.registers[x_register_index] %= SCREEN_WIDTH as u8;
                // self.registers[y_register_index] %= SCREEN_HEIGHT as u8;

                for delta_y in 0..sprite_height {
                    let y = (self.registers[y_register_index] + delta_y) as usize;
                    for delta_x in 0..8 {
                        let x = (self.registers[x_register_index] + delta_x) as usize;
                        let byte = self.ram[self.index_register + delta_y as usize];
                        let pixel = byte & (0x80 >> delta_x) > 0;
                        self.vram[y % SCREEN_HEIGHT][x % SCREEN_WIDTH] ^= pixel;
                    }
                }
                Ok(())
            }
            Opcode::SkipKeyPressed { register_index } => {
                self.validate_register(opcode, register_index)?;

                if self.keypad[self.registers[register_index] as usize] {
                    self.instruction_pointer += 2;
                }
                Ok(())
            }
            Opcode::SkipKeyNotPressed { register_index } => {
                self.validate_register(opcode, register_index)?;

                if !self.keypad[self.registers[register_index] as usize] {
                    self.instruction_pointer += 2;
                }
                Ok(())
            }
            Opcode::LoadDelayTimerToRegister { register_index } => {
                self.validate_register(opcode, register_index)?;

                self.registers[register_index] = self.delay_timer;
                Ok(())
            }
            Opcode::WaitForInput { register_index } => todo!(),
            Opcode::LoadDelayTimerFromRegister { register_index } => {
                self.validate_register(opcode, register_index)?;

                self.delay_timer = self.registers[register_index];
                Ok(())
            }
            Opcode::LoadSoundTimerFromRegister { register_index } => {
                self.validate_register(opcode, register_index)?;

                self.sound_timer = self.registers[register_index];
                Ok(())
            }
            Opcode::AddIndexRegister { register_index } => {
                self.validate_register(opcode, register_index)?;

                self.index_register += self.registers[register_index] as usize;
                Ok(())
            }
            Opcode::LoadIndexRegisterFromRegister { register_index } => {
                self.validate_register(opcode, register_index)?;

                self.index_register = (self.registers[register_index] * 0x5) as usize;
                Ok(())
            }
            Opcode::LoadBCD { register_index } => {
                self.validate_register(opcode, register_index)?;

                // TODO: bound check on the indexing
                self.ram[self.index_register] = self.registers[register_index] / 100;
                self.ram[self.index_register + 1] = (self.registers[register_index] / 10) % 10;
                self.ram[self.index_register + 2] = self.registers[register_index] % 10;
                Ok(())
            }
            Opcode::SaveRegisters { register_index } => {
                self.validate_register(opcode, register_index)?;

                for i in 0..=register_index {
                    // TODO: bound check on the indexing
                    self.ram[self.index_register + i] = self.registers[i]
                }
                Ok(())
            }
            Opcode::LoadRegisters { register_index } => {
                self.validate_register(opcode, register_index)?;

                for i in 0..=register_index {
                    // TODO: bound check on the indexing
                    self.registers[i] = self.ram[self.index_register + i]
                }
                Ok(())
            }
        }
    }

    fn validate_address(&self, opcode: Opcode, address: usize) -> Result<(), RuntimeError> {
        if address >= self.ram.len() {
            return Err(RuntimeError::new(
                opcode,
                format!("Invalid address: {}", address),
            ));
        }
        Ok(())
    }

    fn validate_register(&self, opcode: Opcode, register_index: usize) -> Result<(), RuntimeError> {
        if register_index >= self.registers.len() {
            return Err(RuntimeError::new(
                opcode,
                format!("Invalid register: V{:}", register_index),
            ));
        }
        Ok(())
    }

    pub fn set_key(&mut self, index: usize, value: bool) {
        self.keypad[index] = value;
    }
}

#[cfg(test)]
mod chip_8_tests {
    use super::*;

    #[test]
    fn test_new() {
        let emulator = Chip8::new();
        // assert!(emulator.vram.iter().all(|&pixel| pixel == false));
    }
}
