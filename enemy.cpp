//*****************************************************************************
//
//     �G�l�~�[�̏���[enemy.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "modelManager.h"
#include "motion.h"
#include "gauge.h"
//#include "score.h"
//#include "ui.h"
#include "meshField.h"
#include "object.h"
#include "functionlib.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
// �G�̃f�[�^�ݒ�p�}�N��
#define ENEMY_GAUGE_POS (D3DXVECTOR3(0.0f,90.0f,0.0f))    // �G�̃Q�[�W�I�t�Z�b�g�ʒu


// �l�ǂݍ��݂�����ۂ̖ڈ�ƂȂ镶����
// ����
#define POS                "POS = "                // ���W��ǂݎ�鍇�}
#define ROT                "ROT = "                // ������ǂݎ�鍇�}
#define MOVE               "MOVE = "               // �ړ��ʂ�ǂݎ�鍇�}

#define NUM_MODEL          "NUM_MODEL = "          // �ǂݍ��ރ��f���̐�
#define MODEL_FILENAME     "MODEL_FILENAME = "     // �ǂݍ��ރ��f���̃t�@�C����

// �L�����N�^�[���
#define CHARACTERSET       "CHARACTERSET"          // �L�����N�^�[���ǂݍ��݊J�n�̍��}
#define END_CHARACTERSET   "END_CHARACTERSET"      // �L�����N�^�[���ǂݍ��ݏI���̍��}
#define JUMP               "JUMP = "               // �W�����v��
#define GRAVITY            "GRAVITY = "            // �d�͗�
#define COL_HEIGHT         "COL_HEIGHT = "         // �����蔻����Ƃ鍂��
#define COL_RANGE          "COL_RANGE = "          // �����蔻����Ƃ�͈�
#define NUM_PARTS          "NUM_PARTS = "          // �L�����N�^�[�̃p�[�c��

// �p�[�c���
#define PARTSSET           "PARTSSET"              // �p�[�c���ǂݍ��݊J�n�̍��}
#define END_PARTSSET       "END_PARTSSET"          // �p�[�c���ǂݍ��ݏI���̍��}
#define INDEX              "INDEX = "              // �ԍ���ǂݎ�鍇�}
#define PARENT             "PARENT = "             // �e�p�[�c�̔ԍ���ǂݎ�鍇�}

// ���[�V�������
#define MOTIONSET          "MOTIONSET"             // ���[�V�������ǂݍ��݊J�n�̍��}
#define END_MOTIONSET      "END_MOTIONSET"         // ���[�V�������ǂݍ��ݏI���̍��}
#define LOOP               "LOOP = "               // ���[�V���������[�v�Đ����邩�ǂ�����ǂݎ�鍇�}
#define BLEND              "BLEND = "              // ���[�V�����u�����h�̔{����ǂݎ�鍇�}
#define NUM_KEY            "NUM_KEY = "            // ���[�V�����̃L�[�t���[���̐���ǂݎ�鍇�}

// �L�[�t���[�����
#define KEYSET             "KEYSET"                // �L�[�t���[�����ǂݍ��݊J�n�̍��}
#define END_KEYSET         "END_KEYSET"            // �L�[�t���[�����ǂݍ��ݏI���̍��}
#define FRAME              "FRAME = "              // �L�[�t���[���̍Đ��t���[������ǂݎ�鍇�}

// �L�[���
#define KEY                "KEY"                   // �L�[���ǂݍ��݊J�n�̍��}
#define END_KEY            "END_KEY"               // �L�[���ǂݍ��ݏI���̍��}

