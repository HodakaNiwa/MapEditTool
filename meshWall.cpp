//*****************************************************************************
//
//     ���b�V���E�H�[���̏���[meshWall.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "meshWall.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************


//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//*****************************************************************************
//    CMeshWall�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMeshWall::CMeshWall(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_nTexIdx = 0;                              // ���b�V���E�H�[���̃e�N�X�`���ԍ�
	m_pVtxBuff = NULL;                          // ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;                          // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	m_pTexture = NULL;	                        // �e�N�X�`���ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ���b�V���t�B�[���h�̍��W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ���b�V���t�B�[���h�̌���
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // ���b�V���t�B�[���h�̐F
	m_fWidth = 0.0f;                            // 1�u���b�N���̕�
	m_fHeight = 0.0f;                           // 1�u���b�N������
	m_nXBlock = 0;                              // ���̕�����
	m_nYBlock = 0;                              // �c�̕�����
	m_nNumVertex = 0;                           // ���_��
	m_nNumIndex = 0;                            // �C���f�b�N�X��
	m_nNumPolygon = 0;                          // �|���S����
	D3DXMatrixIdentity(&m_MtxWorld);            // ���[���h�}�g���b�N�X
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMeshWall::~CMeshWall()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMeshWall *CMeshWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx, int nPriority)
{
	CMeshWall *pMeshWall = NULL;           // ���b�V���E�H�[���N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pMeshWall == NULL)
		{// ����������ɂȂ��Ă���
			pMeshWall = new CMeshWall(nPriority);
			if (pMeshWall != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pMeshWall->SetTexIdx(nTexIdx);            // ���b�V���E�H�[���̃e�N�X�`���ԍ�
				pMeshWall->SetPos(pos);                   // ���b�V���E�H�[���̍��W
				pMeshWall->SetRot(rot);                   // ���b�V���E�H�[���̌���
				pMeshWall->SetCol(col);                   // ���b�V���E�H�[���̐F
				pMeshWall->SetWidth(fWidth);              // 1�u���b�N���̕�
				pMeshWall->SetHeight(fHeight);            // 1�u���b�N���̍���
				pMeshWall->SetXBlock(nXBlock);            // ���̕�����
				pMeshWall->SetYBlock(nYBlock);            // �c�̕�����
				pMeshWall->SetTexSplitU(nTexSplitU);      // �e�N�X�`��U���W�̕�����
				pMeshWall->SetTexSplitV(nTexSplitV);      // �e�N�X�`��V���W�̕�����

				if (FAILED(pMeshWall->Init()))
				{// �������Ɏ��s����
					return NULL;
				}
			}
			else
			{// �C���X�^���X�𐶐��ł��Ȃ�����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ����ȏ�V�[�����쐬�ł��Ȃ�
		return NULL;
	}

	return pMeshWall;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMeshWall::Init(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
			m_nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);                                                               // ���_�����v�Z
			m_nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);  // �C���f�b�N�X����ݒ�
			m_nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);                                          // �`�悷��|���S������ݒ�

			// ���_�o�b�t�@�̐���
			MakeVertex(pDevice);

			// �C���f�b�N�X�o�b�t�@�̐���
			MakeIndex(pDevice);
		}
		else
		{// �f�o�C�X���擾�ł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// �����_�����O�N���X����������Ă��Ȃ�
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CMeshWall::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �C���f�b�N�X�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pIdxBuff);

	// �����[�X����
	CScene::Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CMeshWall::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CMeshWall::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // ���[���h�}�g���b�N�X�̐ݒ菈��
			SetMtxWorld(pDevice);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetIndices(m_pIdxBuff);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_pTexture);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
		}
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�v�Z����
//=============================================================================
void CMeshWall::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CMeshWall::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �����ɕK�v�ȕϐ���錾
	D3DXCOLOR col = GetCol();
	float XPos = -(m_fWidth * m_nXBlock) / 2;  // X���W�����ɐݒ�
	float YPos = (m_fHeight * m_nYBlock) / 2;  // Y���W�����ɐݒ�
	float fTexU = 0.0f;                        // �e�N�X�`����U���W���E��ɐݒ�
	float fTexV = 0.0f;                        // �e�N�X�`����V���W���E��ɐݒ�

	for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
	{// ���������̕����� + 1�����J��Ԃ�
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// ���������̕����� + 1�����J��Ԃ�
		    // ���_���W
			pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, 0.0f);

			// �@���x�N�g��
			pVtx[nCntH].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// ���_�J���[
			pVtx[nCntH].col = col;

			// �e�N�X�`�����W
			pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

			XPos += m_fWidth;                              // X���W�����ɐi�߂�
			fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // �e�N�X�`��U���W��i�߂�
		}
		XPos -= m_fWidth * (m_nXBlock + 1);                               // X���W��i�߂Ă������߂�
		YPos -= m_fHeight;                                                // Y���W�����ɐi�߂�
		fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // �e�N�X�`��U���W��i�߂Ă������߂�
		fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);                     // �e�N�X�`��V���W��i�߂�

		// �|�C���^��i�߂�
		pVtx += m_nXBlock + 1;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@��������
