//*****************************************************************************
//
//     敵の処理[enemy.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "character.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define MAX_ENEMY_TYPE  (10)    // 読み込める敵の種類の数

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CMotion;
class CGaugeLife;

//*****************************************************************************
//    エネミークラスの定義
//*****************************************************************************
class CEnemy : public CCharacter
{
public:    // 誰でもアクセス可能
	//----------------------
	// 種類
	//----------------------
	typedef enum
	{
		TYPE_NORMAL = 0,
		TYPE_NORMAL2,
		TYPE_MAX
	}TYPE;

	//----------------------
	// 状態
	//----------------------
	typedef enum
	{
		STATE_NORMAL = 0,   // 通常状態
		STATE_MOVE,         // 移動状態
		STATE_ACTION,       // アクション状態
		STATE_DAMAGE,       // ダメージ状態
		STATE_DEATH,        // 死亡状態
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

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void Collision(void);
	void Statement(void);
	void Death(void);

	STATE       m_State;       // 状態
	TYPE        m_Type;        // 種類
	D3DXVECTOR3 m_Move;        // 移動量
	int         m_nLife;       // 体力
	int         m_nAddScore;   // 倒した時に増えるスコア量
	CGaugeLife  *m_pGaugeLife; // 体力ゲージ
	D3DXVECTOR3 m_PosStd;      // モデルの座標(生成時)
	D3DXVECTOR3 m_RotStd;      // モデルの向き(生成時)
};

//*****************************************************************************
//    配置用エネミークラスの定義
//*****************************************************************************
class CSetEnemy : public CEnemy
{
public:    // 誰でもアクセス可能
	CSetEnemy(int nPriority = 3, OBJTYPE objType = OBJTYPE_SETENEMY);
	~CSetEnemy();

	static CSetEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife, int nAddScore = 1000, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife, int nAddScore = 1000);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
};

//*****************************************************************************
//    エネミー管轄クラスの定義
//*****************************************************************************
class CEnemyManager : CScene
{
public:    // 誰でもアクセス可能
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

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	CEnemy *CreateEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nType, const int nLife, const int nAddScore = 1000);
	CSetEnemy *CreateSetEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nType, const int nLife, const int nAddScore = 1000);
	CModel **ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pGravity, float *pColHeight, float *pColRange);
	CModel *ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel);
	CMotion *ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion);
	void ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	void ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);

	char           m_aFileName[256];    // エネミーデータのスクリプトファイル名
	CModelManager  *m_pModelManager;    // モデル管轄クラスへのポインタ

	int            m_nNumModel;         // 読み込むモデル数
	int            m_nType;             // 読み込んだ敵の種類
	int            m_nNumParts;         // 読み込むパーツ数
	int            *m_pNumParent;       // 親モデルの番号
	CMotionManager *m_pMotionManager;   // モーション管理クラスへのポインタ
	CModel         **m_apModel;         // モデルクラスへのポインタ
	float          m_fGravity;          // 敵の重力情報
	float          m_fColHeight;        // 当たり判定を取る高さ
	float          m_fColRange;         // 当たり判定を取る範囲
};

//*****************************************************************************
//    エネミー管轄まとめクラスの定義
//*****************************************************************************
class CEnemyAssembly
{
public:    // 誰でもアクセス可能
	CEnemyAssembly();
	~CEnemyAssembly();

	void SetEnemyManager(CEnemyManager *pEnemyManager, int nIdx);
	CEnemyManager *GetEnemyManager(int nIdx);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	CEnemyManager *m_pEnemyManager[MAX_ENEMY_TYPE];   // 敵管轄クラスへのポインタ
};

#endif