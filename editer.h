//*****************************************************************************
//
//     �G�f�B�^�[�̏���[editer.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _EDITER_H_
#define _EDITER_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
//     �O���錾
//*****************************************************************************
class CMap;
class CSetBillboardObject;
class CEmitter;
class CSetEnemy;
class CSetMeshWall;
class CSetItem;
class CModelManager;
class CTextureManager;
class CEffectManager;
class CEnemyAssembly;

//*****************************************************************************
//     �G�f�B�^�[�N���X�̒�`
//*****************************************************************************
class CEditer
{
public:     // �N�ł��A�N�Z�X�\
	//----------------
	// �G�t�F�N�g�̏��
	//----------------
	typedef enum
	{
		EFFECTSTATE_PLAY = 0,   // �Đ����
		EFFECTSTATE_STOP,       // �I�����
		EFFECTSTATE_MAX
	}EFFECTSTATE;

	//---------------------
	//  �z�u���̎��
	//---------------------
	typedef enum
	{
		TYPE_OBJECT = 0,
		TYPE_OBJECTMOVE,
		TYPE_BILLBOARD,
		TYPE_EFFECT,
		TYPE_ENEMY,
		TYPE_WALL,
		TYPE_ITEM,
		TYPE_MAX
	}TYPE;

	CEditer();
	~CEditer();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void ModelChange(void);
	void ObjectChange(TYPE typeOld);
	void EmitterChange(void);
	void EnemyChange(void);

	// ����
	void SetType(const TYPE Type);
	void SetEffectState(const EFFECTSTATE state);
	void SetPos(const D3DXVECTOR3 SetPos);
	void SetRot(const D3DXVECTOR3 SetRot);

	// �z�u���f���p
	void SetModelColType(const CObject::COL_TYPE ModelColType);
	void SetModelScale(const D3DXVECTOR3 ModelScale);
	void SetModelRollSpeed(const D3DXVECTOR3 ModelRollSpeed);
	void SetModelColRange(const float fModelColRange);
	void SetModelMove(const D3DXVECTOR3 ModelMove);
	void SetModelMaxMoveCounter(const int nModelMaxMoveCounter);
	void SetNumModel(const int nNumModel);
	void SetSelectModelIdx(const int nSelectModelIdx);

	// �r���{�[�h�p
	void SetBillCol(const D3DXCOLOR BillCol);
	void SetBillColRange(const float fBillColRange);
	void SetBillRot(const float fBillRot);
	void SetBillWidth(const float fBillWidth);
	void SetBillHeight(const float fBillHeight);
	void SetBillTexIdx(const int nTexIdx);
	void SetBillLighting(const bool bLighting);
	void SetBillDrawAddtive(const bool bDrawAddtive);

	// �G�t�F�N�g�p
	void SetEffectType(const int nEffectType);

	// �G�p
	void SetEnemyType(const int nEnemyType);
	void SetEnemyLife(const int nEnemyLife);
	void SetEnemyScore(const int nEnemyScore);

	// �Ǘp
	void SetWallTexIdx(const int nWallTexIdx);
	void SetWallCol(const D3DXCOLOR WallCol);
	void SetWallWidth(const float fWidth);
	void SetWallHeight(const float fHeight);
	void SetWallXBlock(const int nXBlock);
	void SetWallYBlock(const int nYBlock);
	void SetWallTexSplitU(const int nWallTexSplitU);
	void SetWallTexSplitV(const int nWallTexSplitV);

	// �A�C�e���p
	void SetItemCol(const D3DXCOLOR ItemCol);
	void SetItemColRange(const float fItemColRange);
	void SetItemRot(const float fItemRot);
	void SetItemWidth(const float fItemWidth);
	void SetItemHeight(const float fItemHeight);
	void SetItemTexIdx(const int nTexIdx);
	void SetItemType(const int nType);
	void SetItemLighting(const bool bLighting);
	void SetItemDrawAddtive(const bool bDrawAddtive);


