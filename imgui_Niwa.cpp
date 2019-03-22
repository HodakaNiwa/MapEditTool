//*****************************************************************************
//
//     ImGuiの処理(Version.Niwa)[imgui_Niwa.cpp]
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
//    マクロ定義
//*****************************************************************************

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CImGui_Niwa::CImGui_Niwa()
{
	// 各種値のクリア
	m_bDisp = true;      // 描画するかしないか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CImGui_Niwa::~CImGui_Niwa()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CImGui_Niwa::Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice)
{
	// ImGui用のテキストデータを生成する
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//ImFontConfig config;
	//config.MergeMode = true;
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:/Windows/Fonts/meiryo.ttc", 18.0f, &config, io.Fonts->GetGlyphRangesJapanese());

	// AddFontFromFileTTF関数
	// 第１引数 : フォントデータのファイルパス名
	// 第２引数 : フォントのサイズ
	// 第３引数 : NULLでOK
	// 第４引数 : 日本語にしたければio.Fonts->GetGlyphRangesJapaneseで基本的にOK
	io.Fonts->AddFontFromFileTTF("c:/Windows/Fonts/meiryo.ttc",20.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	// ImGuiを生成する
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);
	ImGui::StyleColorsClassic();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CImGui_Niwa::Uninit(void)
{
	// ImGuiを終了させる
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//=============================================================================
//    更新処理
//=============================================================================
void CImGui_Niwa::Update(void)
{
	// 新しいフレームを用意する
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// デバッグウィンドウを生成する
	SetWindowDebug();

	// フレームを終了させる
	ImGui::EndFrame();
}

//=============================================================================
//    描画処理
//=============================================================================
void CImGui_Niwa::Draw(void)
{
	if (m_bDisp == true)
	{// 描画する状態ならば
		// Updateで生成したImGuiを表示する
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
}

//=============================================================================
//    デバイスがロストした場合の処理
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
//    デバッグウィンドウを表示する処理
//=============================================================================
void CImGui_Niwa::SetWindowDebug(void)
{
	// 現在生成できるオブジェクトの種類によって処理わけ
	// エディタークラスを取得する
	CEditer *pEditer = CManager::GetEditer();

	if (pEditer != NULL)
	{// エディタークラスが取得できた
	    // ウィンドウ生成開始
		ImGui::Begin(u8"マップエディター Ver1.6", false, ImGuiWindowFlags_NoMove);

		// FPS描画
		ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);

		// カメラ情報描画
		ImGui::Text(u8"カメラの視点    : %.1f %.1f %.1f", CManager::GetCamera()->GetPosV().x, CManager::GetCamera()->GetPosV().y, CManager::GetCamera()->GetPosV().z);
		ImGui::Text(u8"カメラの注視点 : %.1f %.1f %.1f", CManager::GetCamera()->GetPosR().x, CManager::GetCamera()->GetPosR().y, CManager::GetCamera()->GetPosR().z);
		ImGui::Text(u8"カメラの向き    : %.1f %.1f %.1f", CManager::GetCamera()->GetRot().x, CManager::GetCamera()->GetRot().y, CManager::GetCamera()->GetRot().z);
		ImGui::Text("");

		// 配置するオブジェクトの種類を表示
		ImGui::CollapsingHeader(u8"配置するオブジェクトの種類");
		int nObjType = pEditer->GetType();
		int nObjTypeOld = nObjType;
		int nObjTypeDef = nObjType;
		ImGui::RadioButton(u8"配置物", &nObjType, 0);
		ImGui::RadioButton(u8"動く配置物", &nObjType, 1);
		ImGui::RadioButton(u8"ビルボード", &nObjType, 2);
		ImGui::RadioButton(u8"エフェクト", &nObjType, 3);
		ImGui::RadioButton(u8"敵", &nObjType, 4);
		ImGui::RadioButton(u8"壁", &nObjType, 5);
		ImGui::RadioButton(u8"アイテム", &nObjType, 6);
		ImGui::Text("");

		if (nObjType != nObjTypeDef)
		{// 配置するオブジェクトの種類が変更された
			pEditer->SetType((CEditer::TYPE)nObjType);
			pEditer->ObjectChange((CEditer::TYPE)nObjTypeOld);
		}

		// ウィンドウ生成終了
		ImGui::End();

		// ウィンドウ生成開始
		ImGui::Begin(u8"オブジェクトの情報", false, ImGuiWindowFlags_NoMove);

		// オブジェクトの種類によって表示するデータを入れ替える
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

		// ウィンドウ生成終了
		ImGui::End();
	}
}

//=============================================================================
//    オブジェクトの情報を表示する処理
//=============================================================================
void CImGui_Niwa::SetObjectDebug(void)
{
	// エディタークラスを取得する
	CEditer *pEditer = CManager::GetEditer();

	if (pEditer != NULL)
	{// エディタークラスが取得できた
		// 配置用モデルの情報を取得
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
		{// 普通のオブジェクトの種類だったら
			// 通常のオブジェクトのヘッダ作成
			ImGui::CollapsingHeader(u8"配置物");

			// 配置するモデルの番号
			int nSelectModelDef = nSelectModel;
			bCostom = ImGui::InputInt(u8"使用するモデルの番号", &nSelectModel);
			if (nSelectModel != nSelectModelDef)
			{
				if (nSelectModel >= 0 && nSelectModel < pEditer->GetNumModel())
				{
					pEditer->SetSelectModelIdx(nSelectModel);
					pEditer->ModelChange();
				}
			}

			// 当たり判定の種類
			ImGui::Text(u8"当たり判定の種類   ");
			ImGui::SameLine();
			ImGui::RadioButton(u8"四角", &nModelColType, 0);
			ImGui::SameLine();
			ImGui::RadioButton(u8"円形", &nModelColType, 1);
			ImGui::SameLine();
			ImGui::RadioButton(u8"判定しない", &nModelColType, 2);

			// 当たり判定を取る範囲
			ImGui::InputFloat(u8"当たり判定を取る範囲", &fModelColRange);

			// 配置用モデルの座標
			ImGui::InputFloat3(u8"座標", &fModelPos[0]);

			// 配置用モデルの向き
			float fModelRotDef[3] = { fModelRot[0], fModelRot[1] ,fModelRot[2] };
			// 向きを補正しておく
			fModelRot[0] = D3DXToDegree(fModelRot[0]);
			fModelRot[1] = D3DXToDegree(fModelRot[1]);
			fModelRot[2] = D3DXToDegree(fModelRot[2]);

			ImGui::InputFloat3(u8"向き", &fModelRot[0]);

			// 向きを補正しておく
			fModelRot[0] = D3DXToRadian(fModelRot[0]);
			fModelRot[1] = D3DXToRadian(fModelRot[1]);
			fModelRot[2] = D3DXToRadian(fModelRot[2]);

			bool bCheakRot = false;
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{// 軸の数分繰り返し
				if (fModelRotDef[nCnt] != fModelRot[nCnt])
				{// 向きが変わっている
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

			// 配置用モデルの大きさ
			ImGui::InputFloat3(u8"大きさ", &fModelScale[0]);

			// 配置用モデルの回転スピード
			ImGui::InputFloat3(u8"回転スピード", &fModelRollSpeed[0]);
		}

		if (nType == CEditer::TYPE_OBJECTMOVE)
		{// 動くオブジェクトの種類だったら
			// 動くオブジェクトのヘッダ作成
			ImGui::CollapsingHeader(u8"動く配置物");

			// 配置するモデルの番号
			int nSelectModelDef = nSelectModel;
			bCostom = ImGui::InputInt(u8"使用するモデルの番号", &nSelectModel);
			if (nSelectModel != nSelectModelDef)
			{
				if (nSelectModel >= 0 && nSelectModel < pEditer->GetNumModel())
				{
					pEditer->SetSelectModelIdx(nSelectModel);
					pEditer->ModelChange();
				}
			}

			// 配置用モデルの座標
			ImGui::InputFloat3(u8"座標", &fModelPos[0]);

			// 配置用モデルの向き
			float fModelRotDef[3] = { fModelRot[0], fModelRot[1] ,fModelRot[2] };
			// 向きを補正しておく
			fModelRot[0] = D3DXToDegree(fModelRot[0]);
			fModelRot[1] = D3DXToDegree(fModelRot[1]);
			fModelRot[2] = D3DXToDegree(fModelRot[2]);

			ImGui::InputFloat3(u8"向き", &fModelRot[0]);

			// 向きを補正しておく
			fModelRot[0] = D3DXToRadian(fModelRot[0]);
			fModelRot[1] = D3DXToRadian(fModelRot[1]);
			fModelRot[2] = D3DXToRadian(fModelRot[2]);

			bool bCheakRot = false;
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{// 軸の数分繰り返し
				if (fModelRotDef[nCnt] != fModelRot[nCnt])
				{// 向きが変わっている
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

			// 配置用モデルの大きさ
			ImGui::InputFloat3(u8"大きさ", &fModelScale[0]);

			// 配置用モデルの回転スピード
			ImGui::InputFloat3(u8"回転スピード", &fModelRollSpeed[0]);

			// 配置用モデルの移動量
			ImGui::InputFloat3(u8"移動量", &fModelMove[0]);

			// 配置用モデルの移動量
			int nModelMaxCounterDef = nModelMaxCounter;
			ImGui::InputInt(u8"移動量切り替え値", &nModelMaxCounter);
			if (nModelMaxCounter < 0)
			{// 0未満になった
				nModelMaxCounter = nModelMaxCounterDef;
			}
		}

		// 配置用モデルの情報を設定
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
		{// 何かデータを編集している
			CManager::GetCamera()->SetMove(false);
		}
		else if (bCostom == false)
		{// 何かデータを編集していない
			CManager::GetCamera()->SetMove(true);
		}
	}
}

//=============================================================================
//    配置ビルボードの情報を表示する処理
//=============================================================================
void CImGui_Niwa::SetBillboardDebug(void)
{
	// エディタークラスを取得する
	CEditer *pEditer = CManager::GetEditer();

	if (pEditer != NULL)
	{// エディタークラスが取得できた
		// 配置ビルボードの情報を取得
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

		// 向きを補正しておく
		fBillRot = D3DXToDegree(fBillRot);

		// ヘッダ生成開始
		ImGui::CollapsingHeader(u8"ビルボード");

		// 配置ビルボードの座標
		ImGui::InputFloat3(u8"座標", &fBillPos[0]);

		// 配置ビルボードの色
		bCostom = ImGui::ColorEdit4(u8"色", &fBillCol[0]);

		// 配置ビルボードの当たり判定を取る範囲
		ImGui::InputFloat(u8"当たり判定を取る範囲", &fBillColRange);

		// 配置ビルボードの幅
		ImGui::InputFloat(u8"幅", &fBillWidth);

		// 配置ビルボードの高さ
		ImGui::InputFloat(u8"高さ", &fBillHeight);

		// 配置ビルボードの向き
		ImGui::InputFloat(u8"向き", &fBillRot);

		// 向きを補正しておく
		fBillRot = D3DXToRadian(fBillRot);

		// 配置ビルボードをライティングするかしないか
		ImGui::Checkbox(u8"ライティングの有無", &bBillLighting);

		// 配置ビルボードを加算合成で描画するかしないか
		ImGui::Checkbox(u8"加算合成するかしないか", &bBillDrawAddtive);

		// 配置ビルボードのテクスチャ番号
		int nBillTexIdxDef = nBillTexIdx;
		bool bTexChange = false;
		ImGui::InputInt(u8"テクスチャ番号", &nBillTexIdx);
		if (nBillTexIdx < 0 || nBillTexIdx >= pEditer->GetNumTex())
		{// 読み込んだテクスチャの数を上回った
			nBillTexIdx = nBillTexIdxDef;
		}
		if (nBillTexIdx != nBillTexIdxDef)
		{
			bTexChange = true;
			pEditer->GetSetBillObj()->BindTexture(pEditer->GetTextureManager()->GetTexture(nBillTexIdx));
		}


		// 各種値を設定する
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
		{// 何かデータを編集している
			CManager::GetCamera()->SetMove(false);
		}
		else if (bCostom == false)
		{// 何かデータを編集していない
			CManager::GetCamera()->SetMove(true);
		}
	}
}

//=============================================================================
//    配置エフェクトの情報を表示する処理
//=============================================================================
void CImGui_Niwa::SetEffectDebug(void)
{
	// エディタークラスを取得する
	CEditer *pEditer = CManager::GetEditer();

	if (pEditer != NULL)
	{// エディタークラスが取得できた
		// 配置エフェクトの情報を取得
		bool bCostom = false;
		float fEffectPos[3] = { pEditer->GetPos().x,pEditer->GetPos().y,pEditer->GetPos().z };
		float fEffectRot[3] = { pEditer->GetRot().x,pEditer->GetRot().y,pEditer->GetRot().z };
		int nEffectType = pEditer->GetEffectType();

		// 向きを補正しておく
		fEffectRot[0] = D3DXToDegree(fEffectRot[0]);
		fEffectRot[1] = D3DXToDegree(fEffectRot[1]);
		fEffectRot[2] = D3DXToDegree(fEffectRot[2]);

		// ヘッダ生成開始
		ImGui::CollapsingHeader(u8"エフェクト");
		// 配置エフェクトの座標
		ImGui::InputFloat3(u8"座標", &fEffectPos[0]);

		// 配置エフェクトの向き
		ImGui::InputFloat3(u8"向き", &fEffectRot[0]);

		// 向きを補正しておく
		fEffectRot[0] = D3DXToRadian(fEffectRot[0]);
		fEffectRot[1] = D3DXToRadian(fEffectRot[1]);
		fEffectRot[2] = D3DXToRadian(fEffectRot[2]);

		// 配置エフェクトの種類
		int nEffectTypeDef = nEffectType;
		ImGui::InputInt(u8"種類番号", &nEffectType);
		if (nEffectType < 0 || nEffectType >= pEditer->GetEffectManager()->GetNumEmitterData())
		{// 読み込んだエフェクトデータの数を上回った
			nEffectType = nEffectTypeDef;
			pEditer->SetEffectType(nEffectType);
		}
		if (nEffectType != nEffectTypeDef)
		{
			pEditer->SetEffectType(nEffectType);
			pEditer->EmitterChange();
		}

		// 各種値を設定する
		pEditer->SetPos(D3DXVECTOR3(fEffectPos[0], fEffectPos[1], fEffectPos[2]));
		pEditer->SetRot(D3DXVECTOR3(fEffectRot[0], fEffectRot[1], fEffectRot[2]));

		CEmitter *pEmitter = pEditer->GetSetEmitter();
		if (pEmitter != NULL)
		{
			pEmitter->SetPos(D3DXVECTOR3(fEffectPos[0], fEffectPos[1], fEffectPos[2]));
			pEmitter->SetRot(D3DXVECTOR3(fEffectRot[0], fEffectRot[1], fEffectRot[2]));
		}

		if (bCostom == true)
		{// 何かデータを編集している
			CManager::GetCamera()->SetMove(false);
		}
		else if (bCostom == false)
		{// 何かデータを編集していない
			CManager::GetCamera()->SetMove(true);
		}
	}
}

//=============================================================================
//    敵の配置情報を表示する処理
//=============================================================================
void CImGui_Niwa::SetEnemyDebug(void)
{
	// エディタークラスを取得する
	CEditer *pEditer = CManager::GetEditer();

	if (pEditer != NULL)
	{// エディタークラスが取得できた
		// 敵の配置情報を取得
		bool bCostom = false;
		float fEnemyPos[3] = { pEditer->GetPos().x,pEditer->GetPos().y,pEditer->GetPos().z };
		float fEnemyRot[3] = { pEditer->GetRot().x,pEditer->GetRot().y,pEditer->GetRot().z };
		int nEnemyLife = pEditer->GetEnemyLife();
		int nEnemyScore = pEditer->GetEnemyScore();
		int nEnemyType = pEditer->GetEnemyType();

		// 向きを補正しておく
		fEnemyRot[0] = D3DXToDegree(fEnemyRot[0]);
		fEnemyRot[1] = D3DXToDegree(fEnemyRot[1]);
		fEnemyRot[2] = D3DXToDegree(fEnemyRot[2]);

		// ヘッダ生成開始
		ImGui::CollapsingHeader(u8"敵");

		// 敵の種類
		int nEnemyTypeDef = nEnemyType;
		ImGui::InputInt(u8"種類番号", &nEnemyType);
		if (nEnemyType < 0 || nEnemyType >= pEditer->GetMap()->GetCntEnemyLoad())
		{
			nEnemyType = nEnemyTypeDef;
		}
		if (nEnemyType != nEnemyTypeDef)
		{
			pEditer->SetEnemyType(nEnemyType);
			pEditer->EnemyChange();
		}

		// 敵の座標
		ImGui::InputFloat3(u8"座標", &fEnemyPos[0]);

		// 敵の向き
		ImGui::InputFloat3(u8"向き", &fEnemyRot[0]);

		// 向きを補正しておく
		fEnemyRot[0] = D3DXToRadian(fEnemyRot[0]);
		fEnemyRot[1] = D3DXToRadian(fEnemyRot[1]);
		fEnemyRot[2] = D3DXToRadian(fEnemyRot[2]);

		// 敵の体力
		int nEnemyLifeDef = nEnemyLife;
		ImGui::InputInt(u8"体力", &nEnemyLife);
		if (nEnemyLife < 1)
		{// 体力が1未満である
			nEnemyLife = nEnemyLifeDef;
		}

		// 敵のスコア
		int nEnemyScoreDef = nEnemyScore;
		ImGui::InputInt(u8"撃破時のスコア", &nEnemyScore);
		if (nEnemyScore < 1)
		{// 体力が1未満である
			nEnemyScore = nEnemyScoreDef;
		}

		// 各種値を設定する
		pEditer->SetPos(D3DXVECTOR3(fEnemyPos[0], fEnemyPos[1], fEnemyPos[2]));
		pEditer->SetRot(D3DXVECTOR3(fEnemyRot[0], fEnemyRot[1], fEnemyRot[2]));
		pEditer->SetEnemyLife(nEnemyLife);
		pEditer->SetEnemyScore(nEnemyScore);

		if (bCostom == true)
		{// 何かデータを編集している
			CManager::GetCamera()->SetMove(false);
		}
		else if (bCostom == false)
		{// 何かデータを編集していない
			CManager::GetCamera()->SetMove(true);
		}
	}
}

//=============================================================================
//    壁の配置情報を表示する処理
//=============================================================================
void CImGui_Niwa::SetWallDebug(void)
{
	// エディタークラスを取得する
	CEditer *pEditer = CManager::GetEditer();

	if (pEditer != NULL)
	{// エディタークラスが取得できた
	    // 壁の配置情報を取得
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

		// 向きを補正しておく
		fWallRot[0] = D3DXToDegree(fWallRot[0]);
		fWallRot[1] = D3DXToDegree(fWallRot[1]);
		fWallRot[2] = D3DXToDegree(fWallRot[2]);

		// ヘッダ生成開始
		ImGui::CollapsingHeader(u8"壁");

		// 壁のテクスチャ番号
		int nWallTexIdxDef = nWallTexIdx;
		ImGui::InputInt(u8"テクスチャ番号", &nWallTexIdx);
		if (nWallTexIdx < 0 || nWallTexIdx >= pEditer->GetNumTex())
		{
			nWallTexIdx = nWallTexIdxDef;
		}
		if (nWallTexIdx != nWallTexIdxDef)
		{
			pEditer->GetSetMeshWall()->BindTexture(pEditer->GetTextureManager()->GetTexture(nWallTexIdx));
		}

		// 壁の座標
		ImGui::InputFloat3(u8"座標", &fWallPos[0]);

		// 壁の向き
		ImGui::InputFloat3(u8"向き", &fWallRot[0]);

		// 壁の色
		bCostom = ImGui::ColorEdit4(u8"色", &fWallCol[0]);

		// 壁の1ブロック分の幅
		ImGui::InputFloat(u8"1ブロック分の幅", &fWallWidth);

		// 壁の1ブロック分の高さ
		ImGui::InputFloat(u8"1ブロック分の高さ", &fWallHeight);

		// 壁の横の分割数
		int nWallXBlockDef = nWallXBlock;
		ImGui::InputInt(u8"横の分割数", &nWallXBlock);
		if (nWallXBlock < 1)
		{
			nWallXBlock = nWallXBlockDef;
		}

		// 壁の縦の分割数
		int nWallYBlockDef = nWallYBlock;
		ImGui::InputInt(u8"縦の分割数", &nWallYBlock);
		if (nWallYBlock < 1)
		{
			nWallYBlock = nWallYBlockDef;
		}

		// 壁のテクスチャU座標の分割数
		int nWallTexSplitUDef = nWallTexSplitU;
		ImGui::InputInt(u8"テクスチャU座標の分割数", &nWallTexSplitU);
		if (nWallTexSplitU < 1)
		{
			nWallTexSplitU = nWallTexSplitUDef;
		}

		// 壁のテクスチャV座標の分割数
		int nWallTexSplitVDef = nWallTexSplitV;
		ImGui::InputInt(u8"テクスチャV座標の分割数", &nWallTexSplitV);
		if (nWallTexSplitV < 1)
		{
			nWallTexSplitV = nWallTexSplitVDef;
		}

		// 向きを補正しておく
		fWallRot[0] = D3DXToRadian(fWallRot[0]);
		fWallRot[1] = D3DXToRadian(fWallRot[1]);
		fWallRot[2] = D3DXToRadian(fWallRot[2]);

		// 各種値を設定する
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
		{// 何かデータを編集している
			CManager::GetCamera()->SetMove(false);
		}
		else if (bCostom == false)
		{// 何かデータを編集していない
			CManager::GetCamera()->SetMove(true);
		}
	}
}

//=============================================================================
//    アイテムの情報を表示する処理
//=============================================================================
void CImGui_Niwa::SetItemDebug(void)
{
	// エディタークラスを取得する
	CEditer *pEditer = CManager::GetEditer();

	if (pEditer != NULL)
	{// エディタークラスが取得できた
	    // 配置アイテムの情報を取得
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

		// 向きを補正しておく
		fItemRot = D3DXToDegree(fItemRot);

		// ヘッダ生成開始
		ImGui::CollapsingHeader(u8"アイテム");

		// アイテムの座標
		ImGui::InputFloat3(u8"座標", &fItemPos[0]);

		// アイテムの色
		bCostom = ImGui::ColorEdit4(u8"色", &fItemCol[0]);

		// アイテムの種類番号
		ImGui::InputInt(u8"種類", &nItemType);

		// アイテムの当たり判定を取る範囲
		ImGui::InputFloat(u8"当たり判定を取る範囲", &fItemColRange);

		// アイテムの幅
		ImGui::InputFloat(u8"幅", &fItemWidth);

		// アイテムの高さ
		ImGui::InputFloat(u8"高さ", &fItemHeight);

		// アイテムの向き
		ImGui::InputFloat(u8"向き", &fItemRot);

		// 向きを補正しておく
		fItemRot = D3DXToRadian(fItemRot);

		// アイテムをライティングするかしないか
		ImGui::Checkbox(u8"ライティングの有無", &bItemLighting);

		// アイテムを加算合成で描画するかしないか
		ImGui::Checkbox(u8"加算合成するかしないか", &bItemDrawAddtive);

		// 配置ビルボードのテクスチャ番号
		int nItemTexIdxDef = nItemTexIdx;
		bool bTexChange = false;
		ImGui::InputInt(u8"テクスチャ番号", &nItemTexIdx);
		if (nItemTexIdx < 0 || nItemTexIdx >= pEditer->GetNumTex())
		{// 読み込んだテクスチャの数を上回った
			nItemTexIdx = nItemTexIdxDef;
		}
		if (nItemTexIdx != nItemTexIdxDef)
		{
			bTexChange = true;
			pEditer->GetSetBillObj()->BindTexture(pEditer->GetTextureManager()->GetTexture(nItemTexIdx));
		}


		// 各種値を設定する
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
		{// 何かデータを編集している
			CManager::GetCamera()->SetMove(false);
		}
		else if (bCostom == false)
		{// 何かデータを編集していない
			CManager::GetCamera()->SetMove(true);
		}
	}
}

//=============================================================================
//    描画するかしないかを設定する処理
//=============================================================================
void CImGui_Niwa::SetDisp(bool bDisp)
{
	m_bDisp = bDisp;
}

//=============================================================================
//    描画するかしないかを取得する処理
//=============================================================================
bool CImGui_Niwa::GetDisp(void)
{
	return m_bDisp;
}