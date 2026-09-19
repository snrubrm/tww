/**
 * d_a_npc_kk1.cpp
 * NPC - Mila (poor)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_kk1.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_swc00.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo.h"
#include "d/d_s_play.h"
#include "f_op/f_op_actor_mng.h"
#include "c/c_dylink.h"
#include "m_Do/m_Do_mtx.h"
#include "res/Object/Kk.h"
#include "JSystem/J3DGraphBase/J3DSys.h"
#include "JSystem/J3DGraphAnimator/J3DJoint.h"
#include "JSystem/J3DGraphAnimator/J3DModel.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JUtility/JUTAssert.h"
#include <string.h>

static daNpc_Kk1_HIO_c l_HIO;
static fopAc_ac_c* l_check_inf[20];
static int l_check_wrk;

/* 000000EC-00000150       .text __ct__15daNpc_Kk1_HIO_cFv */
daNpc_Kk1_HIO_c::daNpc_Kk1_HIO_c() {
    static hio_prm_c a_prm_tbl = {
        0x2000, 0x251C, (s16)0xE002, (s16)0xDAE4,
        0, 0x2AF8, 0, (s16)0xD508,
        0x640, 0x4B0,
        150.0f,
        0,
        0xD2,
        0x28, 0x5A, 0x1E, 0x14, 4, 0x1000,
        40.0f, 3.0f, 0.2f, 0.5f, 13.0f, 1.0f, 0.1f, 10.0f, 1.0f, 0.2f, 1000.0f,
    };
    memcpy(&mPrm, &a_prm_tbl, sizeof(mPrm));
    mNo = -1;
    mCount = -1;
}

daNpc_Kk1_HIO_c::~daNpc_Kk1_HIO_c() {}

static char* l_evn_tbl[] = {
    "run_start",
    "run_start_2",
    "catch",
    "get_empty_btl",
    "bye",
    "otoboke",
    "runaway",
    "bye_2",
};

/* 00000198-000001E4       .text nodeCB_Head__FP7J3DNodei */
static BOOL nodeCB_Head(J3DNode* node, int phase) {
    if (phase == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Kk1_c* npc = (daNpc_Kk1_c*)model->getUserArea();
        if (npc != NULL) {
            npc->_nodeCB_Head(node, model);
        }
    }
    return TRUE;
}

/* 000001E4-00000304       .text _nodeCB_Head__11daNpc_Kk1_cFP7J3DNodeP8J3DModel */
void daNpc_Kk1_c::_nodeCB_Head(J3DNode* node, J3DModel* model) {
    static cXyz a_eye_pos_off(14.0f, 18.0f, 0.0f);
    int joint = ((J3DJoint*)node)->getJntNo();
    mDoMtx_stack_c::copy(model->getAnmMtx(joint));
    m750.x = mDoMtx_stack_c::get()[0][3];
    m750.y = mDoMtx_stack_c::get()[1][3];
    m750.z = mDoMtx_stack_c::get()[2][3];
    mDoMtx_stack_c::multVec(&a_eye_pos_off, &m72C);
    mDoMtx_stack_c::XrotM(m_jnt.getHead_y());
    mDoMtx_stack_c::ZrotM(-m_jnt.getHead_x());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    MTXCopy(mDoMtx_stack_c::get(), model->getAnmMtx(joint));
}

/* 00000340-0000038C       .text nodeCB_BackBone__FP7J3DNodei */
static BOOL nodeCB_BackBone(J3DNode* node, int phase) {
    if (phase == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Kk1_c* npc = (daNpc_Kk1_c*)model->getUserArea();
        if (npc != NULL) {
            npc->_nodeCB_BackBone(node, model);
        }
    }
    return TRUE;
}

/* 0000038C-0000042C       .text _nodeCB_BackBone__11daNpc_Kk1_cFP7J3DNodeP8J3DModel */
void daNpc_Kk1_c::_nodeCB_BackBone(J3DNode* node, J3DModel* model) {
    int joint = ((J3DJoint*)node)->getJntNo();
    mDoMtx_stack_c::copy(model->getAnmMtx(joint));
    mDoMtx_stack_c::XrotM(m_jnt.getBackbone_y());
    mDoMtx_stack_c::ZrotM(-m_jnt.getBackbone_x());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    MTXCopy(mDoMtx_stack_c::get(), model->getAnmMtx(joint));
}

/* 0000042C-0000044C       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daNpc_Kk1_c*)i_this)->CreateHeap();
}

/* 0000044C-000004EC       .text searchActor_SWC00__FPvPv */
static void* searchActor_SWC00(void* i_actor, void* i_data) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_actor;
    daNpc_Kk1_c* i_this = (daNpc_Kk1_c*)i_data;
    if (l_check_wrk < 20) {
        if (fopAc_IsActor(actor) && fopAcM_GetName(actor) == fpcNm_SWC00_e) {
            u32 param = fopAcM_GetParam(actor);
            if ((param >> 0x10 & 3) == 0 && (param & 0xFF) == i_this->mSwNo) {
                l_check_inf[l_check_wrk] = actor;
                l_check_wrk++;
            }
        }
    }
    return NULL;
}

