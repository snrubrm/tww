/**
 * d_a_npc_ko1.cpp
 * NPC - Joel & Zill (Outset Island)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_ko1.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo.h"
#include "d/d_particle_name.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_audio.h"
#include "m_Do/m_Do_mtx.h"
#include "JSystem/J3DGraphAnimator/J3DJoint.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "res/Object/Ko.h"
#include <string.h>

static daNpc_Ko1_HIO_c l_HIO;
static fopAc_ac_c* l_check_inf[20];
static int l_check_wrk;

/* 000000EC-00000108       .text __ct__20daNpc_Ko1_childHIO_cFv */
daNpc_Ko1_childHIO_c::daNpc_Ko1_childHIO_c() {}

/* 00000150-0000020C       .text __ct__15daNpc_Ko1_HIO_cFv */
daNpc_Ko1_HIO_c::daNpc_Ko1_HIO_c() {
    static daNpc_Ko1_childHIO_c::hio_prm_c a_prm_tbl[] = {
        {
            0x2000, 0x1F40, (s16)0xE0C0, (s16)0xE0C0,
            0x0000, 0x1F40, 0x0000, (s16)0xE0C0,
            0x07D0, 0x0640,
            90.0f,
            0,
            4, 0x0800, 0,
            0.9f, 1.0f, 0.5f, 21.0f, 0.1f, 10.0f, 0.3f, 40.0f,
            80.0f, 200.0f, 300.0f, 150.0f, 1100.0f, 1500.0f,
        },
        {
            0x2000, 0x1F40, (s16)0xE0C0, (s16)0xE0C0,
            0x0000, 0x1F40, 0x0000, (s16)0xE0C0,
            0x07D0, 0x0640,
            90.0f,
            0,
            6, 0x0800, 0,
            0.3f, 6.0f, 0.4f, 21.0f, 0.1f, 12.0f, 1.5f, 100.0f,
            0.0f, 350.0f, 500.0f, 150.0f, 0.0f, 0.0f,
        },
    };
    for (int i = 0; i < 2; i++) {
        mChild[i].mIndex = i;
        memcpy(&mChild[i].mPrm, &a_prm_tbl[i], sizeof(daNpc_Ko1_childHIO_c::hio_prm_c));
    }
    mNo = -1;
    mSelected = -1;
}

inline daNpc_Ko1_childHIO_c::~daNpc_Ko1_childHIO_c() {}

/* 00000268-000002E0       .text searchActor_Ko_Hna__FPvPv */
static void* searchActor_Ko_Hna(void* actor, void*) {
    if (l_check_wrk < 20 && fopAc_IsActor(actor) && fopAcM_GetName(actor) == fpcNm_NPC_KO1_e) {
        l_check_inf[l_check_wrk] = (fopAc_ac_c*)actor;
        l_check_wrk++;
    }
    return NULL;
}

/* 000002E0-00000358       .text searchActor_Ko_Bou__FPvPv */
static void* searchActor_Ko_Bou(void* actor, void*) {
    if (l_check_wrk < 20 && fopAc_IsActor(actor) && fopAcM_GetName(actor) == fpcNm_NPC_KO2_e) {
        l_check_inf[l_check_wrk] = (fopAc_ac_c*)actor;
        l_check_wrk++;
    }
    return NULL;
}

/* 00000358-000003D0       .text searchActor_Ob__FPvPv */
static void* searchActor_Ob(void* actor, void*) {
    if (l_check_wrk < 20 && fopAc_IsActor(actor) && fopAcM_GetName(actor) == fpcNm_NPC_OB1_e) {
        l_check_inf[l_check_wrk] = (fopAc_ac_c*)actor;
        l_check_wrk++;
    }
    return NULL;
}

/* 000003D0-0000041C       .text nodeCallBack_Hed__FP7J3DNodei */
static BOOL nodeCallBack_Hed(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Ko1_c* i_this = (daNpc_Ko1_c*)model->getUserArea();
        if (i_this != NULL) {
            i_this->nodeHedControl(node, model);
        }
    }
    return TRUE;
}

/* 0000041C-00000488       .text nodeHedControl__11daNpc_Ko1_cFP7J3DNodeP8J3DModel */
void daNpc_Ko1_c::nodeHedControl(J3DNode* node, J3DModel* model) {
    int joint = ((J3DJoint*)node)->getJntNo();
    mDoMtx_stack_c::copy(model->getAnmMtx(joint));
    if (joint == m_hed_2_jnt_num) {
        cMtx_copy(mDoMtx_stack_c::get(), mHeadMtx);
    }
}

/* 00000488-000004D4       .text nodeCallBack_Bln__FP7J3DNodei */
static BOOL nodeCallBack_Bln(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Ko1_c* i_this = (daNpc_Ko1_c*)model->getUserArea();
        if (i_this != NULL) {
            i_this->nodeBlnControl(node, model);
        }
    }
    return TRUE;
}

/* 000004D4-00000554       .text nodeBlnControl__11daNpc_Ko1_cFP7J3DNodeP8J3DModel */
void daNpc_Ko1_c::nodeBlnControl(J3DNode* node, J3DModel* model) {
    int joint = ((J3DJoint*)node)->getJntNo();
    mDoMtx_stack_c::copy(model->getAnmMtx(joint));
    if (joint == m_bln_loc_jnt_num) {
        cMtx_copy(mHeadMtx, J3DSys::mCurrentMtx);
        model->setAnmMtx(joint, mHeadMtx);
    }
}

/* 00000554-000005A0       .text nodeCallBack_Ko1__FP7J3DNodei */
static BOOL nodeCallBack_Ko1(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Ko1_c* i_this = (daNpc_Ko1_c*)model->getUserArea();
        if (i_this != NULL) {
            i_this->nodeKo1Control(node, model);
        }
    }
    return TRUE;
}

/* 000005A0-000006E8       .text nodeKo1Control__11daNpc_Ko1_cFP7J3DNodeP8J3DModel */
void daNpc_Ko1_c::nodeKo1Control(J3DNode* node, J3DModel* model) {
    static cXyz a_eye_pos_off(18.0f, 20.0f, 0.0f);
    int joint = ((J3DJoint*)node)->getJntNo();
    mDoMtx_stack_c::copy(model->getAnmMtx(joint));
    if (joint == m_hed_jnt_num) {
        mDoMtx_stack_c::XrotM(m_jnt.getHead_y());
        mDoMtx_stack_c::ZrotM(-m_jnt.getHead_x());
        mDoMtx_stack_c::multVec(&a_eye_pos_off, &mEyePos);
    }
    if (joint == m_bbone_jnt_num) {
        mDoMtx_stack_c::XrotM(m_jnt.getBackbone_y());
        mDoMtx_stack_c::YrotM(m_jnt.getBackbone_x());
    }
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    model->setAnmMtx(joint, mDoMtx_stack_c::get());
}

/* 00000724-000007CC       .text init_HNA_0__11daNpc_Ko1_cFv */
bool daNpc_Ko1_c::init_HNA_0() {
    if (dComIfGs_isEventBit(0x2A80)) {
        if (!dComIfGs_isEventBit(0xE20)) {
            mpBalloonMorf = NULL;
            set_action(&daNpc_Ko1_c::hana_action1, NULL);
            return true;
        }
    }
    return false;
}

/* 000007CC-00000860       .text init_HNA_1__11daNpc_Ko1_cFv */
bool daNpc_Ko1_c::init_HNA_1() {
    if (dComIfGs_isEventBit(0xE20)) {
        mpBalloonMorf = NULL;
        fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
        set_action(&daNpc_Ko1_c::hana_action2, NULL);
        return true;
    }
    return false;
}

/* 00000860-000008E8       .text init_HNA_2__11daNpc_Ko1_cFv */
bool daNpc_Ko1_c::init_HNA_2() {
    if (!dComIfGs_isEventBit(0x2A80)) {
        mpBalloonMorf = NULL;
        set_action(&daNpc_Ko1_c::hana_action3, NULL);
        return true;
    }
    return false;
}

