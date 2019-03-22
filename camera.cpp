//*****************************************************************************
//
//     �J�����̏���[camera.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define CAMERA_ANGEL_VIEW  (45.0f)    // �J�����̉�p
#define CAMERA_ANGLE_SPEED (0.01f)    // �J�������}�E�X�ŉ�]������X�s�[�h�{��
#define CAMERA_MOVE_SPEED  (1.3f)     // �J�������}�E�X�ňړ�������X�s�[�h�{��
#define CAMERA_LENGTH_MIN  (40.0f)    // ���_�����_�̋����̍ŏ��l
#define CAMERA_LENGTH_MAX  (400.0f)   // ���_�����_�̋����̍ő�l

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{
	// �e��l�̃N���A
	m_PosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ���_
	m_PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // �����_
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ����
	m_VecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ������x�N�g��
	m_bMove = true;                             // �J�����𓮂����邩�ǂ���
	D3DXMatrixIdentity(&m_MtxProjection);       // �v���W�F�N�V�����}�g���b�N�X
	D3DXMatrixIdentity(&m_MtxView);             // �r���[�}�g���b�N�X
	m_fLength = 0.0f;                           // ����
	m_ViewPort.X = 0;                           // �`��̈捶��X���W
	m_ViewPort.Y = 0;                           // �`��̈捶��Y���W
	m_ViewPort.Width = 0;                       // �`��̈�̕�
	m_ViewPort.Height = 0;                      // �`��̈�̍���
	m_ViewPort.MinZ = 0.0f;                     // �`��̈�[�x�o�b�t�@�ŏ��l
	m_ViewPort.MaxZ = 0.0f;                     // �`��̈�[�x�o�b�t�@�ő�l
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CCamera::Init(void)
{
	// �e��l�̏�����
	m_PosV = D3DXVECTOR3(0.0f, 300.0f, -600.0f); // ���_
	m_PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // �����_
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // ����
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);      // ������x�N�g��

	// ���_�ƒ����_�Ƃ̋������v�Z
	m_fLength = sqrtf((m_PosV.x - m_PosR.x) * (m_PosV.x - m_PosR.x) + (m_PosV.y - m_PosR.y) * (m_PosV.y - m_PosR.y) + (m_PosV.z - m_PosR.z) * (m_PosV.z - m_PosR.z));
	m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
	m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
	m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;

	// �r���[�|�[�g�ݒ�
	m_ViewPort.X = 0;                           // �`��̈捶��X���W
	m_ViewPort.Y = 0;                           // �`��̈捶��Y���W
	m_ViewPort.Width = SCREEN_WIDTH;            // �`��̈�̕�
	m_ViewPort.Height = SCREEN_HEIGHT;          // �`��̈�̍���
	m_ViewPort.MinZ = 0.0f;                     // �`��̈�[�x�o�b�t�@�ŏ��l
	m_ViewPort.MaxZ = 1.0f;                     // �`��̈�[�x�o�b�t�@�ő�l

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
//    �X�V����
//=============================================================================
void CCamera::Update(void)
{
	if (m_bMove == true)
	{// �J�����𓮂������Ԃ�������
	    // �J�����𓮂���
		CInputMouse *pMouse = CManager::GetMouse();
		if (pMouse != NULL)
		{// �}�E�X���擾�ł���
			if (pMouse->GetMouseAxisZ() != 0)
			{// �}�E�X�̃z�C�[���{�^�����X�N���[�����ꂽ
				m_fLength += 0.1f * pMouse->GetMouseAxisZ();
				if (m_fLength < CAMERA_LENGTH_MIN)
				{// ���������̒l���߂��Ȃ���
					m_fLength = CAMERA_LENGTH_MIN;
				}
				else if (m_fLength > CAMERA_LENGTH_MAX)
				{// ���������̒l��艓���Ȃ���
					m_fLength = CAMERA_LENGTH_MAX;
				}

				// ���_�̌v�Z
				m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
				m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
				m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
			}

			if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE && pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
			{// �}�E�X�̍��{�^���ƉE�{�^���������ɉ����ꂽ
				if (pMouse->GetMouseAxisX() != 0 || pMouse->GetMouseAxisY() != 0)
				{// �}�E�X����������Ă���
				    // �����ɉ����ꂽ�Ƃ��̈ʒu���獡�̃}�E�X�J�[�\���̈ʒu���王�_�����_���ړ�������������v�Z
					float fAngle = atan2f(((float)pMouse->GetPoint().x - (float)pMouse->GetPointOld().x), ((float)pMouse->GetPoint().y - (float)pMouse->GetPointOld().y));
					fAngle -= m_Rot.y;
					if (fAngle < -D3DX_PI)
					{// �������~�����𒴂���
						fAngle += D3DX_PI * 2.0f;
					}
					if (fAngle > D3DX_PI)
					{// �������~�����𒴂���
						fAngle -= D3DX_PI * 2.0f;
					}

					// �}�E�X��X��,Y������ړ��ʂ��v�Z
					float fMovement = sqrtf((float)pMouse->GetMouseAxisX() * (float)pMouse->GetMouseAxisX() + (float)pMouse->GetMouseAxisY() * (float)pMouse->GetMouseAxisY());
					fMovement *= CAMERA_MOVE_SPEED;

					// ���_���ړ�������
					m_PosR.x += sinf(fAngle) * fMovement;
					m_PosR.z -= cosf(fAngle) * fMovement;

					// �����_���ړ�������
					m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
					m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
				}
			}
			else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE)
			{// �}�E�X�̍��N���b�N�݂̂������ꂽ
			    // �������}�E�X�̑��x�ɍ��킹�ĉ�]������
			    // �c��
				m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
				if (m_Rot.y > D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.y -= D3DX_PI * 2.0f;
				}
				if (m_Rot.y < -D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.y += D3DX_PI * 2.0f;
				}

				// ����
				m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
				if (m_Rot.z < -D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.z = -D3DX_PI;
				}
				if (m_Rot.z > 0.0f)
				{// �������~�����𒴂���
					m_Rot.z = 0.0f;
				}

				// ���_�̈ʒu���v�Z����
				m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
				m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
				m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
			}
			else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
			{// �}�E�X�̉E�N���b�N�݂̂������ꂽ
			    // �������}�E�X�̑��x�ɍ��킹�ĉ�]������
			    // �c��
				m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
				if (m_Rot.y > D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.y -= D3DX_PI * 2.0f;
				}
				if (m_Rot.y < -D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.y += D3DX_PI * 2.0f;
				}

				// ����
				m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
				if (m_Rot.z < -D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.z = -D3DX_PI;
				}
				if (m_Rot.z > 0.0f)
				{// �������~�����𒴂���
					m_Rot.z = 0.0f;
				}

				// �����_�̈ʒu���v�Z����
				m_PosR.x = m_PosV.x + sinf(m_Rot.y) * m_fLength;
				m_PosR.y = m_PosV.y - cosf(m_Rot.z) * m_fLength;
				m_PosR.z = m_PosV.z + cosf(m_Rot.y) * m_fLength;
			}
		}
	}
}