/* 000004EC-00000598       .text init_KK1_0__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::init_KK1_0() {
    if (dComIfGs_isEventBit(0x2D01)) {
        if (!dKy_daynight_check() || !dComIfGs_isEventBit(0xE08)) {
            set_action(&daNpc_Kk1_c::wait_action1, NULL);
            return true;
        }
    }
    return false;
}

/* 00000598-000007B0       .text createInit__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::createInit() {
    for (int i = 0; i < 8; i++) {
        mEventIds[i] = dComIfGp_evmng_getEventIdx(l_evn_tbl[i]);
    }
    mEventCut.setActorInfo2("Kk1", this);
    mSwNo = (fopAcM_GetParam(this) >> 8) & 0xFF;
    u8 path = (fopAcM_GetParam(this) >> 16) & 0xFF;
    if (path != 0xFF) {
        mPath.setInf(path, current.roomNo, 1);
        if (mPath.getPath()) {
            fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
            set_pthPoint(0);
        } else {
            return false;
        }
    }
    if (!mPath.getPath()) {
        return false;
    }
    attention_info.flags = 0xA;
    switch (mSubType) {
    case 0:
    default:
        attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0xA9;
        attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0xA9;
        break;
    }
    gravity = -4.5f;
    mAnmNo = 12;
    bool result;
    switch (mSubType) {
    case 0:
        result = init_KK1_0();
        break;
    default:
        result = false;
        break;
    }
    if (!result) {
        return false;
    }
    mModelAngle = current.angle;
    shape_angle = mModelAngle;
    mStts.Init(0xFF, 0xFF, this);
    mCyl.SetStts(&mStts);
    mCyl.Set(dNpc_cyl_src);
    mObjAcch.CrrPos(*dComIfG_Bgsp());
    play_animation();
    tevStr.mRoomNo = dComIfG_Bgsp()->GetRoomId(mObjAcch.m_gnd);
    tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mObjAcch.m_gnd);
    mpMorf->setMorf(0.0f);
    setMtx(true);
    return true;
}

/* 000007B0-00000918       .text play_animation__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::play_animation() {
    u32 sound = 0;
    play_btp_anm();
    if (mObjAcch.ChkGroundHit()) {
        sound = dComIfG_Bgsp()->GetMtrlSndId(mObjAcch.m_gnd);
    }
    mAnmEnd = mpMorf->play(&eyePos, sound, dComIfGp_getReverb(current.roomNo));
    if (mpMorf->getFrame() < mFrame) {
        mAnmEnd = 1;
    }
    mFrame = mpMorf->getFrame();
    switch (mAnmNo) {
    case 8:
        if (mpMorf->checkFrame(4.0f)) {
            setBikon(cXyz(0.0f, -50.0f, -15.0f));
        }
        break;
    case 9:
        if (mpMorf->checkFrame(4.0f)) {
            setBikon(cXyz(0.0f, -50.0f, 0.0f));
        }
        break;
    }
    play_eff_anm();
}

/* 00000918-00000A98       .text setMtx__11daNpc_Kk1_cFb */
void daNpc_Kk1_c::setMtx(bool force) {
    J3DModel* model = mpMorf->getModel();
    model->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mModelAngle);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->calc();
    cXyz offset(0.0f, 120.0f, 30.0f);
    mBckAnm.entry(mpEffModel->getModelData(), mBckFrame);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::transM(offset.x, offset.y, offset.z);
    mpEffModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mpEffModel->calc();
    mpEffModel->getModelData()->getJointNodePointer(0)->setMtxCalc(NULL);
    flwAse();
    setAttention(force);
}

/* 00000A98-00000AAC       .text bckResID__11daNpc_Kk1_cFi */
int daNpc_Kk1_c::bckResID(int index) {
    static const int a_resID_tbl[] = {
        dRes_ID_KK_BCK_KK_WAIT01_e,
        dRes_ID_KK_BCK_KK_WAIT02_e,
        dRes_ID_KK_BCK_KK_WAIT03_e,
        dRes_ID_KK_BCK_KK_WALK_e,
        dRes_ID_KK_BCK_KK_RUN_e,
        dRes_ID_KK_BCK_KK_TALK01_e,
        dRes_ID_KK_BCK_KK_TALK02_e,
        dRes_ID_KK_BCK_KK_TALK03_e,
        dRes_ID_KK_BCK_KK_FURIMUKI_e,
        dRes_ID_KK_BCK_KK_FURIMUKI02_e,
        dRes_ID_KK_BCK_KK_OJIGI_e,
        dRes_ID_KK_BCK_KK_TEBYOUSHI_e,
    };
    return a_resID_tbl[index];
}

/* 00000AAC-00000AC0       .text btpResID__11daNpc_Kk1_cFi */
int daNpc_Kk1_c::btpResID(int index) {
    static const int a_resID_tbl[] = {
        dRes_ID_KK_BTP_KK_e,
    };
    return a_resID_tbl[index];
}

/* 00000AC0-00000BC4       .text setBtp__11daNpc_Kk1_cFScb */
bool daNpc_Kk1_c::setBtp(s8 number, bool modify) {
    J3DModel* model = mpMorf->getModel();
    if (number < 0) {
        return false;
    }
    J3DAnmTexPattern* a_btp = (J3DAnmTexPattern*)dComIfG_getObjectIDRes(mArcName, btpResID(number));
    JUT_ASSERT(0x234, a_btp != 0);
    mBtpNo = number;
    mBtpFrame = 0;
    mBtpTimer = 0;
    return mBtpAnm.init(model->getModelData(), a_btp, true, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, modify, 0) != 0;
}

/* 00000BC4-00000BE4       .text init_texPttrnAnm__11daNpc_Kk1_cFScb */
bool daNpc_Kk1_c::init_texPttrnAnm(s8 number, bool modify) {
    return setBtp(number, modify);
}