/* 000008E8-00000994       .text init_HNA_3__11daNpc_Ko1_cFv */
bool daNpc_Ko1_c::init_HNA_3() {
    if (dComIfGs_isEventBit(0x520)) {
        if (dKy_daynight_check() == 0) {
            fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
            fopAcM_OnStatus(this, fopAcStts_UNK4000_e);
            mpBalloonMorf = NULL;
            set_action(&daNpc_Ko1_c::hana_action4, NULL);
            return true;
        }
    }
    return false;
}

/* 00000994-00000A20       .text init_HNA_4__11daNpc_Ko1_cFv */
bool daNpc_Ko1_c::init_HNA_4() {
    if (dComIfGs_isEventBit(0x520)) {
        if (dKy_daynight_check() == 1) {
            set_action(&daNpc_Ko1_c::hana_action5, NULL);
            return true;
        }
    }
    return false;
}

/* 00000A20-00000AAC       .text init_BOU_0__11daNpc_Ko1_cFv */
bool daNpc_Ko1_c::init_BOU_0() {
    if (!dComIfGs_isEventBit(0xE20)) {
        set_action(&daNpc_Ko1_c::wait_action1, NULL);
        fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
        return true;
    }
    return false;
}

/* 00000AAC-00000B38       .text init_BOU_1__11daNpc_Ko1_cFv */
bool daNpc_Ko1_c::init_BOU_1() {
    if (dComIfGs_isEventBit(0xE20)) {
        set_action(&daNpc_Ko1_c::wait_action2, NULL);
        fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
        return true;
    }
    return false;
}

/* 00000B38-00000BE4       .text init_BOU_2__11daNpc_Ko1_cFv */
bool daNpc_Ko1_c::init_BOU_2() {
    if (dComIfGs_isEventBit(0x520)) {
        if (dKy_daynight_check() == 0) {
            fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
            fopAcM_OnStatus(this, fopAcStts_UNK4000_e);
            set_action(&daNpc_Ko1_c::wait_action3, NULL);
            mpItemModel = NULL;
            return true;
        }
    }
    return false;
}

/* 00000BE4-00000C78       .text init_BOU_3__11daNpc_Ko1_cFv */
bool daNpc_Ko1_c::init_BOU_3() {
    if (dComIfGs_isEventBit(0x520)) {
        if (dKy_daynight_check() == 1) {
            set_action(&daNpc_Ko1_c::wait_action4, NULL);
            mpItemModel = NULL;
            return true;
        }
    }
    return false;
}

/* 00000C78-00000F14       .text createInit__11daNpc_Ko1_cFv */
bool daNpc_Ko1_c::createInit() {
    static char* a_staff_tbl[] = {
        "Ko1", "Ko1", "Ko1", "Ko1", "Ko1",
        "Ko2", "Ko2", "Ko2", "Ko2",
    };

    attention_info.flags = 0xA;
    switch (mType) {
    case 0:
        attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0xA9;
        attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0xA9;
        break;
    case 1:
        attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0xA7;
        attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0xA9;
        break;
    }

    gravity = -4.5f;
    mHomeAngle = current.angle;
    mHomePos = current.pos;
    mTargetPos = mHomePos;
    mLookPos = mTargetPos;

    int weight = 0xFF;
    u8 pathIdx = (fopAcM_GetParam(this) >> 16) & 0xFF;
    if (pathIdx != 0xFF) {
        mPath.setInf(pathIdx, current.roomNo, 1);
        if (mPath.getPath() == NULL) {
            return false;
        }
        fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
        weight = 0xF0;
    }

    mCut.setActorInfo2(a_staff_tbl[mStaff], this);
    mAnmIndex = 0xE;

    bool result;
    switch (mStaff) {
    case 0:
        result = init_HNA_0();
        break;
    case 1:
        result = init_HNA_1();
        weight = 0xF0;
        break;
    case 2:
        result = init_HNA_2();
        weight = 0xF0;
        break;
    case 3:
        result = init_HNA_3();
        break;
    case 4:
        result = init_HNA_4();
        break;
    case 5:
        result = init_BOU_0();
        break;
    case 6:
        result = init_BOU_1();
        weight = 0xF0;
        break;
    case 7:
        result = init_BOU_2();
        break;
    case 8:
        result = init_BOU_3();
        break;
    default:
        result = false;
        break;
    }
    if (result) {
        mModelAngle = current.angle;
        shape_angle = mModelAngle;
    } else {
        return false;
    }
    mStts.Init(weight, 0xFF, this);
    mCyl.SetStts(&mStts);
    mCyl.Set(dNpc_cyl_src);
    mpMorf->setMorf(0.0f);
    mpHeadMorf->setMorf(0.0f);
    setMtx(true);
    return true;
}

