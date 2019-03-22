//*****************************************************************************
//
//     �G�f�B�^�[�̏���[editer.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "editer.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "debuglog.h"
#include "input.h"
#include "functionlib.h"
#include "map.h"
#include "meshField.h"
#include "meshWall.h"
#include "billboardObject.h"
#include "emitter.h"
#include "enemy.h"
#include "item.h"
#include "modelManager.h"
#include "effectManager.h"
#include "textureManager.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define EDITER_SYSTEM_FILENAME "data/TEXT/system.ini"   // �G�f�B�^�[�̃V�X�e���t�@�C����
#define EDITER_PLAY_TIMING     (60)                     // ��~��Ԃ���Đ���ԂɂȂ�܂ł̎���

// �l��ǂݎ��ϐ�
#define EFFECT_FILENAME        "EFFECT_FILENAME = "     // �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C����
#define MAP_FILENAME           "MAP_FILENAME = "        // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����
#define NUM_TEXTURE            "NUM_TEXTURE = "         // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME       "TEXTURE_FILENAME = "    // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define NUM_MODEL              "NUM_MODEL = "           // �ǂݍ��ރ��f���̐�
#define MODEL_FILENAME         "MODEL_FILENAME = "      // �ǂݍ��ރ��f���t�@�C����

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEditer::CEditer()
{
	// �e��l�̃N���A
	// ����
	m_nCounter = 0;                                    // ��Ԃ��Ǘ�����J�E���^�[
	m_pMap = NULL;                                     // �}�b�v�N���X�ւ̃|�C���^
	m_pSetObject = NULL;                               // �z�u���ւ̃|�C���^
	m_pSetObjectMove = NULL;                           // �����z�u���ւ̃|�C���^
	m_pSetBillboardObj = NULL;                         // �z�u�ł���r���{�[�h�ւ̃|�C���^
	m_pSetEmitter = NULL;                              // �z�u�ł���G�t�F�N�g�ւ̃|�C���^
	m_pSetEnemy = NULL;                                // �z�u�ł���G�ւ̃|�C���^
	m_pSetWall = NULL;                                 // �z�u�ł���ǂւ̃|�C���^
	m_pSetItem = NULL;                                 // �z�u�ł���A�C�e���ւ̃|�C���^
	m_pModelManager = NULL;                            // ���f���Ǌ��N���X�ւ̃|�C���^
	m_pEffectManager = NULL;                           // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	m_pEnemyAssmbly = NULL;                            // �G�Ǌ��܂Ƃ߃N���X�ւ̃|�C���^
	m_Type = TYPE_OBJECT;                              // ���ݔz�u�ł���z�u���̎��
	m_SetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // �z�u�I�u�W�F�N�g�̍��W
	m_SetRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // �z�u�I�u�W�F�N�g�̌���

	// �z�u���f���p
	m_ModelColType = CObject::COL_TYPE_SQUARE;         // �z�u�ł��郂�f���̓����蔻��̎��
	m_ModelScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // �z�u�ł��郂�f���̑傫���̔{��
	m_fModelColRange = 0.0f;                           // �z�u�ł��郂�f���̓����蔻������͈�
	m_ModelMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // �z�u�ł��郂�f���̓���
	m_nModelMaxMoveCounter = 0;                        // �z�u�ł��郂�f���̈ړ��ʂ�؂�ւ��鎞��
	m_ModelRollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �z�u�ł��郂�f���̉�]����X�s�[�h
	m_nSelectModelIdx = 0;                             // ���ݔz�u�ł��郂�f���̔ԍ�

	// �r���{�[�h�p
	m_BillCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);     // �z�u�ł���r���{�[�h�̐F
	m_fBillRot = 0.0f;                                 // �z�u�ł���r���{�[�h�̌���
	m_fBillWidth = 50.0f;                              // �z�u�ł���r���{�[�h�̕�
	m_fBillHeight = 50.0f;                             // �z�u�ł���r���{�[�h�̍���
	m_nBillTexIdx = 0;                                 // �z�u�ł���r���{�[�h�̃e�N�X�`���̔ԍ�
	m_bBillLighting = false;                           // �z�u�ł���r���{�[�h�̃��C�e�B���O���邩���Ȃ���
	m_bBillDrawAddtive = false;                        // �z�u�ł���r���{�[�h�̉��Z�����ŕ`�悷�邩���Ȃ���

	// �G�t�F�N�g�p
	m_nEffectType = 0;                                 // �z�u�ł���G�t�F�N�g�̎�ޔԍ�

	// �G�p
	m_nEnemyType = 0;                                  // �z�u�ł���G�̎��
	m_nEnemyLife = 1;                                  // �z�u�ł���G�̗̑�
	m_nEnemyScore = 1;                                 // �z�u�ł���G�̃X�R�A

	// �Ǘp
	m_nWallTexIdx = 0;                                 // �z�u�ł���ǂ̃e�N�X�`���ԍ�
	m_WallCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);     // �z�u�ł���ǂ̐F
	m_fWallWidth = 10.0f;                              // �z�u�ł���ǂ̕�
	m_fWallHeight = 10.0f;                             // �z�u�ł���ǂ̍���
	m_nWallXBlock = 1;                                 // �z�u�ł���ǂ̉��̕�����
	m_nWallYBlock = 1;                                 // �z�u�ł���ǂ̏c�̕�����
	m_nWallTexSplitU = 1;                              // �z�u�ł���ǂ̃e�N�X�`��U���W�̕�����
	m_nWallTexSplitV = 1;                              // �z�u�ł���ǂ̃e�N�X�`��V���W�̕�����

	// �A�C�e���p
	m_ItemCol = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);        // �z�u�ł���A�C�e���̐F
	m_fItemColRange = 0.0f;                            // �z�u�ł���A�C�e���̓����蔻������͈�
	m_fItemRot = 0.0f;                                 // �z�u�ł���A�C�e���̌���
	m_fItemWidth = 2.0f;                               // �z�u�ł���A�C�e���̕�
	m_fItemHeight = 2.0f;                              // �z�u�ł���A�C�e���̍���
	m_nItemTexIdx = 0;                                 // �z�u�ł���A�C�e���̃e�N�X�`���̔ԍ�
	m_nItemType = 0;                                   // �z�u�ł���A�C�e���̎�ޔԍ�
	m_bItemLighting = false;                           // �z�u�ł���A�C�e���̃��C�e�B���O���邩���Ȃ���
	m_bItemDrawAddtive = false;                        // �z�u�ł���A�C�e���̉��Z�����ŕ`�悷�邩���Ȃ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEditer::~CEditer()
{

}