	// ����
	EFFECTSTATE GetEffectState(void);
	CSetObject *GetSetObject(void);
	CSetObjectMove *GetSetObjectMove(void);
	CSetBillboardObject *GetSetBillObj(void);
	CEmitter *GetSetEmitter(void);
	CSetEnemy *GetEnemy(void);
	CSetMeshWall *GetSetMeshWall(void);
	CSetItem *GetSetItem(void);
	CTextureManager *GetTextureManager(void);
	CEffectManager *GetEffectManager(void);
	CEnemyAssembly *GetEnemyAssembly(void);
	CMap *GetMap(void);
	TYPE GetType(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);

	// �z�u���f���p
	CObject::COL_TYPE GetModelColType(void);
	D3DXVECTOR3 GetModelScale(void);
	D3DXVECTOR3 GetModelRollSpeed(void);
	float GetModelColRange(void);
	D3DXVECTOR3 GetModelMove(void);
	int GetModelMaxMoveCounter(void);
	int GetNumModel(void);
	int GetSelectModelIdx(void);

	// �r���{�[�h�p
	D3DXCOLOR GetBillCol(void);
	float GetBillColRange(void);
	float GetBillRot(void);
	float GetBillWidth(void);
	float GetBillHeight(void);
	int GetBillTexIdx(void);
	bool GetBillLighting(void);
	bool GetBillDrawAddtive(void);
	int GetNumTex(void);

	// �G�t�F�N�g�p
	int GetEffectType(void);

	// �G�p
	int GetEnemyType(void);
	int GetEnemyLife(void);
	int GetEnemyScore(void);

	// �Ǘp
	int GetWallTexIdx(void);
	D3DXCOLOR GetWallCol(void);
	float GetWallWidth(void);
	float GetWallHeight(void);
	int GetWallXBlock(void);
	int GetWallYBlock(void);
	int GetWallTexSplitU(void);
	int GetWallTexSplitV(void);