/* 00000BE4-00000C80       .text play_btp_anm__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::play_btp_anm() {
    u8 end = mBtpAnm.getBtpAnm()->getFrameMax();
    if (mBtpNo != 0 || cLib_calcTimer(&mBtpTimer) == 0) {
        mBtpFrame++;
        if (mBtpFrame >= end) {
            if (mBtpNo != 0) {
                mBtpFrame = end;
            } else {
                mBtpTimer = cLib_getRndValue(60, 90);
                mBtpFrame = 0;
            }
        }
    }
}

/* 00000C80-00000CFC       .text play_eff_anm__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::play_eff_anm() {
    mBpkFrame++;
    if (mBpkFrame >= 0x1E) {
        mBpkFrame = 0x1D;
    }
    s16 btkMax = mBtkAnm.getBtkAnm()->getFrameMax();
    mBtkFrame++;
    if (mBtkFrame >= btkMax) {
        mBtkFrame = btkMax - 1;
    }
    s16 bckMax = mBckAnm.getBckAnm()->getFrameMax();
    mBckFrame++;
    if (mBckFrame >= bckMax) {
        mBckFrame = 0x3B;
    }
}

/* 00000CFC-00000DDC       .text setAnm_anm__11daNpc_Kk1_cFPQ211daNpc_Kk1_c9anm_prm_c */
void daNpc_Kk1_c::setAnm_anm(daNpc_Kk1_c::anm_prm_c* prm) {
    s8 number = prm->mAnm;
    if (number < 0 || mAnmNo == number) {
        return;
    }
    dNpc_setAnmIDRes(mpMorf, prm->mLoop, prm->mMorf, prm->mSpeed, bckResID(number), -1, mArcName);
    mAnmNo = prm->mAnm;
    delBikon();
    delAse();
    m7BE = 1;
    if (mAnmNo == 1) {
        setAse();
        mBckFrame = 0;
        mBtkFrame = 0;
        mBpkFrame = 0;
        m7BE = 0;
    }
    mAnmEnd = 0;
    mAnmRepeat = 0;
    mFrame = 0.0f;
}

