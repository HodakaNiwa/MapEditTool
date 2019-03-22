//*****************************************************************************
//
//     �z�u���̏���[object.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "sceneX.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CGoalCylinder;

//*****************************************************************************
//    �I�u�W�F�N�g�N���X�̒�`
//*****************************************************************************
class CObject : public CSceneX
{
public:    // �N�ł��A�N�Z�X�\
	//------------------------
	//  �R���W�����̎��
	//------------------------
	typedef enum
	{
		COL_TYPE_SQUARE = 0,  // �l�p�Ŕ���
		COL_TYPE_CIRCLE,      // �~�`�Ŕ���
		COL_TYPE_NONE,        // ���肵�Ȃ�
		COL_TYPE_MAX
	}COL_TYPE;

	CObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECT);
	~CObject();

	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, COL_TYPE Collision = COL_TYPE_SQUARE, float fColRange = 0.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, COL_TYPE Collision = COL_TYPE_SQUARE, float fColRange = 0.0f);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject);
	void SetTransParent(float fAlpha);

	void SetCollision(const COL_TYPE Collision);
	void SetRollSpeed(const D3DXVECTOR3 RollSpeed);
	void SetPosOld(const D3DXVECTOR3 PosOld);
	void SetModelIdx(const int ModelIdx) { m_ModelIdx = ModelIdx; };
	void SetRotStd(const D3DXVECTOR3 RotStd);
	void SetColRange(const float fColRange);

	COL_TYPE GetCollision(void);
	int GetModelIdx(void) { return m_ModelIdx; };
	D3DXVECTOR3 GetRollSpeed(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetRotStd(void);
	float GetColRange(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	bool SquareCol(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject);
	bool CircleCol(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject);

	COL_TYPE    m_Collision;    // �R���W�����̎��
	int         m_ModelIdx;     // �g�p���Ă��郂�f���̔ԍ�
	D3DXVECTOR3 m_PosOld;       // �O��̍��W
	D3DXVECTOR3 m_RollSpeed;    // ��������]������X�s�[�h
	D3DXVECTOR3 m_PosStd;       // ���f���̍��W(������)
	D3DXVECTOR3 m_RotStd;       // ���f���̌���(������)
	float       m_fColRange;    // �����蔻��̎��(�~�`�̏ꍇ�Ɏg�p)
};

//*****************************************************************************
//    �����I�u�W�F�N�g�N���X�̒�`
//*****************************************************************************
class CObjectMove : public CObject
{
public:    // �N�ł��A�N�Z�X�\
	CObjectMove(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTMOVE);
	~CObjectMove();

	static CObjectMove *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, D3DXVECTOR3 Move, int nMaxMoveCounter, COL_TYPE Collision = COL_TYPE_SQUARE, float fColRange = 0.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, D3DXVECTOR3 Move, int nMaxMoveCounter, COL_TYPE Collision = COL_TYPE_SQUARE, float fColRange = 0.0f);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMove(const D3DXVECTOR3 Move);
	void SetMoveCurrent(const D3DXVECTOR3 MoveCurrent);
	void SetCounter(const int nCounter);
	void SetMaxMoveCounter(const int nMaxMoveCounter);
	void SetRivisionMove(const float fRivisionMove);
	void SetPosStd(const D3DXVECTOR3 PosStd);

	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetMoveCurrent(void);
	int GetCounter(void);
	int GetMaxMoveCounter(void);
	float GetRivisionMove(void);
	D3DXVECTOR3 GetPosStd(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXVECTOR3 m_Move;             // �ړ���
	D3DXVECTOR3 m_MoveCurrent;      // ���݂̈ړ���
	int         m_nCounter;         // �ړ��ʂ��Ǘ�����J�E���^�[
	int         m_nMaxMoveCounter;  // �ړ��ʂ��ő�l�ɂȂ鎞��
	float       m_fRivisionMove;    // �ړ��̔{��
	D3DXVECTOR3 m_PosStd;           // ���f���̍��W(������)
};

//*****************************************************************************
//    �S�[���p�I�u�W�F�N�g�N���X�̒�`
//*****************************************************************************
class CObjectGoal : public CObject
{
public:    // �N�ł��A�N�Z�X�\
	CObjectGoal(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTGOAL);
	~CObjectGoal();

	static CObjectGoal *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	CGoalCylinder *m_pGoalCylinder;    // �S�[���p�~���N���X�ւ̃|�C���^
};

//*****************************************************************************
//    �z�u����I�u�W�F�N�g�N���X�̒�`
//*****************************************************************************
class CSetObject : public CObject
{
public:    // �N�ł��A�N�Z�X�\
	CSetObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_SETOBJECT);
	~CSetObject();

	static CSetObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, COL_TYPE Collision = COL_TYPE_SQUARE, float fColRange = 0.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, COL_TYPE Collision = COL_TYPE_SQUARE, float fColRange = 0.0f);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetTransParent(float fAlpha);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
};


//*****************************************************************************
//    �z�u���铮���I�u�W�F�N�g�N���X�̒�`
//*****************************************************************************
class CSetObjectMove : public CObjectMove
{
public:    // �N�ł��A�N�Z�X�\
	CSetObjectMove(int nPriority = 3, OBJTYPE objType = OBJTYPE_SETOBJECT);
	~CSetObjectMove();

	static CSetObjectMove *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, D3DXVECTOR3 Move, int nMaxMoveCounter, COL_TYPE Collision = COL_TYPE_SQUARE, float fColRange = 0.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, D3DXVECTOR3 Move, int nMaxMoveCounter, COL_TYPE Collision = COL_TYPE_SQUARE, float fColRange = 0.0f);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetTransParent(float fAlpha);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
};

#endif