/* 00000F14-000011C4       .text setMtx__11daNpc_Ko1_cFb */
void daNpc_Ko1_c::setMtx(bool param) {
    if (mDemo == 0) {
        u32 sound = 0;
        plyTexPttrnAnm();
        if (mObjAcch.ChkGroundHit()) {
            sound = dComIfG_Bgsp()->GetMtrlSndId(mObjAcch.m_gnd);
        }
        mAnmEnded = mpMorf->play(&eyePos, sound, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        if (mpMorf->getFrame() < mLastFrame) {
            mAnmEnded = 1;
        }
        mLastFrame = mpMorf->getFrame();
        if (mAnmIndex == 4) {
            if (mpMorf->checkFrame(10.0f)) {
                mDoAud_seStart(JA_SE_CM_GAKI_HANASUSURI, &eyePos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            }
        }
        mpHeadMorf->play(&eyePos, 0, 0);
        if (mpBalloonMorf != NULL && m865 < 2) {
            mpBalloonMorf->setFrame(mpMorf->getFrame());
        }
        m86C = mObjAcch.ChkGroundHit();
        mObjAcch.CrrPos(*dComIfG_Bgsp());
    }
    tevStr.mRoomNo = dComIfG_Bgsp()->GetRoomId(mObjAcch.m_gnd);
    tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mObjAcch.m_gnd);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(mModelAngle.y);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->calc();
    mpHeadMorf->getModel()->setBaseTRMtx(mpMorf->getModel()->getAnmMtx(m_hed_jnt_num));
    mpHeadMorf->calc();
    if (mpBalloonMorf != NULL) {
        mpBalloonMorf->calc();
    }
    if (mpItemModel != NULL) {
        mpItemModel->setBaseTRMtx(mpMorf->getModel()->getAnmMtx(m_armR2_jnt_num));
        mpItemModel->calc();
    }
    setAttention(param);
}

/* 000011C4-000011D8       .text anmNum_toResID__11daNpc_Ko1_cFi */
int daNpc_Ko1_c::anmNum_toResID(int idx) {
    static const int a_bck_resID_tbl[] = {
        dRes_ID_KO_BCK_KO_WAIT01_e,
        dRes_ID_KO_BCK_KO_RUN01_e,
        dRes_ID_KO_BCK_KO_WALK01_e,
        dRes_ID_KO_BCK_KO_TALK01_e,
        dRes_ID_KO_BCK_KO_SUSURI_e,
        dRes_ID_KO_BCK_KO_BUTUKARI_e,
        dRes_ID_KO_BCK_KO_WAIT02_e,
        dRes_ID_KO_BCK_KO_SLEEP01_e,
        dRes_ID_KO_BCK_KO_SLEEP02_e,
        dRes_ID_KO_BCK_KO_SLEEP03_e,
        dRes_ID_KO_BCK_KO_SLEEP04_e,
        dRes_ID_KO_BCK_KO_OISI_e,
        dRes_ID_KO_BCK_KO_BANG_e,
        dRes_ID_KO_BCK_KO_OHOHO_e,
    };
    return a_bck_resID_tbl[idx];
}

/* 000011D8-00001200       .text headAnmNum_toResID__11daNpc_Ko1_cFi */
int daNpc_Ko1_c::headAnmNum_toResID(int idx) {
    if (mType == 1) {
        return dRes_ID_KO_BCK_KOHEAD02_WAIT01_e;
    }
    static const int a_bck_resID_tbl[] = {
        dRes_ID_KO_BCK_KOHEAD01_WAIT01_e,
        dRes_ID_KO_BCK_KOHEAD01_RUN01_e,
        dRes_ID_KO_BCK_KOHEAD01_WALK01_e,
        dRes_ID_KO_BCK_KOHEAD01_TALK01_e,
        dRes_ID_KO_BCK_KOHEAD01_SUSURI_e,
        dRes_ID_KO_BCK_KOHEAD01_BUTUKARI_e,
        dRes_ID_KO_BCK_KOHEAD01_WAIT01_e,
        dRes_ID_KO_BCK_KOHEAD01_WAIT01_e,
        dRes_ID_KO_BCK_KOHEAD01_WAIT01_e,
        dRes_ID_KO_BCK_KOHEAD01_SLEEP03_e,
        dRes_ID_KO_BCK_KOHEAD01_SLEEP04_e,
        dRes_ID_KO_BCK_KOHEAD01_OISI_e,
        dRes_ID_KO_BCK_KOHEAD01_BANG_e,
        dRes_ID_KO_BCK_KOHEAD01_WAIT01_e,
    };
    return a_bck_resID_tbl[idx];
}

/* 00001200-00001214       .text balloon_anmNum_toResID__11daNpc_Ko1_cFi */
int daNpc_Ko1_c::balloon_anmNum_toResID(int idx) {
    static const int a_bck_resID_tbl[] = {
        dRes_ID_KO_BCK_BALLOON_SLEEP04_e,
        dRes_ID_KO_BCK_BALLOON_BANG_e,
        dRes_ID_KO_BCK_BALLOON_BANG_e,
        dRes_ID_KO_BCK_BALLOON_BANG_e,
        dRes_ID_KO_BCK_BALLOON_BANG_e,
        dRes_ID_KO_BCK_BALLOON_BANG_e,
        dRes_ID_KO_BCK_BALLOON_BANG_e,
        dRes_ID_KO_BCK_BALLOON_BANG_e,
        dRes_ID_KO_BCK_BALLOON_BANG_e,
        dRes_ID_KO_BCK_BALLOON_BANG_e,
        dRes_ID_KO_BCK_BALLOON_BANG_e,
        dRes_ID_KO_BCK_BALLOON_BANG_e,
        dRes_ID_KO_BCK_BALLOON_BANG_e,
        dRes_ID_KO_BCK_BALLOON_BANG_e,
    };
    return a_bck_resID_tbl[idx];
}

/* 00001214-0000129C       .text btpNum_toResID__11daNpc_Ko1_cFi */
int daNpc_Ko1_c::btpNum_toResID(int idx) {
    static const int a_btp_resID_tbl[] = {
        dRes_ID_KO_BTP_KOHEAD01_e,
        dRes_ID_KO_BTP_KOHEAD01_SUSURI_e,
        dRes_ID_KO_BTP_KOHEAD01_SLEEP_e,
        dRes_ID_KO_BTP_KOHEAD01_SLEEP04_e,
    };
    switch (a_btp_resID_tbl[idx]) {
    case dRes_ID_KO_BTP_KOHEAD01_e:
        switch (mType) {
        case 0:
            return dRes_ID_KO_BTP_KOHEAD01_e;
        case 1:
            return dRes_ID_KO_BTP_KOHEAD02_e;
        }
        break;
    case dRes_ID_KO_BTP_KOHEAD01_SLEEP_e:
        switch (mType) {
        case 0:
            return dRes_ID_KO_BTP_KOHEAD01_SLEEP_e;
        case 1:
            return dRes_ID_KO_BTP_KOHEAD02_SLEEP_e;
        }
        break;
    }
    return a_btp_resID_tbl[idx];
}

/* 0000129C-000013AC       .text setBtp__11daNpc_Ko1_cFbi */
bool daNpc_Ko1_c::setBtp(bool modify, int index) {
    J3DModelData* data = mpHeadMorf->getModel()->getModelData();
    m_hed_tex_pttrn = (J3DAnmTexPattern*)dComIfG_getObjectIDRes("Ko", btpNum_toResID(index));
    JUT_ASSERT(0x3AA, m_hed_tex_pttrn != 0);
    if (mBtp.init(data, m_hed_tex_pttrn, TRUE, 2, 1.0f, 0, -1, modify, FALSE) != 0) {
        mBtpFrame = 0;
        mBlinkTimer = 0;
        return true;
    }
    return false;
}

/* 000013AC-000013D4       .text iniTexPttrnAnm__11daNpc_Ko1_cFb */
bool daNpc_Ko1_c::iniTexPttrnAnm(bool modify) {
    return setBtp(modify, mTexIndex);
}

/* 000013D4-00001480       .text plyTexPttrnAnm__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::plyTexPttrnAnm() {
    if (mTexIndex != 0 || cLib_calcTimer(&mBlinkTimer) == 0) {
        mBtpFrame++;
        int max = m_hed_tex_pttrn->getFrameMax();
        if (mBtpFrame >= (s16)max) {
            if (mTexIndex != 0) {
                mBtpFrame = max;
            } else {
                mBlinkTimer = 30 + cM_rndF(60.0f);
                mBtpFrame = 0;
            }
        }
    }
}

/* 00001480-000014BC       .text setAnm_tex__11daNpc_Ko1_cFSc */
void daNpc_Ko1_c::setAnm_tex(s8 idx) {
    if (mTexIndex != idx) {
        mTexIndex = idx;
        iniTexPttrnAnm(true);
    }
}

/* 000014BC-000015B4       .text setAnm_anm__11daNpc_Ko1_cFPQ211daNpc_Ko1_c9anm_prm_c */
BOOL daNpc_Ko1_c::setAnm_anm(daNpc_Ko1_c::anm_prm_c* prm) {
    if (mAnmIndex == prm->mAnm) {
        return TRUE;
    }
    mAnmIndex = prm->mAnm;
    int res = anmNum_toResID(mAnmIndex);
    dNpc_setAnmIDRes(mpMorf, prm->mLoop, prm->mMorf, prm->mSpeed, res, -1, "Ko");
    int headRes = headAnmNum_toResID(mAnmIndex);
    dNpc_setAnmIDRes(mpHeadMorf, prm->mLoop, prm->mMorf, prm->mSpeed, headRes, -1, "Ko");
    mLastFrame = mAnmRepeat = mAnmEnded = 0;
    return TRUE;
}

/* 000015B4-00001674       .text set_balloonAnm_anm__11daNpc_Ko1_cFPQ211daNpc_Ko1_c9anm_prm_c */
BOOL daNpc_Ko1_c::set_balloonAnm_anm(daNpc_Ko1_c::anm_prm_c* prm) {
    if (mBalloonAnmIndex == prm->mAnm) {
        return TRUE;
    }
    mBalloonAnmIndex = prm->mAnm;
    int res = balloon_anmNum_toResID(mBalloonAnmIndex);
    dNpc_setAnmIDRes(mpBalloonMorf, prm->mLoop, prm->mMorf, prm->mSpeed, res, -1, "Ko");
    mBalloonLastFrame = mBalloonAnmRepeat = mBalloonAnmEnded = 0;
    return TRUE;
}

/* 00001674-000016A4       .text set_balloonAnm_NUM__11daNpc_Ko1_cFi */
void daNpc_Ko1_c::set_balloonAnm_NUM(int idx) {
    static anm_prm_c a_anm_prm_tbl[] = {
        {0, -1, 0.0f, 0.0f, 2, 0},
        {1, -1, 0.0f, 0.0f, 0, 0},
    };
    set_balloonAnm_anm(&a_anm_prm_tbl[idx]);
}

/* 000016A4-00001710       .text setAnm_NUM__11daNpc_Ko1_cFii */
void daNpc_Ko1_c::setAnm_NUM(int index, int tex) {
    static anm_prm_c a_anm_prm_tbl[] = {
        {0, 0, 8.0f, 1.0f, 2, 0},
        {1, 0, 8.0f, 1.0f, 2, 0},
        {2, 0, 8.0f, 1.0f, 2, 0},
        {3, 0, 8.0f, 1.0f, 2, 0},
        {4, 1, 8.0f, 1.0f, 2, 0},
        {5, 0, 8.0f, 1.0f, 0, 0},
        {6, 0, 8.0f, 1.0f, 2, 0},
        {7, 2, 8.0f, 1.0f, 2, 0},
        {8, 2, 8.0f, 1.0f, 2, 0},
        {9, 2, 8.0f, 1.0f, 2, 0},
        {0xA, 3, 8.0f, 1.0f, 2, 0},
        {0xB, 0, 8.0f, 1.0f, 2, 0},
        {0xC, 2, 8.0f, 1.0f, 0, 0},
        {0xD, 0, 8.0f, 1.0f, 2, 0},
    };
    if (tex) {
        setAnm_tex(a_anm_prm_tbl[index].mTex);
    }
    setAnm_anm(&a_anm_prm_tbl[index]);
}

/* 00001710-00001790       .text setAnm__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::setAnm() {
    static anm_prm_c a_anm_prm_tbl[] = {
        {-1, -1, 0.0f, 0.0f, -1, 0},
        {0, 0, 8.0f, 1.0f, 2, 0},
        {6, 0, 8.0f, 1.0f, 2, 0},
        {-1, -1, 0.0f, 0.0f, -1, 0},
        {2, 0, 8.0f, 1.0f, 2, 0},
        {4, 1, 8.0f, 1.0f, 2, 0},
        {1, 0, 8.0f, 1.0f, 2, 0},
        {2, 0, 8.0f, 1.0f, 2, 0},
        {2, 0, 8.0f, 1.0f, 2, 0},
        {5, 0, 8.0f, 1.0f, 0, 0},
        {0, 0, 8.0f, 1.0f, 2, 0},
        {2, 0, 8.0f, 1.0f, 2, 0},
        {0, 0, 8.0f, 1.0f, 2, 0},
        {1, 0, 8.0f, 1.0f, 2, 0},
        {0, 0, 8.0f, 1.0f, 2, 0},
        {2, 0, 8.0f, 1.0f, 2, 0},
        {5, 0, 8.0f, 1.0f, 0, 0},
        {1, 0, 8.0f, 1.0f, 2, 0},
        {5, 0, 8.0f, 1.0f, 0, 0},
        {0, 0, 8.0f, 1.0f, 2, 0},
        {-1, -1, 0.0f, 0.0f, -1, 0},
        {-1, -1, 0.0f, 0.0f, -1, 0},
        {0, 0, 8.0f, 1.0f, 2, 0},
        {1, 0, 8.0f, 1.0f, 2, 0},
        {2, 0, 8.0f, 1.0f, 2, 0},
        {5, 0, 8.0f, 1.0f, 0, 0},
        {0, 0, 8.0f, 1.0f, 2, 0},
        {7, 2, 8.0f, 1.0f, 2, 0},
        {9, 2, 8.0f, 1.0f, 2, 0},
        {0, 0, 8.0f, 1.0f, 2, 0},
    };
    if (a_anm_prm_tbl[mState].mTex >= 0) {
        setAnm_tex(a_anm_prm_tbl[mState].mTex);
    }
    if (a_anm_prm_tbl[mState].mAnm >= 0) {
        setAnm_anm(&a_anm_prm_tbl[mState]);
    }
    return TRUE;
}

/* 00001790-000017A4       .text setPlaySpd__11daNpc_Ko1_cFf */
void daNpc_Ko1_c::setPlaySpd(float speed) {
    mpHeadMorf->setPlaySpeed(speed);
    mpMorf->setPlaySpeed(speed);
}

/* 000017A4-000017DC       .text chg_anmTag__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::chg_anmTag() {
    switch (mAnmTag) {
    case 0x0A:
        setAnm_NUM(4, 1);
        break;
    }
}

/* 000017DC-00001824       .text control_anmTag__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::control_anmTag() {
    switch (mAnmTag) {
    case 0x0A:
        if (mAnmEnded) {
            mAnmTag = 0xFF;
            setAnm_ATR(1);
        }
        break;
    }
}

/* 00001824-000018D8       .text chg_anmAtr__11daNpc_Ko1_cFUc */
void daNpc_Ko1_c::chg_anmAtr(u8 attr) {
    if (mPairedMsgNo != 0xFFFFFFFF) {
        switch (mPairedMsgNo) {
        case 0xB03:
        case 0xB0A:
        case 0xB64:
        case 0xB65:
            mLookMode = 1;
            mNoTurn = false;
            m_jnt.setTrn();
            break;
        }
    }
    if (attr >= 0xD || attr == mAnmAttr) {
        return;
    }
    mAnmAttr = attr;
    if (mAnmAttr == 0xB) {
        mLookMode = 1;
        mNoTurn = false;
        m_jnt.setTrn();
    }
    setAnm_ATR(1);
}

/* 000018D8-0000192C       .text control_anmAtr__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::control_anmAtr() {
    switch (mAnmAttr) {
    default:
        break;
    case 0xB:
        if (mAnmEnded) {
            mAnmAttr = 0;
            setAnm_NUM(0, 1);
        }
        break;
    }
}

/* 0000192C-00001994       .text setAnm_ATR__11daNpc_Ko1_cFi */
void daNpc_Ko1_c::setAnm_ATR(int tex) {
    static anm_prm_c a_anm_prm_tbl[] = {
        {0, 0, 8.0f, 1.0f, 2, 0},
        {1, 0, 8.0f, 1.0f, 2, 0},
        {2, 0, 8.0f, 1.0f, 2, 0},
        {3, 0, 8.0f, 1.0f, 2, 0},
        {4, 1, 8.0f, 1.0f, 2, 0},
        {5, 0, 8.0f, 1.0f, 0, 0},
        {6, 0, 8.0f, 1.0f, 2, 0},
        {7, 0, 8.0f, 1.0f, 2, 0},
        {8, 0, 8.0f, 1.0f, 2, 0},
        {9, 0, 8.0f, 1.0f, 2, 0},
        {0xA, 3, 8.0f, 1.0f, 2, 0},
        {0xB, 0, 8.0f, 1.0f, 2, 0},
        {0xD, 0, 8.0f, 1.0f, 2, 0},
    };
    if (tex) {
        setAnm_tex(a_anm_prm_tbl[mAnmAttr].mTex);
    }
    setAnm_anm(&a_anm_prm_tbl[mAnmAttr]);
}

/* 00001994-00001A9C       .text anmAtr__11daNpc_Ko1_cFUs */
void daNpc_Ko1_c::anmAtr(u16 status) {
    dComIfG_MesgCamInfo_c* info = dComIfGp_getMesgCameraInfo();
    if (field_0x6bc[0] == 2 && this != info->mActor[info->mBasicID - 1]) {
        control_anmTag();
        control_anmAtr();
        return;
    }
    switch (status) {
    case 6:
        if (mMessageState == 0) {
            mAnmAttr = 0xFF;
            chg_anmAtr(dComIfGp_getMesgAnimeAttrInfo());
            mMessageState++;
        }
        {
            u8 tag = dComIfGp_getMesgAnimeTagInfo();
            dComIfGp_clearMesgAnimeTagInfo();
            if (tag != 0xFF && mAnmTag != tag) {
                mAnmTag = tag;
                chg_anmTag();
            }
        }
        break;
    case 14:
        mMessageState = 0;
        break;
    }
    control_anmTag();
    control_anmAtr();
}

/* 00001A9C-00001AEC       .text eventOrder__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::eventOrder() {
    if (mEventOrder == 1 || mEventOrder == 2) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (mEventOrder == 1) {
            fopAcM_orderSpeakEvent(this);
        }
    }
}

/* 00001AEC-00001B2C       .text checkOrder__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::checkOrder() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        return;
    }
    if (eventInfo.checkCommandTalk() && (mEventOrder == 1 || mEventOrder == 2)) {
        mEventOrder = 0;
        mTalking = true;
    }
}

/* 00001B2C-00001BCC       .text chk_talk__11daNpc_Ko1_cFv */
bool daNpc_Ko1_c::chk_talk() {
    bool result = false;
    if (dComIfGp_event_chkTalkXY()) {
        if (dComIfGp_evmng_ChkPresentEnd()) {
            mPresentItem = dComIfGp_event_getPreItemNo();
            result = true;
        }
    } else {
        mPresentItem = 0xFF;
        result = true;
    }
    return result;
}

/* 00001BCC-00001E0C       .text chk_manzai_1__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::chk_manzai_1() {
    /* Nonmatching */
}