/* 00000DDC-00000E4C       .text setAnm_NUM__11daNpc_Kk1_cFii */
void daNpc_Kk1_c::setAnm_NUM(int index, int tex) {
    static anm_prm_c a_anm_prm_tbl[] = {
        {0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {1, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {2, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {3, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {4, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {5, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {6, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {7, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {8, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {9, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {10, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {11, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {0, 0, 0.0f, 0.0f, J3DFrameCtrl::EMode_NONE},
        {0, 0, 0.0f, 0.0f, J3DFrameCtrl::EMode_NONE},
    };
    if (tex) {
        init_texPttrnAnm(a_anm_prm_tbl[index].mBtp, true);
    }
    setAnm_anm(&a_anm_prm_tbl[index]);
}

/* 00000E4C-00000EB8       .text setAnm__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::setAnm() {
    static anm_prm_c a_anm_prm_tbl[] = {
        {-1, -1, 0.0f, 0.0f, J3DFrameCtrl::EMode_NULL},
        {0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {-1, -1, 0.0f, 0.0f, J3DFrameCtrl::EMode_NULL},
        {3, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {-1, -1, 0.0f, 0.0f, J3DFrameCtrl::EMode_NULL},
        {0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
    };
    init_texPttrnAnm(a_anm_prm_tbl[mState].mBtp, true);
    setAnm_anm(&a_anm_prm_tbl[mState]);
}

/* 00000EB8-00000EBC       .text chngAnmTag__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::chngAnmTag() {}

/* 00000EBC-00000EC0       .text ctrlAnmTag__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::ctrlAnmTag() {}

/* 00000EC0-00000FA0       .text chngAnmAtr__11daNpc_Kk1_cFUc */
void daNpc_Kk1_c::chngAnmAtr(unsigned char attr) {
    switch (mCurrMsgNo) {
    case 0x1CAC: {
        int deleted;
        fopAc_ac_c* actor = searchByID(mPartnerId, &deleted);
        if (actor != NULL && deleted == 0) {
            mLookPos = actor->current.pos;
            mLookPos.y += 200.0f + l_HIO.mPrm.mAttentionOffsetY;
            m81E = 2;
            m7C4 = 1;
            m7A2 = l_HIO.mPrm.m22;
        }
        break;
    }
    }
    if (attr == mAnmAttr || attr > 0xD) {
        return;
    }
    mAnmAttr = attr;
    setAnm_ATR();
}

/* 00000FA0-00001080       .text ctrlAnmAtr__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::ctrlAnmAtr() {
    switch (mAnmAttr) {
    case 8:
        if (mAnmEnd) {
            current.angle.y += 0x8000;
            setAnm_NUM(0, 1);
            mpMorf->setMorf(0.0f);
            mAnmAttr = 0;
        }
        break;
    case 0xB:
        if (mAnmEnd) {
            setAnm_NUM(6, 1);
            mAnmAttr = 6;
        }
        break;
    case 0xA:
        if (mAnmEnd) {
            setAnm_NUM(0, 1);
            mAnmAttr = 0;
        }
        break;
    }
}

/* 00001080-000010E4       .text setAnm_ATR__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::setAnm_ATR() {
    static anm_prm_c a_anm_prm_tbl[] = {
        {0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {1, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {2, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {3, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {4, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {5, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {6, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {7, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {8, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {9, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {10, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {10, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {11, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
    };
    init_texPttrnAnm(a_anm_prm_tbl[mAnmAttr].mBtp, true);
    setAnm_anm(&a_anm_prm_tbl[mAnmAttr]);
}

/* 000010E4-000011A0       .text anmAtr__11daNpc_Kk1_cFUs */
void daNpc_Kk1_c::anmAtr(unsigned short status) {
    switch (status) {
    case 6:
        if (mMsgAnmStarted == 0) {
            chngAnmAtr(dComIfGp_getMesgAnimeAttrInfo());
            mMsgAnmStarted++;
        }
        {
            u8 tag = dComIfGp_getMesgAnimeTagInfo();
            if (tag != 0xFF && tag != mAnmTag) {
                dComIfGp_clearMesgAnimeTagInfo();
                mAnmTag = tag;
                chngAnmTag();
            }
        }
        break;
    case 14:
        mMsgAnmStarted = 0;
        break;
    }
    ctrlAnmAtr();
    ctrlAnmTag();
}

/* 000011A0-0000149C       .text next_msgStatus__11daNpc_Kk1_cFPUl */
u16 daNpc_Kk1_c::next_msgStatus(unsigned long* pMsg) {
    u16 result = fopMsgStts_MSG_CONTINUES_e;
    switch (*pMsg) {
    case 0x1C85:
        *pMsg = 0x1C86;
        break;
    case 0x1C86:
        *pMsg = 0x1CA7;
        break;
    case 0x1C88:
        *pMsg = 0x1C89;
        break;
    case 0x1C89:
        *pMsg = 0x1C8A;
        break;
    case 0x1C8A:
        *pMsg = 0x1CA9;
        break;
    case 0x1C8B:
        *pMsg = 0x1CAC;
        m7B8 = 1;
        break;
    case 0x1C8D:
        *pMsg = 0x1C8E;
        break;
    case 0x1C8E:
        *pMsg = 0x1C8F;
        break;
    case 0x1C8F:
        switch (mpCurrMsg->mSelectNum) {
        case 0:
            *pMsg = 0x1C90;
            break;
        case 1:
            *pMsg = 0x1CA5;
            mLookAngle++;
            break;
        }
        break;
    case 0x1C90:
        *pMsg = 0x1C91;
        break;
    case 0x1C91:
        switch (mpCurrMsg->mSelectNum) {
        case 0:
            *pMsg = 0x1C93;
            break;
        case 1:
            *pMsg = 0x1C92;
            mLookAngle++;
            break;
        }
        break;
    case 0x1C92:
        *pMsg = 0x1C91;
        break;
    case 0x1C93:
        *pMsg = 0x1C94;
        break;
    case 0x1C94:
        switch (mpCurrMsg->mSelectNum) {
        case 0:
            *pMsg = 0x1C96;
            break;
        case 1:
            *pMsg = 0x1C95;
            mLookAngle++;
            break;
        }
        break;
    case 0x1C95:
        *pMsg = 0x1C96;
        break;
    case 0x1C96:
        *pMsg = 0x1C97;
        break;
    case 0x1C97:
        switch (mpCurrMsg->mSelectNum) {
        case 0:
            *pMsg = 0x1C98;
            break;
        case 1:
            *pMsg = 0x1C9B + (mLookAngle < 1 ? -1 : 0);
            break;
        }
        break;
    case 0x1C98:
        switch (mpCurrMsg->mSelectNum) {
        case 0:
            *pMsg = 0x1C99;
            break;
        case 1:
            result = fopMsgStts_MSG_ENDS_e;
            break;
        }
        break;
    case 0x1C99:
        switch (mpCurrMsg->mSelectNum) {
        case 0:
            *pMsg = 0x1C9D + (mLookAngle < 1 ? -1 : 0);
            break;
        case 1:
            *pMsg = 0x1C9A;
            break;
        }
        break;
    case 0x1C9B:
        *pMsg = 0x1C9D;
        break;
    case 0x1C9D:
        *pMsg = 0x1C9E;
        break;
    case 0x1C9E:
        *pMsg = 0x1CAA;
        break;
    case 0x1C9F:
        result = fopMsgStts_MSG_ENDS_e;
        break;
    case 0x1CA1:
        *pMsg = 0x1CA2;
        break;
    case 0x1CA2:
        *pMsg = 0x1CA3;
        break;
    case 0x1CA3:
        *pMsg = 0x1CA4;
        break;
    case 0x1CA4:
        *pMsg = 0x1CAB;
        break;
    case 0x1CA5:
        *pMsg = 0x1C8F;
        break;
    case 0x1CA7:
        *pMsg = 0x1CA8;
        break;
    case 0x1CAA:
        *pMsg = 0x1C9F;
        break;
    default:
        result = fopMsgStts_MSG_ENDS_e;
        break;
    }
    return result;
}

/* 0000149C-00001534       .text getMsg_KK1_0__11daNpc_Kk1_cFv */
u32 daNpc_Kk1_c::getMsg_KK1_0() {
    if (dKy_daynight_check() == 1) {
        if (m7BA != 0) {
            m7BA = 0;
            return 0x1CA1;
        }
        return 0x1C8B;
    }
    if (!dComIfGs_isEventBit(0xE08)) {
        return 0x1C88 + (dComIfGs_isEventBit(0xE10) ? -1 : 0);
    }
    return 0x1C85;
}

/* 00001534-00001570       .text getMsg__11daNpc_Kk1_cFv */
u32 daNpc_Kk1_c::getMsg() {
    u32 msg = 0;
    switch (mSubType) {
    case 0:
        msg = getMsg_KK1_0();
        break;
    }
    return msg;
}

/* 00001570-000015FC       .text eventOrder__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::eventOrder() {
    if (mOrder == 1 || mOrder == 2) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (mOrder == 1) {
            fopAcM_orderSpeakEvent(this);
        }
    } else if (mOrder >= 3) {
        mEventIndex = mOrder - 3;
        fopAcM_orderOtherEventId(this, mEventIds[mEventIndex], 0xFF, 0xFFFF, 0, 1);
    }
}

/* 000015FC-000016DC       .text checkOrder__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::checkOrder() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        if (dComIfGp_evmng_startCheck(mEventIds[mEventIndex]) && mOrder >= 3) {
            switch (mEventIndex) {
            case 4:
                setAnm_NUM(0, 1);
                break;
            }
            mOrder = 0;
            mAnmAttr = 0xFF;
            mAnmTag = 0xFF;
        }
    } else if (checkCommandTalk()) {
        if (mOrder == 1 || mOrder == 2) {
            mOrder = 0;
            m7C3 = 1;
        }
    }
}

/* 000016DC-00001774       .text chk_talk__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::chk_talk() {
    if (dComIfGp_event_chkTalkXY()) {
        if (dComIfGp_evmng_ChkPresentEnd()) {
            mItemNo = dComIfGp_event_getPreItemNo();
            return true;
        }
        return false;
    }
    mItemNo = 0xFF;
    return true;
}

/* 00001774-000017B4       .text chk_parts_notMov__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::chk_parts_notMov() {
    bool result = false;
    if (mPreviousAngles.y != m_jnt.getHead_y() || mPreviousAngles.z != m_jnt.getBackbone_y() || mPreviousAngles.x != current.angle.y) {
        result = true;
    }
    return result;
}

/* 000017B4-00001808       .text searchByID__11daNpc_Kk1_cFUiPi */
fopAc_ac_c* daNpc_Kk1_c::searchByID(fpc_ProcID id, int* deleted) {
    fopAc_ac_c* actor = NULL;
    *deleted = FALSE;
    if (!fopAcM_SearchByID(id, &actor)) {
        *deleted = TRUE;
    }
    return actor;
}

/* 00001808-000018B8       .text partner_search_sub__11daNpc_Kk1_cFPFPvPv_Pv */
bool daNpc_Kk1_c::partner_search_sub(void* (*judge)(void*, void*)) {
    bool result = false;
    mPartnerId = fpcM_ERROR_PROCESS_ID_e;
    l_check_wrk = 0;
    for (int i = 0; i < ARRAY_SSIZE(l_check_inf); i++) {
        l_check_inf[i] = NULL;
    }
    fpcM_Search(judge, this);
    if (l_check_wrk != 0) {
        mPartnerId = fpcM_GetID(l_check_inf[0]);
        result = true;
    }
    return result;
}

/* 000018B8-00001924       .text partner_search__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::partner_search() {
    bool result = false;
    if (mActionState == 1) {
        switch (mSubType) {
        case 0:
            result = partner_search_sub(searchActor_SWC00);
            break;
        default:
            result = true;
            break;
        }
        if (result) {
            mActionState++;
        }
    }
}

/* 00001924-00001B10       .text lookBack__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::lookBack() {
    /* Nonmatching */
}

/* 00001B10-00001B90       .text chkAttention__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::chkAttention() {
    dAttention_c& attention = dComIfGp_getAttention();
    if (attention.LockonTruth()) {
        return this == attention.LockonTarget(0);
    }
    return this == attention.ActionTarget(0);
}

/* 00001B90-00001BE8       .text setAttention__11daNpc_Kk1_cFb */
void daNpc_Kk1_c::setAttention(bool force) {
    attention_info.position.set(current.pos.x, current.pos.y + l_HIO.mPrm.mAttentionOffsetY, current.pos.z);
    if (!mUpdateEye && !force) {
        return;
    }
    eyePos.set(m72C.x, m72C.y, m72C.z);
}

/* 00001BE8-00001C70       .text decideType__11daNpc_Kk1_cFi */
bool daNpc_Kk1_c::decideType(int) {
    if (mType > 0) {
        return true;
    }
    mType = 1;
    mSubType = 0;
    strcpy(mArcName, "Kk");
    bool result = false;
    if (mType != -1 && mSubType != -1) {
        result = true;
    }
    return result;
}

/* 00001C70-00001D10       .text cut_init_RUN_START__11daNpc_Kk1_cFi */
void daNpc_Kk1_c::cut_init_RUN_START(int) {
    int deleted;
    fopAc_ac_c* a_actor = searchByID(mPartnerId, &deleted);
    JUT_ASSERT(0x54F, a_actor != 0);
    dComIfGp_event_setItemPartner(a_actor);
    mPath.nextIdxAuto();
}

/* 00001D10-00001DD0       .text cut_move_RUN_START__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::cut_move_RUN_START() {
    cXyz pos = mPath.getPoint(mPath.getIdx());
    s16 target = cLib_targetAngleY(&current.pos, &pos);
    cLib_addCalcAngleS(&current.angle.y, target, l_HIO.mPrm.m24, l_HIO.mPrm.m26, 0x80);
    if (current.angle.y == target) {
        daPy_getPlayerActorClass()->cancelOriginalDemo();
        return true;
    }
    return false;
}

/* 00001DD0-00001E58       .text cut_init_RUN__11daNpc_Kk1_cFi */
void daNpc_Kk1_c::cut_init_RUN(int staff) {
    int* timer = dComIfGp_evmng_getMyIntegerP(staff, "Timer");
    m798 = -1;
    if (timer != NULL) {
        m798 = *timer;
    }
    setAnm_NUM(4, 1);
    m7B6 = 1;
    m815 = 2;
    m816 = 0;
}

/* 00001E58-00001EAC       .text cut_move_RUN__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::cut_move_RUN() {
    event_move(false);
    if (m798 >= 0) {
        return cLib_calcTimer(&m798) == 0;
    }
}

/* 00001EAC-00001F08       .text cut_init_CATCH_START__11daNpc_Kk1_cFi */
void daNpc_Kk1_c::cut_init_CATCH_START(int) {
    daPy_getPlayerActorClass()->onPlayerNoDraw();
    setAnm_NUM(8, 1);
    mpMorf->setMorf(0.0f);
}

/* 00001F08-00001F2C       .text cut_move_CATCH_START__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::cut_move_CATCH_START() {
    if (mAnmEnd) {
        mUpdateEye = FALSE;
        return true;
    }
    return false;
}

/* 00001F2C-00001FAC       .text cut_init_CATCH_END__11daNpc_Kk1_cFi */
void daNpc_Kk1_c::cut_init_CATCH_END(int) {
    daPy_getPlayerActorClass()->offPlayerNoDraw();
    current.angle.y += 0x8000;
    setAnm_NUM(0, 1);
    mpMorf->setMorf(0.0f);
    m81E = 1;
    m7C4 = 0;
    m_jnt.setTrn();
}

/* 00001FAC-00001FB4       .text cut_move_CATCH_END__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::cut_move_CATCH_END() {
    return true;
}

/* 00001FB4-000022BC       .text cut_init_TRN__11daNpc_Kk1_cFi */
void daNpc_Kk1_c::cut_init_TRN(int) {
    /* Nonmatching */
}

/* 000022BC-00002364       .text cut_move_TRN__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::cut_move_TRN() {
    /* Nonmatching */
    return false;
}

/* 00002364-00002388       .text cut_init_BYE_START__11daNpc_Kk1_cFi */
void daNpc_Kk1_c::cut_init_BYE_START(int) {
    daPy_getPlayerActorClass()->onPlayerNoDraw();
    m7BB = 1;
}

/* 00002388-00002390       .text cut_move_BYE_START__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::cut_move_BYE_START() {
    return true;
}

/* 00002390-00002490       .text cut_init_BYE__11daNpc_Kk1_cFi */
void daNpc_Kk1_c::cut_init_BYE(int) {
    /* Nonmatching */
}

/* 00002490-00002568       .text cut_move_BYE__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::cut_move_BYE() {
    /* Nonmatching */
    return false;
}

/* 00002568-000025C8       .text cut_init_BYE_CONTINUE__11daNpc_Kk1_cFi */
void daNpc_Kk1_c::cut_init_BYE_CONTINUE(int staff) {
    int* timer = dComIfGp_evmng_getMyIntegerP(staff, "Timer");
    m798 = 30;
    if (timer != NULL) {
        m798 = *timer;
    }
}

/* 000025C8-00002620       .text cut_move_BYE_CONTINUE__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::cut_move_BYE_CONTINUE() {
    event_move(false);
    if (cLib_calcTimer(&m798) == 0) {
        speedF = 0.0f;
        return true;
    }
    return false;
}

/* 00002620-000026BC       .text cut_init_BYE_END__11daNpc_Kk1_cFi */
void daNpc_Kk1_c::cut_init_BYE_END(int) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    player->changeOriginalDemo();
    player->changeDemoMode(daPy_demo_c::DEMO_INIT_WAIT_e);
    s16 angle = cLib_targetAngleY(&player->current.pos, &current.pos);
    player->setPlayerPosAndAngle(&player->current.pos, angle);
    player->offPlayerNoDraw();
    m7C0 = 1;
}

/* 000026BC-000026C4       .text cut_move_BYE_END__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::cut_move_BYE_END() {
    return true;
}

/* 000026C4-000026C8       .text cut_init_PLYER_TRN__11daNpc_Kk1_cFi */
void daNpc_Kk1_c::cut_init_PLYER_TRN(int) {}

/* 000026C8-000026D0       .text cut_move_PLYER_TRN__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::cut_move_PLYER_TRN() {
    return true;
}

/* 000026D0-00002744       .text cut_init_OTOBOKE__11daNpc_Kk1_cFi */
void daNpc_Kk1_c::cut_init_OTOBOKE(int) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    player->changeOriginalDemo();
    player->changeDemoMode(daPy_demo_c::DEMO_INIT_WAIT_e);
    player->setPlayerPosAndAngle(&player->current.pos, current.angle.y);
    m798 = 2;
}

/* 00002744-00002798       .text cut_move_OTOBOKE__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::cut_move_OTOBOKE() {
    if (cLib_calcTimer(&m798) == 0) {
        daPy_getPlayerActorClass()->cancelOriginalDemo();
        return true;
    }
    return false;
}

/* 00002798-000028A4       .text cut_init_PLYER_MOV__11daNpc_Kk1_cFi */
void daNpc_Kk1_c::cut_init_PLYER_MOV(int) {
    /* Nonmatching */
}

/* 000028A4-000028AC       .text cut_move_PLYER_MOV__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::cut_move_PLYER_MOV() {
    return true;
}

/* 000028AC-00002A40       .text cut_init_RUNAWAY_START__11daNpc_Kk1_cFi */
void daNpc_Kk1_c::cut_init_RUNAWAY_START(int) {
    /* Nonmatching */
}

/* 00002A40-00002AD0       .text cut_move_RUNAWAY_START__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::cut_move_RUNAWAY_START() {
    if (mAnmNo == 8) {
        if (mAnmEnd) {
            current.angle.y = cLib_targetAngleY(&current.pos, &dComIfGp_getPlayer(0)->current.pos);
            setAnm_NUM(0, 1);
            mpMorf->setMorf(0.0f);
            return true;
        }
        return false;
    }
    return true;
}

/* 00002AD0-00002B08       .text cut_init_RUNAWAY_END__11daNpc_Kk1_cFi */
void daNpc_Kk1_c::cut_init_RUNAWAY_END(int) {
    daPy_getPlayerActorClass()->offPlayerNoDraw();
    m7B6 = 0;
    speedF = 0.0f;
    m7C0 = 1;
}

/* 00002B08-00002B10       .text cut_move_RUNAWAY_END__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::cut_move_RUNAWAY_END() {
    return true;
}

/* 00002B10-00002D98       .text privateCut__11daNpc_Kk1_cFi */
void daNpc_Kk1_c::privateCut(int) {
    /* Nonmatching */
}

/* 00002D98-00002DBC       .text endEvent__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::endEvent() {
    dComIfGp_event_reset();
    mAnmAttr = 0xFF;
    mAnmTag = 0xFF;
}

/* 00002DBC-00002DF4       .text isEventEntry__11daNpc_Kk1_cFv */
int daNpc_Kk1_c::isEventEntry() {
    return dComIfGp_evmng_getMyStaffId(mEventCut.getActorName(), NULL, 0);
}

/* 00002DF4-00002F80       .text event_proc__11daNpc_Kk1_cFi */
void daNpc_Kk1_c::event_proc(int) {
    /* Nonmatching */
}

/* 00002F80-00002FB8       .text checkCommandTalk__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::checkCommandTalk() {
    bool result = false;
    if (eventInfo.checkCommandTalk()) {
        result = true;
        if (m816 == 5 && m7A4 != 0) {
            result = false;
        }
    }
    return result;
}

/* 00002FB8-00003064       .text set_action__11daNpc_Kk1_cFM11daNpc_Kk1_cFPCvPvPv_iPv */
bool daNpc_Kk1_c::set_action(ActionFunc action, void* arg) {
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

/* 00003064-0000311C       .text setStt__11daNpc_Kk1_cFSc */
void daNpc_Kk1_c::setStt(signed char state) {
    s8 previous = mState;
    mState = state;
    switch (mState) {
    case 1:
    case 4:
    case 6:
    case 7:
        mOrder = 0;
        m7A4 = 0;
        speedF = 0.0f;
        break;
    case 2:
        mOrder = 0;
        mAnmAttr = 0xFF;
        mAnmTag = 0xFF;
        mMsgAnmStarted = 0;
        m7A2 = 0;
        mPreviousState = previous;
        break;
    case 3:
        mOrder = 0;
        break;
    case 5:
        mOrder = 0;
        m815 = 2;
        m816 = 0;
        m7B6 = 1;
        break;
    }
    setAnm();
}

/* 0000311C-000032D8       .text createTama__11daNpc_Kk1_cFf */
void daNpc_Kk1_c::createTama(float) {
    /* Nonmatching */
}

/* 000032D8-0000345C       .text chk_areaIN__11daNpc_Kk1_cFf4cXyz */
bool daNpc_Kk1_c::chk_areaIN(float, cXyz) {
    /* Nonmatching */
    return false;
}

/* 0000345C-00003578       .text startEvent_check__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::startEvent_check() {
    cXyz pos = current.pos;
    if (chk_areaIN(l_HIO.mPrm.m50, pos)) {
        if (current.pos.abs(dComIfGp_getPlayer(0)->current.pos) < 210.0f + REG9_F(0) || field_0x6ba != 0) {
            return true;
        }
    }
    return false;
}

/* 00003578-00003600       .text chkHitPlayer__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::chkHitPlayer() {
    bool result = false;
    if (mCyl.ChkCoHit()) {
        cCcD_Obj* obj = mCyl.GetCoHitObj();
        if (obj != NULL) {
            fopAc_ac_c* actor = obj->GetAc();
            if (actor != NULL) {
                result = fopAcM_GetName(actor) == fpcNm_PLAYER_e;
            }
        }
    }
    return result;
}

/* 00003600-000036A8       .text set_pthPoint__11daNpc_Kk1_cFUc */
void daNpc_Kk1_c::set_pthPoint(unsigned char point) {
    if (mPath.getPath()) {
        mPath.setIdx(point);
        current.pos = mPath.getPoint(mPath.getIdx());
        if (mPath.nextIdx()) {
            cXyz pos = mPath.getPoint(mPath.getIdx());
            current.angle.y = cLib_targetAngleY(&current.pos, &pos);
        }
    }
}

/* 000036A8-00003940       .text event_move__11daNpc_Kk1_cFb */
void daNpc_Kk1_c::event_move(bool) {
    /* Nonmatching */
}

/* 00003940-000039F0       .text kyoroPos__11daNpc_Kk1_cFi */
cXyz daNpc_Kk1_c::kyoroPos(int idx) {
    static Vec a_tgt_offst[] = {
        {0.0f, 0.0f, 0.0f},
        {-100.0f, 0.0f, 0.0f},
        {100.0f, 0.0f, 0.0f},
        {-100.0f, 0.0f, 100.0f},
        {100.0f, 0.0f, 100.0f},
        {0.0f, 0.0f, 100.0f},
        {-100.0f, 40.0f, 0.0f},
        {100.0f, 40.0f, 0.0f},
        {-100.0f, 40.0f, 100.0f},
        {100.0f, 40.0f, 100.0f},
        {0.0f, 40.0f, 100.0f},
        {-40.0f, 10.0f, 40.0f},
    };
    cXyz src(a_tgt_offst[idx].x, a_tgt_offst[idx].y, a_tgt_offst[idx].z);
    mDoMtx_stack_c::transS(eyePos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    cXyz dst;
    mDoMtx_stack_c::multVec(&src, &dst);
    return dst;
}

/* 000039F0-00003A84       .text kyorokyoro__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::kyorokyoro() {
    if (cLib_calcTimer(&m794) != 0) {
        mLookPos = kyoroPos(m796);
        return TRUE;
    }
    m796 = cLib_getRndValue(1, 10);
    m794 = l_HIO.mPrm.m1C;
    m792 = l_HIO.mPrm.m1E;
    return FALSE;
}

/* 00003A84-00003C9C       .text chk_attn__11daNpc_Kk1_cFv */
bool daNpc_Kk1_c::chk_attn() {
    /* Nonmatching */
    return false;
}

/* 00003C9C-00003D9C       .text setBikon__11daNpc_Kk1_cF4cXyz */
void daNpc_Kk1_c::setBikon(cXyz) {
    /* Nonmatching */
}

/* 00003D9C-00003DA0       .text delBikon__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::delBikon() {}

/* 00003DA0-00003E18       .text setAse__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::setAse() {
    delAse();
    mpAse = dComIfGp_particle_set(dPa_name::ID_IT_SN_MURU_ASE00, &current.pos, &current.angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
}

/* 00003E18-00003EA4       .text flwAse__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::flwAse() {
    if (mpAse != NULL) {
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(m_hed_jnt_num));
        cXyz pos(mDoMtx_stack_c::get()[0][3], mDoMtx_stack_c::get()[1][3], mDoMtx_stack_c::get()[2][3]);
        mpAse->setGlobalTranslation(pos.x, pos.y, pos.z);
    }
}

/* 00003EA4-00003ED0       .text delAse__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::delAse() {
    if (mpAse != NULL) {
        mpAse->becomeInvalidEmitter();
        mpAse = NULL;
    }
}

/* 00003ED0-0000415C       .text wait_1__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::wait_1() {
    /* Nonmatching */
    return FALSE;
}

/* 0000415C-0000449C       .text walk_1__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::walk_1() {
    /* Nonmatching */
    return FALSE;
}

/* 0000449C-00004614       .text wait_2__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::wait_2() {
    /* Nonmatching */
    return FALSE;
}

/* 00004614-0000466C       .text init_CMT_WAI__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::init_CMT_WAI() {
    m7A4 = cLib_getRndValue(0x5A, 0xB4);
    m81E = 5;
    m7C4 = 1;
    setAnm_NUM(0, 1);
}

/* 0000466C-00004754       .text move_CMT_WAI__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::move_CMT_WAI() {
    if (cLib_calcTimer(&m7A4) == 0) {
        if (mOrder != 1 && mOrder < 3) {
            cXyz pos = current.pos;
            if (chk_areaIN(l_HIO.mPrm.m50, pos)) {
                mOrder = 8;
                return TRUE;
            }
        }
        m816 = 0;
        setAnm_NUM(3, 1);
        m7B6 = 1;
    } else if (mOrder != 1 && mOrder < 3) {
        if (startEvent_check()) {
            mOrder = 9;
        }
    }
    return TRUE;
}

/* 00004754-000047D4       .text init_CMT_TRN__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::init_CMT_TRN() {
    m7AA = current.angle.y;
    m7A4 = cLib_getRndValue(0x5A, 0xB4);
    m798 = l_HIO.mPrm.m20;
    m794 = l_HIO.mPrm.m1C;
    m792 = l_HIO.mPrm.m1E;
    m81E = 0;
    m7C4 = 1;
    setAnm_NUM(0, 1);
}

/* 000047D4-00004A14       .text move_CMT_TRN__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::move_CMT_TRN() {
    /* Nonmatching */
    return FALSE;
}

/* 00004A14-00004A84       .text init_CMT_PCK__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::init_CMT_PCK() {
    setAnm_NUM(1, 1);
    m7A4 = l_HIO.mPrm.m1A;
    m81E = 0;
    m7C4 = 1;
    mEventIndex = 2;
    eventInfo.setEventId(mEventIds[mEventIndex]);
}

/* 00004A84-00004C34       .text move_CMT_PCK__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::move_CMT_PCK() {
    /* Nonmatching */
    return FALSE;
}

/* 00004C34-00004DD0       .text cmmt_1__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::cmmt_1() {
    /* Nonmatching */
    return FALSE;
}

/* 00004DD0-00004F74       .text wait_3__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::wait_3() {
    /* Nonmatching */
    return FALSE;
}

/* 00004F74-00005170       .text wait_4__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::wait_4() {
    /* Nonmatching */
    return FALSE;
}

/* 00005170-000052B4       .text talk_1__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::talk_1() {
    /* Nonmatching */
    return FALSE;
}

