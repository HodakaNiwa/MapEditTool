//*****************************************************************************
//
//     ImGuiの処理(Version.Niwa)[imgui_Niwa.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _IMGUI_NIWA_H_
#define _IMGUI_NIWA_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"
#include "imgui.h"

//*****************************************************************************
//     ImGui(Version.Niwa)クラスの定義
//*****************************************************************************
class CImGui_Niwa
{
public:     // 誰でもアクセス可能
	CImGui_Niwa();
	~CImGui_Niwa();

	HRESULT Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CheakDeviceLost(HRESULT result, LPDIRECT3DDEVICE9 pDevice);

	void SetDisp(bool bDisp);
	bool GetDisp(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void SetWindowDebug(void);
	void SetObjectDebug(void);
	void SetBillboardDebug(void);
	void SetEffectDebug(void);
	void SetEnemyDebug(void);
	void SetWallDebug(void);
	void SetItemDebug(void);

	bool m_bDisp;      // Imguiを描画するかしないか
};

#endif