/* 00001E0C-00001E4C       .text chk_partsNotMove__11daNpc_Ko1_cFv */
bool daNpc_Ko1_c::chk_partsNotMove() {
    bool result = false;
    if (mOldHeadY == m_jnt.getHead_y() && mOldBackY == m_jnt.getBackbone_y() && mOldActorY == current.angle.y) {
        result = true;
    }
    return result;
}

/* 00001E4C-00001FFC       .text lookBack__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::lookBack() {
    mOldHeadY = m_jnt.getHead_y();
    mOldBackY = m_jnt.getBackbone_y();
    mOldActorY = current.angle.y;
    cXyz target;
    cXyz base = current.pos;
    base.y = eyePos.y;
    target.set(0.0f, 0.0f, 0.0f);
    cXyz* targetP = NULL;
    s16 angle = current.angle.y;
    bool noTurn = mNoTurn;
    switch (mLookMode) {
    case 1:
        target = dNpc_playerEyePos(-20.0f);
        targetP = &target;
        base = current.pos;
        base.y = eyePos.y;
        break;
    case 2:
        target = mLookPos;
        targetP = &target;
        base = current.pos;
        base.y = eyePos.y;
        break;
    case 3:
        angle = mLookAngle;
        break;
    }
    cLib_addCalcAngleS2(&mTurnSpeed, l_HIO.mChild[mType].mPrm.mTurnSpeed, 4, 0x800);
    if (!m_jnt.trnChk()) {
        mTurnSpeed = 0;
    }
    m_jnt.lookAtTarget(&current.angle.y, targetP, base, angle, mTurnSpeed, noTurn);
}

/* 00001FFC-00002204       .text next_msgStatus__11daNpc_Ko1_cFPUl */
u16 daNpc_Ko1_c::next_msgStatus(unsigned long*) {
    /* Nonmatching */
    return 0;
}

