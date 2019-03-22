//*****************************************************************************
//
//     �J�����̏���[camera.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �J�����N���X�̒�`
//*****************************************************************************
class CCamera
{
public:    // �N�ł��A�N�Z�X�\
	CCamera();
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void SetPosV(const D3DXVECTOR3 posV);
	void SetPosR(const D3DXVECTOR3 posR);
	void SetRot(const D3DXVECTOR3 Rot);
	void SetMove(const bool bMove);

	D3DXVECTOR3 GetPosV(void);
	D3DXVECTOR3 GetPosR(void);
	D3DXVECTOR3 GetRot(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXVECTOR3  m_PosV;          // ���_
	D3DXVECTOR3  m_PosR;          // �����_
	D3DXVECTOR3  m_Rot;           // ����
	D3DXVECTOR3  m_VecU;          // ������x�N�g��
	D3DXMATRIX   m_MtxProjection; // �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX   m_MtxView;       // �r���[�}�g���b�N�X
	D3DVIEWPORT9 m_ViewPort;      // �r���[�|�[�g(�`��̈�)
	float        m_fLength;       // ����
	bool         m_bMove;         // �J�����𓮂������ǂ���
};

#endif