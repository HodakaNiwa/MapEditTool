//*****************************************************************************
//
//     ���b�V���E�H�[���̏���[meshWall.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    ���b�V���E�H�[���N���X�̒�`
//*****************************************************************************
class CMeshWall : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CMeshWall(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHWALL);
	~CMeshWall();

	static CMeshWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetTexIdx(const int nTexIdx);
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);
	void SetTexSplitU(const int nTexSplitU);
	void SetTexSplitV(const int nTexSplitV);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	int GetTexIdx(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetTexSplitU(void);
	int GetTexSplitV(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);
	D3DXMATRIX GetMtxWorld(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int                     m_nTexIdx;                // �e�N�X�`���ԍ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;               // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;               // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;	              // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3             m_Pos;                    // ���b�V���E�H�[���̍��W
	D3DXVECTOR3             m_Rot;                    // ���b�V���E�H�[���̌���
	D3DXCOLOR               m_Col;                    // ���b�V���E�H�[���̐F
	float                   m_fWidth;                 // 1�u���b�N���̕�
	float                   m_fHeight;                // 1�u���b�N������
	int                     m_nXBlock;                // ���̕�����
	int                     m_nYBlock;                // �c�̕�����
	int                     m_nTexSplitU;             // �e�N�X�`��U���W�̕�����
	int                     m_nTexSplitV;             // �e�N�X�`��V���W�̕�����
	int                     m_nNumVertex;             // ���_��
	int                     m_nNumIndex;              // �C���f�b�N�X��
	int                     m_nNumPolygon;            // �|���S����
	D3DXMATRIX              m_MtxWorld;               // ���[���h�}�g���b�N�X
};

//*****************************************************************************
//    �z�u�p���b�V���E�H�[���N���X�̒�`
//*****************************************************************************
class CSetMeshWall : public CMeshWall
{
public:    // �N�ł��A�N�Z�X�\
	CSetMeshWall(int nPriority = 3, OBJTYPE objType = OBJTYPE_SETMESHWALL);
	~CSetMeshWall();

	static CSetMeshWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
};

#endif