//=============================================================================
//    ����������
//=============================================================================
void CEditer::Init(void)
{
	// �G�f�B�^�[�̏����������O������ǂݍ���
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(EDITER_SYSTEM_FILENAME, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �X�N���v�g�ǂݍ��ݗp�̕ϐ���p��
			char *pLine = NULL;                 // 1�s���ǂݎ��p
			char *pStrCur = NULL;               // ���݂̐擪�|�C���^
			char *pStr = NULL;                  // �擪�|�C���^�ۑ��p
			int nCntTex = 0;                    // �e�N�X�`����ǂݍ��񂾉�
			int nCntModel = 0;                  // ���f����ǂݍ��񂾉�
			LPDIRECT3DTEXTURE9 pTexture;        // �e�N�X�`���ւ̃|�C���^
			LPD3DXMESH pMesh = NULL;            // ���b�V�����ւ̃|�C���^
			LPD3DXBUFFER pBuffMat = NULL;       // �}�e���A�����ւ̃|�C���^
			DWORD nNumMat = 0;                  // �}�e���A�����̐�
			char aMapFileName[256] = "\0";      // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����
			char aEffectFileName[256] = "\0";   // �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C����
			if (pLine == NULL && pStr == NULL)
			{// ���������m�ۂł����Ԃł���
				pLine = new char[256];
				pStr = new char[256];
				if (pLine != NULL && pStr != NULL)
				{// ���������m�ۂł���
				 // �������������
					strcpy(pLine, "\0");
					strcpy(pStr, "\0");

					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, MAP_FILENAME) == 0)
							{// �}�b�v�f�[�^�̃X�N���v�g�t�@�C����������
							    // �}�b�v�f�[�^�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MAP_FILENAME);
								strcpy(aMapFileName, pStr);

								// �t�@�C������ݒu�ł��郂�f������ǂݎ��
								FILE *pFileMap = NULL;  // �t�@�C���|�C���^
								if (pFileMap == NULL)
								{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
									pFileMap = fopen(aMapFileName, "r");
									if (pFileMap != NULL)
									{// �t�@�C���|�C���^���m�ۂł���
										pStrCur = CFunctionLib::ReadLine(pFileMap, pLine);  // �L���ȕ������������
										if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
										{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
											while (1)
											{// ���[�v�J�n
												pStrCur = CFunctionLib::ReadLine(pFileMap, pLine);  // �L���ȕ������������
												if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
												{// �e�N�X�`���̌���񂪂�����
													m_nNumTex = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
													if (m_nNumTex >= 1)
													{// ���f������1�ȏ゠��
														m_pTextureManager = CTextureManager::Create(m_nNumTex);
													}
												}
												else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
												{// �e�N�X�`���̃t�@�C���p�X����񂪂�����
												    // �e�N�X�`���̃t�@�C���p�X����ǂݎ��
													pStr = CFunctionLib::ReadString(pStrCur, pStr, TEXTURE_FILENAME);

													// �e�N�X�`���̓ǂݍ���
													D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);
													if (pTexture != NULL)
													{// �e�N�X�`����ǂݍ��߂�
														m_pTextureManager->BindTexture(pTexture, nCntTex);
														m_pTextureManager->SetFileName(pStr, nCntTex);
														pTexture = NULL;
														nCntTex++;
													}
												}
												else if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
												{// �e�N�X�`���̌���񂪂�����
													m_nNumModel = CFunctionLib::ReadInt(pStrCur, NUM_MODEL);
													if (m_nNumModel >= 1)
													{// ���f������1�ȏ゠��
														m_pModelManager = CModelManager::Create(m_nNumModel);
													}
												}
												else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
												{// ���f���̃t�@�C���p�X����񂪂�����
												    // ���f���̃t�@�C���p�X����ǂݎ��
													pStr = CFunctionLib::ReadString(pStrCur, pStr, MODEL_FILENAME);

													// ���f���̓ǂݍ���
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
												else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
												{// �X�N���v�g�ǂݍ��ݏI���̍��}������
													break;  // ���[�v�I��
												}
											}
										}
										fclose(pFileMap);
									}
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, EFFECT_FILENAME) == 0)
							{// �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C����������
							    // �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, EFFECT_FILENAME);
								strcpy(aEffectFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}

					// �K�v�ȃN���X���쐬����
					// �G�t�F�N�g�Ǌ��N���X
				    m_pEffectManager = CEffectManager::Create(aEffectFileName);

					// �}�b�v
					m_pMap = CMap::Create(aMapFileName);

					// ���f���z�u�p�N���X
					m_pSetObject = CSetObject::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f) ,D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					if (m_pSetObject != NULL)
					{
						m_pSetObject->BindModel(m_pModelManager->GetMesh(0), m_pModelManager->GetBuffMat(0), m_pModelManager->GetNumMat(0), m_pModelManager->GetVtxMax(0), m_pModelManager->GetVtxMin(0), m_pModelManager->GetTexture(0));
						m_pSetObject->SetTransParent(0.5f);
					}
					m_ModelScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

					// �G�Ǌ��N���X��������
					if (m_pEnemyAssmbly == NULL)
					{// ���������m�ۂł����Ԃł���
						m_pEnemyAssmbly = new CEnemyAssembly;
						if (m_pEnemyAssmbly != NULL)
						{// ���������m�ۂł���
							int nCntEnemy = 0;
							CScene *pScene = NULL;
							CScene *pSceneNext = NULL;
							CEnemyManager *pEnemyManager = NULL;
							for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
							{// �D�揇�ʂ̐������J��Ԃ�
								pScene = CScene::GetTop(nCntPriority);
								while (pScene != NULL)
								{// ����������ɂȂ�܂ŌJ��Ԃ�
									pSceneNext = pScene->GetNext();
									if (pScene->GetObjType() == CScene::OBJTYPE_ENEMYMANAGER)
									{// �G�Ǌ��N���X�ւ̃|�C���^������
										pEnemyManager = (CEnemyManager*)pScene;
										if (pEnemyManager != NULL)
										{// �G�Ǌ��N���X�̃|�C���^���擾�ł���
											m_pEnemyAssmbly->SetEnemyManager(pEnemyManager, nCntEnemy);
											nCntEnemy++;
										}
									}
									pScene = pSceneNext;
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
				}
			}
		}
	}
}

//=============================================================================
//    �I������
//=============================================================================
void CEditer::Uninit(void)
{
	// ���f���Ǌ��N���X���J������
	if (m_pModelManager != NULL)
	{
		m_pModelManager->Uninit();

		// �������̊J��
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	// �e�N�X�`���Ǌ��N���X�ւ̃|�C���^���J������
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// �������̊J��
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// �G�Ǌ��܂Ƃ߃N���X�ւ̃|�C���^����ɂ���
	if (m_pEnemyAssmbly != NULL)
	{
		// �������̊J��
		delete m_pEnemyAssmbly;
		m_pEnemyAssmbly = NULL;
	}

	// �z�u���N���X���J������
	if (m_pSetObject != NULL)
	{
		m_pSetObject->Uninit();
		m_pSetObject = NULL;
	}

	// �����z�u���N���X���J������
	if (m_pSetObjectMove != NULL)
	{
		m_pSetObjectMove->Uninit();
		m_pSetObjectMove = NULL;
	}

	// �z�u�r���{�[�h�N���X���J������
	if (m_pSetBillboardObj != NULL)
	{
		m_pSetBillboardObj->Uninit();
		m_pSetBillboardObj = NULL;
	}

	// �z�u�p�G�~�b�^�N���X���J������
	if (m_pSetEmitter != NULL)
	{
		m_pSetEmitter->Uninit();
		m_pSetEmitter = NULL;
	}

	// �z�u�p�G�N���X���J������
	if (m_pSetEnemy != NULL)
	{
		m_pSetEnemy->Uninit();
		m_pSetEnemy = NULL;
	}

	// �z�u�p�ǃN���X���J������
	if (m_pSetWall != NULL)
	{
		m_pSetWall->Uninit();
		m_pSetWall = NULL;
	}

	// �z�u�p�A�C�e���N���X���J������
	if (m_pSetItem != NULL)
	{
		m_pSetItem->Uninit();
		m_pSetItem = NULL;
	}

	// �}�b�v�N���X���J������
	if (m_pMap != NULL)
	{
		m_pMap->Uninit();

		delete m_pMap;
		m_pMap = NULL;
	}
}

//=============================================================================
//    �X�V����
//=============================================================================
void CEditer::Update(void)
{
	if (m_Type == TYPE_EFFECT)
	{// �G�t�F�N�g������Ԃ�������
		if (m_EffectState == EFFECTSTATE_STOP)
		{// �Đ���~��Ԃ�������
			m_nCounter++;
			if (m_nCounter % EDITER_PLAY_TIMING == 0)
			{// �Đ���ԂɂȂ��ԂɂȂ���
				if (m_pSetEmitter == NULL)
				{
					m_pSetEmitter = m_pEffectManager->SetMapEditEffect(m_SetPos, m_SetRot, m_nEffectType);
					m_EffectState = EFFECTSTATE_PLAY;
					m_nCounter = 0;
				}
			}
		}
	}

	// �L�[�{�[�h�̎擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard != NULL)
	{// �L�[�{�[�h���擾�ł���
		if (pKeyboard->GetTrigger(DIK_RETURN) == true)
		{// ENTER�L�[�������ꂽ
			// �I�u�W�F�N�g��z�u����
			if (m_Type == TYPE_OBJECT)
			{
				m_pMap->SetObject(m_SetPos, m_SetRot, m_ModelScale, m_ModelRollSpeed, m_ModelColType, m_fModelColRange, m_nSelectModelIdx);
			}
			else if (m_Type == TYPE_OBJECTMOVE)
			{
				m_pMap->SetObjectMove(m_SetPos, m_SetRot, m_ModelScale, m_ModelRollSpeed, m_nSelectModelIdx, m_ModelMove, m_nModelMaxMoveCounter, m_ModelColType, m_fModelColRange);
			}
			else if (m_Type == TYPE_BILLBOARD)
			{
				m_pMap->SetBillboard(m_SetPos, m_BillCol, m_fBillWidth, m_fBillHeight, m_fBillRot, m_bBillLighting, m_bBillDrawAddtive,m_nBillTexIdx, m_fBillColRange);
			}
			else if (m_Type == TYPE_EFFECT)
			{
				m_pMap->SetEffect(m_SetPos, m_SetRot, m_nEffectType);
			}
			else if (m_Type == TYPE_ENEMY)
			{
				m_pMap->SetEnemy(m_SetPos, m_SetRot, m_nEnemyType, m_nEnemyLife, m_nEnemyScore);
			}
			else if (m_Type == TYPE_WALL)
			{
				m_pMap->SetWall(m_SetPos, m_SetRot, m_WallCol, m_fWallWidth, m_fWallHeight, m_nWallXBlock, m_nWallYBlock, m_nWallTexSplitU, m_nWallTexSplitV, m_nWallTexIdx);
			}
			else if (m_Type == TYPE_ITEM)
			{
				m_pMap->SetItem(m_SetPos, m_ItemCol, m_fItemWidth, m_fItemHeight, m_fItemRot, m_bItemLighting, m_bItemDrawAddtive, m_nItemTexIdx, m_fItemColRange, m_nItemType);
			}
		}
	}

	// �I�u�W�F�N�g���ړ������鏈��
	ObjectMove();
}

//=============================================================================
//    �I�u�W�F�N�g�𓮂�������
//=============================================================================
void CEditer::ObjectMove(void)
{
	// �J�����̎擾
	CCamera *pCamera = CManager::GetCamera();

	// �L�[�{�[�h�̎擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// �}�E�X�̎擾
	CInputMouse *pMouse = CManager::GetMouse();

	if (pKeyboard != NULL)
	{// �L�[�{�[�h���擾�ł���
		if (pKeyboard->GetPress(DIK_A) == true)
		{// �������̓��͂����ꂽ
			if (pKeyboard->GetPress(DIK_W) == true)
			{// �����ɏ�����̓��͂����ꂽ
			    // �ړ�����
				m_SetPos.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * 4.0f;
				m_SetPos.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * 4.0f;
			}
			else if (pKeyboard->GetPress(DIK_S) == true)
			{// �����ɉ������̓��͂����ꂽ
			    // �ړ�����
				m_SetPos.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * 4.0f;
				m_SetPos.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * 4.0f;
			}
			else
			{// ����������ĂȂ�
			    // �ړ�����
				m_SetPos.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * 4.0f;
				m_SetPos.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * 4.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_D) == true)
		{// �E�����̓��͂����ꂽ
			if (pKeyboard->GetPress(DIK_W) == true)
			{// �����ɏ�����̓��͂����ꂽ
			     // �ړ�����
				m_SetPos.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * 4.0f;
				m_SetPos.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * 4.0f;
			}
			else if (pKeyboard->GetPress(DIK_S) == true)
			{// �����ɉ������̓��͂����ꂽ
			    // �ړ�����
				m_SetPos.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * 4.0f;
				m_SetPos.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * 4.0f;
			}
			else
			{// ����������ĂȂ�
			    // �ړ�����
				m_SetPos.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * 4.0f;
				m_SetPos.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * 4.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_W) == true)
		{// ������̓��͂����ꂽ
			if (pKeyboard->GetPress(DIK_LSHIFT) == true)
			{
				// �ړ�����
				m_SetPos.y += 4.0f;
			}
			else
			{
				// �ړ�����
				m_SetPos.x += sinf(pCamera->GetRot().y) * 4.0f;
				m_SetPos.z += cosf(pCamera->GetRot().y) * 4.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// �������̓��͂����ꂽ
			if (pKeyboard->GetPress(DIK_LSHIFT) == true)
			{
				// �ړ�����
				m_SetPos.y -= 4.0f;
			}
			else
			{
				// �ړ�����
				m_SetPos.x += sinf(pCamera->GetRot().y + D3DX_PI) * 4.0f;
				m_SetPos.z += cosf(pCamera->GetRot().y + D3DX_PI) * 4.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_X) == true)
		{// X�L�[�������ꂽ
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
					if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
					{// �n�ʃN���X�ւ̃|�C���^��������
						pField = (CMeshField*)pScene;
						if (pField != NULL)
						{// �n�ʂ��擾�ł���
							float fFieldHeight = pField->GetPolyHeight(m_SetPos);
							if (m_SetPos.y < fFieldHeight)
							{// ���W���n�ʂ�艺�ɂȂ���
								m_SetPos.y = fFieldHeight;
							}
						}
					}
					pScene = pSceneNext;
				}
			}
		}
	}

	if (pMouse != NULL)
	{// �}�E�X���擾�ł���
	    // �����ɉ����ꂽ�Ƃ��̈ʒu���獡�̃}�E�X�J�[�\���̈ʒu���王�_�����_���ړ�������������v�Z
		if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_2) == true)
		{// �}�E�X�̃z�C�[���{�^���������ꂽ
			float fAngle = atan2f(((float)pMouse->GetPoint().x - (float)pMouse->GetPointOld().x), ((float)pMouse->GetPoint().y - (float)pMouse->GetPointOld().y));
			fAngle -= pCamera->GetRot().y;
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
			fMovement *= (SCREEN_WIDTH / SCREEN_HEIGHT);

			// ���f�����W��ς���
			m_SetPos.x += sinf(fAngle) * fMovement;
			m_SetPos.z -= cosf(fAngle) * fMovement;
		}
	}

	// ������␳����
	// X��
	if (m_SetRot.x < -D3DX_PI)
	{// �~�����𒴂���
		m_SetRot.x += D3DX_PI * 2.0f;
	}
	if (m_SetRot.x > D3DX_PI)
	{// �~�����𒴂���
		m_SetRot.x -= D3DX_PI * 2.0f;
	}

	// Y��
	if (m_SetRot.y < -D3DX_PI)
	{// �~�����𒴂���
		m_SetRot.y += D3DX_PI * 2.0f;
	}
	if (m_SetRot.y > D3DX_PI)
	{// �~�����𒴂���
		m_SetRot.y -= D3DX_PI * 2.0f;
	}

	// Z��
	if (m_SetRot.z < -D3DX_PI)
	{// �~�����𒴂���
		m_SetRot.z += D3DX_PI * 2.0f;
	}
	if (m_SetRot.z > D3DX_PI)
	{// �~�����𒴂���
		m_SetRot.z -= D3DX_PI * 2.0f;
	}


	// ��]����X�s�[�h��␳����
	// X��
	if (m_ModelRollSpeed.x < -D3DX_PI)
	{// �~�����𒴂���
		m_ModelRollSpeed.x += D3DX_PI * 2.0f;
	}
	if (m_ModelRollSpeed.x > D3DX_PI)
	{// �~�����𒴂���
		m_ModelRollSpeed.x -= D3DX_PI * 2.0f;
	}

	// Y��
	if (m_ModelRollSpeed.y < -D3DX_PI)
	{// �~�����𒴂���
		m_ModelRollSpeed.y += D3DX_PI * 2.0f;
	}
	if (m_ModelRollSpeed.y > D3DX_PI)
	{// �~�����𒴂���
		m_ModelRollSpeed.y -= D3DX_PI * 2.0f;
	}

	// Z��
	if (m_ModelRollSpeed.z < -D3DX_PI)
	{// �~�����𒴂���
		m_ModelRollSpeed.z += D3DX_PI * 2.0f;
	}
	if (m_ModelRollSpeed.z > D3DX_PI)
	{// �~�����𒴂���
		m_ModelRollSpeed.z -= D3DX_PI * 2.0f;
	}


	// �l�̐ݒ�
	if (m_Type == TYPE_OBJECT)
	{
		if (m_pSetObject != NULL)
		{
			m_pSetObject->SetPos(m_SetPos);
			m_pSetObject->SetRollSpeed(m_ModelRollSpeed);
		}
	}
	else if (m_Type == TYPE_OBJECTMOVE)
	{
		if (m_pSetObjectMove != NULL)
		{
			m_pSetObjectMove->SetPos(m_SetPos);
			m_pSetObjectMove->SetRollSpeed(m_ModelRollSpeed);
			m_pSetObjectMove->SetMove(m_ModelMove);
			m_pSetObjectMove->SetMaxMoveCounter(m_nModelMaxMoveCounter);
		}
	}
	else if (m_Type == TYPE_BILLBOARD)
	{
		if (m_pSetBillboardObj != NULL)
		{
			m_pSetBillboardObj->SetPos(m_SetPos);
		}
	}
	else if (m_Type == TYPE_EFFECT)
	{
		if (m_pSetEmitter != NULL)
		{
			m_pSetEmitter->SetPos(m_SetPos);
			m_pSetEmitter->SetRot(m_SetRot);
		}
	}
	else if (m_Type == TYPE_ENEMY)
	{
		if (m_pSetEnemy != NULL)
		{
			m_pSetEnemy->SetPos(m_SetPos);
			m_pSetEnemy->SetRot(m_SetRot);
		}
	}
	else if (m_Type == TYPE_WALL)
	{
		if (m_pSetWall != NULL)
		{
			m_pSetWall->SetPos(m_SetPos);
			m_pSetWall->SetRot(m_SetRot);
		}
	}
	else if (m_Type == TYPE_ITEM)
	{
		if (m_pSetItem != NULL)
		{
			m_pSetItem->SetPos(m_SetPos);
		}
	}
}

