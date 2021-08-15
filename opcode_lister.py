# raw copy past of the opcode table found at https://pastraiser.com/cpu/i8080/i8080_opcodes.html
raw_table_string = """
0x 	NOP
1  4
- - - - -	LXI B,d16
3  10
- - - - -	STAX B
1  7
- - - - -	INX B
1  5
- - - - -	INR B
1  5
S Z A P -	DCR B
1  5
S Z A P -	MVI B,d8
2  7
- - - - -	RLC
1  4
- - - - C	*NOP
1  4
- - - - -	DAD B
1  10
- - - - C	LDAX B
1  7
- - - - -	DCX B
1  5
- - - - -	INR C
1  5
S Z A P -	DCR C
1  5
S Z A P -	MVI C,d8
2  7
- - - - -	RRC
1  4
- - - - C
 1x 	*NOP
1  4
- - - - -	LXI D,d16
3  10
- - - - -	STAX D
1  7
- - - - -	INX D
1  5
- - - - -	INR D
1  5
S Z A P -	DCR D
1  5
S Z A P -	MVI D,d8
2  7
- - - - -	RAL
1  4
- - - - C	*NOP
1  4
- - - - -	DAD D
1  10
- - - - C	LDAX D
1  7
- - - - -	DCX D
1  5
- - - - -	INR E
1  5
S Z A P -	DCR E
1  5
S Z A P -	MVI E,d8
2  7
- - - - -	RAR
1  4
- - - - C
 2x 	*NOP
1  4
- - - - -	LXI H,d16
3  10
- - - - -	SHLD a16
3  16
- - - - -	INX H
1  5
- - - - -	INR H
1  5
S Z A P -	DCR H
1  5
S Z A P -	MVI H,d8
2  7
- - - - -	DAA
1  4
S Z A P C	*NOP
1  4
- - - - -	DAD H
1  10
- - - - C	LHLD a16
3  16
- - - - -	DCX H
1  5
- - - - -	INR L
1  5
S Z A P -	DCR L
1  5
S Z A P -	MVI L,d8
2  7
- - - - -	CMA
1  4
- - - - -
 3x 	*NOP
1  4
- - - - -	LXI SP,d16
3  10
- - - - -	STA a16
3  13
- - - - -	INX SP
1  5
- - - - -	INR M
1  10
S Z A P -	DCR M
1  10
S Z A P -	MVI M,d8
2  10
- - - - -	STC
1  4
- - - - C	*NOP
1  4
- - - - -	DAD SP
1  10
- - - - C	LDA a16
3  13
- - - - -	DCX SP
1  5
- - - - -	INR A
1  5
S Z A P -	DCR A
1  5
S Z A P -	MVI A,d8
2  7
- - - - -	CMC
1  4
- - - - C
 4x 	MOV B,B
1  5
- - - - -	MOV B,C
1  5
- - - - -	MOV B,D
1  5
- - - - -	MOV B,E
1  5
- - - - -	MOV B,H
1  5
- - - - -	MOV B,L
1  5
- - - - -	MOV B,M
1  7
- - - - -	MOV B,A
1  5
- - - - -	MOV C,B
1  5
- - - - -	MOV C,C
1  5
- - - - -	MOV C,D
1  5
- - - - -	MOV C,E
1  5
- - - - -	MOV C,H
1  5
- - - - -	MOV C,L
1  5
- - - - -	MOV C,M
1  7
- - - - -	MOV C,A
1  5
- - - - -
 5x 	MOV D,B
1  5
- - - - -	MOV D,C
1  5
- - - - -	MOV D,D
1  5
- - - - -	MOV D,E
1  5
- - - - -	MOV D,H
1  5
- - - - -	MOV D,L
1  5
- - - - -	MOV D,M
1  7
- - - - -	MOV D,A
1  5
- - - - -	MOV E,B
1  5
- - - - -	MOV E,C
1  5
- - - - -	MOV E,D
1  5
- - - - -	MOV E,E
1  5
- - - - -	MOV E,H
1  5
- - - - -	MOV E,L
1  5
- - - - -	MOV E,M
1  7
- - - - -	MOV E,A
1  5
- - - - -
 6x 	MOV H,B
1  5
- - - - -	MOV H,C
1  5
- - - - -	MOV H,D
1  5
- - - - -	MOV H,E
1  5
- - - - -	MOV H,H
1  5
- - - - -	MOV H,L
1  5
- - - - -	MOV H,M
1  7
- - - - -	MOV H,A
1  5
- - - - -	MOV L,B
1  5
- - - - -	MOV L,C
1  5
- - - - -	MOV L,D
1  5
- - - - -	MOV L,E
1  5
- - - - -	MOV L,H
1  5
- - - - -	MOV L,L
1  5
- - - - -	MOV L,M
1  7
- - - - -	MOV L,A
1  5
- - - - -
 7x 	MOV M,B
1  7
- - - - -	MOV M,C
1  7
- - - - -	MOV M,D
1  7
- - - - -	MOV M,E
1  7
- - - - -	MOV M,H
1  7
- - - - -	MOV M,L
1  7
- - - - -	HLT
1  7
- - - - -	MOV M,A
1  7
- - - - -	MOV A,B
1  5
- - - - -	MOV A,C
1  5
- - - - -	MOV A,D
1  5
- - - - -	MOV A,E
1  5
- - - - -	MOV A,H
1  5
- - - - -	MOV A,L
1  5
- - - - -	MOV A,M
1  7
- - - - -	MOV A,A
1  5
- - - - -
 8x 	ADD B
1  4
S Z A P C	ADD C
1  4
S Z A P C	ADD D
1  4
S Z A P C	ADD E
1  4
S Z A P C	ADD H
1  4
S Z A P C	ADD L
1  4
S Z A P C	ADD M
1  7
S Z A P C	ADD A
1  4
S Z A P C	ADC B
1  4
S Z A P C	ADC C
1  4
S Z A P C	ADC D
1  4
S Z A P C	ADC E
1  4
S Z A P C	ADC H
1  4
S Z A P C	ADC L
1  4
S Z A P C	ADC M
1  7
S Z A P C	ADC A
1  4
S Z A P C
 9x 	SUB B
1  4
S Z A P C	SUB C
1  4
S Z A P C	SUB D
1  4
S Z A P C	SUB E
1  4
S Z A P C	SUB H
1  4
S Z A P C	SUB L
1  4
S Z A P C	SUB M
1  7
S Z A P C	SUB A
1  4
S Z A P C	SBB B
1  4
S Z A P C	SBB C
1  4
S Z A P C	SBB D
1  4
S Z A P C	SBB E
1  4
S Z A P C	SBB H
1  4
S Z A P C	SBB L
1  4
S Z A P C	SBB M
1  7
S Z A P C	SBB A
1  4
S Z A P C
 Ax 	ANA B
1  4
S Z A P C	ANA C
1  4
S Z A P C	ANA D
1  4
S Z A P C	ANA E
1  4
S Z A P C	ANA H
1  4
S Z A P C	ANA L
1  4
S Z A P C	ANA M
1  7
S Z A P C	ANA A
1  4
S Z A P C	XRA B
1  4
S Z A P C	XRA C
1  4
S Z A P C	XRA D
1  4
S Z A P C	XRA E
1  4
S Z A P C	XRA H
1  4
S Z A P C	XRA L
1  4
S Z A P C	XRA M
1  7
S Z A P C	XRA A
1  4
S Z A P C
 Bx 	ORA B
1  4
S Z A P C	ORA C
1  4
S Z A P C	ORA D
1  4
S Z A P C	ORA E
1  4
S Z A P C	ORA H
1  4
S Z A P C	ORA L
1  4
S Z A P C	ORA M
1  7
S Z A P C	ORA A
1  4
S Z A P C	CMP B
1  4
S Z A P C	CMP C
1  4
S Z A P C	CMP D
1  4
S Z A P C	CMP E
1  4
S Z A P C	CMP H
1  4
S Z A P C	CMP L
1  4
S Z A P C	CMP M
1  7
S Z A P C	CMP A
1  4
S Z A P C
 Cx 	RNZ
1  11/5
- - - - -	POP B
1  10
- - - - -	JNZ a16
3  10
- - - - -	JMP a16
3  10
- - - - -	CNZ a16
3  17/11
- - - - -	PUSH B
1  11
- - - - -	ADI d8
2  7
S Z A P C	RST 0
1  11
- - - - -	RZ
1  11/5
- - - - -	RET
1  10
- - - - -	JZ a16
3  10
- - - - -	*JMP a16
3  10
- - - - -	CZ a16
3  17/11
- - - - -	CALL a16
3  17
- - - - -	ACI d8
2  7
S Z A P C	RST 1
1  11
- - - - -
 Dx 	RNC
1  11/5
- - - - -	POP D
1  10
- - - - -	JNC a16
3  10
- - - - -	OUT d8
2  10
- - - - -	CNC a16
3  17/11
- - - - -	PUSH D
1  11
- - - - -	SUI d8
2  7
S Z A P C	RST 2
1  11
- - - - -	RC
1  11/5
- - - - -	*RET
1  10
- - - - -	JC a16
3  10
- - - - -	IN d8
2  10
- - - - -	CC a16
3  17/11
- - - - -	*CALL a16
3  17
- - - - -	SBI d8
2  7
S Z A P C	RST 3
1  11
- - - - -
 Ex 	RPO
1  11/5
- - - - -	POP H
1  10
- - - - -	JPO a16
3  10
- - - - -	XTHL
1  18
- - - - -	CPO a16
3  17/11
- - - - -	PUSH H
1  11
- - - - -	ANI d8
2  7
S Z A P C	RST 4
1  11
- - - - -	RPE
1  11/5
- - - - -	PCHL
1  5
- - - - -	JPE a16
3  10
- - - - -	XCHG
1  5
- - - - -	CPE a16
3  17/11
- - - - -	*CALL a16
3  17
- - - - -	XRI d8
2  7
S Z A P C	RST 5
1  11
- - - - -
 Fx 	RP
1  11/5
- - - - -	POP PSW
1  10
S Z A P C	JP a16
3  10
- - - - -	DI
1  4
- - - - -	CP a16
3  17/11
- - - - -	PUSH PSW
1  11
- - - - -	ORI d8
2  7
S Z A P C	RST 6
1  11
- - - - -	RM
1  11/5
- - - - -	SPHL
1  5
- - - - -	JM a16
3  10
- - - - -	EI
1  4
- - - - -	CM a16
3  17/11
- - - - -	*CALL a16
3  17
- - - - -	CPI d8
2  7
S Z A P C	RST 7
1  11
- - - - -
"""