/* 00002204-00002240       .text getMsg_HNA_0__11daNpc_Ko1_cFv */
u32 daNpc_Ko1_c::getMsg_HNA_0() {
    return 0xB55 + (dComIfGs_isEventBit(0x220) ? 1 : 0);
}

/* 00002240-0000227C       .text getMsg_HNA_1__11daNpc_Ko1_cFv */
u32 daNpc_Ko1_c::getMsg_HNA_1() {
    return 0xAF4 + (dComIfGs_isEventBit(0x240) ? 1 : 0);
}

/* 0000227C-000022B8       .text getMsg_HNA_2__11daNpc_Ko1_cFv */
u32 daNpc_Ko1_c::getMsg_HNA_2() {
    return 0xB62 + (dComIfGs_isEventBit(0x3101) ? 1 : 0);
}

/* 000022B8-00002318       .text getMsg_HNA_3__11daNpc_Ko1_cFv */
u32 daNpc_Ko1_c::getMsg_HNA_3() {
    if (dComIfGs_isEventBit(0x2C04)) {
        return m86D ? 0xB5E : 0xB5A;
    }
    return 0xB59;
}

/* 00002318-0000237C       .text getMsg_BOU_0__11daNpc_Ko1_cFv */
u32 daNpc_Ko1_c::getMsg_BOU_0() {
    if (dComIfGs_isEventBit(0x104)) {
        return 0xAF3;
    }
    return 0xAF1 + (dComIfGs_isEventBit(0x210) ? 1 : 0);
}

/* 0000237C-000023B8       .text getMsg_BOU_1__11daNpc_Ko1_cFv */
u32 daNpc_Ko1_c::getMsg_BOU_1() {
    return 0xAF4 + (dComIfGs_isEventBit(0x208) ? 1 : 0);
}

/* 000023B8-000023E8       .text bitCount__11daNpc_Ko1_cFUc */
s8 daNpc_Ko1_c::bitCount(u8 bits) {
    s8 count = 0;
    for (int i = 0; i < 8; i++) {
        int shifted = bits;
        if (bits & 1) {
            count++;
        }
        bits = shifted >> 1;
    }
    return count;
}

/* 000023E8-0000248C       .text getMsg_BOU_2__11daNpc_Ko1_cFv */
u32 daNpc_Ko1_c::getMsg_BOU_2() {
    if (dComIfGs_isEventBit(0x3801)) {
        if (dComIfGs_isEventBit(0x3340)) {
            return 0xB08;
        }
        if (bitCount(dComIfGs_getEventReg(0xBFFF)) < 2) {
            return 0xB00;
        }
        return 0xB07;
    }
    return 0xAF6;
}

/* 0000248C-00002528       .text getMsg__11daNpc_Ko1_cFv */
u32 daNpc_Ko1_c::getMsg() {
    u32 msg = 0;
    switch (mStaff) {
    case 0:
        msg = getMsg_HNA_0();
        break;
    case 1:
        msg = getMsg_HNA_1();
        break;
    case 2:
        msg = getMsg_HNA_2();
        break;
    case 3:
        msg = getMsg_HNA_3();
        break;
    case 4:
        break;
    case 5:
        msg = getMsg_BOU_0();
        break;
    case 6:
        msg = getMsg_BOU_1();
        break;
    case 7:
        msg = getMsg_BOU_2();
        break;
    case 8:
        break;
    }
    return msg;
}

/* 00002528-000025A8       .text chkAttention__11daNpc_Ko1_cFv */
bool daNpc_Ko1_c::chkAttention() {
    dAttention_c& attention = dComIfGp_getAttention();
    if (attention.LockonTruth()) {
        return this == attention.LockonTarget(0);
    }
    return this == attention.ActionTarget(0);
}

/* 000025A8-00002610       .text setAttention__11daNpc_Ko1_cFb */
void daNpc_Ko1_c::setAttention(bool force) {
    attention_info.position.set(
        current.pos.x,
        current.pos.y + l_HIO.mChild[mType].mPrm.mAttentionOffsetY,
        current.pos.z
    );
    if (mUpdateEye != 0 || force) {
        eyePos.set(mEyePos.x, mEyePos.y, mEyePos.z);
    }
}

/* 00002610-00002644       .text searchByID__11daNpc_Ko1_cFUi */
fopAc_ac_c* daNpc_Ko1_c::searchByID(fpc_ProcID id) {
    fopAc_ac_c* actor = NULL;
    fopAcM_SearchByID(id, &actor);
    return actor;
}

/* 00002644-000026DC       .text partner_srch_sub__11daNpc_Ko1_cFPFPvPv_Pv */
fpc_ProcID daNpc_Ko1_c::partner_srch_sub(void* (*judge)(void*, void*)) {
    fpc_ProcID id = fpcM_ERROR_PROCESS_ID_e;
    l_check_wrk = 0;
    for (int i = 0; i < ARRAY_SSIZE(l_check_inf); i++) {
        l_check_inf[i] = NULL;
    }
    fpcM_Search(judge, this);
    if (l_check_wrk != 0) {
        id = fpcM_GetID(l_check_inf[0]);
    }
    return id;
}