//=============================================================================
//    ���f����ς��鏈��
//=============================================================================
void CEditer::ModelChange(void)
{
	// �z�u���N���X���J������
	if (m_pSetObject != NULL)
	{
		m_pSetObject->Uninit();
		m_pSetObject = NULL;
	}

	// �����z�u���N���X���J������
	if (m_pSetObjectMove != NULL)
	{
		m_pSetObjectMove->Uninit();
		m_pSetObjectMove = NULL;
	}

	if (m_Type == TYPE_OBJECT)
	{
		m_pSetObject = CSetObject::Create(m_SetPos, m_SetRot, m_ModelScale, m_ModelRollSpeed);
		m_pSetObject->BindModel(m_pModelManager->GetMesh(m_nSelectModelIdx), m_pModelManager->GetBuffMat(m_nSelectModelIdx), m_pModelManager->GetNumMat(m_nSelectModelIdx), m_pModelManager->GetVtxMax(m_nSelectModelIdx), m_pModelManager->GetVtxMin(m_nSelectModelIdx), m_pModelManager->GetTexture(m_nSelectModelIdx));
		m_pSetObject->SetTransParent(0.5f);
	}
	else if (m_Type == TYPE_OBJECTMOVE)
	{
		m_pSetObjectMove = CSetObjectMove::Create(m_SetPos, m_SetRot, m_ModelScale, m_ModelRollSpeed, m_ModelMove, m_nModelMaxMoveCounter);
		m_pSetObjectMove->BindModel(m_pModelManager->GetMesh(m_nSelectModelIdx), m_pModelManager->GetBuffMat(m_nSelectModelIdx), m_pModelManager->GetNumMat(m_nSelectModelIdx), m_pModelManager->GetVtxMax(m_nSelectModelIdx), m_pModelManager->GetVtxMin(m_nSelectModelIdx), m_pModelManager->GetTexture(m_nSelectModelIdx));
		m_pSetObjectMove->SetVtxMin(m_pModelManager->GetVtxMin(m_nSelectModelIdx));
		m_pSetObjectMove->SetVtxMax(m_pModelManager->GetVtxMax(m_nSelectModelIdx));
		m_pSetObjectMove->SetTransParent(0.5f);
	}
}