/* 000052B4-000053F4       .text wait_action1__11daNpc_Kk1_cFPv */
int daNpc_Kk1_c::wait_action1(void*) {
    /* Nonmatching */
    return 0;
}

/* 000053F4-00005534       .text demo__11daNpc_Kk1_cFv */
u8 daNpc_Kk1_c::demo() {
    /* Nonmatching */
    return 0;
}

/* 00005534-000055C4       .text shadowDraw__11daNpc_Kk1_cFv */
void daNpc_Kk1_c::shadowDraw() {
    cXyz pos(current.pos.x, 150.0f + current.pos.y, current.pos.z);
    mShadowId = dComIfGd_setShadow(mShadowId, 1, mpMorf->getModel(), &pos, 800.0f, 40.0f, current.pos.y, mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
}

/* 000055C4-00005798       .text _draw__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::_draw() {
    /* Nonmatching */
    return FALSE;
}

/* 00005798-000059EC       .text _execute__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::_execute() {
    /* Nonmatching */
    return FALSE;
}

/* 000059EC-00005A58       .text _delete__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::_delete() {
    cDyl_Unlink(fpcNm_TAMA_e);
    dComIfG_resDelete(&mPhs, mArcName);
    delBikon();
    delAse();
    if (heap != NULL && mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
    return TRUE;
}

/* 00005A58-00005B98       .text _create__11daNpc_Kk1_cFv */
cPhs_State daNpc_Kk1_c::_create() {
    /* Nonmatching */
    return cPhs_ERROR_e;
}

