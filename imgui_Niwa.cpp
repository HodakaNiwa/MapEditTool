//*****************************************************************************
//
//     ImGui�̏���(Version.Niwa)[imgui_Niwa.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "imgui_Niwa.h"
#include "imconfig.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"
#include "imstb_rectpack.h"
#include "imstb_textedit.h"
#include "imstb_truetype.h"

#include "manager.h"
#include "camera.h"
#include "object.h"
#include "billboardObject.h"
#include "textureManager.h"
#include "effectManager.h"
#include "emitter.h"
#include "meshWall.h"
#include "item.h"
#include "editer.h"
#include "map.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CImGui_Niwa::CImGui_Niwa()
{
	// �e��l�̃N���A
	m_bDisp = true;      // �`�悷�邩���Ȃ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CImGui_Niwa::~CImGui_Niwa()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CImGui_Niwa::Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice)
{
	// ImGui�p�̃e�L�X�g�f�[�^�𐶐�����
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//ImFontConfig config;
	//config.MergeMode = true;
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:/Windows/Fonts/meiryo.ttc", 18.0f, &config, io.Fonts->GetGlyphRangesJapanese());

	// AddFontFromFileTTF�֐�
	// ��P���� : �t�H���g�f�[�^�̃t�@�C���p�X��
	// ��Q���� : �t�H���g�̃T�C�Y
	// ��R���� : NULL��OK
	// ��S���� : ���{��ɂ��������io.Fonts->GetGlyphRangesJapanese�Ŋ�{�I��OK
	io.Fonts->AddFontFromFileTTF("c:/Windows/Fonts/meiryo.ttc",20.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	// ImGui�𐶐�����
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);
	ImGui::StyleColorsClassic();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CImGui_Niwa::Uninit(void)
{
	// ImGui���I��������
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CImGui_Niwa::Update(void)
{
	// �V�����t���[����p�ӂ���
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// �f�o�b�O�E�B���h�E�𐶐�����
	SetWindowDebug();

	// �t���[�����I��������
	ImGui::EndFrame();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CImGui_Niwa::Draw(void)
{
	if (m_bDisp == true)
	{// �`�悷���ԂȂ��
		// Update�Ő�������ImGui��\������
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
}

//=============================================================================
//    �f�o�C�X�����X�g�����ꍇ�̏���
//=============================================================================
void CImGui_Niwa::CheakDeviceLost(HRESULT result, LPDIRECT3DDEVICE9 pDevice)
{
	D3DPRESENT_PARAMETERS d3dpp;
	if (result == D3DERR_DEVICELOST && pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		pDevice->Reset(&d3dpp);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

//=============================================================================
//    �f�o�b�O�E�B���h�E��\�����鏈��
//=============================================================================
void CImGui_Niwa::SetWindowDebug(void)
{
	// ���ݐ����ł���I�u�W�F�N�g�̎�ނɂ���ď����킯
	// �G�f�B�^�[�N���X���擾����
	CEditer *pEditer = CManager::GetEditer();

	if (pEditer != NULL)
	{// �G�f�B�^�[�N���X���擾�ł���
	    // �E�B���h�E�����J�n
		ImGui::Begin(u8"�}�b�v�G�f�B�^�[ Ver1.6", false, ImGuiWindowFlags_NoMove);

		// FPS�`��
		ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);

		// �J�������`��
		ImGui::Text(u8"�J�����̎��_    : %.1f %.1f %.1f", CManager::GetCamera()->GetPosV().x, CManager::GetCamera()->GetPosV().y, CManager::GetCamera()->GetPosV().z);
		ImGui::Text(u8"�J�����̒����_ : %.1f %.1f %.1f", CManager::GetCamera()->GetPosR().x, CManager::GetCamera()->GetPosR().y, CManager::GetCamera()->GetPosR().z);
		ImGui::Text(u8"�J�����̌���    : %.1f %.1f %.1f", CManager::GetCamera()->GetRot().x, CManager::GetCamera()->GetRot().y, CManager::GetCamera()->GetRot().z);
		ImGui::Text("");

		// �z�u����I�u�W�F�N�g�̎�ނ�\��
		ImGui::CollapsingHeader(u8"�z�u����I�u�W�F�N�g�̎��");
		int nObjType = pEditer->GetType();
		int nObjTypeOld = nObjType;
		int nObjTypeDef = nObjType;
		ImGui::RadioButton(u8"�z�u��", &nObjType, 0);
		ImGui::RadioButton(u8"�����z�u��", &nObjType, 1);
		ImGui::RadioButton(u8"�r���{�[�h", &nObjType, 2);
		ImGui::RadioButton(u8"�G�t�F�N�g", &nObjType, 3);
		ImGui::RadioButton(u8"�G", &nObjType, 4);
		ImGui::RadioButton(u8"��", &nObjType, 5);
		ImGui::RadioButton(u8"�A�C�e��", &nObjType, 6);
		ImGui::Text("");

		if (nObjType != nObjTypeDef)
		{// �z�u����I�u�W�F�N�g�̎�ނ��ύX���ꂽ
			pEditer->SetType((CEditer::TYPE)nObjType);
			pEditer->ObjectChange((CEditer::TYPE)nObjTypeOld);
		}

		// �E�B���h�E�����I��
		ImGui::End();

		// �E�B���h�E�����J�n
		ImGui::Begin(u8"�I�u�W�F�N�g�̏��", false, ImGuiWindowFlags_NoMove);

		// �I�u�W�F�N�g�̎�ނɂ���ĕ\������f�[�^�����ւ���
		if (nObjType == CEditer::TYPE_OBJECT || nObjType == CEditer::TYPE_OBJECTMOVE)
		{
			SetObjectDebug();
		}
		else if (nObjType == CEditer::TYPE_BILLBOARD)
		{
			SetBillboardDebug();
		}
		else if (nObjType == CEditer::TYPE_EFFECT)
		{
			SetEffectDebug();
		}
		else if (nObjType == CEditer::TYPE_ENEMY)
		{
			SetEnemyDebug();
		}
		else if (nObjType == CEditer::TYPE_WALL)
		{
			SetWallDebug();
		}
		else if (nObjType == CEditer::TYPE_ITEM)
		{
			SetItemDebug();
		}

		// �E�B���h�E�����I��
		ImGui::End();
	}
}

//=============================================================================
//    �I�u�W�F�N�g�̏���\�����鏈��
//=============================================================================
void CImGui_Niwa::SetObjectDebug(void)
{
	// �G�f�B�^�[�N���X���擾����
	CEditer *pEditer = CManager::GetEditer();

	if (pEditer != NULL)
	{// �G�f�B�^�[�N���X���擾�ł���
		// �z�u�p���f���̏����擾
		bool bCostom = false;
		int nType = (int)pEditer->GetType();
		int nSelectModel = pEditer->GetSelectModelIdx();
		int nModelColType = pEditer->GetModelColType();
		float fModelColRange = pEditer->GetModelColRange();
		float fModelPos[3] = { pEditer->GetPos().x,pEditer->GetPos().y,pEditer->GetPos().z };
		float fModelRot[3] = { pEditer->GetRot().x,pEditer->GetRot().y,pEditer->GetRot().z };
		float fModelScale[3] = { pEditer->GetModelScale().x,pEditer->GetModelScale().y,pEditer->GetModelScale().z };
		float fModelRollSpeed[3] = { pEditer->GetModelRollSpeed().x,pEditer->GetModelRollSpeed().y,pEditer->GetModelRollSpeed().z };
		float fModelMove[3] = { pEditer->GetModelMove().x,pEditer->GetModelMove().y,pEditer->GetModelMove().z };
		int nModelMaxCounter = pEditer->GetModelMaxMoveCounter();


		if (nType == CEditer::TYPE_OBJECT)
		{// ���ʂ̃I�u�W�F�N�g�̎�ނ�������
			// �ʏ�̃I�u�W�F�N�g�̃w�b�_�쐬
			ImGui::CollapsingHeader(u8"�z�u��");

			// �z�u���郂�f���̔ԍ�
			int nSelectModelDef = nSelectModel;
			bCostom = ImGui::InputInt(u8"�g�p���郂�f���̔ԍ�", &nSelectModel);
			if (nSelectModel != nSelectModelDef)
			{
				if (nSelectModel >= 0 && nSelectModel < pEditer->GetNumModel())
				{
					pEditer->SetSelectModelIdx(nSelectModel);
					pEditer->ModelChange();
				}
			}

			// �����蔻��̎��
			ImGui::Text(u8"�����蔻��̎��   ");
			ImGui::SameLine();
			ImGui::RadioButton(u8"�l�p", &nModelColType, 0);
			ImGui::SameLine();
			ImGui::RadioButton(u8"�~�`", &nModelColType, 1);
			ImGui::SameLine();
			ImGui::RadioButton(u8"���肵�Ȃ�", &nModelColType, 2);

			// �����蔻������͈�
			ImGui::InputFloat(u8"�����蔻������͈�", &fModelColRange);

			// �z�u�p���f���̍��W
			ImGui::InputFloat3(u8"���W", &fModelPos[0]);

			// �z�u�p���f���̌���
			float fModelRotDef[3] = { fModelRot[0], fModelRot[1] ,fModelRot[2] };
			// ������␳���Ă���
			fModelRot[0] = D3DXToDegree(fModelRot[0]);
			fModelRot[1] = D3DXToDegree(fModelRot[1]);
			fModelRot[2] = D3DXToDegree(fModelRot[2]);

			ImGui::InputFloat3(u8"����", &fModelRot[0]);

			// ������␳���Ă���
			fModelRot[0] = D3DXToRadian(fModelRot[0]);
			fModelRot[1] = D3DXToRadian(fModelRot[1]);
			fModelRot[2] = D3DXToRadian(fModelRot[2]);

			bool bCheakRot = false;
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{// ���̐����J��Ԃ�
				if (fModelRotDef[nCnt] != fModelRot[nCnt])
				{// �������ς���Ă���
					bCheakRot = true;
				}
			}
			if (bCheakRot == true)
			{
				if (nType == CEditer::TYPE_OBJECT)
				{
					pEditer->GetSetObject()->SetRot(D3DXVECTOR3(fModelRot[0], fModelRot[1], fModelRot[2]));
				}
				else if (nType == CEditer::TYPE_OBJECTMOVE)
				{
					pEditer->GetSetObjectMove()->SetRot(D3DXVECTOR3(fModelRot[0], fModelRot[1], fModelRot[2]));
				}
			}

			// �z�u�p���f���̑傫��
			ImGui::InputFloat3(u8"�傫��", &fModelScale[0]);

			// �z�u�p���f���̉�]�X�s�[�h
			ImGui::InputFloat3(u8"��]�X�s�[�h", &fModelRollSpeed[0]);
		}

		if (nType == CEditer::TYPE_OBJECTMOVE)
		{// �����I�u�W�F�N�g�̎�ނ�������
			// �����I�u�W�F�N�g�̃w�b�_�쐬
			ImGui::CollapsingHeader(u8"�����z�u��");

			// �z�u���郂�f���̔ԍ�
			int nSelectModelDef = nSelectModel;
			bCostom = ImGui::InputInt(u8"�g�p���郂�f���̔ԍ�", &nSelectModel);
			if (nSelectModel != nSelectModelDef)
			{
				if (nSelectModel >= 0 && nSelectModel < pEditer->GetNumModel())
				{
					pEditer->SetSelectModelIdx(nSelectModel);
					pEditer->ModelChange();
				}
			}

			// �z�u�p���f���̍��W
			ImGui::InputFloat3(u8"���W", &fModelPos[0]);

			// �z�u�p���f���̌���
			float fModelRotDef[3] = { fModelRot[0], fModelRot[1] ,fModelRot[2] };
			// ������␳���Ă���
			fModelRot[0] = D3DXToDegree(fModelRot[0]);
			fModelRot[1] = D3DXToDegree(fModelRot[1]);
			fModelRot[2] = D3DXToDegree(fModelRot[2]);

			ImGui::InputFloat3(u8"����", &fModelRot[0]);

			// ������␳���Ă���
			fModelRot[0] = D3DXToRadian(fModelRot[0]);
			fModelRot[1] = D3DXToRadian(fModelRot[1]);
			fModelRot[2] = D3DXToRadian(fModelRot[2]);

			bool bCheakRot = false;
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{// ���̐����J��Ԃ�
				if (fModelRotDef[nCnt] != fModelRot[nCnt])
				{// �������ς���Ă���
					bCheakRot = true;
				}
			}
			if (bCheakRot == true)
			{
				if (nType == CEditer::TYPE_OBJECT)
				{
					pEditer->GetSetObject()->SetRot(D3DXVECTOR3(fModelRot[0], fModelRot[1], fModelRot[2]));
				}
				else if (nType == CEditer::TYPE_OBJECTMOVE)
				{
					pEditer->GetSetObjectMove()->SetRot(D3DXVECTOR3(fModelRot[0], fModelRot[1], fModelRot[2]));
				}
			}

			// �z�u�p���f���̑傫��
			ImGui::InputFloat3(u8"�傫��", &fModelScale[0]);

			// �z�u�p���f���̉�]�X�s�[�h
			ImGui::InputFloat3(u8"��]�X�s�[�h", &fModelRollSpeed[0]);

			// �z�u�p���f���̈ړ���
			ImGui::InputFloat3(u8"�ړ���", &fModelMove[0]);

			// �z�u�p���f���̈ړ���
			int nModelMaxCounterDef = nModelMaxCounter;
			ImGui::InputInt(u8"�ړ��ʐ؂�ւ��l", &nModelMaxCounter);
			if (nModelMaxCounter < 0)
			{// 0�����ɂȂ���
				nModelMaxCounter = nModelMaxCounterDef;
			}
		}

		// �z�u�p���f���̏���ݒ�
		if (nType == CEditer::TYPE_OBJECT)
		{
			pEditer->GetSetObject()->SetScale(D3DXVECTOR3(fModelScale[0], fModelScale[1], fModelScale[2]));
		}
		else if (nType == CEditer::TYPE_OBJECTMOVE)
		{
			pEditer->GetSetObjectMove()->SetScale(D3DXVECTOR3(fModelScale[0], fModelScale[1], fModelScale[2]));
		}

		pEditer->SetPos(D3DXVECTOR3(fModelPos[0], fModelPos[1], fModelPos[2]));
		pEditer->SetRot(D3DXVECTOR3(fModelRot[0], fModelRot[1], fModelRot[2]));
		pEditer->SetModelScale(D3DXVECTOR3(fModelScale[0], fModelScale[1], fModelScale[2]));
		pEditer->SetModelRollSpeed(D3DXVECTOR3(fModelRollSpeed[0], fModelRollSpeed[1], fModelRollSpeed[2]));
		pEditer->SetModelColType((CObject::COL_TYPE)nModelColType);
		pEditer->SetModelColRange(fModelColRange);
		pEditer->SetModelMove(D3DXVECTOR3(fModelMove[0], fModelMove[1], fModelMove[2]));
		pEditer->SetModelMaxMoveCounter(nModelMaxCounter);

		if (bCostom == true)
		{// �����f�[�^��ҏW���Ă���
			CManager::GetCamera()->SetMove(false);
		}
		else if (bCostom == false)
		{// �����f�[�^��ҏW���Ă��Ȃ�
			CManager::GetCamera()->SetMove(true);
		}
	}
}

//=============================================================================
//    �z�u�r���{�[�h�̏���\�����鏈��
//=============================================================================
void CImGui_Niwa::SetBillboardDebug(void)
{
	// �G�f�B�^�[�N���X���擾����
	CEditer *pEditer = CManager::GetEditer();

	if (pEditer != NULL)
	{// �G�f�B�^�[�N���X���擾�ł���
		// �z�u�r���{�[�h�̏����擾
		bool bCostom = false;
		float fBillPos[3] = { pEditer->GetPos().x,pEditer->GetPos().y,pEditer->GetPos().z };
		float fBillCol[4] = { pEditer->GetBillCol().r ,pEditer->GetBillCol().g ,pEditer->GetBillCol().b ,pEditer->GetBillCol().a };
		float fBillColRange = pEditer->GetBillColRange();
		float fBillWidth = pEditer->GetBillWidth();
		float fBillHeight = pEditer->GetBillHeight();
		float fBillRot = pEditer->GetBillRot();
		bool bBillLighting = pEditer->GetBillLighting();
		bool bBillDrawAddtive = pEditer->GetBillDrawAddtive();
		int nBillTexIdx = pEditer->GetBillTexIdx();

		// ������␳���Ă���
		fBillRot = D3DXToDegree(fBillRot);

		// �w�b�_�����J�n
		ImGui::CollapsingHeader(u8"�r���{�[�h");

		// �z�u�r���{�[�h�̍��W
		ImGui::InputFloat3(u8"���W", &fBillPos[0]);

		// �z�u�r���{�[�h�̐F
		bCostom = ImGui::ColorEdit4(u8"�F", &fBillCol[0]);

		// �z�u�r���{�[�h�̓����蔻������͈�
		ImGui::InputFloat(u8"�����蔻������͈�", &fBillColRange);

		// �z�u�r���{�[�h�̕�
		ImGui::InputFloat(u8"��", &fBillWidth);

		// �z�u�r���{�[�h�̍���
		ImGui::InputFloat(u8"����", &fBillHeight);

		// �z�u�r���{�[�h�̌���
		ImGui::InputFloat(u8"����", &fBillRot);

		// ������␳���Ă���
		fBillRot = D3DXToRadian(fBillRot);

		// �z�u�r���{�[�h�����C�e�B���O���邩���Ȃ���
		ImGui::Checkbox(u8"���C�e�B���O�̗L��", &bBillLighting);

		// �z�u�r���{�[�h�����Z�����ŕ`�悷�邩���Ȃ���
		ImGui::Checkbox(u8"���Z�������邩���Ȃ���", &bBillDrawAddtive);

		// �z�u�r���{�[�h�̃e�N�X�`���ԍ�
		int nBillTexIdxDef = nBillTexIdx;
		bool bTexChange = false;
		ImGui::InputInt(u8"�e�N�X�`���ԍ�", &nBillTexIdx);
		if (nBillTexIdx < 0 || nBillTexIdx >= pEditer->GetNumTex())
		{// �ǂݍ��񂾃e�N�X�`���̐���������
			nBillTexIdx = nBillTexIdxDef;
		}
		if (nBillTexIdx != nBillTexIdxDef)
		{
			bTexChange = true;
			pEditer->GetSetBillObj()->BindTexture(pEditer->GetTextureManager()->GetTexture(nBillTexIdx));
		}


		// �e��l��ݒ肷��
		pEditer->SetPos(D3DXVECTOR3(fBillPos[0], fBillPos[1], fBillPos[2]));
		pEditer->SetBillCol(D3DXCOLOR(fBillCol[0], fBillCol[1], fBillCol[2], fBillCol[3]));
		pEditer->SetBillColRange(fBillColRange);
		pEditer->SetBillWidth(fBillWidth);
		pEditer->SetBillHeight(fBillHeight);
		pEditer->SetBillRot(fBillRot);
		pEditer->SetBillLighting(bBillLighting);
		pEditer->SetBillDrawAddtive(bBillDrawAddtive);
		pEditer->SetBillTexIdx(nBillTexIdx);

		CSetBillboardObject *pSetBillSbj = pEditer->GetSetBillObj();
		if (pSetBillSbj != NULL)
		{
			pSetBillSbj->SetPos(D3DXVECTOR3(fBillPos[0], fBillPos[1], fBillPos[2]));
			pSetBillSbj->SetCol(D3DXCOLOR(fBillCol[0], fBillCol[1], fBillCol[2], fBillCol[3]));
			pSetBillSbj->SetColRange(fBillColRange);
			pSetBillSbj->SetWidth(fBillWidth);
			pSetBillSbj->SetHeight(fBillHeight);
			pSetBillSbj->SetRot(fBillRot);
			pSetBillSbj->SetLighting(bBillLighting);
			pSetBillSbj->SetDrawAddtive(bBillDrawAddtive);
			pSetBillSbj->SetTexIdx(nBillTexIdx);
			pSetBillSbj->ChangeVertex();
		}

		if (bCostom == true)
		{// �����f�[�^��ҏW���Ă���
			CManager::GetCamera()->SetMove(false);
		}
		else if (bCostom == false)
		{// �����f�[�^��ҏW���Ă��Ȃ�
			CManager::GetCamera()->SetMove(true);
		}
	}
}

//=============================================================================
//    �z�u�G�t�F�N�g�̏���\�����鏈��
//=============================================================================
void CImGui_Niwa::SetEffectDebug(void)
{
	// �G�f�B�^�[�N���X���擾����
	CEditer *pEditer = CManager::GetEditer();

	if (pEditer != NULL)
	{// �G�f�B�^�[�N���X���擾�ł���
		// �z�u�G�t�F�N�g�̏����擾
		bool bCostom = false;
		float fEffectPos[3] = { pEditer->GetPos().x,pEditer->GetPos().y,pEditer->GetPos().z };
		float fEffectRot[3] = { pEditer->GetRot().x,pEditer->GetRot().y,pEditer->GetRot().z };
		int nEffectType = pEditer->GetEffectType();

		// ������␳���Ă���
		fEffectRot[0] = D3DXToDegree(fEffectRot[0]);
		fEffectRot[1] = D3DXToDegree(fEffectRot[1]);
		fEffectRot[2] = D3DXToDegree(fEffectRot[2]);

		// �w�b�_�����J�n
		ImGui::CollapsingHeader(u8"�G�t�F�N�g");
		// �z�u�G�t�F�N�g�̍��W
		ImGui::InputFloat3(u8"���W", &fEffectPos[0]);

		// �z�u�G�t�F�N�g�̌���
		ImGui::InputFloat3(u8"����", &fEffectRot[0]);

		// ������␳���Ă���
		fEffectRot[0] = D3DXToRadian(fEffectRot[0]);
		fEffectRot[1] = D3DXToRadian(fEffectRot[1]);
		fEffectRot[2] = D3DXToRadian(fEffectRot[2]);

		// �z�u�G�t�F�N�g�̎��
		int nEffectTypeDef = nEffectType;
		ImGui::InputInt(u8"��ޔԍ�", &nEffectType);
		if (nEffectType < 0 || nEffectType >= pEditer->GetEffectManager()->GetNumEmitterData())
		{// �ǂݍ��񂾃G�t�F�N�g�f�[�^�̐���������
			nEffectType = nEffectTypeDef;
			pEditer->SetEffectType(nEffectType);
		}
		if (nEffectType != nEffectTypeDef)
		{
			pEditer->SetEffectType(nEffectType);
			pEditer->EmitterChange();
		}

		// �e��l��ݒ肷��
		pEditer->SetPos(D3DXVECTOR3(fEffectPos[0], fEffectPos[1], fEffectPos[2]));
		pEditer->SetRot(D3DXVECTOR3(fEffectRot[0], fEffectRot[1], fEffectRot[2]));

		CEmitter *pEmitter = pEditer->GetSetEmitter();
		if (pEmitter != NULL)
		{
			pEmitter->SetPos(D3DXVECTOR3(fEffectPos[0], fEffectPos[1], fEffectPos[2]));
			pEmitter->SetRot(D3DXVECTOR3(fEffectRot[0], fEffectRot[1], fEffectRot[2]));
		}

		if (bCostom == true)
		{// �����f�[�^��ҏW���Ă���
			CManager::GetCamera()->SetMove(false);
		}
		else if (bCostom == false)
		{// �����f�[�^��ҏW���Ă��Ȃ�
			CManager::GetCamera()->SetMove(true);
		}
	}
}

//=============================================================================
//    �G�̔z�u����\�����鏈��
//=============================================================================
void CImGui_Niwa::SetEnemyDebug(void)
{
	// �G�f�B�^�[�N���X���擾����
	CEditer *pEditer = CManager::GetEditer();

	if (pEditer != NULL)
	{// �G�f�B�^�[�N���X���擾�ł���
		// �G�̔z�u�����擾
		bool bCostom = false;
		float fEnemyPos[3] = { pEditer->GetPos().x,pEditer->GetPos().y,pEditer->GetPos().z };
		float fEnemyRot[3] = { pEditer->GetRot().x,pEditer->GetRot().y,pEditer->GetRot().z };
		int nEnemyLife = pEditer->GetEnemyLife();
		int nEnemyScore = pEditer->GetEnemyScore();
		int nEnemyType = pEditer->GetEnemyType();

		// ������␳���Ă���
		fEnemyRot[0] = D3DXToDegree(fEnemyRot[0]);
		fEnemyRot[1] = D3DXToDegree(fEnemyRot[1]);
		fEnemyRot[2] = D3DXToDegree(fEnemyRot[2]);

		// �w�b�_�����J�n
		ImGui::CollapsingHeader(u8"�G");

		// �G�̎��
		int nEnemyTypeDef = nEnemyType;
		ImGui::InputInt(u8"��ޔԍ�", &nEnemyType);
		if (nEnemyType < 0 || nEnemyType >= pEditer->GetMap()->GetCntEnemyLoad())
		{
			nEnemyType = nEnemyTypeDef;
		}
		if (nEnemyType != nEnemyTypeDef)
		{
			pEditer->SetEnemyType(nEnemyType);
			pEditer->EnemyChange();
		}

		// �G�̍��W
		ImGui::InputFloat3(u8"���W", &fEnemyPos[0]);

		// �G�̌���
		ImGui::InputFloat3(u8"����", &fEnemyRot[0]);

		// ������␳���Ă���
		fEnemyRot[0] = D3DXToRadian(fEnemyRot[0]);
		fEnemyRot[1] = D3DXToRadian(fEnemyRot[1]);
		fEnemyRot[2] = D3DXToRadian(fEnemyRot[2]);

		// �G�̗̑�
		int nEnemyLifeDef = nEnemyLife;
		ImGui::InputInt(u8"�̗�", &nEnemyLife);
		if (nEnemyLife < 1)
		{// �̗͂�1�����ł���
			nEnemyLife = nEnemyLifeDef;
		}

		// �G�̃X�R�A
		int nEnemyScoreDef = nEnemyScore;
		ImGui::InputInt(u8"���j���̃X�R�A", &nEnemyScore);
		if (nEnemyScore < 1)
		{// �̗͂�1�����ł���
			nEnemyScore = nEnemyScoreDef;
		}

		// �e��l��ݒ肷��
		pEditer->SetPos(D3DXVECTOR3(fEnemyPos[0], fEnemyPos[1], fEnemyPos[2]));
		pEditer->SetRot(D3DXVECTOR3(fEnemyRot[0], fEnemyRot[1], fEnemyRot[2]));
		pEditer->SetEnemyLife(nEnemyLife);
		pEditer->SetEnemyScore(nEnemyScore);

		if (bCostom == true)
		{// �����f�[�^��ҏW���Ă���
			CManager::GetCamera()->SetMove(false);
		}
		else if (bCostom == false)
		{// �����f�[�^��ҏW���Ă��Ȃ�
			CManager::GetCamera()->SetMove(true);
		}
	}
}

//=============================================================================
//    �ǂ̔z�u����\�����鏈��
//=============================================================================
void CImGui_Niwa::SetWallDebug(void)
{
	// �G�f�B�^�[�N���X���擾����
	CEditer *pEditer = CManager::GetEditer();

	if (pEditer != NULL)
	{// �G�f�B�^�[�N���X���擾�ł���
	    // �ǂ̔z�u�����擾
		bool bCostom = false;
		int nWallTexIdx = pEditer->GetWallTexIdx();
		float fWallPos[3] = { pEditer->GetPos().x,pEditer->GetPos().y,pEditer->GetPos().z };
		float fWallRot[3] = { pEditer->GetRot().x,pEditer->GetRot().y,pEditer->GetRot().z };
		float fWallCol[4] = { pEditer->GetWallCol().r,pEditer->GetWallCol().b,pEditer->GetWallCol().g,pEditer->GetWallCol().a };
		float fWallWidth = pEditer->GetWallWidth();
		float fWallHeight = pEditer->GetWallHeight();
		int nWallXBlock = pEditer->GetWallXBlock();
		int nWallYBlock = pEditer->GetWallYBlock();
		int nWallTexSplitU = pEditer->GetWallTexSplitU();
		int nWallTexSplitV = pEditer->GetWallTexSplitV();

		// ������␳���Ă���
		fWallRot[0] = D3DXToDegree(fWallRot[0]);
		fWallRot[1] = D3DXToDegree(fWallRot[1]);
		fWallRot[2] = D3DXToDegree(fWallRot[2]);

		// �w�b�_�����J�n
		ImGui::CollapsingHeader(u8"��");

		// �ǂ̃e�N�X�`���ԍ�
		int nWallTexIdxDef = nWallTexIdx;
		ImGui::InputInt(u8"�e�N�X�`���ԍ�", &nWallTexIdx);
		if (nWallTexIdx < 0 || nWallTexIdx >= pEditer->GetNumTex())
		{
			nWallTexIdx = nWallTexIdxDef;
		}
		if (nWallTexIdx != nWallTexIdxDef)
		{
			pEditer->GetSetMeshWall()->BindTexture(pEditer->GetTextureManager()->GetTexture(nWallTexIdx));
		}

		// �ǂ̍��W
		ImGui::InputFloat3(u8"���W", &fWallPos[0]);

		// �ǂ̌���
		ImGui::InputFloat3(u8"����", &fWallRot[0]);

		// �ǂ̐F
		bCostom = ImGui::ColorEdit4(u8"�F", &fWallCol[0]);

		// �ǂ�1�u���b�N���̕�
		ImGui::InputFloat(u8"1�u���b�N���̕�", &fWallWidth);

		// �ǂ�1�u���b�N���̍���
		ImGui::InputFloat(u8"1�u���b�N���̍���", &fWallHeight);

		// �ǂ̉��̕�����
		int nWallXBlockDef = nWallXBlock;
		ImGui::InputInt(u8"���̕�����", &nWallXBlock);
		if (nWallXBlock < 1)
		{
			nWallXBlock = nWallXBlockDef;
		}

		// �ǂ̏c�̕�����
		int nWallYBlockDef = nWallYBlock;
		ImGui::InputInt(u8"�c�̕�����", &nWallYBlock);
		if (nWallYBlock < 1)
		{
			nWallYBlock = nWallYBlockDef;
		}

		// �ǂ̃e�N�X�`��U���W�̕�����
		int nWallTexSplitUDef = nWallTexSplitU;
		ImGui::InputInt(u8"�e�N�X�`��U���W�̕�����", &nWallTexSplitU);
		if (nWallTexSplitU < 1)
		{
			nWallTexSplitU = nWallTexSplitUDef;
		}

		// �ǂ̃e�N�X�`��V���W�̕�����
		int nWallTexSplitVDef = nWallTexSplitV;
		ImGui::InputInt(u8"�e�N�X�`��V���W�̕�����", &nWallTexSplitV);
		if (nWallTexSplitV < 1)
		{
			nWallTexSplitV = nWallTexSplitVDef;
		}

		// ������␳���Ă���
		fWallRot[0] = D3DXToRadian(fWallRot[0]);
		fWallRot[1] = D3DXToRadian(fWallRot[1]);
		fWallRot[2] = D3DXToRadian(fWallRot[2]);

		// �e��l��ݒ肷��
		pEditer->SetWallTexIdx(nWallTexIdx);
		pEditer->SetPos(D3DXVECTOR3(fWallPos[0], fWallPos[1], fWallPos[2]));
		pEditer->SetRot(D3DXVECTOR3(fWallRot[0], fWallRot[1], fWallRot[2]));
		pEditer->SetWallCol(D3DXCOLOR(fWallCol[0], fWallCol[1], fWallCol[2], fWallCol[3]));
		pEditer->SetWallWidth(fWallWidth);
		pEditer->SetWallHeight(fWallHeight);
		pEditer->SetWallXBlock(nWallXBlock);
		pEditer->SetWallYBlock(nWallYBlock);
		pEditer->SetWallTexSplitU(nWallTexSplitU);
		pEditer->SetWallTexSplitV(nWallTexSplitV);

		CMeshWall *pMeshWall = pEditer->GetSetMeshWall();
		if (pMeshWall != NULL)
		{
			pMeshWall->SetTexIdx(nWallTexIdx);
			pMeshWall->SetPos(D3DXVECTOR3(fWallPos[0], fWallPos[1], fWallPos[2]));
			pMeshWall->SetRot(D3DXVECTOR3(fWallRot[0], fWallRot[1], fWallRot[2]));
			pMeshWall->SetCol(D3DXCOLOR(fWallCol[0], fWallCol[1], fWallCol[2], 0.7f));
			pMeshWall->SetWidth(fWallWidth);
			pMeshWall->SetHeight(fWallHeight);
			pMeshWall->SetXBlock(nWallXBlock);
			pMeshWall->SetYBlock(nWallYBlock);
			pMeshWall->SetTexSplitU(nWallTexSplitU);
			pMeshWall->SetTexSplitV(nWallTexSplitV);
		}

		if (bCostom == true)
		{// �����f�[�^��ҏW���Ă���
			CManager::GetCamera()->SetMove(false);
		}
		else if (bCostom == false)
		{// �����f�[�^��ҏW���Ă��Ȃ�
			CManager::GetCamera()->SetMove(true);
		}
	}
}

//=============================================================================
//    �A�C�e���̏���\�����鏈��
//=============================================================================
void CImGui_Niwa::SetItemDebug(void)
{
	// �G�f�B�^�[�N���X���擾����
	CEditer *pEditer = CManager::GetEditer();

	if (pEditer != NULL)
	{// �G�f�B�^�[�N���X���擾�ł���
	    // �z�u�A�C�e���̏����擾
		bool bCostom = false;
		float fItemPos[3] = { pEditer->GetPos().x,pEditer->GetPos().y,pEditer->GetPos().z };
		float fItemCol[4] = { pEditer->GetItemCol().r ,pEditer->GetItemCol().g ,pEditer->GetItemCol().b ,pEditer->GetItemCol().a };
		float fItemColRange = pEditer->GetItemColRange();
		float fItemWidth = pEditer->GetItemWidth();
		float fItemHeight = pEditer->GetItemHeight();
		float fItemRot = pEditer->GetItemRot();
		bool bItemLighting = pEditer->GetItemLighting();
		bool bItemDrawAddtive = pEditer->GetItemDrawAddtive();
		int nItemTexIdx = pEditer->GetItemTexIdx();
		int nItemType = pEditer->GetItemType();

		// ������␳���Ă���
		fItemRot = D3DXToDegree(fItemRot);

		// �w�b�_�����J�n
		ImGui::CollapsingHeader(u8"�A�C�e��");

		// �A�C�e���̍��W
		ImGui::InputFloat3(u8"���W", &fItemPos[0]);

		// �A�C�e���̐F
		bCostom = ImGui::ColorEdit4(u8"�F", &fItemCol[0]);

		// �A�C�e���̎�ޔԍ�
		ImGui::InputInt(u8"���", &nItemType);

		// �A�C�e���̓����蔻������͈�
		ImGui::InputFloat(u8"�����蔻������͈�", &fItemColRange);

		// �A�C�e���̕�
		ImGui::InputFloat(u8"��", &fItemWidth);

		// �A�C�e���̍���
		ImGui::InputFloat(u8"����", &fItemHeight);

		// �A�C�e���̌���
		ImGui::InputFloat(u8"����", &fItemRot);

		// ������␳���Ă���
		fItemRot = D3DXToRadian(fItemRot);

		// �A�C�e�������C�e�B���O���邩���Ȃ���
		ImGui::Checkbox(u8"���C�e�B���O�̗L��", &bItemLighting);

		// �A�C�e�������Z�����ŕ`�悷�邩���Ȃ���
		ImGui::Checkbox(u8"���Z�������邩���Ȃ���", &bItemDrawAddtive);

		// �z�u�r���{�[�h�̃e�N�X�`���ԍ�
		int nItemTexIdxDef = nItemTexIdx;
		bool bTexChange = false;
		ImGui::InputInt(u8"�e�N�X�`���ԍ�", &nItemTexIdx);
		if (nItemTexIdx < 0 || nItemTexIdx >= pEditer->GetNumTex())
		{// �ǂݍ��񂾃e�N�X�`���̐���������
			nItemTexIdx = nItemTexIdxDef;
		}
		if (nItemTexIdx != nItemTexIdxDef)
		{
			bTexChange = true;
			pEditer->GetSetBillObj()->BindTexture(pEditer->GetTextureManager()->GetTexture(nItemTexIdx));
		}


		// �e��l��ݒ肷��
		pEditer->SetPos(D3DXVECTOR3(fItemPos[0], fItemPos[1], fItemPos[2]));
		pEditer->SetItemCol(D3DXCOLOR(fItemCol[0], fItemCol[1], fItemCol[2], fItemCol[3]));
		pEditer->SetItemColRange(fItemColRange);
		pEditer->SetItemWidth(fItemWidth);
		pEditer->SetItemHeight(fItemHeight);
		pEditer->SetItemRot(fItemRot);
		pEditer->SetItemLighting(bItemLighting);
		pEditer->SetItemDrawAddtive(bItemDrawAddtive);
		pEditer->SetItemTexIdx(nItemTexIdx);
		pEditer->SetItemType(nItemType);

		CSetItem *pSetItem = pEditer->GetSetItem();
		if (pSetItem != NULL)
		{
			pSetItem->SetPos(D3DXVECTOR3(fItemPos[0], fItemPos[1], fItemPos[2]));
			pSetItem->SetCol(D3DXCOLOR(fItemCol[0], fItemCol[1], fItemCol[2], fItemCol[3]));
			pSetItem->SetColRange(fItemColRange);
			pSetItem->SetWidth(fItemWidth);
			pSetItem->SetHeight(fItemHeight);
			pSetItem->SetRot(fItemRot);
			pSetItem->SetLighting(bItemLighting);
			pSetItem->SetDrawAddtive(bItemDrawAddtive);
			pSetItem->SetTexIdx(nItemTexIdx);
			pSetItem->SetType(nItemType);
			pSetItem->ChangeVertex();
		}

		if (bCostom == true)
		{// �����f�[�^��ҏW���Ă���
			CManager::GetCamera()->SetMove(false);
		}
		else if (bCostom == false)
		{// �����f�[�^��ҏW���Ă��Ȃ�
			CManager::GetCamera()->SetMove(true);
		}
	}
}

//=============================================================================
//    �`�悷�邩���Ȃ�����ݒ肷�鏈��
//=============================================================================
void CImGui_Niwa::SetDisp(bool bDisp)
{
	m_bDisp = bDisp;
}

//=============================================================================
//    �`�悷�邩���Ȃ������擾���鏈��
//=============================================================================
bool CImGui_Niwa::GetDisp(void)
{
	return m_bDisp;
}