//=============================================================================
//    �z�u����G�t�F�N�g�̎�ނ�ύX���鏈��
//=============================================================================
void CEditer::EmitterChange(void)
{
	// �����̃G�~�b�^���폜����
	if (m_pSetEmitter != NULL)
	{
		m_pSetEmitter->Uninit();
		m_pSetEmitter = NULL;
	}

	// �V������蒼��
	m_pSetEmitter = m_pEffectManager->SetMapEditEffect(m_SetPos, m_SetRot, m_nEffectType);
}

//=============================================================================
//    �z�u����G�̎�ނ�ύX���鏈��
//=============================================================================
void CEditer::EnemyChange(void)
{
	// �����̓G�N���X���폜����
	if (m_pSetEnemy != NULL)
	{
		m_pSetEnemy->Uninit();
		m_pSetEnemy = NULL;
	}

	// �V�����G�𐶐�����
	m_pSetEnemy = m_pEnemyAssmbly->GetEnemyManager(m_nEnemyType)->SetMapEditEnemy(m_SetPos, m_SetRot, m_nEnemyType, m_nEnemyLife, m_nEnemyScore);
}

//=============================================================================
//    �z�u����I�u�W�F�N�g�̎�ނ�ύX���鏈��
//=============================================================================
void CEditer::ObjectChange(TYPE typeOld)
{
	// �O��̎�ނ̃I�u�W�F�N�g���폜����
	switch (typeOld)
	{// �O��̎�ނŏ����킯
	case TYPE_OBJECT:       // �z�u�p���f��
		if (m_pSetObject != NULL)
		{
			m_pSetObject->Uninit();
			m_pSetObject = NULL;
		}
		break;
	case TYPE_OBJECTMOVE:   // �����z�u�p���f��
		if (m_pSetObjectMove != NULL)
		{
			m_pSetObjectMove->Uninit();
			m_pSetObjectMove = NULL;
		}
		break;
	case TYPE_BILLBOARD:    // �z�u�p�r���{�[�h
		if (m_pSetBillboardObj != NULL)
		{
			m_pSetBillboardObj->Uninit();
			m_pSetBillboardObj = NULL;
		}
		break;
	case TYPE_EFFECT:       // �z�u�p�G�t�F�N�g
		if (m_pSetEmitter != NULL)
		{
			m_pSetEmitter->Uninit();
			m_pSetEmitter = NULL;
		}
		break;
	case TYPE_ENEMY:        // �G
		if (m_pSetEnemy != NULL)
		{
			m_pSetEnemy->Uninit();
			m_pSetEnemy = NULL;
		}
		break;
	case TYPE_WALL:        // ��
		if (m_pSetWall != NULL)
		{
			m_pSetWall->Uninit();
			m_pSetWall = NULL;
		}
		break;
	case TYPE_ITEM:        // �A�C�e��
		if (m_pSetItem != NULL)
		{
			m_pSetItem->Uninit();
			m_pSetItem = NULL;
		}
		break;
	}

	// ����̎�ނ̃I�u�W�F�N�g���쐬����
	switch (m_Type)
	{// �O��̎�ނŏ����킯
	case TYPE_OBJECT:       // �z�u�p���f��
		m_pSetObject = CSetObject::Create(m_SetPos, m_SetRot, m_ModelScale, m_ModelRollSpeed);
		if (m_pSetObject != NULL)
		{
			m_pSetObject->BindModel(m_pModelManager->GetMesh(m_nSelectModelIdx), m_pModelManager->GetBuffMat(m_nSelectModelIdx), m_pModelManager->GetNumMat(m_nSelectModelIdx), m_pModelManager->GetVtxMax(m_nSelectModelIdx), m_pModelManager->GetVtxMin(m_nSelectModelIdx), m_pModelManager->GetTexture(m_nSelectModelIdx));
			m_pSetObject->SetTransParent(0.5f);
		}
		break;
	case TYPE_OBJECTMOVE:   // �����z�u�p���f��
		m_pSetObjectMove = CSetObjectMove::Create(m_SetPos, m_SetRot, m_ModelScale, m_ModelRollSpeed, m_ModelMove, m_nModelMaxMoveCounter);
		if (m_pSetObjectMove != NULL)
		{
			m_pSetObjectMove->BindModel(m_pModelManager->GetMesh(m_nSelectModelIdx), m_pModelManager->GetBuffMat(m_nSelectModelIdx), m_pModelManager->GetNumMat(m_nSelectModelIdx), m_pModelManager->GetVtxMax(m_nSelectModelIdx), m_pModelManager->GetVtxMin(m_nSelectModelIdx), m_pModelManager->GetTexture(m_nSelectModelIdx));
			m_pSetObjectMove->SetTransParent(0.5f);
		}
		break;
	case TYPE_BILLBOARD:    // �z�u�p�r���{�[�h
		m_pSetBillboardObj = CSetBillboardObject::Create(m_SetPos,m_BillCol,m_fBillWidth,m_fBillHeight,m_fBillRot,m_bBillLighting,m_bBillDrawAddtive,m_nBillTexIdx);
		if (m_pSetBillboardObj != NULL)
		{
			m_pSetBillboardObj->BindTexture(m_pTextureManager->GetTexture(m_nBillTexIdx));
		}
		break;
	case TYPE_EFFECT:        // �z�u�G�t�F�N�g
		m_pSetEmitter = m_pEffectManager->SetMapEditEffect(m_SetPos, m_SetRot, m_nEffectType);
		break;
	case TYPE_ENEMY:         // �G
		m_pSetEnemy = m_pEnemyAssmbly->GetEnemyManager(m_nEnemyType)->SetMapEditEnemy(m_SetPos, m_SetRot, m_nEnemyType, m_nEnemyLife, m_nEnemyScore);
		break;
	case TYPE_WALL:          // ��
		m_pSetWall = CSetMeshWall::Create(m_SetPos, m_SetRot,m_WallCol, m_fWallWidth, m_fWallHeight, m_nWallXBlock, m_nWallYBlock, m_nWallTexSplitU, m_nWallTexSplitV, m_nWallTexIdx);
		if (m_pSetWall != NULL)
		{
			m_pSetWall->BindTexture(m_pTextureManager->GetTexture(m_nWallTexIdx));
		}
		break;
	case TYPE_ITEM:          //	�A�C�e��
		m_pSetItem = CSetItem::Create(m_SetPos, m_ItemCol, m_fItemWidth, m_fItemHeight, m_fItemRot, m_bItemLighting, m_bItemDrawAddtive, m_nItemTexIdx, m_fItemColRange, m_nItemType);
		if (m_pSetItem != NULL)
		{
			m_pSetItem->BindTexture(m_pTextureManager->GetTexture(m_nItemTexIdx));
		}
		break;
	}
}

