//*****************************************************************************
//
//     �A�C�e���̏���[item.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "item.h"
#include "functionlib.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//*****************************************************************************
//    CItem�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CItem::CItem(int nPriority, OBJTYPE objType) : CBillboardObject(nPriority, objType)
{
	// �e��l�̃N���A
	m_nType = 0;     // �A�C�e���̎�ޔԍ�
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CItem::~CItem()
{

}

//=============================================================================
//    ��������
//=============================================================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType, int nPriority)
{
	CItem *pItem = NULL;                   // �A�C�e���N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pItem == NULL)
		{// ����������ɂȂ��Ă���
			pItem = new CItem(nPriority);
			if (pItem != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pItem->Init(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange, nType)))
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

	return pItem;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType)
{
	// �e��l�̑��
	m_nType = nType;    // �A�C�e���̎�ޔԍ�

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
void CItem::Uninit(void)
{
	// ���ʂ̏I������
	CBillboardObject::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CItem::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CItem::Draw(void)
{
	// ���ʂ̕`�揈��
	CBillboardObject::Draw();
}

//=============================================================================
//    ��ޔԍ���ݒ肷�鏈��
//=============================================================================
void CItem::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    ��ޔԍ����擾���鏈��
//=============================================================================
int CItem::GetType(void)
{
	return m_nType;
}


//*****************************************************************************
//    CSetItem�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CSetItem::CSetItem(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CSetItem::~CSetItem()
{

}

//=============================================================================
//    ��������
//=============================================================================
CSetItem *CSetItem::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType, int nPriority)
{
	CSetItem *pSetItem = NULL;             // �z�u�p�̃A�C�e���N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pSetItem == NULL)
		{// ����������ɂȂ��Ă���
			pSetItem = new CSetItem(nPriority);
			if (pSetItem != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pSetItem->Init(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange, nType)))
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

	return pSetItem;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CSetItem::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType)
{
	// �����x���グ�Ă���
	col.a = 0.5f;

	// ���ʂ̏���������
	if (FAILED(CItem::Init(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange, nType)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CSetItem::Uninit(void)
{
	// ���ʂ̏I������
	CItem::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CSetItem::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CSetItem::Draw(void)
{
	// ���ʂ̕`�揈��
	CItem::Draw();
}

//=============================================================================
//    ���_�������������鏈��
//=============================================================================
void CSetItem::ChangeVertex(void)
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