//*****************************************************************************
//     CEnemyManager�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEnemyManager::CEnemyManager(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	strcpy(m_aFileName, "\0");      // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����
	m_pModelManager = NULL;         // ���f���Ǌ��N���X�ւ̃|�C���^

	m_nNumModel = 0;                // �ǂݍ��ރ��f����
	m_nType = 0;                    // ���
	m_nNumParts = 0;                // �ǂݍ��ރp�[�c��
	m_pNumParent = NULL;            // �e���f���̔ԍ�
	m_pMotionManager = NULL;        // ���[�V�����Ǘ��N���X�ւ̃|�C���^
	m_apModel = NULL;               // ���f���N���X�ւ̃|�C���^
	m_fGravity = 0.0f;              // �G�̏d�͏��
	m_fColHeight = 0.0f;            // �����蔻�����鍂��
	m_fColRange = 0.0f;             // �����蔻������͈�
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEnemyManager::~CEnemyManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CEnemyManager *CEnemyManager::Create(int nType, char *pFileName, int nPriority)
{
	CEnemyManager *pEnemyManager = NULL;   // �G�l�~�[�Ǌ��N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pEnemyManager == NULL)
		{// ����������ɂȂ��Ă���
			pEnemyManager = new CEnemyManager(nPriority);
			if (pEnemyManager != NULL)
			{// �C���X�^���X�𐶐��ł���
				pEnemyManager->SetFileName(pFileName);
				pEnemyManager->SetType(nType);
				if (FAILED(pEnemyManager->Init()))
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

	return pEnemyManager;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CEnemyManager::Init(void)
{
	// �G�l�~�[�̃p�[�c���𔲂��o��
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(m_aFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �X�N���v�g�ǂݍ��ݗp�̕ϐ���p��
			char *pLine = NULL;                         // 1�s���ǂݎ��p
			char *pStrCur = NULL;                       // ���݂̐擪�|�C���^
			char *pStr = NULL;                          // �擪�|�C���^�ۑ��p
			int nCntModel = 0;                          // ���f����ǂݍ��񂾉񐔂𐔂���
			int nCntParts = 0;                          // �p�[�c����ǂݍ��񂾉񐔂𐔂���
			int nCntMotion = 0;                         // ���[�V������ǂݍ��񂾉񐔂𐔂���
			LPD3DXMESH pMesh = NULL;                    // ���b�V�����ւ̃|�C���^
			LPD3DXBUFFER pBuffMat = NULL;               // �}�e���A�����ւ̃|�C���^
			DWORD nNumMat = 0;                          // �}�e���A�����̐�
			CMotion *pMotion[CEnemy::STATE_MAX] = {};   // ���[�V�����N���X�ւ̃|�C���^

			if (pLine == NULL && pStr == NULL)
			{// ���������m�ۂł����Ԃł���
				pLine = new char[256];
				pStr = new char[256];
				strcpy(pLine, "\0");
				strcpy(pStr, "\0");
				if (pLine != NULL && pStr != NULL)
				{// ���������m�ۂ���Ă���
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
							{// ���f������ǂݍ��ނȂ�
								strcpy(pStr, pStrCur);
								m_nNumModel = CFunctionLib::ReadInt(pStrCur, NUM_MODEL);
								if (m_nNumModel >= 1)
								{// ���f������1�ȏ゠��
									m_pNumParent = new int[m_nNumModel];
									m_pModelManager = CModelManager::Create(m_nNumModel);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
							{// ���f���̃t�@�C���p�X����ǂݍ���
								if (m_pModelManager != NULL && nCntModel < m_nNumModel)
								{// ���f���Ǌ��N���X�ւ̃|�C���^���m�ۂ���Ă���
									// ���f���̃t�@�C���p�X����ǂݎ��
									pStr = CFunctionLib::ReadString(pStrCur, pStr, MODEL_FILENAME);

									// x�t�@�C���̓ǂݍ���
									D3DXLoadMeshFromX(pStr,
										D3DXMESH_SYSTEMMEM,
										CManager::GetRenderer()->GetDevice(),
										NULL,
										&pBuffMat,
										NULL,
										&nNumMat,
										&pMesh);

									// ���f���Ǌ��N���X�Ƀ|�C���^��ݒ肷��
									m_pModelManager->SetMesh(pMesh, nCntModel);
									m_pModelManager->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);

									// �g�p�����ϐ������������Ă���
									pMesh = NULL;
									pBuffMat = NULL;
									nNumMat = 0;

									nCntModel++;  // ���f����ǂݍ��񂾉񐔂𑝂₷
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, CHARACTERSET) == 0)
							{// �L�����N�^�[���ǂݍ��݊J�n�̍��}��������
								m_apModel = ReadCharacterSet(pLine, pStrCur, pFile, &m_nNumParts, m_pNumParent, &m_fGravity, &m_fColHeight, &m_fColRange);
								nCntParts++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOTIONSET) == 0)
							{// ���[�V�������ǂݍ��݊J�n�̍��}��������
								pMotion[nCntMotion] = ReadMotionSet(pLine, pStrCur, pFile, &m_nNumParts, pMotion[nCntMotion], nCntMotion);
								nCntMotion++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}
				}
			}

			// �������̊J��
			if (pStr != NULL)
			{// ���������m�ۂ���Ă���
				delete[] pStr;
				pStr = NULL;
			}
			if (pLine != NULL)
			{// ���������m�ۂ���Ă���
				delete[] pLine;
				pLine = NULL;
			}

			// �K�v�ȃN���X���쐬���Ă���
			// ���[�V�����Ǌ��N���X
			if (m_pMotionManager == NULL)
			{// �|�C���^���m�ۂ���Ă���
				m_pMotionManager = CMotionManager::Create(CEnemy::STATE_MAX, m_nNumParts);
				if (m_pMotionManager != NULL)
				{// ���[�V�����Ǘ��N���X���쐬�ł���
					for (int nCntMotion = 0; nCntMotion < CEnemy::STATE_MAX; nCntMotion++)
					{// ��Ԃ̐������J�肩����
						m_pMotionManager->SetMotion(pMotion[nCntMotion], nCntMotion);
					}
				}
			}

			// �t�@�C�������
			fclose(pFile);
		}
		else
		{// �t�@�C���|�C���^���m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// �t�@�C���|�C���^���m�ۂł����ԂłȂ�
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CEnemyManager::Uninit(void)
{
	// �ǂݎ��p�̃|�C���^���J�����Ă���
	if (m_apModel != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// �g�p�ł��郂�f�������J��Ԃ�
			if (m_apModel[nCntModel] != NULL)
			{// �|�C���^���m�ۂ���Ă���
				m_apModel[nCntModel]->Uninit();

				// �������̊J��
				delete m_apModel[nCntModel];
				m_apModel[nCntModel] = NULL;
			}
		}
		// �������̊J��
		delete[] m_apModel;
		m_apModel = NULL;
	}

	// ���[�V�����̔j��
	if (m_pMotionManager != NULL)
	{// �|�C���^���m�ۂ���Ă���
		m_pMotionManager->Uninit();

		// �������̊J��
		delete m_pMotionManager;
		m_pMotionManager = NULL;
	}

	// �e���f���̔ԍ�
	if (m_pNumParent != NULL)
	{// ���������m�ۂ���Ă���
		delete[] m_pNumParent;
		m_pNumParent = NULL;
	}

	// ���f���Ǌ��N���X�̔j��
	if (m_pModelManager != NULL)
	{// ���������m�ۂ���Ă���
	    // �I������
		m_pModelManager->Uninit();

		// �������̊J��
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	// ���g�̃|�C���^��j��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CEnemyManager::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CEnemyManager::Draw(void)
{

}

//=============================================================================
//    �G��z�u���鏈��
//=============================================================================
void CEnemyManager::SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nLife, int nAddScore)
{
	// �G�l�~�[�N���X���쐬����
	CreateEnemy(m_apModel, m_pMotionManager, m_nNumParts, m_pNumParent, m_fGravity, m_fColHeight, m_fColRange, pos, rot, nType, nLife, nAddScore);
}

//=============================================================================
//    �}�b�v�G�f�B�b�g�p�̓G��z�u���鏈��
//=============================================================================
CSetEnemy *CEnemyManager::SetMapEditEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nLife, int nAddScore)
{
	// �G�l�~�[�N���X���쐬����
	CSetEnemy *pSetEnemy = NULL;
	return pSetEnemy = CreateSetEnemy(m_apModel, m_pMotionManager, m_nNumParts, m_pNumParent, m_fGravity, m_fColHeight, m_fColRange, pos, rot, nType, nLife, nAddScore);
}

//=============================================================================
//    �G�l�~�[�N���X���쐻���鏈��
//=============================================================================
CEnemy *CEnemyManager::CreateEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nType, const int nLife, const int nAddScore)
{
	CModel **apModelCpy = NULL;                    // ���f���N���X�R�s�[�p
	CMotion *pMotionCpy[CEnemy::STATE_MAX] = {};   // ���[�W�����N���X�R�s�[�p
	CMotionManager *pMotionManagerCpy = NULL;      // ���[�V�����Ǌ��N���X�R�s�[�p
	CEnemy *pEnemy = NULL;                         // �G�l�~�[�N���X�ւ̃|�C���^

	if (apModel != NULL && pMotionManager != NULL && m_pModelManager != NULL)
	{// ���f���ւ̃|�C���^�ƃ��[�V�����N���X�ւ̃|�C���^�ƃ��f���Ǌ��N���X�ւ̃|�C���^���쐬����Ă���
	    // ���f�������R�s�[����
		if (apModelCpy == NULL)
		{// ���������m�ۂ���Ă���
			apModelCpy = new CModel*[nNumParts];
			if (apModelCpy != NULL)
			{// ���������m�ۂł���
				for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
				{// �p�[�c�����J��Ԃ�
					apModelCpy[nCntParts] = NULL;
					if (apModelCpy[nCntParts] == NULL)
					{// ���������m�ۂł����Ԃł���
						apModelCpy[nCntParts] = CModel::Create(apModel[nCntParts]->GetPos(), apModel[nCntParts]->GetRot(), m_pModelManager->GetMesh(nCntParts), m_pModelManager->GetBuffMat(nCntParts), m_pModelManager->GetNumMat(nCntParts), m_pModelManager->GetVtxMax(nCntParts), m_pModelManager->GetVtxMin(nCntParts), m_pModelManager->GetTexture(nCntParts));
						if (apModelCpy[nCntParts] != NULL)
						{// ���������m�ۂł���
							if (pNumParent[nCntParts] != -1)
							{// �e���f�������݂���
								apModelCpy[nCntParts]->SetParent(apModelCpy[pNumParent[nCntParts]]);
							}
						}
					}
				}
			}
		}

		// ���[�V���������R�s�[����
		if (pMotionManagerCpy == NULL)
		{// ���������m�ۂł����Ԃł���
			pMotionManagerCpy = CMotionManager::Create(CEnemy::STATE_MAX, nNumParts);
			if (pMotionManagerCpy != NULL)
			{// ���[�V�����Ǘ��N���X���쐬�ł���
				for (int nCntMotion = 0; nCntMotion < CEnemy::STATE_MAX; nCntMotion++)
				{// �G�̏�Ԃ̐������J��Ԃ�
					if (pMotionCpy[nCntMotion] == NULL)
					{// ���������m�ۂł����Ԃł���
						pMotionCpy[nCntMotion] = CMotion::Create(nNumParts, pMotionManager->GetMotion()[nCntMotion]->GetNumKey(), pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(), pMotionManager->GetMotion()[nCntMotion]->GetLoop(), pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());
						if (pMotionCpy[nCntMotion] != NULL)
						{// ���������m�ۂł���
							for (int nCntKey = 0; nCntKey < pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
							{// �L�[�t���[���̑������J��Ԃ�
								for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
								{// �L�[�t���[���̑������J��Ԃ�
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
								}
							}

							// ���[�V�����Ǌ��N���X�Ƀ|�C���^��n��
							pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
						}
					}
				}
			}
		}

		// �G�l�~�[�N���X���쐬����
		if (pEnemy == NULL)
		{// ���������m�ۂł����Ԃł���
			pEnemy = CEnemy::Create(pos, rot, (CEnemy::TYPE)nType, apModelCpy, pMotionManagerCpy, nNumParts, fGravity, fColHeight, fColRange, nLife, nAddScore);
		}
	}

	return pEnemy;
}

//=============================================================================
//    �z�u�p�̃G�l�~�[�N���X���쐻���鏈��
//=============================================================================
CSetEnemy *CEnemyManager::CreateSetEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nType, const int nLife, const int nAddScore)
{
	CModel **apModelCpy = NULL;                    // ���f���N���X�R�s�[�p
	CMotion *pMotionCpy[CEnemy::STATE_MAX] = {};   // ���[�W�����N���X�R�s�[�p
	CMotionManager *pMotionManagerCpy = NULL;      // ���[�V�����Ǌ��N���X�R�s�[�p
	CSetEnemy *pSetEnemy = NULL;                   // �z�u�p�G�l�~�[�N���X�ւ̃|�C���^

	if (apModel != NULL && pMotionManager != NULL && m_pModelManager != NULL)
	{// ���f���ւ̃|�C���^�ƃ��[�V�����N���X�ւ̃|�C���^�ƃ��f���Ǌ��N���X�ւ̃|�C���^���쐬����Ă���
	    // ���f�������R�s�[����
		if (apModelCpy == NULL)
		{// ���������m�ۂ���Ă���
			apModelCpy = new CModel*[nNumParts];
			if (apModelCpy != NULL)
			{// ���������m�ۂł���
				for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
				{// �p�[�c�����J��Ԃ�
					apModelCpy[nCntParts] = NULL;
					if (apModelCpy[nCntParts] == NULL)
					{// ���������m�ۂł����Ԃł���
						apModelCpy[nCntParts] = CModel::Create(apModel[nCntParts]->GetPos(), apModel[nCntParts]->GetRot(), m_pModelManager->GetMesh(nCntParts), m_pModelManager->GetBuffMat(nCntParts), m_pModelManager->GetNumMat(nCntParts), m_pModelManager->GetVtxMax(nCntParts), m_pModelManager->GetVtxMin(nCntParts), m_pModelManager->GetTexture(nCntParts));
						if (apModelCpy[nCntParts] != NULL)
						{// ���������m�ۂł���
							if (pNumParent[nCntParts] != -1)
							{// �e���f�������݂���
								apModelCpy[nCntParts]->SetParent(apModelCpy[pNumParent[nCntParts]]);
							}
						}
					}
				}
			}
		}

		// ���[�V���������R�s�[����
		if (pMotionManagerCpy == NULL)
		{// ���������m�ۂł����Ԃł���
			pMotionManagerCpy = CMotionManager::Create(CEnemy::STATE_MAX, nNumParts);
			if (pMotionManagerCpy != NULL)
			{// ���[�V�����Ǘ��N���X���쐬�ł���
				for (int nCntMotion = 0; nCntMotion < CEnemy::STATE_MAX; nCntMotion++)
				{// �G�̏�Ԃ̐������J��Ԃ�
					if (pMotionCpy[nCntMotion] == NULL)
					{// ���������m�ۂł����Ԃł���
						pMotionCpy[nCntMotion] = CMotion::Create(nNumParts, pMotionManager->GetMotion()[nCntMotion]->GetNumKey(), pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(), pMotionManager->GetMotion()[nCntMotion]->GetLoop(), pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());
						if (pMotionCpy[nCntMotion] != NULL)
						{// ���������m�ۂł���
							for (int nCntKey = 0; nCntKey < pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
							{// �L�[�t���[���̑������J��Ԃ�
								for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
								{// �L�[�t���[���̑������J��Ԃ�
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
								}
							}

							// ���[�V�����Ǌ��N���X�Ƀ|�C���^��n��
							pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
						}
					}
				}
			}
		}

		// �z�u�p�G�l�~�[�N���X���쐬����
		if (pSetEnemy == NULL)
		{// ���������m�ۂł����Ԃł���
			pSetEnemy = CSetEnemy::Create(pos, rot, (CEnemy::TYPE)nType, apModelCpy, pMotionManagerCpy, nNumParts, fGravity, fColHeight, fColRange, nLife, nAddScore);
		}
	}

	return pSetEnemy;
}

//=============================================================================
//    �G�l�~�[����ǂݎ�鏈��
//=============================================================================
CModel **CEnemyManager::ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pGravity, float *pColHeight, float *pColRange)
{
	int nCntParts = 0;
	int nNumParts = 0;
	CModel **apModel = NULL;
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, GRAVITY) == 0)
		{// �d�͗ʂ�����
			*pGravity = CFunctionLib::ReadFloat(pStrCur, GRAVITY);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_HEIGHT) == 0)
		{//	�����蔻�����鍂��������
			*pColHeight = CFunctionLib::ReadFloat(pStrCur, COL_HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
		{// �����蔻������͈͂�����
			*pColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, NUM_PARTS) == 0)
		{// �p�[�c����񂪂���
			*pNumParts = (CFunctionLib::ReadInt(pStrCur, NUM_PARTS));
			if (apModel == NULL)
			{// ���������m�ۂł����Ԃł���
				apModel = new CModel*[*pNumParts];
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARTSSET) == 0)
		{// �p�[�c���ǂݍ��݊J�n�̍��}������
			if (nCntParts < *pNumParts)
			{// �܂��p�[�c����ǂݍ��߂�
				apModel[nCntParts] = ReadPartsSet(pLine, pStrCur, pFile, pNumParts, pNumParent, apModel);
				nCntParts++;
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_CHARACTERSET) == 0)
		{// �L�����N�^�[���ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	return apModel;
}

//=============================================================================
//    �p�[�c����ǂݎ�鏈��
//=============================================================================
CModel *CEnemyManager::ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel)
{
	int nIdx = 0;                                      // �p�[�c�ԍ����
	int nParent = 0;                                   // �e���f���̃p�[�c�ԍ�
	CModel *pModel = NULL;                             // ���f���N���X�ւ̃|�C���^
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // ���f���̍��W���
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // ���f���̌������
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
		{// �p�[�c�ԍ���񂪂���
			nIdx = CFunctionLib::ReadInt(pStrCur, INDEX);
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARENT) == 0)
		{// �e���f���̃p�[�c�ԍ���񂪂���
			pNumParent[nIdx] = CFunctionLib::ReadInt(pStrCur, PARENT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���f���̍��W��񂪂���
			Pos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ���f���̌�����񂪂���
			Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_PARTSSET) == 0)
		{// �p�[�c���ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	if (pModel == NULL)
	{// ���������m�ۂł����Ԃł���
		pModel = CModel::Create(Pos, Rot, m_pModelManager->GetMesh(nIdx), m_pModelManager->GetBuffMat(nIdx), m_pModelManager->GetNumMat(nIdx), m_pModelManager->GetVtxMax(nIdx), m_pModelManager->GetVtxMin(nIdx),m_pModelManager->GetTexture(nIdx));
		if (pModel != NULL)
		{// ���f���|�C���^���m�ۂ��ꂽ
			if (nParent != -1)
			{// �e���f�������݂���
				pModel->SetParent(apModel[nParent]);
			}
		}
	}

	return pModel;
}

//=============================================================================
//    ���[�V��������ǂݎ�鏈��
//=============================================================================
CMotion *CEnemyManager::ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion)
{
	int nCntKeySet = 0;              // �L�[�t���[���t���[������ǂݍ��񂾉�
	bool bLoop = false;              // ���[�v���邩���Ȃ���
	float fBlendMag = 0.0f;          // �u�����h�{��
	int nNumKey = 0;                 // �L�[�t���[���̑���
	int *pPlaybackKey = NULL;        // �Đ��t���[����(��������̂œ��I�m��)
	D3DXVECTOR3 **pAddPos = NULL;    // ���f���̍��W
	D3DXVECTOR3 **pDestRot = NULL;   // ���f���̌���
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, LOOP) == 0)
		{// ���[�v���邩���Ȃ����̏�񂪂���
			bLoop = CFunctionLib::ReadBool(pStrCur, LOOP);
		}
		else if (CFunctionLib::Memcmp(pStrCur, BLEND) == 0)
		{// �u�����h�{���̏�񂪂���
			fBlendMag = CFunctionLib::ReadFloat(pStrCur, BLEND);
		}
		else if (CFunctionLib::Memcmp(pStrCur, NUM_KEY) == 0)
		{// �L�[�t���[���̑����̏�񂪂���
			nNumKey = (CFunctionLib::ReadInt(pStrCur, NUM_KEY));
			if (nNumKey >= 1)
			{// 1�ȏ�L�[�t���[��������
			 // �K�v�ȕϐ��𓮓I�Ɋm�ۂ���
			 // �Đ��t���[����
				pPlaybackKey = new int[nNumKey];

				// ���W
				if (pAddPos == NULL)
				{// ���������m�ۂł����Ԃł���
					pAddPos = new D3DXVECTOR3*[nNumKey];
					if (pAddPos != NULL)
					{// ���������m�ۂł���
						for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
						{// �L�[�t���[���̑������J��Ԃ�
							pAddPos[nCntKey] = new D3DXVECTOR3[*pNumParts];
						}
					}
				}

				if (pDestRot == NULL)
				{// ���������m�ۂł����Ԃł���
				 // ����
					pDestRot = new D3DXVECTOR3*[nNumKey];
					if (pDestRot != NULL)
					{// ���������m�ۂł���
						for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
						{// �L�[�t���[���̑������J��Ԃ�
							pDestRot[nCntKey] = new D3DXVECTOR3[*pNumParts];
						}
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, KEYSET) == 0)
		{// �L�[�t���[�����ǂݍ��݊J�n�̍��}������
			ReadKeySet(pLine, pStrCur, pFile, nCntKeySet, pPlaybackKey, pAddPos[nCntKeySet], pDestRot[nCntKeySet]);
			nCntKeySet++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_MOTIONSET) == 0)
		{// ���[�V�������ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	if (pMotion == NULL)
	{// ���������m�ۂł����Ԃł���
		pMotion = CMotion::Create(*pNumParts, nNumKey, fBlendMag, bLoop, pPlaybackKey);
		if (pMotion != NULL)
		{// ���������m�ۂł���
			for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
			{// �L�[�t���[���̑������J��Ԃ�
				for (int nCntParts = 0; nCntParts < *pNumParts; nCntParts++)
				{// �L�[�t���[���̑������J��Ԃ�
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pAddPos[nCntKey][nCntParts]);
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pDestRot[nCntKey][nCntParts]);
				}
			}
		}
	}

	// ���������m�ۂ������J������
	// �Đ��t���[����
	if (pPlaybackKey != NULL)
	{// ���������m�ۂ���Ă���
		delete[] pPlaybackKey;
		pPlaybackKey = NULL;
	}

	// ���W
	if (pAddPos != NULL)
	{// ���������m�ۂł���
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// �L�[�t���[���̑������J��Ԃ�
			delete[] pAddPos[nCntKey];
			pAddPos[nCntKey] = NULL;
		}
		delete[] pAddPos;
		pAddPos = NULL;
	}

	// ����
	if (pDestRot != NULL)
	{// ���������m�ۂł���
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// �L�[�t���[���̑������J��Ԃ�
			delete[] pDestRot[nCntKey];
			pDestRot[nCntKey] = NULL;
		}
		delete[] pDestRot;
		pDestRot = NULL;
	}

	return pMotion;
}

