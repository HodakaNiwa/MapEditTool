//*****************************************************************************
//
//     エディターの処理[editer.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _EDITER_H_
#define _EDITER_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
//     前方宣言
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
//     エディタークラスの定義
//*****************************************************************************
class CEditer
{
public:     // 誰でもアクセス可能
	//----------------
	// エフェクトの状態
	//----------------
	typedef enum
	{
		EFFECTSTATE_PLAY = 0,   // 再生状態
		EFFECTSTATE_STOP,       // 終了状態
		EFFECTSTATE_MAX
	}EFFECTSTATE;

	//---------------------
	//  配置物の種類
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

	// 共通
	void SetType(const TYPE Type);
	void SetEffectState(const EFFECTSTATE state);
	void SetPos(const D3DXVECTOR3 SetPos);
	void SetRot(const D3DXVECTOR3 SetRot);

	// 配置モデル用
	void SetModelColType(const CObject::COL_TYPE ModelColType);
	void SetModelScale(const D3DXVECTOR3 ModelScale);
	void SetModelRollSpeed(const D3DXVECTOR3 ModelRollSpeed);
	void SetModelColRange(const float fModelColRange);
	void SetModelMove(const D3DXVECTOR3 ModelMove);
	void SetModelMaxMoveCounter(const int nModelMaxMoveCounter);
	void SetNumModel(const int nNumModel);
	void SetSelectModelIdx(const int nSelectModelIdx);

	// ビルボード用
	void SetBillCol(const D3DXCOLOR BillCol);
	void SetBillColRange(const float fBillColRange);
	void SetBillRot(const float fBillRot);
	void SetBillWidth(const float fBillWidth);
	void SetBillHeight(const float fBillHeight);
	void SetBillTexIdx(const int nTexIdx);
	void SetBillLighting(const bool bLighting);
	void SetBillDrawAddtive(const bool bDrawAddtive);

	// エフェクト用
	void SetEffectType(const int nEffectType);

	// 敵用
	void SetEnemyType(const int nEnemyType);
	void SetEnemyLife(const int nEnemyLife);
	void SetEnemyScore(const int nEnemyScore);

	// 壁用
	void SetWallTexIdx(const int nWallTexIdx);
	void SetWallCol(const D3DXCOLOR WallCol);
	void SetWallWidth(const float fWidth);
	void SetWallHeight(const float fHeight);
	void SetWallXBlock(const int nXBlock);
	void SetWallYBlock(const int nYBlock);
	void SetWallTexSplitU(const int nWallTexSplitU);
	void SetWallTexSplitV(const int nWallTexSplitV);

	// アイテム用
	void SetItemCol(const D3DXCOLOR ItemCol);
	void SetItemColRange(const float fItemColRange);
	void SetItemRot(const float fItemRot);
	void SetItemWidth(const float fItemWidth);
	void SetItemHeight(const float fItemHeight);
	void SetItemTexIdx(const int nTexIdx);
	void SetItemType(const int nType);
	void SetItemLighting(const bool bLighting);
	void SetItemDrawAddtive(const bool bDrawAddtive);


	// 共通
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

	// 配置モデル用
	CObject::COL_TYPE GetModelColType(void);
	D3DXVECTOR3 GetModelScale(void);
	D3DXVECTOR3 GetModelRollSpeed(void);
	float GetModelColRange(void);
	D3DXVECTOR3 GetModelMove(void);
	int GetModelMaxMoveCounter(void);
	int GetNumModel(void);
	int GetSelectModelIdx(void);

	// ビルボード用
	D3DXCOLOR GetBillCol(void);
	float GetBillColRange(void);
	float GetBillRot(void);
	float GetBillWidth(void);
	float GetBillHeight(void);
	int GetBillTexIdx(void);
	bool GetBillLighting(void);
	bool GetBillDrawAddtive(void);
	int GetNumTex(void);

	// エフェクト用
	int GetEffectType(void);

	// 敵用
	int GetEnemyType(void);
	int GetEnemyLife(void);
	int GetEnemyScore(void);

	// 壁用
	int GetWallTexIdx(void);
	D3DXCOLOR GetWallCol(void);
	float GetWallWidth(void);
	float GetWallHeight(void);
	int GetWallXBlock(void);
	int GetWallYBlock(void);
	int GetWallTexSplitU(void);
	int GetWallTexSplitV(void);