//=============================================================================
void CMeshWall::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;       // �C���f�b�N�X�f�[�^�ւ̃|�C���^
	int nCntIdx = 0;  // �C���f�b�N�X�ԍ�

	// �C���f�b�N�X�o�b�t�@�����b�N��,�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < m_nYBlock; nCntIdxY++)
	{// �c�̕��������J��Ԃ�
		for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
		{// ���̕�����+�P�J��Ԃ�
		 // �㉺�̓���C���f�b�N�X�ԍ���ݒ�
			pIdx[0] = nCntIdx + m_nXBlock + 1; // ����
			pIdx[1] = nCntIdx;                 // �㑤

			pIdx += 2;  // 2���i�߂�
			if (nCntIdxY < m_nYBlock - 1 && nCntIdxX == m_nXBlock)
			{// 1 , �������c�̕������̍ŉ��w�ł͂Ȃ�
			 // 2 , ���̕��������ݒ肪�I�����
				pIdx[0] = nCntIdx;                       // �㑤
				pIdx[1] = nCntIdx + (m_nXBlock + 1) + 1; // ���̉���

				pIdx += 2; // 2���i�߂�
			}
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();
}

//=============================================================================
//    �e�N�X�`���ԍ��ݒ菈��
//=============================================================================
void CMeshWall::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    �e�N�X�`���ݒ菈��
//=============================================================================
void CMeshWall::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    ���_�o�b�t�@�ݒ菈��
//=============================================================================
void CMeshWall::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@�ݒ菈��
//=============================================================================
void CMeshWall::SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff)
{
	m_pIdxBuff = pIdxBuff;
}

//=============================================================================
//    ���b�V���E�H�[���̍��W�ݒ菈��
//=============================================================================
void CMeshWall::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ���b�V���E�H�[���̌����ݒ菈��
//=============================================================================
void CMeshWall::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ���b�V���E�H�[���̐F�ݒ菈��
//=============================================================================
void CMeshWall::SetCol(const D3DXCOLOR col)
{
	// �F��ݒ�
	m_Col = col;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
			    // ���_�J���[
				pVtx[nCntH].col = m_Col;
			}
			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    1�u���b�N���̕��ݒ菈��
//=============================================================================
void CMeshWall::SetWidth(const float fWidth)
{
	// ����ݒ�
	m_fWidth = fWidth;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float XPos = -(m_fWidth * m_nXBlock) / 2; // X���W�����ɐݒ�

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				pVtx[nCntH].pos.x = XPos;

				XPos += m_fWidth;   // X���W�����ɐi�߂�
			}
			XPos -= m_fWidth * (m_nXBlock + 1); // X���W��i�߂Ă������߂�

			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    1�u���b�N���̍����ݒ菈��
//=============================================================================
void CMeshWall::SetHeight(const float fHeight)
{
	// ������ݒ�
	m_fHeight = fHeight;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float YPos = (m_fHeight * m_nYBlock) / 2; // Y���W�����ɐݒ�

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				pVtx[nCntH].pos.y = YPos;
			}
			YPos -= m_fHeight;   // Y���W�����ɐi�߂�

			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ���̕������ݒ菈��
//=============================================================================
void CMeshWall::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;

	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �C���f�b�N�X�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pIdxBuff);

	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
			m_nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);                                                               // ���_�����v�Z
			m_nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);  // �C���f�b�N�X����ݒ�
			m_nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);                                          // �`�悷��|���S������ݒ�

			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&m_pVtxBuff,
				NULL);

			// ���_���̐ݒ�
			VERTEX_3D *pVtx;

			// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// �����ɕK�v�ȕϐ���錾
			D3DXCOLOR col = GetCol();
			float XPos = -(m_fWidth * m_nXBlock) / 2;  // X���W�����ɐݒ�
			float YPos = (m_fHeight * m_nYBlock) / 2;  // Y���W�����ɐݒ�
			float fTexU = 0.0f;                        // �e�N�X�`����U���W���E��ɐݒ�
			float fTexV = 0.0f;                        // �e�N�X�`����V���W���E��ɐݒ�

			for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
			{// ���������̕����� + 1�����J��Ԃ�
				for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
				{// ���������̕����� + 1�����J��Ԃ�
				    // ���_���W
					pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, 0.0f);

					// �@���x�N�g��
					pVtx[nCntH].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// ���_�J���[
					pVtx[nCntH].col = col;

					// �e�N�X�`�����W
					pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

					XPos += m_fWidth;                              // X���W�����ɐi�߂�
					fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // �e�N�X�`��U���W��i�߂�
				}
				XPos -= m_fWidth * (m_nXBlock + 1);                               // X���W��i�߂Ă������߂�
				YPos -= m_fHeight;                                                // Y���W�����ɐi�߂�
				fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // �e�N�X�`��U���W��i�߂Ă������߂�
				fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);                     // �e�N�X�`��V���W��i�߂�

				// �|�C���^��i�߂�
				pVtx += m_nXBlock + 1;
			}

			// ���_�o�b�t�@���A�����b�N����
			m_pVtxBuff->Unlock();

			// �C���f�b�N�X�o�b�t�@�̐���
			MakeIndex(pDevice);
		}
	}
}