//=============================================================================
//    �L�[�t���[������ǂݎ�鏈��
//=============================================================================
void CEnemyManager::ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	int nCntKey = 0;
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, FRAME) == 0)
		{// �Đ��t���[����������
			pPlaybackKey[nCntKeySet] = CFunctionLib::ReadInt(pStrCur, FRAME);
		}
		if (CFunctionLib::Memcmp(pStrCur, KEY) == 0)
		{// �L�[���ǂݍ��݊J�n�̍��}������
			ReadKey(pLine, pStrCur, pFile, nCntKey, AddPos, DestRot);
			nCntKey++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_KEYSET) == 0)
		{// �L�[�t���[�����ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �L�[����ǂݎ�鏈��
//=============================================================================
void CEnemyManager::ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���W������
			AddPos[nCntKey] = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ����������
			DestRot[nCntKey] = CFunctionLib::ReadVector3(pStrCur, ROT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_KEY) == 0)
		{// �L�[���ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �X�N���v�g�t�@�C������ݒ肷�鏈��
//=============================================================================
void CEnemyManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    �G�̎�ނ�ݒ肷�鏈��
//=============================================================================
void CEnemyManager::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CEnemyManager::SetModelManager(CModelManager *pModelManager)
{
	m_pModelManager = pModelManager;
}

//=============================================================================
//    �X�N���v�g�t�@�C�������擾���鏈��
//=============================================================================
char *CEnemyManager::GetFileName(void)
{
	return m_aFileName;
}

//=============================================================================
//    �G�̎�ނ��擾���鏈��
//=============================================================================
int CEnemyManager::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CModelManager *CEnemyManager::GetModelManager(void)
{
	return m_pModelManager;
}


//*****************************************************************************
//     CEnemy�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	// �e��l�̐ݒ�
	m_Type = TYPE_NORMAL;                      // ���
	m_State = STATE_NORMAL;                    // ���
	m_nLife = 0;                               // �̗�
	m_nAddScore = 0;                           // �|�������ɑ�����X�R�A��
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �ړ���
	m_pGaugeLife = NULL;                       // �̗̓Q�[�W
	m_PosStd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �f�t�H���g�̈ʒu
	m_RotStd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �f�t�H���g�̌���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
//    ��������
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife, int nAddScore, int nPriority)
{
	CEnemy *pEnemy = NULL;                 // �G�l�~�[�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pEnemy == NULL)
		{// ����������ɂȂ��Ă���
			pEnemy = new CEnemy(nPriority);
			if (pEnemy != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pEnemy->Init(pos, rot, type, apModel, pMotionManager, nNumParts, fGravity, fColHeight, fColRange, nLife, nAddScore)))
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

	return pEnemy;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife, int nAddScore)
{
	// �e��l�̐ݒ�
	SetPos(pos);                       // ���݂̍��W
	SetRot(rot);                       // ���݂̌���
	SetType(type);                     // �G�̎��
	SetNumParts(nNumParts);            // �p�[�c��
	SetModel(apModel);                 // ���f���N���X�ւ̃|�C���^
	SetMotionManager(pMotionManager);  // ���[�V�����Ǌ��N���X�ւ̃|�C���^
	SetGravity(fGravity);              // �d��
	SetColHeight(fColHeight);          // �����蔻�����鍂��
	SetColRange(fColRange);            // �����蔻������͈�
	SetLife(nLife);                    // �̗�
	SetAddScore(nAddScore);            // �|�������ɑ�����X�R�A��
	m_PosStd = pos;                    // �������̍��W
	m_RotStd = rot;                    // �������̌���

	// �̗̓Q�[�W�����
	if (m_pGaugeLife == NULL)
	{
		m_pGaugeLife = CGaugeLife::Create(ENEMY_GAUGE_POS + pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, 40.0f, 8.0f, m_nLife);
	}

	// ���f���̈ʒu���j���[�g�����̈ʒu�ɂ���
	SetDefaultPos();

	// ���f���̌������j���[�g�����̌����ɂ���
	SetDefaultRot();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	// �̗̓Q�[�W�̊J��
	if (m_pGaugeLife != NULL)
	{
		m_pGaugeLife->Uninit();
		m_pGaugeLife = NULL;
	}

	// ���ʂ̏I������
	CCharacter::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	// �O�����Ă����I�u�W�F�N�g�ւ̃|�C���^��ۑ�
	SetLandSceneOld(GetLandScene());

	// ����Ă���I�u�W�F�N�g�ւ̃|�C���^����ɂ���
	SetLandScene(NULL);

	// �����蔻�菈��
	Collision();

	// ��ԑJ�ڏ���
	Statement();

	// �Q�[�W�̍��W�����炵�Ă���
	if (m_pGaugeLife != NULL)
	{
		m_pGaugeLife->SetGaugePos(ENEMY_GAUGE_POS + GetPos());
	}

	if (GetMotionManager() != NULL)
	{// ���[�V�����Ǘ��N���X����������Ă���
		GetMotionManager()->Update(GetModel());
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	// ���ʂ̕`�揈��
	CCharacter::Draw();
}

//=============================================================================
//    �G�̃_���[�W����
//=============================================================================
void CEnemy::Hit(int nDamage)
{
	// �̗͂����炷
	m_nLife -= nDamage;

	if (m_nLife <= 0)
	{// �̗͂�0�ȉ��ɂȂ���
		Death();

		//// �X�R�A�N���X���擾���A�X�R�A�����Z����
		//CScene *pScene = NULL;
		//CScene *pSceneNext = NULL;
		//CScore *pScore = NULL;
		//pScene = GetTop(UI_SCORE_PRIORITY);
		//while (pScene != NULL)
		//{// ����������ɂȂ�܂ŌJ��Ԃ�
		//	pSceneNext = pScene->GetNext();
		//	if (pScene->GetObjType() == OBJTYPE_SCORE)
		//	{// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^������
		//		pScore = (CScore*)pScene;
		//		if (pScore != NULL)
		//		{// �X�R�A�N���X���擾�ł���
		//			pScore->AddScore(m_nAddScore);
		//		}
		//	}
		//	pScene = pSceneNext;
		//}
	}
	else
	{// �܂��̗͂�����
		// �̗̓Q�[�W�����炷
		if (m_pGaugeLife != NULL)
		{// ���������m�ۂ���Ă���
			m_pGaugeLife->CutGauge(nDamage);
		}

		m_State = STATE_DAMAGE;
		GetMotionManager()->SwitchMotion(GetModel(), m_State);
	}
}

//=============================================================================
//    �G�����񂾂Ƃ��̏���
//=============================================================================
void CEnemy::Death(void)
{
	// ��Ԃƃ��[�V������ς���
	m_State = STATE_DEATH;
	GetMotionManager()->SwitchMotion(GetModel(), m_State);
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
void CEnemy::Collision(void)
{
	bool bLand = false;   // ����Ă��邩�ǂ���

	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();           // ���݂̍��W

	// �z�u���ƒn�ʂւ̃|�C���^�擾����
	CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
	CObject *pObject = NULL;             // �z�u���N���X�ւ̃|�C���^
	CMeshField *pField = NULL;           // �n�ʃN���X�ւ̃|�C���^
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (bLand != true)
			{// �܂��I�u�W�F�N�g�ɏ���Ă��Ȃ�
				if (pScene->GetObjType() == OBJTYPE_MESHFIELD)
				{// �n�ʃN���X�ւ̃|�C���^��������
					pField = (CMeshField*)pScene;
					if (pField != NULL)
					{// �n�ʂ��擾�ł���
						float fFieldHeight = pField->GetPolyHeight(pos);
						if (pos.y < fFieldHeight)
						{// ���W���n�ʂ�艺�ɂȂ���
							m_Move.y = 0.0f;
							pos.y = fFieldHeight;
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_OBJECT)
				{// �z�u����������
					pObject = (CObject*)pScene;
					if (pObject != NULL)
					{// �z�u�����擾�ł���
						if (pObject->Collision(&pos, &GetPosOld(), &m_Move, 70.0f, 10.0f, this) == true)
						{// �z�u���̏�ɏ���Ă���
							if (GetLand() == false)
							{// ���n���Ă��Ȃ�������
								SetLand(true);
							}

							if (GetLandScene() == NULL && GetLand() == true)
							{// �����ɏ���Ă���
								pos.y = pObject->GetPos().y + pObject->GetVtxMax().y;
							}
							SetLandScene((CScene*)pObject);
							bLand = true;
						}
					}
				}
			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;
		}
	}

	// �e��l�̐ݒ�
	SetPos(pos);         // ���݂̍��W
	SetLand(bLand);      // �I�u�W�F�N�g�ɏ���Ă��邩�ǂ���
}

//=============================================================================
//    ��ԑJ�ڏ���
//=============================================================================
void CEnemy::Statement(void)
{
	if (m_State == STATE_DAMAGE)
	{// �_���[�W��Ԃ�������
		if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
		{// ���[�V�����Đ����I������
			m_State = STATE_NORMAL;
			GetMotionManager()->SwitchMotion(GetModel(), m_State);
		}
	}
	else if (m_State == STATE_DEATH)
	{// ���S��Ԃ�������
		if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
		{// ���[�V�����Đ����I������
			Uninit();
		}
	}
}

//=============================================================================
//    ��ނ�ݒ肷�鏈��
//=============================================================================
void CEnemy::SetType(const TYPE type)
{
	m_Type = type;
}

//=============================================================================
//    ��Ԃ�ݒ肷�鏈��
//=============================================================================
void CEnemy::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    �̗͂�ݒ肷�鏈��
//=============================================================================
void CEnemy::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    �|�������ɑ�����X�R�A�ʂ�ݒ肷�鏈��
//=============================================================================
void CEnemy::SetAddScore(const int nAddScore)
{
	m_nAddScore = nAddScore;
}

//=============================================================================
//    ��ނ��擾���鏈��
//=============================================================================
CEnemy::TYPE CEnemy::GetType(void)
{
	return m_Type;
}

//=============================================================================
//    ��Ԃ��擾���鏈��
//=============================================================================
CEnemy::STATE CEnemy::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �̗͂��擾���鏈��
//=============================================================================
int CEnemy::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    �|�������ɑ�����X�R�A�ʂ��擾���鏈��
//=============================================================================
int CEnemy::GetAddScore(void)
{
	return m_nAddScore;
}

//=============================================================================
//    �̗̓Q�[�W�N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CGaugeLife *CEnemy::GetGaugeLife(void)
{
	return m_pGaugeLife;
}

//=============================================================================
//    �������̍��W���擾���鏈��
//=============================================================================
D3DXVECTOR3 CEnemy::GetPosStd(void)
{
	return m_PosStd;
}

//=============================================================================
//    �������̌������擾���鏈��
//=============================================================================
D3DXVECTOR3 CEnemy::GetRotStd(void)
{
	return m_RotStd;
}


//*****************************************************************************
//     CSetEnemy�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CSetEnemy::CSetEnemy(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CSetEnemy::~CSetEnemy()
{

}

//=============================================================================
//    ��������
//=============================================================================
CSetEnemy *CSetEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife, int nAddScore, int nPriority)
{
	CSetEnemy *pSetEnemy = NULL;           // �z�u�p�G�l�~�[�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pSetEnemy == NULL)
		{// ����������ɂȂ��Ă���
			pSetEnemy = new CSetEnemy(nPriority);
			if (pSetEnemy != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pSetEnemy->Init(pos, rot, type, apModel, pMotionManager, nNumParts, fGravity, fColHeight, fColRange, nLife, nAddScore)))
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

	return pSetEnemy;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CSetEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife, int nAddScore)
{
	// ���ʂ̏���������
	if (FAILED(CEnemy::Init(pos, rot, type, apModel, pMotionManager, nNumParts, fGravity, fColHeight, fColRange, nLife, nAddScore)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CSetEnemy::Uninit(void)
{
	// ���ʂ̏I������
	CEnemy::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CSetEnemy::Update(void)
{
	// �Q�[�W�̍��W�����炵�Ă���
	CGaugeLife *pGaugeLife = GetGaugeLife();
	if (pGaugeLife != NULL)
	{
		pGaugeLife->SetGaugePos(ENEMY_GAUGE_POS + GetPos());
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CSetEnemy::Draw(void)
{
	// �����x��������
	SetModelAlpha(0.7f);

	// ���ʂ̕`�揈��
	CEnemy::Draw();

	// �����x�����ɖ߂��Ă���
	SetModelAlpha(1.0f);
}

//*****************************************************************************
//    CEnemyAssembly�̏���
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEnemyAssembly::CEnemyAssembly()
{
	// �e��l�̃N���A
	for (int nCntEnemyType = 0; nCntEnemyType < MAX_ENEMY_TYPE; nCntEnemyType++)
	{// �ǂݍ��߂�G�̎�ނ̐������J��Ԃ�
		m_pEnemyManager[nCntEnemyType] = NULL;
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEnemyAssembly::~CEnemyAssembly()
{

}


//=============================================================================
//    �G�Ǌ��N���X�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CEnemyAssembly::SetEnemyManager(CEnemyManager *pEnemyManager, int nIdx)
{
	m_pEnemyManager[nIdx] = pEnemyManager;
}

//=============================================================================
//    �G�Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CEnemyManager *CEnemyAssembly::GetEnemyManager(int nIdx)
{
	return m_pEnemyManager[nIdx];
}