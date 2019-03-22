//*****************************************************************************
//
//     �}�l�[�W���[�̏���[manager.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "debuglog.h"
#include "scene.h"
#include "editer.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define MANAGER_DEBUGKEY_DEBUGPROC_CHANGE (DIK_F1)   // �f�o�b�O�\�����邩���Ȃ�����؂�ւ���L�[
#define MANAGER_DEBUGKEY_LIGHTING_CHANGE  (DIK_F2)   // ���C�e�B���OON/OFF��؂�ւ���L�[
#define MANAGER_DEBUGKEY_WIREFRAME_CHANGE (DIK_F3)   // ���C���[�t���[��ON/OFF��؂�ւ���L�[
#define MANAGER_DEBUGKEY_UPDATE_STOP      (DIK_F4)   // �A�b�v�f�[�g���~����L�[
#define MANAGER_DEBUGKEY_RELEASEALL       (DIK_F11)  // �S�ẴI�u�W�F�N�g��j������L�[

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
CRenderer         *CManager::m_pRenderer = NULL;     // �����_�����O�N���X�^�̃|�C���^
CCamera           *CManager::m_pCamera = NULL;       // �J�����N���X�^�̃|�C���^
CLight            *CManager::m_pLight = NULL;        // ���C�g�N���X�^�̃|�C���^
CInputKeyboard    *CManager::m_pKeyboard = NULL;     // �L�[�{�[�h�N���X�^�̃|�C���^
CInputMouse       *CManager::m_pMouse = NULL;        // �}�E�X�N���X�^�̃|�C���^
CInputJoyStick    *CManager::m_pJoyStick = NULL;     // �W���C�X�e�B�b�N�N���X�^�̃|�C���^
CXInput           *CManager::m_pXInput = NULL;       // XInput�N���X�^�̃|�C���^
CDebugProc        *CManager::m_pDebugProc = NULL;    // �f�o�b�O���N���X�^�̃|�C���^
CEditer           *CManager::m_pEditer = NULL;       // �G�f�B�^�[�N���X�^�̃|�C���^

