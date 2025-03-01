use super::error::DecodeError;

// TODO: figure out how to not clone the opcode
#[derive(Debug, PartialEq, Clone)]
pub enum Opcode {
    ClearDisplay,
    Return,
    Jump {
        address: usize,
    },
    Call {
        address: usize,
    },
    SkipEqualImmediate {
        register_index: usize,
        immediate: u8,
    },
    SkipNotEqualImmediate {
        register_index: usize,
        immediate: u8,
    },
    SkipEqualRegister {
        left_register_index: usize,
        right_register_index: usize,
    },
    MoveImmediate {
        register_index: usize,
        immediate: u8,
    },
    AddImmediate {
        register_index: usize,
        immediate: u8,
    },
    MoveRegister {
        destination_register_index: usize,
        source_register_index: usize,
    },
    Or {
        left_register_index: usize,
        right_register_index: usize,
    },
    And {
        left_register_index: usize,
        right_register_index: usize,
    },
    Xor {
        left_register_index: usize,
        right_register_index: usize,
    },
    Add {
        left_register_index: usize,
        right_register_index: usize,
    },
    Substract {
        left_register_index: usize,
        right_register_index: usize,
    },
    ShiftRight {
        left_register_index: usize,
        right_register_index: usize,
    },
    LSubstract {
        left_register_index: usize,
        right_register_index: usize,
    },
    ShiftLeft {
        left_register_index: usize,
        right_register_index: usize,
    },
    SkipNotEqualRegister {
        left_register_index: usize,
        right_register_index: usize,
    },
    MoveIndexRegister {
        address: usize,
    },
    OffsetJump {
        address: usize,
    },
    Rand {
        register_index: usize,
        immediate: u8,
    },
    Draw {
        x_register_index: usize,
        y_register_index: usize,
        sprite_height: usize,
    },
    SkipKeyPressed {
        register_index: usize,
    },
    SkipKeyNotPressed {
        register_index: usize,
    },
    LoadDelayTimerToRegister {
        register_index: usize,
    },
    WaitForInput {
        register_index: usize,
    },
    LoadDelayTimerFromRegister {
        register_index: usize,
    },
    LoadSoundTimerFromRegister {
        register_index: usize,
    },
    AddIndexRegister {
        register_index: usize,
    },
    LoadIndexRegisterFromRegister {
        register_index: usize,
    },
    LoadBCD {
        register_index: usize,
    },
    SaveRegisters {
        register_index: usize,
    },
    LoadRegisters {
        register_index: usize,
    },
}