//=============================================================================
//    �c�̕������ݒ菈��
//=============================================================================
void CMeshWall::SetYBlock(const int nYBlock)
{
	m_nYBlock = nYBlock;

	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �C���f�b�N�X�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
			m_nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);                                                               // ���_�����v�Z
			m_nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);  // �C���f�b�N�X����ݒ�
			m_nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);                                          // �`�悷��|���S������ݒ�

			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&m_pVtxBuff,
				NULL);

			// ���_���̐ݒ�
			VERTEX_3D *pVtx;

			// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// �����ɕK�v�ȕϐ���錾
			D3DXCOLOR col = GetCol();
			float XPos = -(m_fWidth * m_nXBlock) / 2;  // X���W�����ɐݒ�
			float YPos = (m_fHeight * m_nYBlock) / 2;  // Y���W�����ɐݒ�
			float fTexU = 0.0f;                        // �e�N�X�`����U���W���E��ɐݒ�
			float fTexV = 0.0f;                        // �e�N�X�`����V���W���E��ɐݒ�

			for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
			{// ���������̕����� + 1�����J��Ԃ�
				for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
				{// ���������̕����� + 1�����J��Ԃ�
				 // ���_���W
					pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, 0.0f);

					// �@���x�N�g��
					pVtx[nCntH].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// ���_�J���[
					pVtx[nCntH].col = col;

					// �e�N�X�`�����W
					pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

					XPos += m_fWidth;                              // X���W�����ɐi�߂�
					fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // �e�N�X�`��U���W��i�߂�
				}
				XPos -= m_fWidth * (m_nXBlock + 1);                               // X���W��i�߂Ă������߂�
				YPos -= m_fHeight;                                                // Y���W�����ɐi�߂�
				fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // �e�N�X�`��U���W��i�߂Ă������߂�
				fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);                     // �e�N�X�`��V���W��i�߂�

				// �|�C���^��i�߂�
				pVtx += m_nXBlock + 1;
			}

			// ���_�o�b�t�@���A�����b�N����
			m_pVtxBuff->Unlock();

			// �C���f�b�N�X�o�b�t�@�̐���
			MakeIndex(pDevice);
		}
	}
}

