/**
 * msg.c
 * Description:
 */

#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msg.h"
#include "trk.h"

extern DSError WriteUART1(char byte);
extern DSError WriteUARTFlush(void);

static inline u8 TRKMessageChecksum(TRKBuffer* msg) {
    int i;
    u8 checksum = 0;
    for (i = 0; i < msg->length; i++) {
        checksum = (u8)(checksum + msg->data[i]);
    }
    return checksum ^ 0xFF;
}

static inline DSError TRKWriteEscapedByte(u8 byte) {
    DSError error;
    do {
        if (byte == 0x7E || byte == 0x7D) {
            error = WriteUART1(0x7D);
            byte ^= 0x20;
            if (error != DS_NoError)
                break;
        }
        error = WriteUART1(byte);
    } while (FALSE);
    return error;
}

DSError TRKMessageSend(TRKBuffer* msg) {
    // Nonmatching: checksum register allocation and escaped checksum instruction ordering.
    DSError error;
    u32 i;
    u8 byte;
    u8 checksum = TRKMessageChecksum(msg);

    error = WriteUART1(0x7E);
    if (error == DS_NoError) {
        for (i = 0; i < msg->length; i++) {
            byte = msg->data[i];
            if (byte == 0x7E || byte == 0x7D) {
                error = WriteUART1(0x7D);
                byte ^= 0x20;
                if (error != DS_NoError)
                    break;
            }
            error = WriteUART1(byte);
            if (error != DS_NoError) {
                break;
            }
        }
    }
    if (error == DS_NoError) {
        error = TRKWriteEscapedByte(checksum);
    }
    if (error == DS_NoError) {
        error = WriteUART1(0x7E);
    }
    if (error == DS_NoError) {
        error = WriteUARTFlush();
    }
    return error;
}
