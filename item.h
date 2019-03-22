//*****************************************************************************
//
//     �A�C�e���̏���[item.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _ITEM_H_
#define _ITEM_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "billboardObject.h"

//*****************************************************************************
//    �A�C�e���N���X�̒�`
//*****************************************************************************
class CItem : public CBillboardObject
{
public:    // �N�ł��A�N�Z�X�\
		   //--------------------
		   //  ���
		   //--------------------
	typedef enum
	{
		TYPE_NORMAL = 0,  // �ʏ�
		TYPE_HIDDEN,      // �B���A�C�e��
		TYPE_MAX,
	}TYPE;

	CItem(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEM);
	~CItem();

	static CItem *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f, int nType = 0, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f, int nType = 0);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetType(const int nType);
	int GetType(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int   m_nType;      // �A�C�e���̎�ޔԍ�
};

//*****************************************************************************
//    �z�u��p�̃A�C�e���N���X�̒�`
//*****************************************************************************
class CSetItem : public CItem
{
public:    // �N�ł��A�N�Z�X�\
	CSetItem(int nPriority = 3, OBJTYPE objType = OBJTYPE_SETITEM);
	~CSetItem();

	static CSetItem *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f, int nType = 0, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f, int nType = 0);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ChangeVertex(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
};

#endif