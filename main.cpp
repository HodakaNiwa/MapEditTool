//*****************************************************************************
//
//     メインの処理[main.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"	                  // ウインドウのクラス名
#define WINDOW_NAME		"マップエディターVer1.6"      // ウインドウのキャプション名
#define WINDOW_MODE     (true)                        // ウィンドウのモード(falseにするとフルスクリーン)
#define WINDOW_ICON     (MAKEINTRESOURCE(IDI_ICON1))  // ウィンドウのアイコン識別子
#define WINDOW_SM_ICON  (MAKEINTRESOURCE(IDI_ICON1))  // ウィンドウのスモールアイコン識別子

#ifdef _DEBUG
#define MEMORY_CHECK    (true)                        // メモリリークチェックをするかどうか
#endif

//*****************************************************************************
//    プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=============================================================================
//    メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	if (MEMORY_CHECK)
	{// メモリリークチェックをするなら
		// メモリリークチェックのフラグを立てる
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}
#endif

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		LoadIcon(hInstance, WINDOW_ICON),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		(HICON)LoadImage(hInstance, WINDOW_SM_ICON,  IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED)
	};

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; // ウィンドウのスクリーンを描画する範囲
	HWND hWnd;                                         // ウィンドウハンドル
	MSG msg;                                           // メッセージ(whileループの度にメッセージが格納される)
	DWORD dwCurrentTime;                               // 現在の時間経過を格納するカウンター
	DWORD dwFrameCount;                                // フレーム数を数えるカウンター
	DWORD dwExecLastTime;                              // Update処理とDraw処理を行った最後の時間を格納するカウンター
	DWORD dwFPSLastTime;                               // FPSを算出した最後の時間を格納するカウンター
	srand((unsigned int)time(0));                      // ランダムな値を出力させるのに必要
#ifdef _DEBUG
	int nCountFPS = 0;                                 // FPSを格納するカウンター
#endif

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						(rect.right - rect.left),
						(rect.bottom - rect.top),
						NULL,
						NULL,
						hInstance,
						NULL);

	CManager *pManager = NULL;
	if (pManager == NULL)
	{// メモリを確保できる
		pManager = new CManager;
		if (pManager != NULL)
		{// メモリを確保できた
			// 初期化処理
			if (FAILED(pManager->Init(hInstance,hWnd, WINDOW_MODE)))
			{// 初期化に失敗した
				return -1;
			}

			// 分解能を設定
			timeBeginPeriod(1);

			// フレームカウント初期化
			dwCurrentTime =
			dwFrameCount = 0;
			dwExecLastTime =
			dwFPSLastTime = timeGetTime();

			// ウインドウの表示
			ShowWindow(hWnd, nCmdShow);
			UpdateWindow(hWnd);

			// メッセージループ
			while (1)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					if (msg.message == WM_QUIT)
					{// PostQuitMessage()が呼ばれたらループ終了
						break;
					}
					else
					{
						// メッセージの翻訳とディスパッチ
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
				else
				{
					dwCurrentTime = timeGetTime();	// 現在の時間を取得
					if ((dwCurrentTime - dwFPSLastTime) >= 500)
					{// 0.5秒ごとに実行
#ifdef _DEBUG
					    // FPSを算出
						nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
						dwFPSLastTime = dwCurrentTime;	// 現在の時間を保存
						dwFrameCount = 0;
					}

					if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
					{// 1/60秒経過
						dwExecLastTime = dwCurrentTime;	// 現在の時間を保存

						// 更新処理
						pManager->Update();

						// 描画処理
						pManager->Draw();

						dwFrameCount++;   // フレーム数のカウンターを増やす

#ifdef _DEBUG
						// FPS描画
						CDebugProc::Print("FPS:%d\n", nCountFPS);
#endif
					}
				}
			}

			if (pManager != NULL)
			{// メモリが解放されていない
			    // 終了処理
				pManager->Uninit();

				// メモリの開放
				delete pManager;
				pManager = NULL;
			}
		}
		else
		{// メモリが確保できなかった
			return -1;
		}
	}
	else
	{// メモリが確保できる状態にない
		return -1;
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);

#ifdef _DEBUG
	if (MEMORY_CHECK)
	{// メモリリークチェックをしていたなら
	    // この時点で開放されていないメモリの情報の表示
		_CrtDumpMemoryLeaks();
	}
#endif

	return (int)msg.wParam;
}

//=============================================================================
//    ウインドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch(uMsg)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:				// [ESC]キーが押された
			DestroyWindow(hWnd);	// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}