//*****************************************************************************
//
//     �����_�����O�̏���[renderer.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "imgui_Niwa.h"

#include "debuglog.h"

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRenderer::CRenderer()
{
	// �e��l�̃N���A
	m_pD3DDevice = NULL;   // �f�o�C�X�ւ̃|�C���^
	m_pD3D = NULL;         // Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	m_pImGui_Niwa = NULL;  // ImGui�N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;							            // �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				            // �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				            // �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;			            	// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		                // �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil = TRUE;						    // �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					    // �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;						                // �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	    // �C���^�[�o��

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);              // ���ʂ��J�����O
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);                      // Z�o�b�t�@���g�p
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);             // ���u�����h���s��
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);                     // ���C�e�B���O���s��
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);        // ���\�[�X�J���[�̎w��
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);    // ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // �A���t�@�u�����f�B���O������ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); // �ŏ��̃A���t�@����
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT); // �Q�Ԗڂ̃A���t�@����


	// Imgui�N���X�̐���
	if (m_pImGui_Niwa == NULL)
	{// ���������m�ۂł���^�C�~���O�ł���
		m_pImGui_Niwa = new CImGui_Niwa;
		if (m_pImGui_Niwa != NULL)
		{// ���������m�ۂł���
			m_pImGui_Niwa->Init(hWnd, m_pD3DDevice);
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRenderer::Uninit(void)
{
	// Imgui�N���X���J������
	if (m_pImGui_Niwa != NULL)
	{// ���������m�ۂ���Ă���
		m_pImGui_Niwa->Uninit();

		// �������̊J��
		delete m_pImGui_Niwa;
		m_pImGui_Niwa = NULL;
	}

	// �f�o�C�X�̔j��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRenderer::Update(void)
{
	// �V�[���N���X�̍X�V����
	CScene::UpdateAll();

	// ImGui�N���X�̍X�V����
	if (m_pImGui_Niwa != NULL)
	{// ���������m�ۂ���Ă���
		m_pImGui_Niwa->Update();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRenderer::Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// �J�����̐ݒ�
		CManager::GetCamera()->SetCamera();

		// �V�[���N���X�̕`�揈��
		CScene::DrawAll();

		// �f�o�b�O���\������
		CDebugProc::Draw();

		// ImGui�N���X�̕`�揈��
		if (m_pImGui_Niwa != NULL)
		{// ���������m�ۂ���Ă���
			m_pImGui_Niwa->Draw();
		}

		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	HRESULT result = m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	if (m_pImGui_Niwa != NULL)
	{// ���������m�ۂ���Ă���
		m_pImGui_Niwa->CheakDeviceLost(result, m_pD3DDevice);
	}
}

//=============================================================================
//    �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}