#ifndef BEARCLAVE_COMMON_H
#define BEARCLAVE_COMMON_H

// NOLINTBEGIN
#define CK_VERSION_MAJOR '3'
#define CK_VERSION_MINOR '0'
#define FLAGS 0UL
#define FW_VERSION_MAJOR '0'
#define FW_VERSION_MINOR '1'
#define HW_VERSION_MAJOR '0'
#define HW_VERSION_MINOR '1'
#define LIB_DESC "Bearclave PKCS#11 HSM Emulator"
#define LIB_DESC_SIZE 32
#define LIB_VERSION_MAJOR '0'
#define LIB_VERSION_MINOR '1'
#define MAN_ID "Bearclave"
#define MAN_ID_SIZE 32
#define MODEL_SIZE 16
#define PAD_VAL ' '
#define SERIAL_SIZE 16
#define SLOT_DESC_SIZE 64
#define TIME_SIZE 16
#define TOKEN_LABEL_SIZE 32
// NOLINTEND

typedef struct version {
	unsigned char major;
	unsigned char minor;
} version_t;

#endif // BEARCLAVE_COMMON_H