//=============================================================================
//    �z�u�I�u�W�F�N�g�̍��W��ݒ肷�鏈��
//=============================================================================
void CEditer::SetPos(const D3DXVECTOR3 SetPos)
{
	m_SetPos = SetPos;
}

//=============================================================================
//    �z�u�I�u�W�F�N�g�̌�����ݒ肷�鏈��
//=============================================================================
void CEditer::SetRot(const D3DXVECTOR3 SetRot)
{
	m_SetRot = SetRot;
}

//=============================================================================
//    �z�u�p���f���̎�ނ�ݒ肷�鏈��
//=============================================================================
void CEditer::SetType(const TYPE Type)
{
	m_Type = Type;
}

//=============================================================================
//    �G�t�F�N�g�̏�Ԃ�ݒ肷�鏈��
//=============================================================================
void CEditer::SetEffectState(const EFFECTSTATE state)
{
	m_EffectState = state;
	if (m_EffectState == EFFECTSTATE_STOP)
	{
		m_pSetEmitter = NULL;
		m_pEffectManager->SetEmitter(NULL);
	}
}

//=============================================================================
//    �z�u�p���f���̑傫���̔{����ݒ肷�鏈��
//=============================================================================
void CEditer::SetModelScale(const D3DXVECTOR3 ModelScale)
{
	m_ModelScale = ModelScale;
}