pub fn decode_opcode(raw_opcode: u16) -> Result<Opcode, DecodeError> {
    let hb1: u8 = (raw_opcode & 0x000F) as u8;
    let hb2: u8 = ((raw_opcode & 0x00F0) >> 4) as u8;
    let hb3: u8 = ((raw_opcode & 0x0F00) >> 8) as u8;
    let hb4: u8 = ((raw_opcode & 0xF000) >> 12) as u8;

    match (hb4, hb3, hb2, hb1) {
        (0, 0, 0xE, 0) => Ok(Opcode::ClearDisplay),
        (0, 0, 0xE, 0xE) => Ok(Opcode::Return),
        (0x1, _, _, _) => Ok(Opcode::Jump {
            address: (raw_opcode & 0x0FFF) as usize,
        }),
        (0x2, _, _, _) => Ok(Opcode::Call {
            address: (raw_opcode & 0x0FFF) as usize,
        }),
        (0x3, _, _, _) => Ok(Opcode::SkipEqualImmediate {
            register_index: hb3 as usize,
            immediate: (raw_opcode & 0x00FF) as u8,
        }),
        (0x4, _, _, _) => Ok(Opcode::SkipNotEqualImmediate {
            register_index: hb3 as usize,
            immediate: (raw_opcode & 0x00FF) as u8,
        }),
        (0x5, _, _, 0x0) => Ok(Opcode::SkipEqualRegister {
            left_register_index: hb3 as usize,
            right_register_index: hb2 as usize,
        }),
        (0x6, _, _, _) => Ok(Opcode::MoveImmediate {
            register_index: hb3 as usize,
            immediate: (raw_opcode & 0x00FF) as u8,
        }),
        (0x7, _, _, _) => Ok(Opcode::AddImmediate {
            register_index: hb3 as usize,
            immediate: (raw_opcode & 0x00FF) as u8,
        }),
        (0x8, _, _, 0x0) => Ok(Opcode::MoveRegister {
            destination_register_index: hb3 as usize,
            source_register_index: hb2 as usize,
        }),
        (0x8, _, _, 0x1) => Ok(Opcode::Or {
            left_register_index: hb3 as usize,
            right_register_index: hb2 as usize,
        }),
        (0x8, _, _, 0x2) => Ok(Opcode::And {
            left_register_index: hb3 as usize,
            right_register_index: hb2 as usize,
        }),
        (0x8, _, _, 0x3) => Ok(Opcode::Xor {
            left_register_index: hb3 as usize,
            right_register_index: hb2 as usize,
        }),
        (0x8, _, _, 0x4) => Ok(Opcode::Add {
            left_register_index: hb3 as usize,
            right_register_index: hb2 as usize,
        }),
        (0x8, _, _, 0x5) => Ok(Opcode::Substract {
            left_register_index: hb3 as usize,
            right_register_index: hb2 as usize,
        }),
        (0x8, _, _, 0x6) => Ok(Opcode::ShiftRight {
            left_register_index: hb3 as usize,
            right_register_index: hb2 as usize,
        }),
        (0x8, _, _, 0x7) => Ok(Opcode::LSubstract {
            left_register_index: hb3 as usize,
            right_register_index: hb2 as usize,
        }),
        (0x8, _, _, 0xE) => Ok(Opcode::ShiftLeft {
            left_register_index: hb3 as usize,
            right_register_index: hb2 as usize,
        }),
        (0x9, _, _, 0x0) => Ok(Opcode::SkipNotEqualRegister {
            left_register_index: hb3 as usize,
            right_register_index: hb2 as usize,
        }),
        (0xA, _, _, _) => Ok(Opcode::MoveIndexRegister {
            address: (raw_opcode & 0x0FFF) as usize,
        }),
        (0xB, _, _, _) => Ok(Opcode::OffsetJump {
            address: (raw_opcode & 0x0FFF) as usize,
        }),
        (0xC, _, _, _) => Ok(Opcode::Rand {
            register_index: hb3 as usize,
            immediate: (raw_opcode & 0x00FF) as u8,
        }),
        (0xD, _, _, _) => Ok(Opcode::Draw {
            x_register_index: hb3 as usize,
            y_register_index: hb2 as usize,
            sprite_height: hb1 as usize,
        }),
        (0xE, _, 0x9, 0xE) => Ok(Opcode::SkipKeyPressed {
            register_index: hb3 as usize,
        }),
        (0xE, _, 0xA, 0x1) => Ok(Opcode::SkipKeyNotPressed {
            register_index: hb3 as usize,
        }),
        (0xF, _, 0x0, 0x7) => Ok(Opcode::LoadDelayTimerToRegister {
            register_index: hb3 as usize,
        }),
        (0xF, _, 0x0, 0xA) => Ok(Opcode::WaitForInput {
            register_index: hb3 as usize,
        }),
        (0xF, _, 0x1, 0x5) => Ok(Opcode::LoadDelayTimerFromRegister {
            register_index: hb3 as usize,
        }),
        (0xF, _, 0x1, 0x8) => Ok(Opcode::LoadSoundTimerFromRegister {
            register_index: hb3 as usize,
        }),
        (0xF, _, 0x1, 0xE) => Ok(Opcode::AddIndexRegister {
            register_index: hb3 as usize,
        }),
        (0xF, _, 0x2, 0x9) => Ok(Opcode::LoadIndexRegisterFromRegister {
            register_index: hb3 as usize,
        }),
        (0xF, _, 0x3, 0x3) => Ok(Opcode::LoadBCD {
            register_index: hb3 as usize,
        }),
        (0xF, _, 0x5, 0x5) => Ok(Opcode::SaveRegisters {
            register_index: hb3 as usize,
        }),
        (0xF, _, 0x6, 0x5) => Ok(Opcode::LoadRegisters {
            register_index: hb3 as usize,
        }),
        (_, _, _, _) => Err(DecodeError(raw_opcode)),
    }
}

#[cfg(test)]
mod opcode_tests {
    use super::*;

    #[test]
    fn test_decode_clear_display() {
        assert_eq!(decode_opcode(0x00E0), Ok(Opcode::ClearDisplay));
    }

    #[test]
    fn test_decode_return() {
        assert_eq!(decode_opcode(0x00EE), Ok(Opcode::Return));
    }

    #[test]
    fn test_decode_jump() {
        assert_eq!(decode_opcode(0x1FFF), Ok(Opcode::Jump { address: 0xFFF }));
    }

    #[test]
    fn test_decode_call() {
        assert_eq!(decode_opcode(0x2123), Ok(Opcode::Call { address: 0x123 }));
    }

    #[test]
    fn test_decode_skip_equal_immediate() {
        assert_eq!(
            decode_opcode(0x3456),
            Ok(Opcode::SkipEqualImmediate {
                register_index: 0x4,
                immediate: 0x56
            })
        );
    }

    #[test]
    fn test_decode_skip_not_equal_immediate() {
        assert_eq!(
            decode_opcode(0x4987),
            Ok(Opcode::SkipNotEqualImmediate {
                register_index: 0x9,
                immediate: 0x87
            })
        );
    }

    #[test]
    fn test_decode_skip_equal_register() {
        assert_eq!(
            decode_opcode(0x5AB0),
            Ok(Opcode::SkipEqualRegister {
                left_register_index: 0xA,
                right_register_index: 0xB
            })
        );
        assert_eq!(decode_opcode(0x5AB1), Err(DecodeError(0x5AB1)));
    }

    #[test]
    fn test_decode() {
        assert_eq!(
            decode_opcode(0x6732),
            Ok(Opcode::MoveImmediate {
                register_index: 0x7,
                immediate: 0x32
            })
        )
    }
}
