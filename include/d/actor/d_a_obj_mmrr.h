#ifndef D_A_OBJ_MMRR_H
#define D_A_OBJ_MMRR_H

#include "f_op/f_op_actor.h"
#include "d/d_cc_d.h"
#include "d/d_particle.h"
#include "m_Do/m_Do_ext.h"

namespace daObjMmrr {
    class Eff_c : public dPa_followEcallBack {
    public:
        Eff_c() {}
        virtual ~Eff_c() {}
        virtual void end();
        void remove();
    };

    class Act_c : public fopAc_ac_c {
    public:
        void setup(const cXyz* pos) {
            current.pos = *pos;
            mBF8 = 1;
        }

        static BOOL solidHeapCB(fopAc_ac_c*);
        bool create_heap();
        void init_cc();
        void set_cc_rec_pos();
        void set_cc_trans_pos();
        void set_cull();
        cPhs_State _create();
        bool _delete();
        void set_mtx();
        void init_mtx();
        bool chk_light();
        void eff_start();
        void eff_stop();
        void eff_remove();
        bool _execute();
        bool _draw();

    public:
        static const char M_arcname[];
        static const dCcD_SrcTri M_tri_src;
        static const dCcD_SrcCps M_cps_src;

        /* 0x290 */ request_of_phase_process_class mPhase;
        /* 0x298 */ J3DModel* mpMirrorModel;
        /* 0x29C */ J3DModel* mpLightModel;
        /* 0x2A0 */ mDoExt_btkAnm mMirrorBtk;
        /* 0x2B4 */ mDoExt_btkAnm mLightBtk;
        /* 0x2C8 */ dCcD_Tri mTri[5];
        /* 0x958 */ dCcD_Stts mTriStts[5];
        /* 0xA84 */ dCcD_Cps mLightCps;
        /* 0xBBC */ dCcD_Stts mLightStts;
        /* 0xBF8 */ u8 mBF8;
        /* 0xBF9 */ bool mLit;
        /* 0xBFC */ f32 mLightAmount;
        /* 0xC00 */ f32 mLightScale;
        /* 0xC04 */ Eff_c mEffect;
    };

} // namespace daObjMmrr

STATIC_ASSERT(sizeof(daObjMmrr::Act_c) == 0xC18);

#endif /* D_A_OBJ_MMRR_H */
