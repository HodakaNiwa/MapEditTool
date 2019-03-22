//*****************************************************************************
//
//     �G�̏���[enemy.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "character.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define MAX_ENEMY_TYPE  (10)    // �ǂݍ��߂�G�̎�ނ̐�

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CMotion;
class CGaugeLife;

//*****************************************************************************
//    �G�l�~�[�N���X�̒�`
//*****************************************************************************
class CEnemy : public CCharacter
{
public:    // �N�ł��A�N�Z�X�\
	//----------------------
	// ���
	//----------------------
	typedef enum
	{
		TYPE_NORMAL = 0,
		TYPE_NORMAL2,
		TYPE_MAX
	}TYPE;

	//----------------------
	// ���
	//----------------------
	typedef enum
	{
		STATE_NORMAL = 0,   // �ʏ���
		STATE_MOVE,         // �ړ����
		STATE_ACTION,       // �A�N�V�������
		STATE_DAMAGE,       // �_���[�W���
		STATE_DEATH,        // ���S���
		STATE_MAX
	}STATE;

	CEnemy(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CEnemy();

	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife, int nAddScore = 1000, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife,int nAddScore = 1000);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(int nDamage);

	void SetType(const TYPE type);
	void SetState(const STATE State);
	void SetLife(const int nLife);
	void SetAddScore(const int nAddScore);

	TYPE GetType(void);
	STATE GetState(void);
	int GetLife(void);
	int GetAddScore(void);
	CGaugeLife *GetGaugeLife(void);
	D3DXVECTOR3 GetPosStd(void);
	D3DXVECTOR3 GetRotStd(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Collision(void);
	void Statement(void);
	void Death(void);

	STATE       m_State;       // ���
	TYPE        m_Type;        // ���
	D3DXVECTOR3 m_Move;        // �ړ���
	int         m_nLife;       // �̗�
	int         m_nAddScore;   // �|�������ɑ�����X�R�A��
	CGaugeLife  *m_pGaugeLife; // �̗̓Q�[�W
	D3DXVECTOR3 m_PosStd;      // ���f���̍��W(������)
	D3DXVECTOR3 m_RotStd;      // ���f���̌���(������)
};

//*****************************************************************************
//    �z�u�p�G�l�~�[�N���X�̒�`
//*****************************************************************************
class CSetEnemy : public CEnemy
{
public:    // �N�ł��A�N�Z�X�\
	CSetEnemy(int nPriority = 3, OBJTYPE objType = OBJTYPE_SETENEMY);
	~CSetEnemy();

	static CSetEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife, int nAddScore = 1000, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife, int nAddScore = 1000);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
};

//*****************************************************************************
//    �G�l�~�[�Ǌ��N���X�̒�`
//*****************************************************************************
class CEnemyManager : CScene
{
public:    // �N�ł��A�N�Z�X�\
	CEnemyManager(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMYMANAGER);
	~CEnemyManager();

	static CEnemyManager *Create(int nType, char *pFileName, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nLife, int nAddScore);
	CSetEnemy *SetMapEditEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nLife, int nAddScore);

	void SetFileName(char *pFileName);
	void SetType(const int nType);
	void SetModelManager(CModelManager *pModelManager);

	char *GetFileName(void);
	int GetType(void);
	CModelManager *GetModelManager(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	CEnemy *CreateEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nType, const int nLife, const int nAddScore = 1000);
	CSetEnemy *CreateSetEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nType, const int nLife, const int nAddScore = 1000);
	CModel **ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pGravity, float *pColHeight, float *pColRange);
	CModel *ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel);
	CMotion *ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion);
	void ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	void ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);

	char           m_aFileName[256];    // �G�l�~�[�f�[�^�̃X�N���v�g�t�@�C����
	CModelManager  *m_pModelManager;    // ���f���Ǌ��N���X�ւ̃|�C���^

	int            m_nNumModel;         // �ǂݍ��ރ��f����
	int            m_nType;             // �ǂݍ��񂾓G�̎��
	int            m_nNumParts;         // �ǂݍ��ރp�[�c��
	int            *m_pNumParent;       // �e���f���̔ԍ�
	CMotionManager *m_pMotionManager;   // ���[�V�����Ǘ��N���X�ւ̃|�C���^
	CModel         **m_apModel;         // ���f���N���X�ւ̃|�C���^
	float          m_fGravity;          // �G�̏d�͏��
	float          m_fColHeight;        // �����蔻�����鍂��
	float          m_fColRange;         // �����蔻������͈�
};

//*****************************************************************************
//    �G�l�~�[�Ǌ��܂Ƃ߃N���X�̒�`
//*****************************************************************************
class CEnemyAssembly
{
public:    // �N�ł��A�N�Z�X�\
	CEnemyAssembly();
	~CEnemyAssembly();

	void SetEnemyManager(CEnemyManager *pEnemyManager, int nIdx);
	CEnemyManager *GetEnemyManager(int nIdx);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	CEnemyManager *m_pEnemyManager[MAX_ENEMY_TYPE];   // �G�Ǌ��N���X�ւ̃|�C���^
};

#endif