/* 00006118-0000638C       .text bodyCreateHeap__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::bodyCreateHeap() {
    /* Nonmatching */
    return FALSE;
}

/* 0000638C-000065E0       .text effcCreateHeap__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::effcCreateHeap() {
    /* Nonmatching */
    return FALSE;
}

/* 000065E0-00006684       .text CreateHeap__11daNpc_Kk1_cFv */
BOOL daNpc_Kk1_c::CreateHeap() {
    if (!bodyCreateHeap()) {
        return FALSE;
    }
    if (!effcCreateHeap()) {
        mpMorf = NULL;
        return FALSE;
    }
    mAcchCir.SetWall(30.0f, 40.0f);
    mObjAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, NULL, NULL);
    return TRUE;
}

/* 00006684-000066A4       .text daNpc_Kk1_Create__FP10fopAc_ac_c */
static cPhs_State daNpc_Kk1_Create(fopAc_ac_c* i_this) {
    return ((daNpc_Kk1_c*)i_this)->_create();
}

/* 000066A4-000066C4       .text daNpc_Kk1_Delete__FP11daNpc_Kk1_c */
static BOOL daNpc_Kk1_Delete(daNpc_Kk1_c* i_this) {
    return ((daNpc_Kk1_c*)i_this)->_delete();
}

