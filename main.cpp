//*****************************************************************************
//
//     ���C���̏���[main.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"	                  // �E�C���h�E�̃N���X��
#define WINDOW_NAME		"�}�b�v�G�f�B�^�[Ver1.6"      // �E�C���h�E�̃L���v�V������
#define WINDOW_MODE     (true)                        // �E�B���h�E�̃��[�h(false�ɂ���ƃt���X�N���[��)
#define WINDOW_ICON     (MAKEINTRESOURCE(IDI_ICON1))  // �E�B���h�E�̃A�C�R�����ʎq
#define WINDOW_SM_ICON  (MAKEINTRESOURCE(IDI_ICON1))  // �E�B���h�E�̃X���[���A�C�R�����ʎq

#ifdef _DEBUG
#define MEMORY_CHECK    (true)                        // ���������[�N�`�F�b�N�����邩�ǂ���
#endif

//*****************************************************************************
//    �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=============================================================================
//    ���C���֐�
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	if (MEMORY_CHECK)
	{// ���������[�N�`�F�b�N������Ȃ�
		// ���������[�N�`�F�b�N�̃t���O�𗧂Ă�
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

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; // �E�B���h�E�̃X�N���[����`�悷��͈�
	HWND hWnd;                                         // �E�B���h�E�n���h��
	MSG msg;                                           // ���b�Z�[�W(while���[�v�̓x�Ƀ��b�Z�[�W���i�[�����)
	DWORD dwCurrentTime;                               // ���݂̎��Ԍo�߂��i�[����J�E���^�[
	DWORD dwFrameCount;                                // �t���[�����𐔂���J�E���^�[
	DWORD dwExecLastTime;                              // Update������Draw�������s�����Ō�̎��Ԃ��i�[����J�E���^�[
	DWORD dwFPSLastTime;                               // FPS���Z�o�����Ō�̎��Ԃ��i�[����J�E���^�[
	srand((unsigned int)time(0));                      // �����_���Ȓl���o�͂�����̂ɕK�v
#ifdef _DEBUG
	int nCountFPS = 0;                                 // FPS���i�[����J�E���^�[
#endif

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�̍쐬
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
	{// ���������m�ۂł���
		pManager = new CManager;
		if (pManager != NULL)
		{// ���������m�ۂł���
			// ����������
			if (FAILED(pManager->Init(hInstance,hWnd, WINDOW_MODE)))
			{// �������Ɏ��s����
				return -1;
			}

			// ����\��ݒ�
			timeBeginPeriod(1);

			// �t���[���J�E���g������
			dwCurrentTime =
			dwFrameCount = 0;
			dwExecLastTime =
			dwFPSLastTime = timeGetTime();

			// �E�C���h�E�̕\��
			ShowWindow(hWnd, nCmdShow);
			UpdateWindow(hWnd);

			// ���b�Z�[�W���[�v
			while (1)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					if (msg.message == WM_QUIT)
					{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
						break;
					}
					else
					{
						// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
				else
				{
					dwCurrentTime = timeGetTime();	// ���݂̎��Ԃ��擾
					if ((dwCurrentTime - dwFPSLastTime) >= 500)
					{// 0.5�b���ƂɎ��s
#ifdef _DEBUG
					    // FPS���Z�o
						nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
						dwFPSLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�
						dwFrameCount = 0;
					}

					if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
					{// 1/60�b�o��
						dwExecLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�

						// �X�V����
						pManager->Update();

						// �`�揈��
						pManager->Draw();

						dwFrameCount++;   // �t���[�����̃J�E���^�[�𑝂₷

#ifdef _DEBUG
						// FPS�`��
						CDebugProc::Print("FPS:%d\n", nCountFPS);
#endif
					}
				}
			}

			if (pManager != NULL)
			{// ���������������Ă��Ȃ�
			    // �I������
				pManager->Uninit();

				// �������̊J��
				delete pManager;
				pManager = NULL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return -1;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return -1;
	}

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// ����\��߂�
	timeEndPeriod(1);

#ifdef _DEBUG
	if (MEMORY_CHECK)
	{// ���������[�N�`�F�b�N�����Ă����Ȃ�
	    // ���̎��_�ŊJ������Ă��Ȃ��������̏��̕\��
		_CrtDumpMemoryLeaks();
	}
#endif

	return (int)msg.wParam;
}

//=============================================================================
//    �E�C���h�E�v���V�[�W��
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
		case VK_ESCAPE:				// [ESC]�L�[�������ꂽ
			DestroyWindow(hWnd);	// �E�B���h�E��j������悤�w������
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}