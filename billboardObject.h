//*****************************************************************************
//
//     �z�u�r���{�[�h�̏���[billboardObject.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _BILLBOARDOBJECT_H_
#define _BILLBOARDOBJECT_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "sceneBillboard.h"

//*****************************************************************************
//    �z�u�r���{�[�h�N���X�̒�`
//*****************************************************************************
class CBillboardObject : public CSceneBillboard
{
public:    // �N�ł��A�N�Z�X�\
	CBillboardObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_BILLBOARDOBJ);
	~CBillboardObject();

	static CBillboardObject *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	bool Collision(D3DXVECTOR3 *pPos, float fColRange);

	void SetTexIdx(const int nTexIdx);
	void SetLighting(const bool bLighting);
	void SetDrawAddtive(const bool bDrawAddtive);
	void SetColRange(const float fColRange);

	int GetTexIdx(void);
	bool GetLighting(void);
	bool GetDrawAddtive(void);
	float GetColRange(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int   m_nTexIdx;       // �g�p���Ă���e�N�X�`���̔ԍ�
	bool  m_bLighting;     // ���C�e�B���O�����邩�ǂ���
	bool  m_bDrawAddtive;  // ���Z�����ŕ`�悷�邩���Ȃ���
	float m_fColRange;     // �����蔻������͈�
};

//*****************************************************************************
//    �z�u��p�̔z�u�r���{�[�h�N���X�̒�`
//*****************************************************************************
class CSetBillboardObject : public CBillboardObject
{
public:    // �N�ł��A�N�Z�X�\
	CSetBillboardObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_BILLBOARD);
	~CSetBillboardObject();

	static CSetBillboardObject *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ChangeVertex(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
};

#endif