/* 000026DC-000027CC       .text partner_srch__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::partner_srch() {
    if (mActionState == 1) {
        switch (mStaff) {
        case 0:
            break;
        case 1:
            mPartnerId = partner_srch_sub(searchActor_Ko_Bou);
            mPartnerNum = 1;
            break;
        case 2:
            break;
        case 3:
            mPartnerId = partner_srch_sub(searchActor_Ko_Bou);
            mObId = partner_srch_sub(searchActor_Ob);
            mPartnerNum = 2;
            break;
        case 4:
        case 5:
            break;
        case 6:
            mPartnerId = partner_srch_sub(searchActor_Ko_Hna);
            mPartnerNum = 1;
            break;
        case 7:
            mPartnerId = partner_srch_sub(searchActor_Ko_Hna);
            mObId = partner_srch_sub(searchActor_Ob);
            mPartnerNum = 2;
            break;
        case 8:
            break;
        }
    }
}

/* 000027CC-00002900       .text check_landOn__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::check_landOn() {
    /* Nonmatching */
}

/* 00002900-0000299C       .text ko_setPthPos__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::ko_setPthPos() {
    if (mPath.getPath() != NULL) {
        current.pos = mPath.getPoint(mPath.getIdx());
        mPath.nextIdxAuto();
        cXyz pos = mPath.getPoint(mPath.getIdx());
        current.angle.y = cLib_targetAngleY(&current.pos, &pos);
    }
}

/* 0000299C-00002AA4       .text set_tgtPos__11daNpc_Ko1_cF4cXyz */
void daNpc_Ko1_c::set_tgtPos(cXyz) {
    /* Nonmatching */
}

/* 00002AA4-00002C14       .text ko_movPass__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::ko_movPass() {
    /* Nonmatching */
}

/* 00002C14-00002D50       .text ko_clcMovSpd__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::ko_clcMovSpd() {
    /* Nonmatching */
}

/* 00002D50-00002E3C       .text ko_clcSwmSpd__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::ko_clcSwmSpd() {
    /* Nonmatching */
}

/* 00002E3C-00003028       .text ko_nMove__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::ko_nMove() {
    /* Nonmatching */
}

/* 00003028-000030E8       .text chk_routeAngle__11daNpc_Ko1_cFR4cXyzPs */
void daNpc_Ko1_c::chk_routeAngle(cXyz&, short*) {
    /* Nonmatching */
}

/* 000030E8-0000334C       .text routeWallCheck__11daNpc_Ko1_cFR4cXyzR4cXyzPs */
void daNpc_Ko1_c::routeWallCheck(cXyz&, cXyz&, short*) {
    /* Nonmatching */
}

/* 00003784-00003A04       .text chk_ForwardGroundY__11daNpc_Ko1_cFs */
void daNpc_Ko1_c::chk_ForwardGroundY(short) {
    /* Nonmatching */
}

/* 00003B9C-00003C54       .text chk_wallJump__11daNpc_Ko1_cFs */
void daNpc_Ko1_c::chk_wallJump(short) {
    /* Nonmatching */
}

/* 00003C54-00003D34       .text routeCheck__11daNpc_Ko1_cFfPs */
void daNpc_Ko1_c::routeCheck(float, short*) {
    /* Nonmatching */
}

/* 00003D34-00003DE8       .text chk_start_swim__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::chk_start_swim() {
    BOOL result = FALSE;
    if (mObjAcch.ChkWaterIn()) {
        result = (mObjAcch.m_wtr.GetHeight() - mObjAcch.GetGroundH()) > 3.2f;
        if (result) {
            if (mState != 7) {
                setPrtcl_Hamon(1.0f, 0.0f);
            }
        } else {
            if (mState == 7) {
                setPrtcl_Hamon(0.5f, 1.0f);
            }
        }
    } else {
        mRippleCallback.end();
        mpHamonEmitter = NULL;
    }
    return result;
}

/* 00003DE8-00003E64       .text get_crsActorID__11daNpc_Ko1_cFv */
fpc_ProcID daNpc_Ko1_c::get_crsActorID() {
    if (mCyl.ChkCoHit()) {
        cCcD_Obj* obj = mCyl.GetCoHitObj();
        if (obj != NULL) {
            fopAc_ac_c* actor = obj->GetAc();
            return fopAcM_GetID(actor);
        }
    }
    return fpcM_ERROR_PROCESS_ID_e;
}

/* 00003E64-00003F50       .text chk_areaIn__11daNpc_Ko1_cFf4cXyz */
void daNpc_Ko1_c::chk_areaIn(float, cXyz) {
    /* Nonmatching */
}

/* 00003F50-00003FF0       .text setPrtcl_Hamon__11daNpc_Ko1_cFff */
void daNpc_Ko1_c::setPrtcl_Hamon(float scale, float rate) {
    cXyz scl(scale, scale, scale);
    mRippleCallback.end();
    mpHamonEmitter = dComIfGp_particle_setShipTail(dPa_name::ID_AK_JN_HAMON00, &current.pos, NULL, &scl, 0xFF, &mRippleCallback);
    if (mpHamonEmitter != NULL) {
        mRippleCallback.setRate(rate);
    }
}

/* 00003FF0-000040F8       .text setPrtcl_HanaPachi__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::setPrtcl_HanaPachi() {
    /* Nonmatching */
}

/* 000040F8-0000420C       .text charDecide__11daNpc_Ko1_cFi */
bool daNpc_Ko1_c::charDecide(int param) {
    mType = -1;
    mStaff = -1;
    switch (fopAcM_GetName(this)) {
    case fpcNm_NPC_KO1_e:
        mType = 0;
        switch (param) {
        case 0:
            mStaff = 0;
            break;
        case 1:
            mStaff = 1;
            break;
        case 2:
            mStaff = 2;
            break;
        case 3:
            mStaff = 3;
            break;
        case 4:
            mStaff = 4;
            break;
        default:
            return false;
        }
        break;
    case fpcNm_NPC_KO2_e:
        mType = 1;
        switch (param) {
        case 0:
            mStaff = 5;
            break;
        case 1:
            mStaff = 6;
            break;
        case 2:
            mStaff = 7;
            break;
        case 3:
            mStaff = 8;
            break;
        default:
            return false;
        }
        break;
    default:
        return false;
    }
    return true;
}

/* 0000420C-00004264       .text event_actionInit__11daNpc_Ko1_cFi */
void daNpc_Ko1_c::event_actionInit(int staff) {
    int* action = dComIfGp_evmng_getMyIntegerP(staff, "ActNo");
    if (action != NULL) {
        mEventAction = *action;
    }
}

/* 00004264-00004274       .text event_action__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::event_action() {
    switch (mEventAction) {
    case 0:
        break;
    }
    return TRUE;
}

/* 00004274-00004360       .text privateCut__11daNpc_Ko1_cFi */
void daNpc_Ko1_c::privateCut(int staff) {
    static char* a_cut_tbl[] = {"ACTION"};
    if (staff != -1) {
        mCutIndex = dComIfGp_evmng_getMyActIdx(staff, a_cut_tbl, 1, TRUE, 0);
        if (mCutIndex == -1) {
            dComIfGp_evmng_cutEnd(staff);
        } else {
            if (dComIfGp_evmng_getIsAddvance(staff)) {
                switch (mCutIndex) {
                case 0:
                    event_actionInit(staff);
                    break;
                }
            }
            BOOL done;
            switch (mCutIndex) {
            case 0:
                done = event_action();
                break;
            default:
                done = TRUE;
                break;
            }
            if (done) {
                dComIfGp_evmng_cutEnd(staff);
            }
        }
    }
}

/* 00004360-00004384       .text endEvent__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::endEvent() {
    dComIfGp_event_reset();
    mAnmAttr = 0xFF;
    mAnmTag = 0xFF;
}

/* 00004384-000043BC       .text isEventEntry__11daNpc_Ko1_cFv */
int daNpc_Ko1_c::isEventEntry() {
    return dComIfGp_evmng_getMyStaffId(mCut.getActorName());
}

/* 000043BC-00004414       .text event_proc__11daNpc_Ko1_cFi */
void daNpc_Ko1_c::event_proc(int staff) {
    if (!mCut.cutProc()) {
        privateCut(staff);
    }
    lookBack();
}

/* 00004414-000044C0       .text set_action__11daNpc_Ko1_cFM11daNpc_Ko1_cFPCvPvPv_iPv */
bool daNpc_Ko1_c::set_action(int (daNpc_Ko1_c::*action)(void*), void* arg) {
    if (mAction != action) {
        if (mAction) {
            mActionState = 9;
            (this->*mAction)(arg);
        }
        mAction = action;
        mActionState = 0;
        (this->*mAction)(arg);
    }
    return true;
}