bool g_bUpdate = true;                                // �A�b�v�f�[�g���邩���Ȃ���
bool g_bLighting = true;                              // ���C�e�B���OON/OFF��؂�ւ���t���O
bool g_bWireFrame = false;                            // ���C���[�t���[���ŕ`�悷�邩���Ȃ���

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// �����_�����O�̍쐬
	if (m_pRenderer == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pRenderer = new CRenderer;
		if (m_pRenderer != NULL)
		{// ���������m�ۂł���
			if (FAILED(m_pRenderer->Init(hWnd, bWindow)))  // FALSE�ɂ���ƃt���X�N���[��
			{// �������Ɏ��s����
				return E_FAIL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return E_FAIL;
	}

	// �J�����̍쐬
	if (m_pCamera == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pCamera = new CCamera;
		if (m_pCamera != NULL)
		{// ���������m�ۂł���
			if (FAILED(m_pCamera->Init()))
			{// �������Ɏ��s����
				return E_FAIL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return E_FAIL;
	}

	// ���C�g�̍쐬
	if (m_pLight == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pLight = new CLight;
		if (m_pLight != NULL)
		{// ���������m�ۂł���
			if (FAILED(m_pLight->Init()))
			{// �������Ɏ��s����
				return E_FAIL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return E_FAIL;
	}

	// �L�[�{�[�h�̍쐬
	if (m_pKeyboard == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pKeyboard = new CInputKeyboard;
		if (m_pKeyboard != NULL)
		{// ���������m�ۂł���
			if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
			{// �������Ɏ��s����
				return E_FAIL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return E_FAIL;
	}

	// �}�E�X�̍쐬
	if (m_pMouse == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pMouse = new CInputMouse;
		if (m_pMouse != NULL)
		{// ���������m�ۂł���
			m_pMouse->Init(hInstance, hWnd);
		}
	}

	// �W���C�X�e�B�b�N�̍쐬
	if (m_pJoyStick == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pJoyStick = new CInputJoyStick(1);
		if (m_pJoyStick != NULL)
		{// ���������m�ۂł���
			m_pJoyStick->Init(hInstance, hWnd);
		}
	}

	// XInput�̍쐬
	if (m_pXInput == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pXInput = new CXInput;
		if (m_pXInput != NULL)
		{// ���������m�ۂł���
			m_pXInput->Init();
		}
	}

	// �G�f�B�^�[�̍쐬
	if (m_pEditer == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pEditer = new CEditer;
		if (m_pEditer != NULL)
		{// ���������m�ۂł���
			m_pEditer->Init();
		}
	}

	// �f�o�b�O���N���X�̍쐬
	if (m_pDebugProc == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pDebugProc = new CDebugProc;
		if (m_pDebugProc != NULL)
		{// ���������m�ۂł���
			m_pDebugProc->Init();
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CManager::Uninit(void)
{
	// �G�f�B�^�[�̍쐬
	if (m_pEditer != NULL)
	{// ���������m�ۂ���Ă���
	    // �G�f�B�^�[�̏I������
		m_pEditer->Uninit();

		// �������̊J��
		delete m_pEditer;
		m_pEditer = NULL;
	}

	// �V�[���̃C���X�^���X�����ׂĔj������
	CScene::ReleaseAll();

	// �f�o�b�O���̔j��
	if (m_pDebugProc != NULL)
	{// ���������m�ۂ���Ă���
	     // �f�o�b�O���̏I������
		m_pDebugProc->Uninit();

		// �������̊J��
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

	// �L�[�{�[�h�̔j��
	if (m_pKeyboard != NULL)
	{// ���������m�ۂ���Ă���
	    // �L�[�{�[�h�̏I������
		m_pKeyboard->Uninit();

		// �������̊J��
		delete m_pKeyboard;
		m_pKeyboard = NULL;
	}

	// �}�E�X�̔j��
	if (m_pMouse != NULL)
	{// ���������m�ۂ���Ă���
	    // �}�E�X�̏I������
		m_pMouse->Uninit();

		// �������̊J��
		delete m_pMouse;
		m_pMouse = NULL;
	}

	// �W���C�X�e�B�b�N�̔j��
	if (m_pJoyStick != NULL)
	{// ���������m�ۂ���Ă���
	    // �W���C�X�e�B�b�N�̏I������
		m_pJoyStick->Uninit();

		// �������̊J��
		delete m_pJoyStick;
		m_pJoyStick = NULL;
	}

	// XInput�̔j��
	if (m_pXInput != NULL)
	{// ���������m�ۂ���Ă���
		// XInput�̏I������
		m_pXInput->Uninit();

		// �������̊J��
		delete m_pXInput;
		m_pXInput = NULL;
	}

	// ���C�g�̔j��
	if (m_pLight != NULL)
	{// ���������m�ۂ���Ă���
	    // ���C�g�̏I������
		m_pLight->Uninit();

		// �������̊J��
		delete m_pLight;
		m_pLight = NULL;
	}

	// �J�����̔j��
	if (m_pCamera != NULL)
	{// ���������m�ۂ���Ă���
	    // �J�����̏I������
		m_pCamera->Uninit();

		// �������̊J��
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// �����_�����O�̔j��
	if (m_pRenderer != NULL)
	{// ���������m�ۂ���Ă���
		// �����_�����O�̏I������
		m_pRenderer->Uninit();

		// �������̊J��
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//=============================================================================
//    �X�V����
//=============================================================================
void CManager::Update(void)
{
	// ������@��`��
	CDebugProc::Print("( ������@�ꗗ )\n");
	//CDebugProc::Print("�f�o�b�O�\���ؑ�                   : [ F1 ]\n");
	//CDebugProc::Print("���C�e�B���O(ON/OFF)               : [ F2 ]\n");
	//CDebugProc::Print("���C���[�t���[��(ON/OFF)           : [ F3 ]\n");
	//CDebugProc::Print("�X�V���~����/�J�n����            : [ F4 ]\n");
	//CDebugProc::Print("���ׂẴI�u�W�F�N�g���폜         : [ F11 ]\n");

	if (m_pKeyboard != NULL)
	{// �L�[�{�[�h����������Ă���
		if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_DEBUGPROC_CHANGE) == true)
		{// �L�[�{�[�h��F1�L�[�������ꂽ
			CDebugProc::SetDisp(CDebugProc::GetDisp() ? false : true);
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_LIGHTING_CHANGE) == true)
		{// �L�[�{�[�h��F2�L�[�������ꂽ
			g_bLighting = g_bLighting ? false : true;
			m_pRenderer->GetDevice()->SetRenderState(D3DRS_LIGHTING, g_bLighting);
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_WIREFRAME_CHANGE) == true)
		{// �L�[�{�[�h��F3�L�[�������ꂽ
			g_bWireFrame = g_bWireFrame ? false : true;
			if (g_bWireFrame == true)
			{
				m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
			else
			{
				m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_UPDATE_STOP) == true)
		{// �L�[�{�[�h��F4�L�[�������ꂽ
			g_bUpdate = g_bUpdate ? false : true;
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_RELEASEALL) == true)
		{// �L�[�{�[�h��F11�L�[�������ꂽ
			CScene::ReleaseAll();
		}
	}

	if (m_pKeyboard != NULL)
	{// ���������m�ۂ���Ă���
	    // �L�[�{�[�h�̍X�V����
		m_pKeyboard->Update();
	}

	if (m_pMouse != NULL)
	{// ���������m�ۂ���Ă���
	    // �}�E�X�̍X�V����
		m_pMouse->Update();
	}

	if (m_pEditer != NULL)
	{// ���������m�ۂ���Ă���
	    // �G�f�B�^�[�̍X�V����
		m_pEditer->Update();
	}

	if (m_pCamera != NULL)
	{// ���������m�ۂ���Ă���
	    // �J�����̍X�V����
		m_pCamera->Update();
	}

	if (g_bUpdate == true)
	{// �X�V�����ԂȂ��
		if (m_pJoyStick != NULL)
		{// ���������m�ۂ���Ă���
		    // �W���C�X�e�B�b�N�̍X�V����
			m_pJoyStick->Update();
		}

		if (m_pXInput != NULL)
		{// ���������m�ۂ���Ă���
		    // XInput�̍X�V����
			m_pXInput->Update();
		}

		if (m_pLight != NULL)
		{// ���������m�ۂ���Ă���
		    // ���C�g�̍X�V����
			m_pLight->Update();
		}

		if (m_pRenderer != NULL)
		{// ���������m�ۂ���Ă���
		    // �����_�����O�̍X�V����
			m_pRenderer->Update();
		}
	}
	else
	{
		CDebugProc::Print(" �X�V��~��\n");
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{// ���������m�ۂ���Ă���
		m_pRenderer->Draw();
	}
}

//=============================================================================
//    �����_�����O�N���X�̃|�C���^���擾
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
//    �J�����N���X�̃|�C���^���擾
//=============================================================================
CCamera *CManager::GetCamera(void)
{
	return m_pCamera;
}

//=============================================================================
//    ���C�g�N���X�̃|�C���^���擾
//=============================================================================
CLight *CManager::GetLight(void)
{
	return m_pLight;
}

//=============================================================================
//    �L�[�{�[�h�N���X�̃|�C���^���擾
//=============================================================================
CInputKeyboard *CManager::GetKeyboard(void)
{
	return m_pKeyboard;
}

//=============================================================================
//    �}�E�X�N���X�̃|�C���^���擾
//=============================================================================
CInputMouse *CManager::GetMouse(void)
{
	return m_pMouse;
}

//=============================================================================
//    �W���C�X�e�B�b�N�N���X�̃|�C���^���擾
//=============================================================================
CInputJoyStick *CManager::GetJoyStick(void)
{
	return m_pJoyStick;
}

//=============================================================================
//    XInput�N���X�̃|�C���^���擾
//=============================================================================
CXInput *CManager::GetXInput(void)
{
	return m_pXInput;
}

//=============================================================================
//    �G�f�B�^�[�N���X�̃|�C���^���擾
//=============================================================================
CEditer *CManager::GetEditer(void)
{
	return m_pEditer;
}

//=============================================================================
//    �X�V���Ă��邩�ǂ�����Ԃ�����
//=============================================================================
bool GetUpdate(void)
{
	return g_bUpdate;
}