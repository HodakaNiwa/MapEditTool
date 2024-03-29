//*****************************************************************************
//
//     エフェクト管轄の処理[effectManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define MAX_EMMITER         (100)     // エミッタデータの最大数
#define MAX_PARTICLEDATA    (100)     // パーティクルデータの最大数
#define MAX_RINGEFFECTDATA  (100)     // リングエフェクトデータの最大数

//*****************************************************************************
//     前方宣言
//*****************************************************************************
class CParData;
class CEmitterData;
class CRingData;
class CTextureManager;
class CEmitter;

//*****************************************************************************
//     エフェクトマネージャークラスの定義
//*****************************************************************************
class CEffectManager : CScene
{
public:   // 誰でもアクセス可能
	typedef enum
	{
		EFFECT_TYPE_FIRE = 0, // 炎
		EFFECT_TYPE_DASH,     // 走った時のエフェクト
		EFFECT_TYPE_SLASH,    // 切った時のエフェクト
		EFFECT_TYPE_SLASHPAR, // 切った時のエフェクト
		EFFECT_TYPE_RINGEFFECT, // 切った時のエフェクト
	}EFFECT_TYPE;

	CEffectManager(int nPriority = 3, OBJTYPE objType = OBJTYPE_EFFECTMANAGER);
	~CEffectManager();

	static CEffectManager *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);
	CEmitter *SetMapEditEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

	void SetFileName(char *pFileName);
	void SetEmitter(CEmitter *pEmitter);
	void SetEmitterData(CEmitterData *pEmitterData, int nIdx);
	void SetParData(CParData *pParData, int nIdx);
	void SetRingEffectData(CRingData *pRingData, int nIdx);
	void SetTexManager(CTextureManager *pTexManager);
	void SetNumEmitterData(int nNumEmitterData);
	void SetNumParData(int nNumParData);
	void SetNumRingEffectData(int nNumRingEffectData);

	LPDIRECT3DTEXTURE9 BindTexture(int nIdx);
	CEmitter *GetEmitter(void);
	CEmitterData *GetEmitterData(int nIdx);
	CParData *GetParData(int nIdx);
	CRingData *GetRingEffectData(int nIdx);
	CTextureManager *GetTexManager(void);
	int GetNumEmitterData(void);
	int GetNumParData(void);
	int GetNumRingEffectData(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void Save(void);
	void SaveTextureData(FILE *pFile);
	void SaveEmitterData(FILE *pFile);
	void SaveParticleData(FILE *pFile);
	void SaveRingEffectData(FILE *pFile);

	CEmitterData *ReadEmitterData(char *pLine, char *pStrCur, FILE *pFile, int nCntEmitter);
	CParData *ReadParticleData(char *pLine, char *pStrCur, FILE *pFile, int nCntParData);
	CRingData *ReadRingEffectData(char *pLine, char *pStrCur, FILE *pFile, int nCntRingData);

	char            m_aFileName[256];                  // スクリプトファイル名
	int             m_nNumEmitterData;                 // エミッタデータの個数
	int             m_nNumParData;                     // パーティクルデータの個数
	int             m_nNumRingEffectData;              // リングエフェクトデータの個数
	CEmitter        *m_pEmitter;                       // 現在生成されているエミッタへのポインタ
	CEmitterData    *m_apEmitterData[MAX_EMMITER];     // エミッタデータクラスへのポインタ
	CParData        *m_apParData[MAX_PARTICLEDATA];    // パーティクルデータクラスへのポインタ
	CRingData       *m_apRingData[MAX_RINGEFFECTDATA]; // リングエフェクトデータクラスへのポインタ
	CTextureManager *m_pTextureManager;                // テクスチャ管理クラスへのポインタ
};

#endif