/* 000044C0-000044E8       .text clrSpd__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::clrSpd() {
    mTargetSpeed = 0.0f;
    mSpeedStep = 0.0f;
    speed.y = 0.0f;
    speedF = 0.0f;
    gravity = -4.5f;
}

/* 000044E8-00004B18       .text setStt__11daNpc_Ko1_cFSc */
void daNpc_Ko1_c::setStt(signed char) {
    /* Nonmatching */
}

/* 00004B18-00004C70       .text wait_1__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::wait_1() {
    /* Nonmatching */
    return TRUE;
}

/* 00004C70-00004CE8       .text wait_2__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::wait_2() {
    if (mAnmEnded) {
        cLib_calcTimer(&mWaitTimer);
        if (mWaitTimer == 0 || m874 != 0 || mTalking != 0) {
            setStt(1);
            return TRUE;
        }
    }
    return TRUE;
}

/* 00004CE8-00004D1C       .text wait_3__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::wait_3() {
    if (mAnmEnded) {
        setStt(4);
    }
    return TRUE;
}

/* 00004D1C-00004E64       .text wait_4__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::wait_4() {
    /* Nonmatching */
    return TRUE;
}

/* 00004E64-00004F30       .text wait_5__11daNpc_Ko1_cFSc */
BOOL daNpc_Ko1_c::wait_5(signed char) {
    /* Nonmatching */
    return TRUE;
}

/* 00004F30-00005088       .text wait_6__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::wait_6() {
    /* Nonmatching */
    return TRUE;
}

/* 00005088-000052D4       .text wait_7__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::wait_7() {
    /* Nonmatching */
    return TRUE;
}

/* 000052D4-000053F8       .text wait_9__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::wait_9() {
    /* Nonmatching */
    return TRUE;
}

/* 000053F8-00005524       .text wait_a__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::wait_a() {
    /* Nonmatching */
    return TRUE;
}

/* 00005524-000055F8       .text walk_1__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::walk_1() {
    /* Nonmatching */
    return TRUE;
}

/* 000055F8-000056D4       .text walk_2__11daNpc_Ko1_cFScSc */
BOOL daNpc_Ko1_c::walk_2(signed char, signed char) {
    /* Nonmatching */
    return TRUE;
}

/* 000056D4-000057B8       .text walk_3__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::walk_3() {
    /* Nonmatching */
    return TRUE;
}

/* 000057B8-000058EC       .text swim_1__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::swim_1() {
    /* Nonmatching */
    return TRUE;
}

/* 000058EC-00005A1C       .text swim_2__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::swim_2() {
    /* Nonmatching */
    return TRUE;
}

/* 00005A1C-00005B64       .text attk_1__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::attk_1() {
    /* Nonmatching */
    return TRUE;
}

/* 00005B64-00005C94       .text attk_2__11daNpc_Ko1_cFScSc */
BOOL daNpc_Ko1_c::attk_2(signed char, signed char) {
    /* Nonmatching */
    return TRUE;
}

/* 00005C94-00005DEC       .text attk_3__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::attk_3() {
    /* Nonmatching */
    return TRUE;
}

/* 00005DEC-00005E1C       .text down_1__11daNpc_Ko1_cFSc */
BOOL daNpc_Ko1_c::down_1(s8 stt) {
    if (m86B != 0) {
        setStt(stt);
    }
    return TRUE;
}

/* 00005E1C-0000609C       .text talk_1__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::talk_1() {
    /* Nonmatching */
    return TRUE;
}

/* 0000609C-00006184       .text talk_2__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::talk_2() {
    /* Nonmatching */
    return TRUE;
}

/* 00006184-000062B4       .text manzai__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::manzai() {
    /* Nonmatching */
    return TRUE;
}

/* 000062B4-00006358       .text neru_1__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::neru_1() {
    /* Nonmatching */
    return TRUE;
}

/* 00006358-00006524       .text neru_2__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::neru_2() {
    /* Nonmatching */
    return TRUE;
}