	// アイテム用
	D3DXCOLOR GetItemCol(void);
	float GetItemColRange(void);
	float GetItemRot(void);
	float GetItemWidth(void);
	float GetItemHeight(void);
	int GetItemTexIdx(void);
	int GetItemType(void);
	bool GetItemLighting(void);
	bool GetItemDrawAddtive(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	void ObjectMove(void);

	// 共通
	CMap                *m_pMap;                 // マップクラスへのポインタ
	CSetObject          *m_pSetObject;           // 配置物へのポインタ
	CSetObjectMove      *m_pSetObjectMove;       // 動く配置物へのポインタ
	CSetBillboardObject *m_pSetBillboardObj;     // 配置用ビルボードへのポインタ
	CEmitter            *m_pSetEmitter;          // 配置用エミッタへのポインタ
	CSetEnemy           *m_pSetEnemy;            // 配置用敵クラスへのポインタ
	CSetMeshWall        *m_pSetWall;             // 配置用壁クラスへのポインタ
	CSetItem            *m_pSetItem;             // 配置用アイテムクラスへのポインタ
	CModelManager       *m_pModelManager;        // モデル管轄クラスへのポインタ
	CTextureManager     *m_pTextureManager;      // テクスチャ管轄クラスへのポインタ
	CEffectManager      *m_pEffectManager;       // エフェクト管轄クラスへのポインタ
	CEnemyAssembly      *m_pEnemyAssmbly;        // 敵管轄まとめクラスへのポインタ
	TYPE                m_Type;                  // 現在配置できる配置物の種類
	EFFECTSTATE         m_EffectState;           // 現在配置できるエミッタの再生状況
	D3DXVECTOR3         m_SetPos;                // 配置できるオブジェクトの座標
	D3DXVECTOR3         m_SetRot;                // 配置できるオブジェクトの向き
	int                 m_nCounter;              // 状態を管理するカウンター


	// 配置モデル用
	CObject::COL_TYPE   m_ModelColType;          // 配置できるモデルの当たり判定の種類
	D3DXVECTOR3         m_ModelScale;            // 配置できるモデルの大きさの倍率
	D3DXVECTOR3         m_ModelRollSpeed;        // 配置できるモデルが回転するスピード
	float               m_fModelColRange;        // 配置できるモデルの当たり判定を取る範囲(円形で当たり判定を取る際に使用)
	D3DXVECTOR3         m_ModelMove;             // 配置できるモデルの移動量
	int                 m_nModelMaxMoveCounter;  // 配置できるモデルの移動量を切り替えるタイミング
	int                 m_nNumModel;             // 配置できるモデルの種類の数
	int                 m_nSelectModelIdx;       // 現在配置できるモデルの番号

	// ビルボード用
	D3DXCOLOR           m_BillCol;               // 配置できるビルボードの色
	float               m_fBillColRange;         // 配置できるビルボードの当たり判定を取る範囲
	float               m_fBillRot;              // 配置できるビルボードの向き
	float               m_fBillWidth;            // 配置できるビルボードの幅
	float               m_fBillHeight;           // 配置できるビルボードの高さ
	int                 m_nBillTexIdx;           // 配置できるビルボードのテクスチャの番号
	bool                m_bBillLighting;         // 配置できるビルボードのライティングするかしないか
	bool                m_bBillDrawAddtive;      // 配置できるビルボードの加算合成で描画するかしないか
	int                 m_nNumTex;               // 設定できるテクスチャの種類の数

	// エフェクト用
	int                 m_nEffectType;           // 配置できるエフェクトの種類番号

	// 敵用
	int                 m_nEnemyType;            // 配置できる敵の種類
	int                 m_nEnemyLife;            // 配置できる敵の体力
	int                 m_nEnemyScore;           // 配置できる敵のスコア

	// 壁用
	int                 m_nWallTexIdx;           // 配置できる壁のテクスチャ番号
	D3DXCOLOR           m_WallCol;               // 配置できる壁の色
	float               m_fWallWidth;            // 配置できる壁の幅
	float               m_fWallHeight;           // 配置できる壁の高さ
	int                 m_nWallXBlock;           // 配置できる壁の横の分割数
	int                 m_nWallYBlock;           // 配置できる壁の縦の分割数
	int                 m_nWallTexSplitU;        // 配置できる壁のテクスチャU座標の分割数
	int                 m_nWallTexSplitV;        // 配置できる壁のテクスチャV座標の分割数

	// アイテム用
	D3DXCOLOR           m_ItemCol;               // 配置できるアイテムの色
	float               m_fItemColRange;         // 配置できるアイテムの当たり判定を取る範囲
	float               m_fItemRot;              // 配置できるアイテムの向き
	float               m_fItemWidth;            // 配置できるアイテムの幅
	float               m_fItemHeight;           // 配置できるアイテムの高さ
	int                 m_nItemTexIdx;           // 配置できるアイテムのテクスチャの番号
	int                 m_nItemType;             // 配置できるアイテムの種類番号
	bool                m_bItemLighting;         // 配置できるアイテムのライティングするかしないか
	bool                m_bItemDrawAddtive;      // 配置できるアイテムの加算合成で描画するかしないか
};

#endif