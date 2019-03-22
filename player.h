//*****************************************************************************
//
//     プレイヤーの処理[player.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "character.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CMotionAttack;
class CMotionCollision;
class CMotionOrbit;
class CMotion;
class CModelManager;
class COrbitEffect;
class CEffectManager;

//*****************************************************************************
//    プレイヤー管轄クラスの定義
//*****************************************************************************
class CPlayerManager : CScene
{
public:    // 誰でもアクセス可能
	CPlayerManager(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYERMANAGER);
	~CPlayerManager();

	static CPlayerManager *Create(char *pFileName, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetFileName(char *pFileName);
	void SetModelManager(CModelManager *pModelManager);

	char *GetFileName(void);
	CModelManager *GetModelManager(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CreatePlayer(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fAccel, const float fInertia, const float fJumpPower, const float fRivisionRot, const float fGravity, const float fColHeight, const float fColRange);
	CModel **ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pAccel, float *pInertia, float *pJumpPower, float *pRivisionRot, float *pGravity, float *pColHeight, float *pColRange);
	CModel *ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel);
	CMotionAttack *ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile);
	CMotionCollision **ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData);
	CMotionOrbit **ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData);
	CMotion *ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion);
	void ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	void ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);

	char          m_aFileName[256];      // プレイヤーデータのスクリプトファイル名
	CModelManager *m_pModelManager;      // モデル管轄クラスへのポインタ
	CTextureManager *m_pTextureManager;  // テクスチャ管轄クラスへのポインタ
};

//*****************************************************************************
//    プレイヤークラスの定義
//*****************************************************************************
class CPlayer : public CCharacter
{
public:    // 誰でもアクセス可能
	//--------------------
	//  状態の種類
	//--------------------
	typedef enum
	{
		STATE_NORMAL = 0,   // 通常状態
		STATE_MOVE,         // 移動状態
		STATE_ATTACK_0,     // 攻撃状態(1モーション目)
		STATE_ATTACK_1,     // 攻撃状態(2モーション目)
		STATE_ATTACK_2,     // 攻撃状態(3モーション目)
		STATE_JUMP,         // ジャンプ状態
		STATE_LANDING,      // 着地状態
		STATE_MAX
	}STATE;

	CPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CPlayer();

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, int nPriority = 3);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE State);
	void SetMove(const D3DXVECTOR3 move);
	void SetAccel(const float fAccel);
	void SetInertia(const float fInertia);
	void SetJumpPower(const float fJumpPower);
	void SetRivisionRot(const float fRivisionRot);
	void SetJump(const bool bJump);
	void SetAction(const bool bAction);

	STATE GetState(void);
	D3DXVECTOR3 GetMove(void);
	float GetAccel(void);
	float GetInertia(void);
	float GetJumpPower(void);
	float GetRivisionRot(void);
	bool GetJump(void);
	bool GetAction(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void InputMove(void);
	void Movement(void);
	void Collision(void);
	void Action(void);
	void Statement(void);
	void Landing(void);
	bool AttackCollision(int nModelIdx, float fRange, D3DXVECTOR3 Offset);
	void CreateEffect(void);
	void CalcOffsetRot(void);

	COrbitEffect    *m_pOrbitEffect;      // 軌跡エフェクトクラスへのポインタ
	CEffectManager  *m_pEffectManager;    // エフェクト管轄クラスへのポインタ
	STATE           m_State;              // 状態
	D3DXVECTOR3     m_Move;               // 移動量
	float           m_fAccel;             // 加速度
	float           m_fInertia;           // 慣性
	float           m_fJumpPower;         // ジャンプ量
	float           m_fRivisionRot;       // 向きを補正する倍率
	bool            m_bJump;              // ジャンプしているかどうか
	bool            m_bAction;            // アクションをしているかどうか
	bool            m_bOrbit;             // 軌跡を出しているかどうか
	bool            m_bBooking;           // アクションの予約が入っているかどうか
};

#endif