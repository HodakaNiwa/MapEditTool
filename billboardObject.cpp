//*****************************************************************************
//
//     �z�u�p�r���{�[�h�̏���[billboardObject.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "billboardObject.h"
#include "manager.h"
#include "renderer.h"
#include "functionlib.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//*****************************************************************************
//    CBillboardObject�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBillboardObject::CBillboardObject(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBillboardObject::~CBillboardObject()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBillboardObject *CBillboardObject::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nPriority)
{
	CBillboardObject *pBillboardObject = NULL;      // �z�u�r���{�[�h�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();            // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pBillboardObject == NULL)
		{// ����������ɂȂ��Ă���
			pBillboardObject = new CBillboardObject(nPriority);
			if (pBillboardObject != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pBillboardObject->Init(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange)))
				{// �������Ɏ��s����
					return NULL;
				}
			}
			else
			{// �C���X�^���X�𐶐��ł��Ȃ�����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ����ȏ�V�[�����쐬�ł��Ȃ�
		return NULL;
	}

	return pBillboardObject;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBillboardObject::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange)
{
	// �e��l�̑��
	SetPos(pos);                    // �|���S���̍��W
	SetCol(col);                    // �|���S���̐F
	SetRot(fRot);                   // �|���S���̌���
	SetWidth(fWidth);               // �|���S���̕�
	SetHeight(fHeight);             // �|���S���̍���
	SetLighting(bLighting);         // ���C�e�B���O���邩���Ȃ���
	SetDrawAddtive(bDrawAddtive);   // ���Z�����ŕ`�悷�邩���Ȃ���
	SetTexIdx(nTexIdx);             // �g�p���Ă���e�N�X�`���̔ԍ�
	SetColRange(fColRange);         // �����蔻������͈�

	// ���ʂ̏���������
	if (FAILED(CSceneBillboard::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBillboardObject::Uninit(void)
{
	// ���ʂ̏I������
	CSceneBillboard::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBillboardObject::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBillboardObject::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			DWORD Lighting;

			if (m_bDrawAddtive == true)
			{// ���u�����f�B���O�����Z�����ɐݒ�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// ���C�e�B���O�̐ݒ�
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			if (m_bLighting == false)
			{// ���C�e�B���O���O��
				pDevice->SetRenderState(D3DRS_LIGHTING, false);
			}

			// ���ʂ̕`�揈��
			CSceneBillboard::Draw();

			// ���C�e�B���O��߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

			// ���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�v�Z����
//=============================================================================
void CBillboardObject::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxWorld, mtxView, mtxTrans; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �J�����̌������擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ���[���h�}�g���b�N�X�Ɍ�����ݒ�(�J�����̋t�s������邱�ƂŃJ�����̐��ʂɌ�����␳)
	mtxWorld._11 = mtxView._11;
	mtxWorld._12 = mtxView._21;
	mtxWorld._13 = mtxView._31;
	//mtxWorld._21 = mtxView._12;
	//mtxWorld._22 = mtxView._22;
	//mtxWorld._23 = mtxView._32;
	mtxWorld._31 = mtxView._13;
	mtxWorld._32 = mtxView._23;
	mtxWorld._33 = mtxView._33;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	CSceneBillboard::SetMtxWorld(mtxWorld);
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
bool CBillboardObject::Collision(D3DXVECTOR3 *pPos, float fColRange)
{
	bool bCol = false;       // �������Ă��邩�ǂ���
	float fHitRange = 0.0f;  // �������������Ԃ��͈�
	float fLength = 0.0f;    // �r���{�[�h�̍��W�ƈ����̍��W�Ƃ̋���

	// �����蔻�����钷�����v�Z
	fHitRange = sqrtf((fColRange + m_fColRange) * (fColRange + m_fColRange));

	// �r���{�[�h�ƈ����̍��W�Ƃ̋������v�Z
	fLength = sqrtf((GetPos().x - pPos->x) * (GetPos().x - pPos->x) + (GetPos().y - pPos->y) * (GetPos().y - pPos->y) + (GetPos().z - pPos->z) * (GetPos().z - pPos->z));

	// ����J�n
	if (fLength <= fHitRange)
	{// �r���{�[�h�̍��W�ƈ����̍��W�Ƃ̋����������蔻�����钷�����Z��
		bCol = true;   // ������������ɂ���
	}

	return bCol;
}

//=============================================================================
//    �g�p���Ă���e�N�X�`���̔ԍ��ݒ菈��
//=============================================================================
void CBillboardObject::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    ���C�e�B���O���邩���Ȃ����ݒ菈��
//=============================================================================
void CBillboardObject::SetLighting(const bool bLighting)
{
	m_bLighting = bLighting;
}

//=============================================================================
//    ���Z�����ŕ`�悷�邩���Ȃ����ݒ菈��
//=============================================================================
void CBillboardObject::SetDrawAddtive(const bool bDrawAddtive)
{
	m_bDrawAddtive = bDrawAddtive;
}

//=============================================================================
//    �����蔻������͈͐ݒ菈��
//=============================================================================
void CBillboardObject::SetColRange(const float fColRange)
{
	m_fColRange = fColRange;
}

//=============================================================================
//    �g�p���Ă���e�N�X�`���̔ԍ��擾����
//=============================================================================
int CBillboardObject::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    ���C�e�B���O���邩���Ȃ����擾����
//=============================================================================
bool CBillboardObject::GetLighting(void)
{
	return m_bLighting;
}

//=============================================================================
//    ���Z�����ŕ`�悷�邩���Ȃ����擾����
//=============================================================================
bool CBillboardObject::GetDrawAddtive(void)
{
	return m_bDrawAddtive;
}

//=============================================================================
//    �����蔻������͈͎擾����
//=============================================================================
float CBillboardObject::GetColRange(void)
{
	return m_fColRange;
}

//*****************************************************************************
//    CSetBillboardObject�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CSetBillboardObject::CSetBillboardObject(int nPriority, OBJTYPE objType) : CBillboardObject(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CSetBillboardObject::~CSetBillboardObject()
{

}

//=============================================================================
//    ��������
//=============================================================================
CSetBillboardObject *CSetBillboardObject::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nPriority)
{
	CSetBillboardObject *pSetBillboardObject = NULL;   // �z�u�p�̔z�u�r���{�[�h�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();               // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pSetBillboardObject == NULL)
		{// ����������ɂȂ��Ă���
			pSetBillboardObject = new CSetBillboardObject(nPriority);
			if (pSetBillboardObject != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pSetBillboardObject->Init(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange)))
				{// �������Ɏ��s����
					return NULL;
				}
			}
			else
			{// �C���X�^���X�𐶐��ł��Ȃ�����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ����ȏ�V�[�����쐬�ł��Ȃ�
		return NULL;
	}

	return pSetBillboardObject;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CSetBillboardObject::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange)
{
	// �����x���グ�Ă���
	col.a = 0.5f;

	// ���ʂ̏���������
	if (FAILED(CBillboardObject::Init(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CSetBillboardObject::Uninit(void)
{
	// ���ʂ̏I������
	CBillboardObject::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CSetBillboardObject::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CSetBillboardObject::Draw(void)
{
	// ���ʂ̕`�揈��
	CBillboardObject::Draw();
}

//=============================================================================
//    ���_�������������鏈��
//=============================================================================
void CSetBillboardObject::ChangeVertex(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �I�t�Z�b�g�̒��������߂�
		float fLength = sqrtf((GetWidth() * GetWidth()) + (GetHeight() * GetHeight()));
		SetLength(fLength);

		// �I�t�Z�b�g�̊p�x�����߂�
		float fAngle = atan2f(GetWidth(), GetHeight());
		SetAngle(fAngle);

		// ���_�̉�]���l�����č��W���v�Z
		float XPos[4];
		float YPos[4];
		XPos[0] = CFunctionLib::RotationVectorX(GetRot(), -sinf(GetAngle()) * GetLength(), cosf(GetAngle()) * GetLength());
		YPos[0] = CFunctionLib::RotationVectorY(GetRot(), -sinf(GetAngle()) * GetLength(), cosf(GetAngle()) * GetLength());
		XPos[1] = CFunctionLib::RotationVectorX(GetRot(), sinf(GetAngle()) * GetLength(), cosf(GetAngle()) * GetLength());
		YPos[1] = CFunctionLib::RotationVectorY(GetRot(), sinf(GetAngle()) * GetLength(), cosf(GetAngle()) * GetLength());
		XPos[2] = CFunctionLib::RotationVectorX(GetRot(), -sinf(GetAngle()) * GetLength(), -cosf(GetAngle()) * GetLength());
		YPos[2] = CFunctionLib::RotationVectorY(GetRot(), -sinf(GetAngle()) * GetLength(), -cosf(GetAngle()) * GetLength());
		XPos[3] = CFunctionLib::RotationVectorX(GetRot(), sinf(GetAngle()) * GetLength(), -cosf(GetAngle()) * GetLength());
		YPos[3] = CFunctionLib::RotationVectorY(GetRot(), sinf(GetAngle()) * GetLength(), -cosf(GetAngle()) * GetLength());

		// ���_���W
		for (int nCntVer = 0; nCntVer < 4; nCntVer++)
		{// ���_�̐������J��Ԃ�
			pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f);
		}

		// ���_�J���[
		pVtx[0].col = D3DXCOLOR(GetCol().r, GetCol().g, GetCol().b, 0.7f);
		pVtx[1].col = D3DXCOLOR(GetCol().r, GetCol().g, GetCol().b, 0.7f);
		pVtx[2].col = D3DXCOLOR(GetCol().r, GetCol().g, GetCol().b, 0.7f);
		pVtx[3].col = D3DXCOLOR(GetCol().r, GetCol().g, GetCol().b, 0.7f);

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}