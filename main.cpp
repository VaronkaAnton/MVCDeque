#undef UNICODE
#define _SCL_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include "resource.h"

#include "Model.h"
#include "View.h"
#include "Controller.h"


KDeque<int> deque;
View<int> *view;
Controller<int> *controller;

INT_PTR CALLBACK MainDlgFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE HPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, MainDlgFunc);
	return(0);
}

INT_PTR CALLBACK MainDlgFunc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	view = new View<int>(&deque, hDlg);
	controller = new Controller<int>(&deque, view);

	switch (msg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_IN:
		{
			char input[50];
			SendMessage(GetDlgItem(hDlg, IDC_EDIT1), EM_GETLINE, 0, (LPARAM)input);
			deque.fromString(input);
			view->update();
		} break;

		case IDC_SIZE:
			controller->showSize();
			break;
		case IDC_IFEMPTY:
			controller->showIfEmpty();
			break;
		case IDC_POPBACK:
			controller->pop_back();
			break;
		case IDC_POPFRONT:
			controller->pop_front();
			break;

		case IDC_PUSHBACK:
			controller->push_back();
			break;

		case IDC_PUSHFRONT:
			controller->push_front();
			break;

		case IDC_CLEAR:
			controller->clear();
			break;

		case IDOK:
			EndDialog(hDlg, 1);
			break;

		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return (DefWindowProc(hDlg, msg, wParam, lParam));
	}
	return (0);
}