//=============================================================================
//    �z�u�p���f���̉�]�X�s�[�h��ݒ肷�鏈��
//=============================================================================
void CEditer::SetModelRollSpeed(const D3DXVECTOR3 ModelRollSpeed)
{
	m_ModelRollSpeed = ModelRollSpeed;
}

//=============================================================================
//    �z�u�p���f���̓����蔻��̎�ނ�ݒ肷�鏈��
//=============================================================================
void CEditer::SetModelColType(const CObject::COL_TYPE ModelColType)
{
	m_ModelColType = ModelColType;
}

//=============================================================================
//    �z�u�p���f���̓����蔻������͈͂�ݒ肷�鏈��
//=============================================================================
void CEditer::SetModelColRange(const float fModelColRange)
{
	m_fModelColRange = fModelColRange;
}

//=============================================================================
//    �z�u�p���f���̉�]�X�s�[�h���擾���鏈��
//=============================================================================
void CEditer::SetModelMove(const D3DXVECTOR3 ModelMove)
{
	m_ModelMove = ModelMove;
}

//=============================================================================
//    �z�u�p���f���̈ړ��ʂ�؂�ւ���^�C�~���O���擾���鏈��
//=============================================================================
void CEditer::SetModelMaxMoveCounter(const int nModelMaxMoveCounter)
{
	m_nModelMaxMoveCounter = nModelMaxMoveCounter;
}

//=============================================================================
//    �z�u�ł��郂�f���̎�ނ̐���ݒ肷�鏈��
//=============================================================================
void CEditer::SetNumModel(const int nNumModel)
{
	m_nNumModel = nNumModel;
}

