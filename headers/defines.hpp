#pragma once

// Should the memory class check the bounds of the index whenever memory is accessed?
#define EMULATOR_CHECK_BOUNDS true

// Should the cpu class check whether unspecified opcodes are used?
#define EMULATOR_CHECK_INVALID_OPCODES true

// Should any errors reported by sfml be saved into an error_log.txt file?
#define EMULATOR_LOG_SFML_ERRORS false