import re

splitter_re = re.compile("[0-9A-F]x")
splitted_string = re.split(splitter_re, raw_table_string);

instruction_re = re.compile("([A-Z*]{2,4})\s+([BCDEHLMA]|PSW|SP)?,?(a16|d8|d16|[BCDEHLMA])?")

del splitted_string[0]

assert len(splitted_string) == 16, "Expected to find 16 opcode groups"

nmemonics = {};
arguments = {};
lengths = {};

for i, part in enumerate(splitted_string):
    instruction_matches = re.findall(instruction_re, part)

    assert len(instruction_matches) == 16, "Expected to find 16 opcode in a group"

    for j, instruction in enumerate(instruction_matches):
        opcode = i << 4 | j

        nmemonics[opcode] = instruction[0]

        second_argument = ""
        length = 1

        if (instruction[0] == "RST"):
            exp = (opcode & 0b00111000) >> 3
            second_argument = f"{exp}"
        else:
            if (instruction[2] == 'd8'):
                second_argument = 'bb'
                length = 2
            elif (instruction[2] in ['a16', 'd16']):
                second_argument = 'wwww'
                length = 3
            elif (instruction[2]):
                second_argument = instruction[2]

        argument = ""
        if (instruction[1]):
            argument += instruction[1]
            if (second_argument):
                argument += ", " + second_argument
        elif (second_argument):
            argument = second_argument

        arguments[opcode] = argument
        lengths[opcode] = length

assert len(nmemonics) == 256, "Expected to find 256 instructions total"
assert len(arguments) == 256, "Expected to find 256 instructions total"

nmemonic_table_string = "const std::string nmemonics[] = {\n"
for i in range(255):
    nmemonic_table_string += '"' + nmemonics[i] + '",\n'
nmemonic_table_string += '"' + nmemonics[255] + '"\n'
nmemonic_table_string += "};"

print(nmemonic_table_string)
print()

argument_table_string = "const std::string arguments[] = {\n"
for i in range(255):
    argument_table_string += '"' + arguments[i] + '",\n'
argument_table_string += '"' + arguments[255] + '"\n'
argument_table_string += "};"

print(argument_table_string)
print()

lengths_table_string = "const byte instructionLengths[] = {\n"
for i in range(255):
    lengths_table_string += f"{lengths[i]},\n"
lengths_table_string += f"{lengths[255]}\n"
lengths_table_string += "};"

print(lengths_table_string)
