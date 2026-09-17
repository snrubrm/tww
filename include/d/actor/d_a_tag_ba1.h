#ifndef D_A_TAG_BA1_H
#define D_A_TAG_BA1_H

#include "f_op/f_op_actor.h"
#include "m_Do/m_Do_hostIO.h"

class daTag_Ba1_c : public fopAc_ac_c {
public:
    bool XyCheck_cB(int);
    s16 XyEvent_cB(int);
    bool createInit();
    BOOL _draw();
    BOOL _execute();
    BOOL _delete();
    cPhs_State _create();

public:
    /* 0x290 */ s16 mEventIdTable[1];
    /* 0x292 */ s16 mEventIndex;
};

class daTag_Ba1_HIO_c : public JORReflexible {
public:
    struct hio_prm_c {
        /* 0x00 */ u8 field_0x00;
    };
    daTag_Ba1_HIO_c();
    virtual ~daTag_Ba1_HIO_c() {}

public:
    /* 0x04 */ s8 mNo;
    /* 0x08 */ int mCount;
    /* 0x0C */ hio_prm_c mPrmTbl;
};

#endif /* D_A_TAG_BA1_H */