//=============================================================================
//    �J�����̐ݒ�
//=============================================================================
void CCamera::SetCamera(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �r���[�|�[�g(�`��̈�)�̐ݒ�
			pDevice->SetViewport(&m_ViewPort);

			// �v���W�F�N�V�����}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&m_MtxProjection);

			// �v���W�F�N�V�����}�g���b�N�X���쐬
			D3DXMatrixPerspectiveFovLH(&m_MtxProjection,
				D3DXToRadian(CAMERA_ANGEL_VIEW),
				(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
				10.0f,
				20000.0f);

			// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_PROJECTION, &m_MtxProjection);

			// �r���[�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&m_MtxView);

			// �r���[�}�g���b�N�X���쐬
			D3DXMatrixLookAtLH(&m_MtxView,
				&m_PosV,
				&m_PosR,
				&m_VecU);

			// �r���[�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_VIEW, &m_MtxView);
		}
	}
}

//=============================================================================
//    �J�����̎��_�̐ݒ菈��
//=============================================================================
void CCamera::SetPosV(const D3DXVECTOR3 posV)
{
	m_PosV = posV;
}

//=============================================================================
//    �J�����̒����_�̐ݒ菈��
//=============================================================================
void CCamera::SetPosR(const D3DXVECTOR3 posR)
{
	m_PosR = posR;
}

//=============================================================================
//    �J�����̌����̎擾����
//=============================================================================
void CCamera::SetRot(const D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//=============================================================================
//    �J�����𓮂������ǂ������擾
//=============================================================================
void CCamera::SetMove(const bool bMove)
{
	m_bMove = bMove;
}

//=============================================================================
//    �J�����̎��_�̐ݒ菈��
//=============================================================================
D3DXVECTOR3 CCamera::GetPosV(void)
{
	return m_PosV;
}

//=============================================================================
//    �J�����̒����_�̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetPosR(void)
{
	return m_PosR;
}

//=============================================================================
//    �J�����̌����̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_Rot;
}