//=============================================================================
//    �e�N�X�`��U���W�̕���������
//=============================================================================
void CMeshWall::SetTexSplitU(const int nTexSplitU)
{
	m_nTexSplitU = nTexSplitU;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fTexU = 0.0f;   // �e�N�X�`��U���W

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				pVtx[nCntH].tex.x = fTexU;
				fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;
			}
			fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1);

			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �e�N�X�`��V���W�̕������ݒ菈��
//=============================================================================
void CMeshWall::SetTexSplitV(const int nTexSplitV)
{
	m_nTexSplitV = nTexSplitV;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fTexV = 0.0f;   // �e�N�X�`��V���W

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				pVtx[nCntH].tex.y = fTexV;
			}
			fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);

			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CMeshWall::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    �e�N�X�`���ԍ��擾����
//=============================================================================
int CMeshWall::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    ���_�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshWall::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DINDEXBUFFER9 CMeshWall::GetIdxBuff(void)
{
	return m_pIdxBuff;
}

//=============================================================================
//    �e�N�X�`���擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CMeshWall::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    ���b�V���t�B�[���h�̍��W�擾����
//=============================================================================
D3DXVECTOR3 CMeshWall::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ���b�V���t�B�[���h�̌����擾����
//=============================================================================
D3DXVECTOR3 CMeshWall::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ���b�V���t�B�[���h�̐F�擾����
//=============================================================================
D3DXCOLOR CMeshWall::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    1�u���b�N���̕��擾����
//=============================================================================
float CMeshWall::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    1�u���b�N���̍����擾����
//=============================================================================
float CMeshWall::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    ���̕������擾����
//=============================================================================
int CMeshWall::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    ���s�̕������擾����
//=============================================================================
int CMeshWall::GetYBlock(void)
{
	return m_nYBlock;
}

//=============================================================================
//    �e�N�X�`��U���W�̕������擾����
//=============================================================================
int CMeshWall::GetTexSplitU(void)
{
	return m_nTexSplitU;
}

//=============================================================================
//    �e�N�X�`��V���W�̕������擾����
//=============================================================================
int CMeshWall::GetTexSplitV(void)
{
	return m_nTexSplitV;
}

//=============================================================================
//    ���_���擾����
//=============================================================================
int CMeshWall::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    �C���f�b�N�X���擾����
//=============================================================================
int CMeshWall::GetNumIndex(void)
{
	return m_nNumIndex;
}

//=============================================================================
//    �|���S�����擾����
//=============================================================================
int CMeshWall::GetNumPolygon(void)
{
	return m_nNumPolygon;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CMeshWall::GetMtxWorld(void)
{
	return m_MtxWorld;
}


//*****************************************************************************
//    CSetMeshWall�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CSetMeshWall::CSetMeshWall(int nPriority, OBJTYPE objType) : CMeshWall(nPriority, objType)
{
	// �e��l�̃N���A
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CSetMeshWall::~CSetMeshWall()
{

}

//=============================================================================
//    ��������
//=============================================================================
CSetMeshWall *CSetMeshWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx, int nPriority)
{
	CSetMeshWall *pSetMeshWall = NULL;     // �z�u�p���b�V���E�H�[���N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pSetMeshWall == NULL)
		{// ����������ɂȂ��Ă���
			pSetMeshWall = new CSetMeshWall(nPriority);
			if (pSetMeshWall != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pSetMeshWall->SetTexIdx(nTexIdx);            // �z�u�p���b�V���E�H�[���̃e�N�X�`���ԍ�
				pSetMeshWall->SetPos(pos);                   // �z�u�p���b�V���E�H�[���̍��W
				pSetMeshWall->SetRot(rot);                   // �z�u�p���b�V���E�H�[���̌���
				pSetMeshWall->SetCol(col);                   // �z�u�p���b�V���E�H�[���̐F
				pSetMeshWall->SetWidth(fWidth);              // 1�u���b�N���̕�
				pSetMeshWall->SetHeight(fHeight);            // 1�u���b�N���̍���
				pSetMeshWall->SetXBlock(nXBlock);            // ���̕�����
				pSetMeshWall->SetYBlock(nYBlock);            // �c�̕�����
				pSetMeshWall->SetTexSplitU(nTexSplitU);      // �e�N�X�`��U���W�̕�����
				pSetMeshWall->SetTexSplitV(nTexSplitV);      // �e�N�X�`��V���W�̕�����

				if (FAILED(pSetMeshWall->Init()))
				{// �������Ɏ��s����
					return NULL;
				}
			}
			else
			{// �C���X�^���X�𐶐��ł��Ȃ�����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ����ȏ�V�[�����쐬�ł��Ȃ�
		return NULL;
	}

	return pSetMeshWall;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CSetMeshWall::Init(void)
{
	// �F�𓧖��ɂ��Ă���
	D3DXCOLOR col = GetCol();
	col.a = -0.7f;
	SetCol(col);

	// ���ʂ̏���������
	if (FAILED(CMeshWall::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CSetMeshWall::Uninit(void)
{
	// ���ʂ̏I������
	CMeshWall::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CSetMeshWall::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CSetMeshWall::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			DWORD Lighting;

			// ���C�e�B���O�����Ȃ��ݒ��
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// ���ʂ̕`�揈��
			CMeshWall::Draw();

			// ���C�e�B���O�̐ݒ��߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
		}
	}
}