//=============================================================================
//    ���ݑI������Ă��郂�f���̔ԍ���ݒ肷�鏈��
//=============================================================================
void CEditer::SetSelectModelIdx(const int nSelectModelIdx)
{
	m_nSelectModelIdx = nSelectModelIdx;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�̐F��ݒ肷�鏈��
//=============================================================================
void CEditer::SetBillCol(const D3DXCOLOR BillCol)
{
	m_BillCol = BillCol;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�̓����蔻������͈͂�ݒ肷�鏈��
//=============================================================================
void CEditer::SetBillColRange(const float fBillColRange)
{
	m_fBillColRange = fBillColRange;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�̌�����ݒ肷�鏈��
//=============================================================================
void CEditer::SetBillRot(const float fBillRot)
{
	m_fBillRot = fBillRot;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�̕����擾���鏈��
//=============================================================================
void CEditer::SetBillWidth(const float fBillWidth)
{
	m_fBillWidth = fBillWidth;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�̍�����ݒ肷�鏈��
//=============================================================================
void CEditer::SetBillHeight(const float fBillHeight)
{
	m_fBillHeight = fBillHeight;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�̍��W��ݒ肷�鏈��
//=============================================================================
void CEditer::SetBillTexIdx(const int nTexIdx)
{
	m_nBillTexIdx = nTexIdx;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�̍��W��ݒ肷�鏈��
//=============================================================================
void CEditer::SetBillLighting(const bool bLighting)
{
	m_bBillLighting = bLighting;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�̍��W��ݒ肷�鏈��
//=============================================================================
void CEditer::SetBillDrawAddtive(const bool bDrawAddtive)
{
	m_bBillDrawAddtive = bDrawAddtive;
}

//=============================================================================
//    �z�u�ł���G�t�F�N�g�̍��W��ݒ肷�鏈��
//=============================================================================
void CEditer::SetEffectType(const int nEffectType)
{
	m_nEffectType = nEffectType;
}

//=============================================================================
//    �z�u�ł���G�̎�ނ�ݒ肷�鏈��
//=============================================================================
void CEditer::SetEnemyType(const int nEnemyType)
{
	m_nEnemyType = nEnemyType;
}

//=============================================================================
//    �z�u�ł���G�̗̑͂�ݒ肷�鏈��
//=============================================================================
void CEditer::SetEnemyLife(const int nEnemyLife)
{
	m_nEnemyLife = nEnemyLife;
}

//=============================================================================
//    �z�u�ł���G�̃X�R�A��ݒ肷�鏈��
//=============================================================================
void CEditer::SetEnemyScore(const int nEnemyScore)
{
	m_nEnemyScore = nEnemyScore;
}

//=============================================================================
//    �z�u�ł���ǂ̃e�N�X�`���ԍ���ݒ肷�鏈��
//=============================================================================
void CEditer::SetWallTexIdx(const int nWallTexIdx)
{
	m_nWallTexIdx = nWallTexIdx;
}

//=============================================================================
//    �z�u�ł���ǂ̐F��ݒ肷�鏈��
//=============================================================================
void CEditer::SetWallCol(const D3DXCOLOR WallCol)
{
	m_WallCol = WallCol;
}

//=============================================================================
//    �z�u�ł���ǂ�1�u���b�N���̕���ݒ肷�鏈��
//=============================================================================
void CEditer::SetWallWidth(const float fWidth)
{
	m_fWallWidth = fWidth;
}

//=============================================================================
//    �z�u�ł���ǂ�1�u���b�N���̍�����ݒ肷�鏈��
//=============================================================================
void CEditer::SetWallHeight(const float fHeight)
{
	m_fWallHeight = fHeight;
}

//=============================================================================
//    �z�u�ł���ǂ̉��̕�������ݒ肷�鏈��
//=============================================================================
void CEditer::SetWallXBlock(const int nXBlock)
{
	m_nWallXBlock = nXBlock;
}

//=============================================================================
//    �z�u�ł���ǂ̏c�̕�������ݒ肷�鏈��
//=============================================================================
void CEditer::SetWallYBlock(const int nYBlock)
{
	m_nWallYBlock = nYBlock;
}

//=============================================================================
//    �z�u�ł���ǂ̃e�N�X�`��V���W�̕�������ݒ肷�鏈��
//=============================================================================
void CEditer::SetWallTexSplitU(const int nWallTexSplitU)
{
	m_nWallTexSplitU = nWallTexSplitU;
}

//=============================================================================
//    �z�u�ł���ǂ̃e�N�X�`��V���W�̕�������ݒ肷�鏈��
//=============================================================================
void CEditer::SetWallTexSplitV(const int nWallTexSplitV)
{
	m_nWallTexSplitV = nWallTexSplitV;
}

//=============================================================================
//    �z�u�ł���A�C�e���̐F��ݒ肷�鏈��
//=============================================================================
void CEditer::SetItemCol(const D3DXCOLOR ItemCol)
{
	m_ItemCol = ItemCol;
}

//=============================================================================
//    �z�u�ł���A�C�e���̓����蔻������͈͂�ݒ肷�鏈��
//=============================================================================
void CEditer::SetItemColRange(const float fItemColRange)
{
	m_fItemColRange = fItemColRange;
}

//=============================================================================
//    �z�u�ł���A�C�e���̌�����ݒ肷�鏈��
//=============================================================================
void CEditer::SetItemRot(const float fItemRot)
{
	m_fItemRot = fItemRot;
}

//=============================================================================
//    �z�u�ł���A�C�e���̕���ݒ肷�鏈��
//=============================================================================
void CEditer::SetItemWidth(const float fItemWidth)
{
	m_fItemWidth = fItemWidth;
}

//=============================================================================
//    �z�u�ł���A�C�e���̍�����ݒ肷�鏈��
//=============================================================================
void CEditer::SetItemHeight(const float fItemHeight)
{
	m_fItemHeight = fItemHeight;
}

//=============================================================================
//    �z�u�ł���A�C�e���̃e�N�X�`���ԍ���ݒ肷�鏈��
//=============================================================================
void CEditer::SetItemTexIdx(const int nTexIdx)
{
	m_nItemTexIdx = nTexIdx;
}

//=============================================================================
//    �z�u�ł���A�C�e���̎�ޔԍ���ݒ肷�鏈��
//=============================================================================
void CEditer::SetItemType(const int nType)
{
	m_nItemType = nType;
}

//=============================================================================
//    �z�u�ł���A�C�e���̃��C�e�B���O���邩���Ȃ�����ݒ肷�鏈��
//=============================================================================
void CEditer::SetItemLighting(const bool bLighting)
{
	m_bItemLighting = bLighting;;
}

//=============================================================================
//    �z�u�ł���A�C�e���̉��Z�����ŕ`�悷�邩���Ȃ�����ݒ肷�鏈��
//=============================================================================
void CEditer::SetItemDrawAddtive(const bool bDrawAddtive)
{
	m_bItemDrawAddtive = bDrawAddtive;
}

//=============================================================================
//    �z�u�I�u�W�F�N�g�̍��W���擾���鏈��
//=============================================================================
D3DXVECTOR3 CEditer::GetPos(void)
{
	return m_SetPos;
}

//=============================================================================
//    �z�u�I�u�W�F�N�g�̌������擾���鏈��
//=============================================================================
D3DXVECTOR3 CEditer::GetRot(void)
{
	return m_SetRot;
}

//=============================================================================
//    �z�u�p���f���̎�ނ��擾���鏈��
//=============================================================================
CEditer::TYPE CEditer::GetType(void)
{
	return m_Type;
}

//=============================================================================
//    �G�t�F�N�g�̏�Ԃ��擾���鏈��
//=============================================================================
CEditer::EFFECTSTATE CEditer::GetEffectState(void)
{
	return m_EffectState;
}

//=============================================================================
//    �z�u�ł��郂�f���ւ̃|�C���^���擾���鏈��
//=============================================================================
CSetObject *CEditer::GetSetObject(void)
{
	return m_pSetObject;
}

//=============================================================================
//    �z�u�ł��铮�����f���ւ̃|�C���^���擾���鏈��
//=============================================================================
CSetObjectMove *CEditer::GetSetObjectMove(void)
{
	return m_pSetObjectMove;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�ւ̃|�C���^���擾���鏈��
//=============================================================================
CSetBillboardObject *CEditer::GetSetBillObj(void)
{
	return m_pSetBillboardObj;
}

//=============================================================================
//    �z�u�ł���G�t�F�N�g�G�~�b�^�ւ̃|�C���^���擾���鏈��
//=============================================================================
CEmitter *CEditer::GetSetEmitter(void)
{
	return m_pSetEmitter;
}

//=============================================================================
//    �z�u�ł���G�N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CSetEnemy *CEditer::GetEnemy(void)
{
	return m_pSetEnemy;
}

//=============================================================================
//    �z�u�p�̕ǃN���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CSetMeshWall *CEditer::GetSetMeshWall(void)
{
	return m_pSetWall;
}

//=============================================================================
//    �z�u�p�̃A�C�e���N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CSetItem *CEditer::GetSetItem(void)
{
	return m_pSetItem;
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CTextureManager *CEditer::GetTextureManager(void)
{
	return m_pTextureManager;
}

//=============================================================================
//    �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CEffectManager *CEditer::GetEffectManager(void)
{
	return m_pEffectManager;
}

//=============================================================================
//    �G�Ǌ��܂Ƃ߃N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CEnemyAssembly *CEditer::GetEnemyAssembly(void)
{
	return m_pEnemyAssmbly;
}

//=============================================================================
//    �}�b�v�N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CMap *CEditer::GetMap(void)
{
	return m_pMap;
}

//=============================================================================
//    �z�u�p���f���̓����蔻��̎�ނ��擾���鏈��
//=============================================================================
CObject::COL_TYPE CEditer::GetModelColType(void)
{
	return m_ModelColType;
}

//=============================================================================
//    �z�u�p���f���̓����蔻������͈͂��擾���鏈��
//=============================================================================
float CEditer::GetModelColRange(void)
{
	return m_fModelColRange;
}

//=============================================================================
//    �z�u�p���f���̑傫���̔{�����擾���鏈��
//=============================================================================
D3DXVECTOR3 CEditer::GetModelScale(void)
{
	return m_ModelScale;
}

//=============================================================================
//    �z�u�p���f���̉�]�X�s�[�h���擾���鏈��
//=============================================================================
D3DXVECTOR3 CEditer::GetModelRollSpeed(void)
{
	return m_ModelRollSpeed;
}

//=============================================================================
//    �z�u�p���f���̈ړ��ʂ��擾���鏈��
//=============================================================================
D3DXVECTOR3 CEditer::GetModelMove(void)
{
	return m_ModelMove;
}

//=============================================================================
//    �z�u�p���f���̈ړ��ʂ�؂�ւ���^�C�~���O���擾���鏈��
//=============================================================================
int CEditer::GetModelMaxMoveCounter(void)
{
	return m_nModelMaxMoveCounter;
}

//=============================================================================
//    �z�u�ł��郂�f���̎�ނ̐����擾���鏈��
//=============================================================================
int CEditer::GetNumModel(void)
{
	return m_nNumModel;
}

//=============================================================================
//    ���ݑI������Ă��郂�f���̔ԍ����擾���鏈��
//=============================================================================
int CEditer::GetSelectModelIdx(void)
{
	return m_nSelectModelIdx;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�̐F���擾���鏈��
//=============================================================================
D3DXCOLOR CEditer::GetBillCol(void)
{
	return m_BillCol;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�̓����蔻������͈͂��擾���鏈��
//=============================================================================
float CEditer::GetBillColRange(void)
{
	return m_fBillColRange;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�̔ԍ����擾���鏈��
//=============================================================================
float CEditer::GetBillRot(void)
{
	return m_fBillRot;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�̔ԍ����擾���鏈��
//=============================================================================
float CEditer::GetBillWidth(void)
{
	return m_fBillWidth;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�̔ԍ����擾���鏈��
//=============================================================================
float CEditer::GetBillHeight(void)
{
	return m_fBillHeight;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�̔ԍ����擾���鏈��
//=============================================================================
int CEditer::GetBillTexIdx(void)
{
	return m_nBillTexIdx;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�̔ԍ����擾���鏈��
//=============================================================================
bool CEditer::GetBillLighting(void)
{
	return m_bBillLighting;
}

//=============================================================================
//    �z�u�ł���r���{�[�h�̉��Z�����ŕ`�悷�邩���Ȃ������擾���鏈��
//=============================================================================
bool CEditer::GetBillDrawAddtive(void)
{
	return m_bBillDrawAddtive;
}

//=============================================================================
//    �ݒ�ł���e�N�X�`���̎�ނ̐����擾���鏈��
//=============================================================================
int CEditer::GetNumTex(void)
{
	return m_nNumTex;
}

//=============================================================================
//    �z�u�ł���G�t�F�N�g�̎�ޔԍ����擾���鏈��
//=============================================================================
int CEditer::GetEffectType(void)
{
	return m_nEffectType;
}

//=============================================================================
//    �z�u�ł���G�̎�ނ��擾���鏈��
//=============================================================================
int CEditer::GetEnemyType(void)
{
	return m_nEnemyType;
}

//=============================================================================
//    �z�u�ł���G�̗̑͂��擾���鏈��
//=============================================================================
int CEditer::GetEnemyLife(void)
{
	return m_nEnemyLife;
}

//=============================================================================
//    �z�u�ł���G�̃X�R�A���擾���鏈��
//=============================================================================
int CEditer::GetEnemyScore(void)
{
	return m_nEnemyScore;
}

//=============================================================================
//    �z�u�ł���ǂ̃e�N�X�`���ԍ����擾���鏈��
//=============================================================================
int CEditer::GetWallTexIdx(void)
{
	return m_nWallTexIdx;
}

//=============================================================================
//    �z�u�ł���ǂ̐F���擾���鏈��
//=============================================================================
D3DXCOLOR CEditer::GetWallCol(void)
{
	return m_WallCol;
}

//=============================================================================
//    �z�u�ł���ǂ�1�u���b�N���̕����擾���鏈��
//=============================================================================
float CEditer::GetWallWidth(void)
{
	return m_fWallWidth;
}

//=============================================================================
//    �z�u�ł���ǂ�1�u���b�N���̍������擾���鏈��
//=============================================================================
float CEditer::GetWallHeight(void)
{
	return m_fWallHeight;
}

//=============================================================================
//    �z�u�ł���ǂ̉��̕��������擾���鏈��
//=============================================================================
int CEditer::GetWallXBlock(void)
{
	return m_nWallXBlock;
}

//=============================================================================
//    �z�u�ł���ǂ̏c�̕��������擾���鏈��
//=============================================================================
int CEditer::GetWallYBlock(void)
{
	return m_nWallYBlock;
}

//=============================================================================
//    �z�u�ł���ǂ̃e�N�X�`��U���W�̕��������擾���鏈��
//=============================================================================
int CEditer::GetWallTexSplitU(void)
{
	return m_nWallTexSplitU;
}

//=============================================================================
//    �z�u�ł���ǂ̃e�N�X�`��V���W�̕��������擾���鏈��
//=============================================================================
int CEditer::GetWallTexSplitV(void)
{
	return m_nWallTexSplitV;
}

//=============================================================================
//    �z�u�ł���A�C�e���̐F���擾���鏈��
//=============================================================================
D3DXCOLOR CEditer::GetItemCol(void)
{
	return m_ItemCol;
}

//=============================================================================
//    �z�u�ł���A�C�e���̓����蔻������͈͂��擾���鏈��
//=============================================================================
float CEditer::GetItemColRange(void)
{
	return m_fItemColRange;
}

//=============================================================================
//    �z�u�ł���A�C�e���̌������擾���鏈��
//=============================================================================
float CEditer::GetItemRot(void)
{
	return m_fItemRot;
}

//=============================================================================
//    �z�u�ł���A�C�e���̕����擾���鏈��
//=============================================================================
float CEditer::GetItemWidth(void)
{
	return m_fItemWidth;
}

//=============================================================================
//    �z�u�ł���A�C�e���̍������擾���鏈��
//=============================================================================
float CEditer::GetItemHeight(void)
{
	return m_fItemHeight;
}

//=============================================================================
//    �z�u�ł���A�C�e���̃e�N�X�`���ԍ����擾���鏈��
//=============================================================================
int CEditer::GetItemTexIdx(void)
{
	return m_nItemTexIdx;
}

//=============================================================================
//    �z�u�ł���A�C�e���̎�ޔԍ����擾���鏈��
//=============================================================================
int CEditer::GetItemType(void)
{
	return m_nItemType;
}

//=============================================================================
//    �z�u�ł���A�C�e���̃��C�e�B���O���邩���Ȃ������擾���鏈��
//=============================================================================
bool CEditer::GetItemLighting(void)
{
	return m_bItemLighting;
}

//=============================================================================
//    �z�u�ł���A�C�e���̉��Z�����ŕ`�悷�邩���Ȃ������擾���鏈��
//=============================================================================
bool CEditer::GetItemDrawAddtive(void)
{
	return m_bItemDrawAddtive;
}