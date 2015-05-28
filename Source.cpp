#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <math.h>

TCHAR szClassName[] = TEXT("Window");

int getzero(){ return 0; }

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	TCHAR szText[32];
	switch (msg)
	{
	case WM_CREATE:
		CreateWindow(TEXT("BUTTON"), TEXT("ゼロで割ってみるテスト(構造化例外をキャッチする)"),
			WS_VISIBLE | WS_CHILD, 10, 10, 512, 30, hWnd, (HMENU)100,
			((LPCREATESTRUCT)lParam)->hInstance, 0);
		CreateWindow(TEXT("BUTTON"), TEXT("ゼロで割ってみるテスト(構造化例外をキャッチしない)"),
			WS_VISIBLE | WS_CHILD, 10, 50, 512, 30, hWnd, (HMENU)101,
			((LPCREATESTRUCT)lParam)->hInstance, 0);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == 100)
		{
			__try
			{
				int n = (int)sin(0.0);
				wsprintf(szText, TEXT("%d"), 1 / n); // 例外発生!
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				const DWORD code = GetExceptionCode();
				if (code == EXCEPTION_INT_DIVIDE_BY_ZERO)
				{
					wsprintf(szText, TEXT("ゼロ割の例外発生(%X)"), code);
					MessageBox(hWnd, szText, 0, 0);
				}
			}
		}
		else if (LOWORD(wParam) == 101)
		{
			int n = (int)sin(0.0);
			wsprintf(szText, TEXT("%d"), 1 / n); // 例外発生!
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

EXTERN_C void __cdecl WinMainCRTStartup()
{
	MSG msg;
	HINSTANCE hInstance = GetModuleHandle(0);
	WNDCLASS wndclass = { CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0,
		hInstance, 0, LoadCursor(0, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1), 0, szClassName };
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(szClassName, TEXT("Window"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, 0, 0, hInstance, 0);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ExitProcess(msg.wParam);
}

#if _DEBUG
void main(){}
#endif