#pragma once

#include "int_types.hpp"
#include "to_hex_string.hpp"

#include <string>

namespace emulator
{
    /*
        Table containing the intel 'nmemonics' for the instructions indexed by their corresponding opcodes.
    */
    const std::string nmemonics[] = {
        "NOP",
        "LXI",
        "STAX",
        "INX",
        "INR",
        "DCR",
        "MVI",
        "RLC",
        "*NOP",
        "DAD",
        "LDAX",
        "DCX",
        "INR",
        "DCR",
        "MVI",
        "RRC",
        "*NOP",
        "LXI",
        "STAX",
        "INX",
        "INR",
        "DCR",
        "MVI",
        "RAL",
        "*NOP",
        "DAD",
        "LDAX",
        "DCX",
        "INR",
        "DCR",
        "MVI",
        "RAR",
        "*NOP",
        "LXI",
        "SHLD",
        "INX",
        "INR",
        "DCR",
        "MVI",
        "DAA",
        "*NOP",
        "DAD",
        "LHLD",
        "DCX",
        "INR",
        "DCR",
        "MVI",
        "CMA",
        "*NOP",
        "LXI",
        "STA",
        "INX",
        "INR",
        "DCR",
        "MVI",
        "STC",
        "*NOP",
        "DAD",
        "LDA",
        "DCX",
        "INR",
        "DCR",
        "MVI",
        "CMC",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "HLT",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "MOV",
        "ADD",
        "ADD",
        "ADD",
        "ADD",
        "ADD",
        "ADD",
        "ADD",
        "ADD",
        "ADC",
        "ADC",
        "ADC",
        "ADC",
        "ADC",
        "ADC",
        "ADC",
        "ADC",
        "SUB",
        "SUB",
        "SUB",
        "SUB",
        "SUB",
        "SUB",
        "SUB",
        "SUB",
        "SBB",
        "SBB",
        "SBB",
        "SBB",
        "SBB",
        "SBB",
        "SBB",
        "SBB",
        "ANA",
        "ANA",
        "ANA",
        "ANA",
        "ANA",
        "ANA",
        "ANA",
        "ANA",
        "XRA",
        "XRA",
        "XRA",
        "XRA",
        "XRA",
        "XRA",
        "XRA",
        "XRA",
        "ORA",
        "ORA",
        "ORA",
        "ORA",
        "ORA",
        "ORA",
        "ORA",
        "ORA",
        "CMP",
        "CMP",
        "CMP",
        "CMP",
        "CMP",
        "CMP",
        "CMP",
        "CMP",
        "RNZ",
        "POP",
        "JNZ",
        "JMP",
        "CNZ",
        "PUSH",
        "ADI",
        "RST",
        "RZ",
        "RET",
        "JZ",
        "*JMP",
        "CZ",
        "CALL",
        "ACI",
        "RST",
        "RNC",
        "POP",
        "JNC",
        "OUT",
        "CNC",
        "PUSH",
        "SUI",
        "RST",
        "RC",
        "*RET",
        "JC",
        "IN",
        "CC",
        "CALL",
        "SBI",
        "RST",
        "RPO",
        "POP",
        "JPO",
        "XTHL",
        "CPO",
        "PUSH",
        "ANI",
        "RST",
        "RPE",
        "PCHL",
        "JPE",
        "XCHG",
        "CPE",
        "CALL",
        "XRI",
        "RST",
        "RP",
        "POP",
        "JP",
        "DI",
        "CP",
        "PUSH",
        "ORI",
        "RST",
        "RM",
        "SPHL",
        "JM",
        "EI",
        "CM",
        "CALL",
        "CPI",
        "RST"
    };