/* 00006524-00006668       .text hana_action1__11daNpc_Ko1_cFPv */
int daNpc_Ko1_c::hana_action1(void*) {
    switch (mActionState) {
    case 0:
        ko_setPthPos();
        setStt(4);
        mActionState++;
        break;
    case 1:
    case 2:
    case 3:
        m874 = chkAttention();
        switch (mState) {
        case 4:
            mUpdateEye = walk_1();
            break;
        case 11:
            mUpdateEye = walk_3();
            break;
        case 5:
            mUpdateEye = wait_3();
            break;
        case 10:
            mUpdateEye = wait_4();
            break;
        case 6:
            mUpdateEye = attk_1();
            break;
        case 7:
            mUpdateEye = swim_1();
            break;
        case 8:
            mUpdateEye = swim_2();
            break;
        case 9:
            mUpdateEye = down_1(0xA);
            break;
        case 3:
            mUpdateEye = talk_1();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return 1;
}

/* 00006668-00006774       .text hana_action2__11daNpc_Ko1_cFPv */
int daNpc_Ko1_c::hana_action2(void*) {
    switch (mActionState) {
    case 0:
        setStt(0x13);
        mActionState++;
        break;
    case 1:
    case 2:
    case 3:
        m874 = chkAttention();
        switch (mState) {
        case 0x13:
            mUpdateEye = wait_7();
            break;
        case 0x11:
            mUpdateEye = attk_3();
            break;
        case 0x12:
            mUpdateEye = down_1(0x13);
            break;
        case 3:
            mUpdateEye = talk_1();
            break;
        case 0x15:
            mUpdateEye = talk_2();
            break;
        case 0x14:
            mUpdateEye = manzai();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return 1;
}

/* 00006774-00006894       .text hana_action3__11daNpc_Ko1_cFPv */
int daNpc_Ko1_c::hana_action3(void*) {
    switch (mActionState) {
    case 0:
        setStt(0x16);
        mActionState++;
        break;
    case 1:
    case 2:
    case 3:
        m874 = chkAttention();
        switch (mState) {
        case 0x16:
            mUpdateEye = wait_5(0x17);
            break;
        case 0x17:
            mUpdateEye = attk_2(0x19, 0x18);
            break;
        case 0x18:
            mUpdateEye = walk_2(0x16, 0x17);
            break;
        case 0x1A:
            mUpdateEye = wait_9();
            break;
        case 0x19:
            mUpdateEye = down_1(0x1A);
            break;
        case 3:
            mUpdateEye = talk_1();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return 1;
}

/* 00006894-00006988       .text hana_action4__11daNpc_Ko1_cFPv */
int daNpc_Ko1_c::hana_action4(void*) {
    switch (mActionState) {
    case 0:
        setStt(0x1D);
        mActionState++;
        break;
    case 1:
    case 2:
    case 3:
        m874 = chkAttention();
        switch (mState) {
        case 0x1D:
            mUpdateEye = wait_a();
            break;
        case 3:
            mUpdateEye = talk_1();
            break;
        case 0x15:
            mUpdateEye = talk_2();
            break;
        case 0x14:
            mUpdateEye = manzai();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return 1;
}

/* 00006988-00006A24       .text hana_action5__11daNpc_Ko1_cFPv */
int daNpc_Ko1_c::hana_action5(void*) {
    switch (mActionState) {
    case 0:
        setStt(0x1C);
        mActionState++;
        break;
    case 1:
    case 2:
    case 3:
        m874 = chkAttention();
        switch (mState) {
        case 0x1C:
            mUpdateEye = neru_2();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return 1;
}

/* 00006A24-00006B24       .text wait_action1__11daNpc_Ko1_cFPv */
int daNpc_Ko1_c::wait_action1(void*) {
    switch (mActionState) {
    case 0:
        setStt(1);
        mActionState++;
        break;
    case 1:
    case 2:
    case 3:
        if (!dComIfGs_isEventBit(0x104)) {
            check_landOn();
        }
        m874 = chkAttention();
        switch (mState) {
        case 1:
            mUpdateEye = wait_1();
            break;
        case 2:
            mUpdateEye = wait_2();
            break;
        case 3:
            mUpdateEye = talk_1();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return 1;
}

/* 00006B24-00006C64       .text wait_action2__11daNpc_Ko1_cFPv */
int daNpc_Ko1_c::wait_action2(void*) {
    switch (mActionState) {
    case 0:
        setStt(0xC);
        mActionState++;
        break;
    case 1:
    case 2:
    case 3:
        m874 = chkAttention();
        switch (mState) {
        case 0xC:
            mUpdateEye = wait_5(0xD);
            break;
        case 0xD:
            mUpdateEye = attk_2(0x10, 0xF);
            break;
        case 0xF:
            mUpdateEye = walk_2(0xC, 0xD);
            break;
        case 0xE:
            mUpdateEye = wait_6();
            break;
        case 0x10:
            mUpdateEye = down_1(0xE);
            break;
        case 3:
            mUpdateEye = talk_1();
            break;
        case 0x15:
            mUpdateEye = talk_2();
            break;
        case 0x14:
            mUpdateEye = manzai();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return 1;
}

/* 00006C64-00006D58       .text wait_action3__11daNpc_Ko1_cFPv */
int daNpc_Ko1_c::wait_action3(void*) {
    switch (mActionState) {
    case 0:
        setStt(0x1D);
        mActionState++;
        break;
    case 1:
    case 2:
    case 3:
        m874 = chkAttention();
        switch (mState) {
        case 0x1D:
            mUpdateEye = wait_a();
            break;
        case 3:
            mUpdateEye = talk_1();
            break;
        case 0x15:
            mUpdateEye = talk_2();
            break;
        case 0x14:
            mUpdateEye = manzai();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return 1;
}

/* 00006D58-00006DF4       .text wait_action4__11daNpc_Ko1_cFPv */
int daNpc_Ko1_c::wait_action4(void*) {
    switch (mActionState) {
    case 0:
        setStt(0x1B);
        mActionState++;
        break;
    case 1:
    case 2:
    case 3:
        m874 = chkAttention();
        switch (mState) {
        case 0x1B:
            mUpdateEye = neru_1();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return 1;
}

/* 00006DF4-00006F34       .text demo__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::demo() {
    /* Nonmatching */
}

/* 00006F34-00007008       .text shadowDraw__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::shadowDraw() {
    cXyz pos(current.pos.x, current.pos.y + 150.0f, current.pos.z);
    mShadowId = dComIfGd_setShadow(
        mShadowId, 1, mpMorf->getModel(), &pos, 800.0f, 40.0f, current.pos.y,
        mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr, 0, 1.0f,
        dDlst_shadowControl_c::getSimpleTex());
    if (mShadowId != 0) {
        if (mpItemModel != NULL) {
            dComIfGd_addRealShadow(mShadowId, mpItemModel);
        }
        dComIfGd_addRealShadow(mShadowId, mpHeadMorf->getModel());
    }
}

/* 00007008-0000728C       .text _draw__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::_draw() {
    /* Nonmatching */
    return TRUE;
}

/* 0000728C-0000756C       .text _execute__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::_execute() {
    /* Nonmatching */
    return TRUE;
}

/* 0000756C-000075F0       .text _delete__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::_delete() {
    dComIfG_resDelete(&mPhs, "Ko");
    if (heap != NULL) {
        if (mpMorf != NULL) {
            mpMorf->stopZelAnime();
        }
        if (mpHeadMorf != NULL) {
            mpHeadMorf->stopZelAnime();
        }
        if (mpBalloonMorf != NULL) {
            mpBalloonMorf->stopZelAnime();
        }
    }
    mRippleCallback.end();
    return TRUE;
}

/* 000075F0-00007610       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daNpc_Ko1_c*)i_this)->CreateHeap();
}

/* 00007610-00007730       .text _create__11daNpc_Ko1_cFv */
cPhs_State daNpc_Ko1_c::_create() {
    fopAcM_SetupActor(this, daNpc_Ko1_c);
    static u32 a_size_tbl[] = {0x272E0, 0x272E0};
    cPhs_State phase = dComIfG_resLoad(&mPhs, "Ko");
    if (phase != cPhs_COMPLEATE_e) {
        return phase;
    }
    if (!charDecide(fopAcM_GetParam(this) & 0xFF)) {
        return cPhs_ERROR_e;
    }
    if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, a_size_tbl[mType])) {
        return cPhs_ERROR_e;
    }
    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -50.0f, -20.0f, -50.0f, 50.0f, 120.0f, 50.0f);
    if (!createInit()) {
        return cPhs_ERROR_e;
    }
    return phase;
}

/* 00007C2C-00007E9C       .text create_Anm__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::create_Anm() {
    /* Nonmatching */
}

/* 00007E9C-00008090       .text create_hed_Anm__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::create_hed_Anm() {
    /* Nonmatching */
}

/* 00008090-000082A4       .text create_bln_Anm__11daNpc_Ko1_cFv */
void daNpc_Ko1_c::create_bln_Anm() {
    /* Nonmatching */
}

/* 000082A4-00008380       .text create_itm_Mdl__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::create_itm_Mdl() {
    if (mType != 1) {
        return TRUE;
    }
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Ko", dRes_ID_KO_BDL_KOEDA_e);
    JUT_ASSERT(0x1126, a_mdl_dat != 0);
    mpItemModel = mDoExt_J3DModel__create(a_mdl_dat, 0x80000, 0x11000022);
    if (mpItemModel == NULL) {
        return FALSE;
    }
    return TRUE;
}

/* 00008380-00008604       .text CreateHeap__11daNpc_Ko1_cFv */
BOOL daNpc_Ko1_c::CreateHeap() {
    /* Nonmatching */
    return FALSE;
}

/* 00008604-00008624       .text daNpc_Ko1_Create__FP10fopAc_ac_c */
static cPhs_State daNpc_Ko1_Create(fopAc_ac_c* i_this) {
    return ((daNpc_Ko1_c*)i_this)->_create();
}

/* 00008624-00008644       .text daNpc_Ko1_Delete__FP11daNpc_Ko1_c */
static BOOL daNpc_Ko1_Delete(daNpc_Ko1_c* i_this) {
    return ((daNpc_Ko1_c*)i_this)->_delete();
}

/* 00008644-00008664       .text daNpc_Ko1_Execute__FP11daNpc_Ko1_c */
static BOOL daNpc_Ko1_Execute(daNpc_Ko1_c* i_this) {
    return ((daNpc_Ko1_c*)i_this)->_execute();
}

/* 00008664-00008684       .text daNpc_Ko1_Draw__FP11daNpc_Ko1_c */
static BOOL daNpc_Ko1_Draw(daNpc_Ko1_c* i_this) {
    return ((daNpc_Ko1_c*)i_this)->_draw();
}

/* 00008684-0000868C       .text daNpc_Ko1_IsDelete__FP11daNpc_Ko1_c */
static BOOL daNpc_Ko1_IsDelete(daNpc_Ko1_c*) {
    return TRUE;
}

static actor_method_class l_daNpc_Ko1_Method = {
    (process_method_func)daNpc_Ko1_Create,
    (process_method_func)daNpc_Ko1_Delete,
    (process_method_func)daNpc_Ko1_Execute,
    (process_method_func)daNpc_Ko1_IsDelete,
    (process_method_func)daNpc_Ko1_Draw,
};

actor_process_profile_definition g_profile_NPC_KO1 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_KO1_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Ko1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_KO1_e,
    /* Actor SubMtd */ &l_daNpc_Ko1_Method,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_NPC_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};

actor_process_profile_definition g_profile_NPC_KO2 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_KO2_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Ko1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_KO2_e,
    /* Actor SubMtd */ &l_daNpc_Ko1_Method,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_NPC_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};

inline daNpc_Ko1_HIO_c::~daNpc_Ko1_HIO_c() {}
