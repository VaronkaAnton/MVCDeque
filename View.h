/*View class for MVC pattern:
implements visualisation using WinAPI*/

#pragma once

template<class T>
class View {
	KDeque<T> *deque;
	HWND hDlg;
public:
	View (KDeque<T> *deque, HWND hDlg) : deque(deque), hDlg(hDlg) {}

	void update() {
		string out = deque->toString();
		SendMessage(GetDlgItem(hDlg, IDC_EDIT2), LB_DELETESTRING, 0, NULL);
		SendMessage(GetDlgItem(hDlg, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)out.c_str());
	}
	
	int toPush() {
		char input[50];
		SendMessage(GetDlgItem(hDlg, IDC_EDIT4), EM_GETLINE, 0, (LPARAM)input);
		int i = atoi(input);
		return i;
	}
	void showSize(int size) {
		int sz = deque->size();
		string out = to_string(sz);
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT3), "");
		SendMessage(GetDlgItem(hDlg, IDC_EDIT3), WM_SETTEXT, 0, (LPARAM)out.c_str());
	}

	void showIfEmpty(bool res) {
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT3), "");
		if (res)
			SendMessage(GetDlgItem(hDlg, IDC_EDIT3), WM_SETTEXT, 0, (LPARAM)"True");
		else
			SendMessage(GetDlgItem(hDlg, IDC_EDIT3), WM_SETTEXT, 0, (LPARAM)"False");
	}

	~View() {
		delete deque;
	}
};