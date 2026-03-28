// Editor_Window.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Editor_Window.h"
#include "../NuNuEngine_CORE/High Level Interface/NApplication.h"
#include "../NuNuEngine_Window/Scenes/NLoadScenes.h"

#include <ctime>

#include "guiEditorApplication.h"

NuNu::Application application;
//#define WITH_EDITOR 

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance, const wchar_t* name, WNDPROC proc);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, // 프로그램 인스턴스 핸들 - 메모리 접근
	_In_opt_ HINSTANCE hPrevInstance, // 바로 앞에 실행한 현재 프로그램의 인스턴스 핸들, 없을경우 NULL
	// 호환성 관련으로 지금은 신경쓰지 않아도 됨
	_In_ LPWSTR    lpCmdLine, // 명령행으로 입력된 프로그램 인수
	_In_ int       nCmdShow)   // 프로그램 실행될 형태, 모양정보등이 전달.
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(414);
	 // TODO: 여기에 코드를 입력합니다.
	 //

	SetProcessDPIAware();

	 // 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_EDITORWINDOW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, szWindowClass, WndProc);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EDITORWINDOW));

	MSG msg;

	while (application.IsRunning())
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // 메세지가 있으면
		{
			if (msg.message == WM_QUIT)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// 메세지가 없을 경우 여기서 게임로직 처리
			application.Run(); // 매 프레임마다 호출

#ifdef WITH_EDITOR
			gui::EditorApplication::Run();
#else
			application.CloseCommandList();
#endif
			//Excute command list
			application.ExcuteCommandList();

#ifdef WITH_EDITOR
			gui::EditorApplication::UpdatePlatformWindows();
#else
			//application.WaitForNextFrameResources();
#endif

			application.Present();	// 화면 출력

#ifdef WITH_EDITOR
#else
			application.MoveToNextFrame();
#endif
		}
	}

#ifdef WITH_EDITOR
	gui::EditorApplication::Release();
#endif
	application.Release();

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance, const wchar_t* name, WNDPROC proc)
{
	WNDCLASSEXW wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EDITORWINDOW));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;// MAKEINTRESOURCEW(IDC_EDITORWINDOW);
	wcex.lpszClassName = name;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	const UINT width = 1600;
	const UINT height = 900;
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, hInstance, nullptr);

	//// 원래는 전체 화면(1600x900)이 제목 표시줄에 먹혀서 작아지는 현상을 방지
	//RECT rect = { 0, 0, (LONG)width, (LONG)height };
	//AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	//// 현재 DX11 해상도 변환(Resize) 기능이 없으므로, 창 늘리기(스케일링)와 최대화 버튼을 잠시 끕니다.
	//// (안 그러면 화면이 늘어나면서 마우스 좌표가 다 망가져버립니다!)
	//HWND hWnd = CreateWindowW(szWindowClass, szTitle, 
	//	WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
	//	0, 0, rect.right - rect.left, rect.bottom - rect.top, 
	//	nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
		assert(false);

	application.Initialize(hWnd, width, height);
#ifdef WITH_EDITOR
#else
	application.WaitforGpu();
#endif

	NuNu::LoadScenes();
#ifdef WITH_EDITOR
	gui::EditorApplication::Initialize();
#endif

	return TRUE;
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_SIZE:
	{
		application.GetWindow().SetWindowResize(LOWORD(lParam), HIWORD(lParam));
	}
	break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		const int keyCode = static_cast<int>(wParam);
		const int scancode = (lParam >> 16) & 0x1ff;

		const int KEY_RELEASE = 0;
		const int KEY_PRESS = 1;

		const int action = ((lParam >> 31) & 1) ? KEY_RELEASE : KEY_PRESS;

		const int mods = []() -> int
			{
				int mod = 0;
				if (GetKeyState(VK_SHIFT) & 0x8000) mod |= 1;
				if (GetKeyState(VK_CONTROL) & 0x8000) mod |= 2;
				if (GetKeyState(VK_MENU) & 0x8000) mod |= 4;

				return mod;
			}();

		gui::EditorApplication::SetKeyPressed(keyCode, scancode, action, mods);

	}
	break;
	case WM_MOUSEMOVE:
	{
		gui::EditorApplication::SetCursorPos(wParam, lParam);
	}
	break;
	case WM_PAINT:
	{
		// 여기는 계속 반복적으로 그려냄
		// DC 화면 출력에 필요한 모든 정보를 가지는 데이터 구조체
		// GDI모듈에 의해서 관리
		// 폰트, 선 굵기, 색상 등
		// 화면 출력에 필요한 모든 경우는 WINAPI에서는 DC를 통해서 작업을 진행

		PAINTSTRUCT ps;
		HDC hdc = NULL;
		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DPICHANGED:
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
		{
			//const int dpi = HIWORD(wParam);
			//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
			const RECT* suggested_rect = (RECT*)lParam;
			::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top
				, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
