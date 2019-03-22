//*****************************************************************************
//
//     ImGui�̏���(Version.Niwa)[imgui_Niwa.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _IMGUI_NIWA_H_
#define _IMGUI_NIWA_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "imgui.h"

//*****************************************************************************
//     ImGui(Version.Niwa)�N���X�̒�`
//*****************************************************************************
class CImGui_Niwa
{
public:     // �N�ł��A�N�Z�X�\
	CImGui_Niwa();
	~CImGui_Niwa();

	HRESULT Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CheakDeviceLost(HRESULT result, LPDIRECT3DDEVICE9 pDevice);

	void SetDisp(bool bDisp);
	bool GetDisp(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void SetWindowDebug(void);
	void SetObjectDebug(void);
	void SetBillboardDebug(void);
	void SetEffectDebug(void);
	void SetEnemyDebug(void);
	void SetWallDebug(void);
	void SetItemDebug(void);

	bool m_bDisp;      // Imgui��`�悷�邩���Ȃ���
};

#endif