/* 000066C4-000066E4       .text daNpc_Kk1_Execute__FP11daNpc_Kk1_c */
static BOOL daNpc_Kk1_Execute(daNpc_Kk1_c* i_this) {
    return ((daNpc_Kk1_c*)i_this)->_execute();
}

/* 000066E4-00006704       .text daNpc_Kk1_Draw__FP11daNpc_Kk1_c */
static BOOL daNpc_Kk1_Draw(daNpc_Kk1_c* i_this) {
    return ((daNpc_Kk1_c*)i_this)->_draw();
}

/* 00006704-0000670C       .text daNpc_Kk1_IsDelete__FP11daNpc_Kk1_c */
static BOOL daNpc_Kk1_IsDelete(daNpc_Kk1_c*) {
    return TRUE;
}

static actor_method_class l_daNpc_Kk1_Method = {
    (process_method_func)daNpc_Kk1_Create,
    (process_method_func)daNpc_Kk1_Delete,
    (process_method_func)daNpc_Kk1_Execute,
    (process_method_func)daNpc_Kk1_IsDelete,
    (process_method_func)daNpc_Kk1_Draw,
};

actor_process_profile_definition g_profile_NPC_KK1 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_KK1_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Kk1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_KK1_e,
    /* Actor SubMtd */ &l_daNpc_Kk1_Method,
    /* Status       */ 0x08 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
