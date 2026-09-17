#ifndef D_A_WARPLS_H
#define D_A_WARPLS_H

#include "f_op/f_op_actor.h"
#include "m_Do/m_Do_ext.h"
#include "d/d_particle.h"

class daWarpls_c : public fopAc_ac_c {
public:
    inline bool _draw();
    void set_mtx(); // weak but not inlined?

    bool _delete();
    BOOL CreateHeap();
    void CreateInit();
    cPhs_State _create();
    bool _execute();
    void checkOrder();
    void eventOrder();
    BOOL setStatus();
    BOOL demo();
    BOOL check_warp_link();
    BOOL check_warp_distance();
    void warp_eff_start();

public:
    static const char* m_arcname[2];
    static const s16 m_bdlidx[2];
    static const s16 m_brkidx[2];
    static const s16 m_bckidx[2];
    static const u32 m_heapsize[2];
    static const f32 m_warp_distance;

    /* 0x290 */ request_of_phase_process_class mPhase;
    /* 0x298 */ J3DModel* mpModel;
    /* 0x29C */ mDoExt_brkAnm* mpBrk;
    /* 0x2A0 */ mDoExt_bckAnm* mpBck;
    /* 0x2A4 */ JPABaseEmitter* mpEmitter;
    /* 0x2A8 */ s32 mSwitch;
    /* 0x2AC */ s32 mExit;
    /* 0x2B0 */ s32 mOrder;
    /* 0x2B4 */ u8 mActive;
    /* 0x2B5 */ u8 mInitiallyActive;
    /* 0x2B6 */ u8 mPrevSwitch;
    /* 0x2B7 */ s8 mTimer;
    /* 0x2B8 */ s16 mAppearEvent;
    /* 0x2BA */ s16 mWarpEvent;
    /* 0x2BC */ u8 mType;
    /* 0x2BD */ u8 mWarpType;
    /* 0x2BE */ u8 mWaitForExit;
};

#endif /* D_A_WARPLS_H */