	// �A�C�e���p
	D3DXCOLOR GetItemCol(void);
	float GetItemColRange(void);
	float GetItemRot(void);
	float GetItemWidth(void);
	float GetItemHeight(void);
	int GetItemTexIdx(void);
	int GetItemType(void);
	bool GetItemLighting(void);
	bool GetItemDrawAddtive(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	void ObjectMove(void);

	// ����
	CMap                *m_pMap;                 // �}�b�v�N���X�ւ̃|�C���^
	CSetObject          *m_pSetObject;           // �z�u���ւ̃|�C���^
	CSetObjectMove      *m_pSetObjectMove;       // �����z�u���ւ̃|�C���^
	CSetBillboardObject *m_pSetBillboardObj;     // �z�u�p�r���{�[�h�ւ̃|�C���^
	CEmitter            *m_pSetEmitter;          // �z�u�p�G�~�b�^�ւ̃|�C���^
	CSetEnemy           *m_pSetEnemy;            // �z�u�p�G�N���X�ւ̃|�C���^
	CSetMeshWall        *m_pSetWall;             // �z�u�p�ǃN���X�ւ̃|�C���^
	CSetItem            *m_pSetItem;             // �z�u�p�A�C�e���N���X�ւ̃|�C���^
	CModelManager       *m_pModelManager;        // ���f���Ǌ��N���X�ւ̃|�C���^
	CTextureManager     *m_pTextureManager;      // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	CEffectManager      *m_pEffectManager;       // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	CEnemyAssembly      *m_pEnemyAssmbly;        // �G�Ǌ��܂Ƃ߃N���X�ւ̃|�C���^
	TYPE                m_Type;                  // ���ݔz�u�ł���z�u���̎��
	EFFECTSTATE         m_EffectState;           // ���ݔz�u�ł���G�~�b�^�̍Đ���
	D3DXVECTOR3         m_SetPos;                // �z�u�ł���I�u�W�F�N�g�̍��W
	D3DXVECTOR3         m_SetRot;                // �z�u�ł���I�u�W�F�N�g�̌���
	int                 m_nCounter;              // ��Ԃ��Ǘ�����J�E���^�[


	// �z�u���f���p
	CObject::COL_TYPE   m_ModelColType;          // �z�u�ł��郂�f���̓����蔻��̎��
	D3DXVECTOR3         m_ModelScale;            // �z�u�ł��郂�f���̑傫���̔{��
	D3DXVECTOR3         m_ModelRollSpeed;        // �z�u�ł��郂�f������]����X�s�[�h
	float               m_fModelColRange;        // �z�u�ł��郂�f���̓����蔻������͈�(�~�`�œ����蔻������ۂɎg�p)
	D3DXVECTOR3         m_ModelMove;             // �z�u�ł��郂�f���̈ړ���
	int                 m_nModelMaxMoveCounter;  // �z�u�ł��郂�f���̈ړ��ʂ�؂�ւ���^�C�~���O
	int                 m_nNumModel;             // �z�u�ł��郂�f���̎�ނ̐�
	int                 m_nSelectModelIdx;       // ���ݔz�u�ł��郂�f���̔ԍ�

	// �r���{�[�h�p
	D3DXCOLOR           m_BillCol;               // �z�u�ł���r���{�[�h�̐F
	float               m_fBillColRange;         // �z�u�ł���r���{�[�h�̓����蔻������͈�
	float               m_fBillRot;              // �z�u�ł���r���{�[�h�̌���
	float               m_fBillWidth;            // �z�u�ł���r���{�[�h�̕�
	float               m_fBillHeight;           // �z�u�ł���r���{�[�h�̍���
	int                 m_nBillTexIdx;           // �z�u�ł���r���{�[�h�̃e�N�X�`���̔ԍ�
	bool                m_bBillLighting;         // �z�u�ł���r���{�[�h�̃��C�e�B���O���邩���Ȃ���
	bool                m_bBillDrawAddtive;      // �z�u�ł���r���{�[�h�̉��Z�����ŕ`�悷�邩���Ȃ���
	int                 m_nNumTex;               // �ݒ�ł���e�N�X�`���̎�ނ̐�

	// �G�t�F�N�g�p
	int                 m_nEffectType;           // �z�u�ł���G�t�F�N�g�̎�ޔԍ�

	// �G�p
	int                 m_nEnemyType;            // �z�u�ł���G�̎��
	int                 m_nEnemyLife;            // �z�u�ł���G�̗̑�
	int                 m_nEnemyScore;           // �z�u�ł���G�̃X�R�A

	// �Ǘp
	int                 m_nWallTexIdx;           // �z�u�ł���ǂ̃e�N�X�`���ԍ�
	D3DXCOLOR           m_WallCol;               // �z�u�ł���ǂ̐F
	float               m_fWallWidth;            // �z�u�ł���ǂ̕�
	float               m_fWallHeight;           // �z�u�ł���ǂ̍���
	int                 m_nWallXBlock;           // �z�u�ł���ǂ̉��̕�����
	int                 m_nWallYBlock;           // �z�u�ł���ǂ̏c�̕�����
	int                 m_nWallTexSplitU;        // �z�u�ł���ǂ̃e�N�X�`��U���W�̕�����
	int                 m_nWallTexSplitV;        // �z�u�ł���ǂ̃e�N�X�`��V���W�̕�����

	// �A�C�e���p
	D3DXCOLOR           m_ItemCol;               // �z�u�ł���A�C�e���̐F
	float               m_fItemColRange;         // �z�u�ł���A�C�e���̓����蔻������͈�
	float               m_fItemRot;              // �z�u�ł���A�C�e���̌���
	float               m_fItemWidth;            // �z�u�ł���A�C�e���̕�
	float               m_fItemHeight;           // �z�u�ł���A�C�e���̍���
	int                 m_nItemTexIdx;           // �z�u�ł���A�C�e���̃e�N�X�`���̔ԍ�
	int                 m_nItemType;             // �z�u�ł���A�C�e���̎�ޔԍ�
	bool                m_bItemLighting;         // �z�u�ł���A�C�e���̃��C�e�B���O���邩���Ȃ���
	bool                m_bItemDrawAddtive;      // �z�u�ł���A�C�e���̉��Z�����ŕ`�悷�邩���Ȃ���
};

#endif