    /*
        Format string the arguments to the differnt instructions indexed by opcodes.
        bb is meant to be replaced by the hexadecimal representation of the byte that follows the instruction.
        wwww is meant to be replaced by the hexadecimal representation of the word that is determined
        by the two bytes following the instruction.
    */
    const std::string arguments[] = {
        "",
        "BC, wwww",
        "B",
        "BC",
        "B",
        "B",
        "B, bb",
        "",
        "",
        "BC",
        "B",
        "BC",
        "C",
        "C",
        "C, bb",
        "",
        "",
        "DE, wwww",
        "D",
        "DE",
        "D",
        "D",
        "D, bb",
        "",
        "",
        "DE",
        "D",
        "DE",
        "E",
        "E",
        "E, bb",
        "",
        "",
        "HL, wwww",
        "wwww",
        "HL",
        "H",
        "H",
        "H, bb",
        "",
        "",
        "HL",
        "wwww",
        "HL",
        "L",
        "L",
        "L, bb",
        "",
        "",
        "SP, wwww",
        "wwww",
        "SP",
        "M",
        "M",
        "M, bb",
        "",
        "",
        "SP",
        "wwww",
        "SP",
        "A",
        "A",
        "A, bb",
        "",
        "B, B",
        "B, C",
        "B, D",
        "B, E",
        "B, H",
        "B, L",
        "B, M",
        "B, A",
        "C, B",
        "C, C",
        "C, D",
        "C, E",
        "C, H",
        "C, L",
        "C, M",
        "C, A",
        "D, B",
        "D, C",
        "D, D",
        "D, E",
        "D, H",
        "D, L",
        "D, M",
        "D, A",
        "E, B",
        "E, C",
        "E, D",
        "E, E",
        "E, H",
        "E, L",
        "E, M",
        "E, A",
        "H, B",
        "H, C",
        "H, D",
        "H, E",
        "H, H",
        "H, L",
        "H, M",
        "H, A",
        "L, B",
        "L, C",
        "L, D",
        "L, E",
        "L, H",
        "L, L",
        "L, M",
        "L, A",
        "M, B",
        "M, C",
        "M, D",
        "M, E",
        "M, H",
        "M, L",
        "",
        "M, A",
        "A, B",
        "A, C",
        "A, D",
        "A, E",
        "A, H",
        "A, L",
        "A, M",
        "A, A",
        "B",
        "C",
        "D",
        "E",
        "H",
        "L",
        "M",
        "A",
        "B",
        "C",
        "D",
        "E",
        "H",
        "L",
        "M",
        "A",
        "B",
        "C",
        "D",
        "E",
        "H",
        "L",
        "M",
        "A",
        "B",
        "C",
        "D",
        "E",
        "H",
        "L",
        "M",
        "A",
        "B",
        "C",
        "D",
        "E",
        "H",
        "L",
        "M",
        "A",
        "B",
        "C",
        "D",
        "E",
        "H",
        "L",
        "M",
        "A",
        "B",
        "C",
        "D",
        "E",
        "H",
        "L",
        "M",
        "A",
        "B",
        "C",
        "D",
        "E",
        "H",
        "L",
        "M",
        "A",
        "",
        "BC",
        "wwww",
        "wwww",
        "wwww",
        "BC",
        "bb",
        "0",
        "",
        "",
        "wwww",
        "wwww",
        "wwww",
        "wwww",
        "bb",
        "1",
        "",
        "DE",
        "wwww",
        "bb",
        "wwww",
        "DE",
        "bb",
        "2",
        "",
        "",
        "wwww",
        "bb",
        "wwww",
        "wwww",
        "bb",
        "3",
        "",
        "HL",
        "wwww",
        "",
        "wwww",
        "HL",
        "bb",
        "4",
        "",
        "",
        "wwww",
        "",
        "wwww",
        "wwww",
        "bb",
        "5",
        "",
        "PSW",
        "wwww",
        "",
        "wwww",
        "PSW",
        "bb",
        "6",
        "",
        "",
        "wwww",
        "",
        "wwww",
        "wwww",
        "bb",
        "7"
    };

    /*
        Length (in bytes) of each instruction including its arguments.
    */
    const byte instructionLengths[] = {
        1,
        3,
        1,
        1,
        1,
        1,
        2,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        2,
        1,
        1,
        3,
        1,
        1,
        1,
        1,
        2,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        2,
        1,
        1,
        3,
        3,
        1,
        1,
        1,
        2,
        1,
        1,
        1,
        3,
        1,
        1,
        1,
        2,
        1,
        1,
        3,
        3,
        1,
        1,
        1,
        2,
        1,
        1,
        1,
        3,
        1,
        1,
        1,
        2,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        3,
        3,
        3,
        1,
        2,
        1,
        1,
        1,
        3,
        3,
        3,
        3,
        2,
        1,
        1,
        1,
        3,
        2,
        3,
        1,
        2,
        1,
        1,
        1,
        3,
        2,
        3,
        3,
        2,
        1,
        1,
        1,
        3,
        1,
        3,
        1,
        2,
        1,
        1,
        1,
        3,
        1,
        3,
        3,
        2,
        1,
        1,
        1,
        3,
        1,
        3,
        1,
        2,
        1,
        1,
        1,
        3,
        1,
        3,
        3,
        2,
        1
    };

    // Formats an argument string of an instruction.
    // Replaces any 'bb' in a argument format string with the hexadecimal representation of byte2
    // Replaces any 'wwww' with the hexadecimal representation of the word determined by byte2 and byte3.
    // Returns the formatted argument string.
    inline std::string formatInstructionArguments(byte opCode, byte byte2, byte byte3)
    {   
        std::size_t pos = arguments[opCode].find("bb", 0);
        if (pos != std::string::npos)
        {
            std::string argumentString(arguments[opCode]);
            argumentString.replace(pos, 2, toHexString(byte2));
            return argumentString;
        }

        pos = arguments[opCode].find("wwww", 0);
        if (pos != std::string::npos)
        {
            word value = bytesAsWord(byte3, byte2);

            std::string argumentString(arguments[opCode]);
            argumentString.replace(pos, 4, toHexString(value));
            return argumentString;
        }

        return arguments[opCode];
    }
} // namespace emulator
