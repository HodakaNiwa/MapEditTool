//*****************************************************************************
//
//     �}�l�[�W���[�̏���[manager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �O���錾(���̂��Ȃ��錾)
//*****************************************************************************
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputJoyStick;
class CXInput;
class CLight;
class CCamera;
class CDebugProc;
class CEditer;

//*****************************************************************************
//    �}�l�[�W���N���X�̒�`
//*****************************************************************************
class CManager
{
public:    // �N�ł��A�N�Z�X�\
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance,HWND hWnd,bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRenderer *GetRenderer(void);
	static CLight *GetLight(void);
	static CCamera *GetCamera(void);
	static CInputKeyboard *GetKeyboard(void);
	static CInputMouse *GetMouse(void);
	static CInputJoyStick *GetJoyStick(void);
	static CXInput *GetXInput(void);
	static CEditer *GetEditer(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static CRenderer         *m_pRenderer;  // �����_�����O�N���X�^�̃|�C���^
	static CLight            *m_pLight;     // ���C�g�N���X�^�̃|�C���^
	static CCamera           *m_pCamera;    // �J�����N���X�^�̃|�C���^
	static CInputKeyboard    *m_pKeyboard;  // �L�[�{�[�h�N���X�^�̃|�C���^
	static CInputMouse       *m_pMouse;     // �}�E�X�N���X�^�̃|�C���^
	static CInputJoyStick    *m_pJoyStick;  // �W���C�X�e�B�b�N�N���X�^�̃|�C���^
	static CXInput           *m_pXInput;    // XInput�N���X�^�̃|�C���^
	static CDebugProc        *m_pDebugProc; // �f�o�b�O���N���X�^�̃|�C���^
	static CEditer           *m_pEditer;    // �G�f�B�^�[�N���X�^�̃|�C���^
};

//*****************************************************************************
//    �v���g�^�C�v�錾
//*****************************************************************************
bool GetUpdate(void);

#endif