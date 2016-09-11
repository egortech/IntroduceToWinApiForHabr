#include <Windows.h>                                            //�������� ������������ ���� WINAPI               
#include <tchar.h>												//������������ ������� ��� ANSII � Unicode ������
#include "resource.h"                                           //����������� ��������� ���������� � ������������

#define TXTBOXBUFFERSIZE 1024                                   //������������ ������ ������ ��� ���������� �� ���������� ����

WNDPROC        pfnOldEditProc = NULL;                           //��������� �� ����������� ������� ��������� ��������� ���������� ����
TCHAR          decimalSeparator;                                //����������� ����� �����

//------------------------------------------���������� �������------------------------------------------

//--------------------------������� WinMain--------------------------
//����� ����������:
//		����� ����� � ���������� WinAPI
//���������:
//		hInstance     - ���������� ���������� ����������
//      hPrevInstance - ���������� ���������� ����������
//      lpCmdLine     - ��������� �� ������ � ����������� �������� 
//                      ������
//      nShowCmd      - ���������� ���� ��� ���
//����������:
//		��� ���������� ���������� ��� ������������ �������
INT WINAPI
WinMain(
	IN          HINSTANCE hInstance,
	OPTIONAL IN HINSTANCE hPrevInstance,
	IN          LPSTR     lpCmdLine,
	IN          INT       nShowCmd);

//-------------------------������� DialogProc-------------------------
//����� ����������:
//		����������� ���� ��������� ��������� ��� �������� �����������
//	    ����.
//���������:
//		hDlg -   ���������� ����������� ����
//      uMsg -   ��� ���������, ������� ����� �������� ���������� ����
//      wParam - ���������� � ���������
//      lParam - ���������� � ���������
//����������:
//		����� �� ������ ������������ ��������� �������
INT_PTR CALLBACK
DialogProc(
	IN HWND     hDlg,
	IN UINT     uMsg,
	IN WPARAM   wParam,
	IN LPARAM   lParam);

//------------------------������� OnInitDialog------------------------
//����� ����������:
//		����������� ��� ������ ������������� ����������� ����
//���������:
//		hDlg - ���������� ����������� ����
//����������:
//		������
inline VOID
OnInitDialog(
	IN HWND     hDlg);

//--------------------------������� OnPaint--------------------------
//����� ����������:
//		����������� ��� ������������� ������������ ����
//���������:
//		hDlg - ���������� ����������� ����
//����������:
//		������
inline VOID
OnPaint(
	IN HWND     hDlg);

//----------------------������� OnEditBoxChange----------------------
//����� ����������:
//		����������� ��� ��������� ������ � ��������� ����
//���������:
//		hDlg - ���������� ����������� ����
//����������:
//		������
inline VOID
OnEditBoxChange(
	IN HWND     hDlg);

//------------------------������� OnCalcClick------------------------
//����� ����������:
//		����������� ��� ������� ������ "���������
//���������:
//		hDlg - ���������� ����������� ����
//����������:
//		������
inline VOID
OnCalcClick(
	IN HWND     hDlg);

//-------------------------������� OnDestroy-------------------------
//����� ����������:
//		����������� ��� ����������� ����������� ����
//���������:
//		hDlg - ���������� ����������� ����
//����������:
//		������
inline VOID
OnDestroy(
	IN HWND     hDlg);

//---------------------������� DoubleEditBoxProc---------------------
//����� ����������:
//		���� ��������� ��������� ��� ���������� ����
//���������:
//		hDlg -   ���������� ���������� ����
//      uMsg -   ��� ���������, ������� ����� �������� ���������� ����
//      wParam - ���������� � ���������
//      lParam - ���������� � ���������
//����������:
//		����� �� ������ ������������ ��������� �������
LRESULT CALLBACK
DoubleEditBoxProc(
	IN HWND     hWnd,
	IN UINT     uMsg,
	IN WPARAM   wParam,
	IN LPARAM   lParam);

//------------------------������� HandlePaste------------------------
//����� ����������:
//		��������� ������� � ��������� ���� �� ������ ������,
//      �������� ��� ������������� ����������� 
//      ������������ �����
//���������:
//		hDlg - ���������� ����������� ����
//����������:
//		������
VOID
HandlePaste(
	IN HWND     hWnd);

//---------------------������� PasteTextInEditBox---------------------
//����� ����������:
//		��������� ������� ������ � ��������� ����, �������� ��� 
//      ������������� ���������� ������������ �����
//���������:
//		hWnd    - ���������� �������� ����������
//      pszText - ����������� �����
//����������:
//		������
VOID PasteTextInEditBox(
	IN HWND     hWnd,
	IN LPTSTR   pszText);

//-----------------------������� IsValidDouble-----------------------
//����� ����������:
//		���������, ��� � ������ ������������ �����
//���������:
//      psz - ����� ��� ��������
//����������:
//		TRUE  -> � ������ ������������ �����
//      FALSE -> � ������ �� ������ ������������ �����
BOOL IsValidDouble(
	IN LPTSTR   psz);
//------------------------------------------------------------------------------------------------------

//------------------------------------------���������� �������------------------------------------------

INT WINAPI
WinMain(
	IN          HINSTANCE hInstance,
	OPTIONAL IN HINSTANCE hPrevInstance,
	IN          LPSTR     lpCmdLine,
	IN          INT       nShowCmd)
{
	MSG  msg;                                                        //���������
	BOOL ret;                                                        //�������� ��� �������� � ��

	//�������� ����������� ������� �����
	GetLocaleInfo(
		LOCALE_USER_DEFAULT,                                         //����������� ������
		LOCALE_SDECIMAL,                                             //����������� ������� �����
		&decimalSeparator,                                           //���� ��������
		sizeof decimalSeparator);                                    //������ ��� �����������

	//�������� ����������� ����
	HWND hDlg = CreateDialogParam(
		hInstance,                                                   //�������� ����������
		MAKEINTRESOURCE(IDD_MAINDIALOG),                             //������ ��������� ���������� � ����� ����
		NULL,													     //������������ ����
		DialogProc,                                                  //��������� �� ���� ��������� ���������
		0);                                                          //�������������� ��������� ��� �������� ����

	//��������� ����
	ShowWindow(
		hDlg,                                                        //���������� ����
		nShowCmd);

	while (true)
	{
		//�������� ��������� ���������
		ret = GetMessage(
			&msg,                                                    //��������� ���������
			NULL,                                                    //���������� ��������� ���������
			0,                                                       //������ ������� ��������� ���������
			0);                                                      //������� ������� ��������� ���������

		//��������� � ���������� ����������
		switch (ret)
		{
		case 0:
			return 0;
		case -1:
			return -1;
		}

		//��������� ������������� �� ��������� �� ������ ����������� ����
		if (!IsDialogMessage(
			hDlg,                                                    //���������� ����
			&msg))                                                   //���������
		{
			TranslateMessage(&msg);                                  //��������� ���������   
			DispatchMessage(&msg);                                   //������������ ��������� ������� ���������
		}
	}
}

INT_PTR CALLBACK DialogProc(
	IN HWND   hDlg,
	IN UINT   uMsg,
	IN WPARAM wParam,
	IN LPARAM lParam)
{
	// � ��� ������ ���������
	if (HIWORD(wParam) == EN_CHANGE)                                 //��������� ������ � ��������� ����
	{
		OnEditBoxChange(hDlg);
		return TRUE;
	}

	switch (uMsg)
	{
	case WM_INITDIALOG:                                              //������������� ����
		OnInitDialog(hDlg);
		return TRUE;

	case WM_PAINT:                                                   //��������� ����
		OnPaint(hDlg);
		return FALSE;

	case WM_COMMAND:                                                 //������ �����-�� �������
		switch (LOWORD(wParam))
		{
		case IDC_CLEARBUTTON:                                        //������� �� ������ "�������"
																	 //������� ������ � ���������� �� ����
			ShowWindow(
				GetDlgItem(                                          //�������� ���������� �������� �� ��� ID
					hDlg,                                            //����������� ����
					IDC_RESULTSTATIC),                               //ID �������� ��� �������
				SW_HIDE);                                            //������� ������
			return TRUE;

		case IDC_CALCBUTTON:                                         //������� ������ "���������"
			OnCalcClick(hDlg);
			return TRUE;

		default:
			return FALSE;
		}

	case WM_CLOSE:                                                   //�������� ����
		DestroyWindow(hDlg);                                         //��� �����������
		return TRUE;

	case WM_DESTROY:                                                 //����������� ����
		OnDestroy(hDlg);
		PostQuitMessage(0);                                          //������� ��� ���������� ���������� ��
		return TRUE;

	default:
		return FALSE;
	}
}


VOID OnInitDialog(
	IN HWND hDlg)
{
	//�������� ����������� ��������� ��� ���������� ���� � ��������� �
	pfnOldEditProc = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(                                            //�������� ��������� � �������� ����������
			GetDlgItem(                                              //���������� ��� ���� Y
				hDlg,
				IDC_XEDIT),
			GWLP_WNDPROC,                                            //������� ��� ����� ������� ���������
			reinterpret_cast<INT_PTR>(
				DoubleEditBoxProc)));                                //����� ���������

	//��������� ���� ��� X
	SetWindowLongPtr(
		GetDlgItem(hDlg, IDC_YEDIT),
		GWLP_WNDPROC,
		reinterpret_cast<INT_PTR>(
			DoubleEditBoxProc));

	//��������� ������ �������
	EnableWindow(
		GetDlgItem(
			hDlg,
			IDC_CALCBUTTON),
		FALSE);                                                      //������� �������?
}

VOID OnPaint(
	IN HWND hDlg)
{
	//������������� ����� ��� ���� ����������� ����������
	SendMessage(
		GetDlgItem(
			hDlg,
			IDC_RESULTSTATIC),
		WM_SETFONT,                                                  //������� ����� ������
		reinterpret_cast<WPARAM>(CreateFont(                         //������ ������ ��� ������
			16,                                                      //�����
			0,                                                       //������ �������
			0,                                                       //������ �������
			0,                                                       //���� �������
			FW_BOLD,                                                 //������� ������ (�������)
			FALSE,                                                   //������?
			FALSE,                                                   //����� �������������?
			FALSE,                                                   //����� ������������?
			DEFAULT_CHARSET,                                         //������������ ��������� (�����������)
			OUT_DEFAULT_PRECIS,                                      //������������� �������� ������ ������
			CLIP_DEFAULT_PRECIS,                                     //������������� �������� ��������� �� ��������.
			DEFAULT_QUALITY,                                         //������������� �������� ������ ������.
			DEFAULT_PITCH,                                           //������������� ��� ����� ��������� � ��������� ������.
			_T("MS Shell Dlg"))),                                    //�������� ������
		TRUE);                                                       //������� �����������
}

VOID OnEditBoxChange(
	IN HWND hDlg)
{
	DWORD xTextBoxTextSize;                                          //���������� �������� � ���� ��� x
	DWORD yTextBoxTextSize;										     //� ���� ��� y

	xTextBoxTextSize = SendMessage(
		GetDlgItem(
			hDlg,
			IDC_XEDIT),
		EM_LINELENGTH,                                               //������� ��� �������� ���-�� �������� � ������
		-1,                                                          //����� ������ (-1 => ������� ������)
		0);
	yTextBoxTextSize = SendMessage(
		GetDlgItem(
			hDlg,
			IDC_YEDIT),
		EM_LINELENGTH,
		-1,
		0);

	//�������� ������ �������, ���� ���� ������� �������� � ��� ����
	EnableWindow(
		GetDlgItem(
			hDlg,
			IDC_CALCBUTTON),
		!(xTextBoxTextSize == 0
			|| yTextBoxTextSize == 0));
}

VOID OnCalcClick(
	IN HWND hDlg)
{
	LPTSTR szTextValueX = new TCHAR[TXTBOXBUFFERSIZE];                //����� � ���� ��� X
	LPTSTR szTextValueY = new TCHAR[TXTBOXBUFFERSIZE];                //����� � ���� ��� Y
	LPTSTR szTextResult = new TCHAR[TXTBOXBUFFERSIZE];                //����� ��� ������
	DOUBLE x, y;                                                      //����� �� ������ �� �����

	szTextResult[0] = '\0';                                           //������������� ������ ��� �����������

	//�������� ����� �� ���� X
	GetDlgItemText(
		hDlg,                                                         //����������� ����
		IDC_XEDIT,                                                    //ID ����
		szTextValueX,                                                 //���������� ��� �����������
		TXTBOXBUFFERSIZE);                                            //������������ ����� �������� ��� �����������

    //�������� ����� �� ���� Y
	GetDlgItemText(
		hDlg,
		IDC_YEDIT,
		szTextValueY,
		TXTBOXBUFFERSIZE);

	//�������� �������� �������� �� ������
	x = _tcstod(szTextValueX, NULL);
	y = _tcstod(szTextValueY, NULL);

	//���������� ������ � ����������
	//���������� �����
	_tcscat_s(
		szTextResult,                                                //����������
		TXTBOXBUFFERSIZE,                                            //������������ ����� �������� ��� �����������
		_T("���������: "));                                          //��������
	LPTSTR res;
	if (x == 0 && y == 0)
		res = _T("������ ���������");
	else if (x == 0)
		res = _T("��� �������");
	else if (y == 0)
		res = _T("��� ������");
	else if (x > 0 && y > 0)
		res = _T("I ��������");
	else if (x < 0 && y > 0)
		res = _T("II ��������");
	else if (x < 0 && y < 0)
		res = _T("III ��������");
	else
		res = _T("IV ��������");

	_tcscat_s(
		szTextResult,
		TXTBOXBUFFERSIZE,
		res);

	//��������� ����� � ����
	SetDlgItemText(
		hDlg,                                                        //���������� ����
		IDC_RESULTSTATIC,                                            //ID �������� ��� �����������
		szTextResult);                                               //����� ��� �����������

	//���������� ������� ������� � ����������
	ShowWindow(
		GetDlgItem(
			hDlg,
			IDC_RESULTSTATIC),
		SW_SHOW);

	//������� �������
	free(szTextValueX);
	free(szTextValueY);
	free(szTextResult);
}

VOID OnDestroy(
	IN HWND hDlg)
{
	//��������� ����������� ��������� ��������� �����
	SetWindowLongPtr(
		GetDlgItem(
			hDlg,
			IDC_YEDIT),
		GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(
			pfnOldEditProc));

	SetWindowLongPtr(
		GetDlgItem(
			hDlg,
			IDC_YEDIT),
		GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(
			pfnOldEditProc));
}

LRESULT CALLBACK
DoubleEditBoxProc(
	IN HWND   hWnd,
	IN UINT   uMsg,
	IN WPARAM wParam,
	IN LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PASTE:                                                   //�������
		HandlePaste(hWnd);
		return FALSE;
	case WM_CHAR:                                                    //� ��������� ���� ����� ���� ������
																	 //������ ������� Ctrl
		if (0x8000
			& GetKeyState(VK_CONTROL))
		{
			if (wParam == 0x16)                                      //Ctrl+V
			{
				HandlePaste(hWnd);                                   //��������� �������
				return 0;
			}
		}
		else if (wParam == VK_BACK);                                 //����������� ���������� ��� BackSpace
		else
		{
			//������ ������ � ����-����������� ������
			LPTSTR s = new TCHAR[2];
			s[0] = wParam;
			s[1] = '\0';
			//��������� � � ������ �����
			PasteTextInEditBox(
				hWnd,
				s);
			free(s);
			return 0;
		}
	}

	//��� ����������� ���������� ���� ������ ���������� ��������������� ���������
	return CallWindowProc(
		pfnOldEditProc,
		hWnd,
		uMsg,
		wParam,
		lParam);
}

VOID
PasteTextInEditBox(
	IN HWND hWnd,
	IN LPTSTR pszText)
{
	LPTSTR pszNewText = new TCHAR[TXTBOXBUFFERSIZE];                 //����� ����� ��� ���� ����� ������
	LPTSTR pszTextInTextBox = new TCHAR[TXTBOXBUFFERSIZE];           //����� � ���� �����

	DWORD  from, to;                                                 //������� ��������� � ��������� ����

																	 //�������� ����� � ��������� ����
	GetWindowText(
		hWnd,
		pszTextInTextBox,
		TXTBOXBUFFERSIZE);

	//�������� ������� ���������
	SendMessage(
		hWnd,
		EM_GETSEL,                                                   //������� ��� ��������� ���������
		reinterpret_cast<WPARAM>(&from),                             //������ ���������
		reinterpret_cast<WPARAM>(&to));                              //����� ���������

																	 //������������ ������ ������ ��� ���������
	_tcsncpy_s(
		pszNewText,                                                  //����������
		TXTBOXBUFFERSIZE,                                            //������������ ������ ��� �����������
		pszTextInTextBox,                                            //��������
		from);                                                       //������� �������� �� ��������� ����� �����������

	//����������� ��������
	_tcscpy_s(
		pszNewText + from,
		TXTBOXBUFFERSIZE - from,
		pszText);

	//�������� ����� ��� ���������
	_tcscpy_s(
		pszNewText + from + _tcslen(pszText),
		TXTBOXBUFFERSIZE - from - _tcslen(pszText),
		pszTextInTextBox + to);

	//��������� ������ �� ���������� ������������� �����
	if (IsValidDouble(pszNewText))
	{
		//���������� ����� � ���� ��� ����� ������
		SetWindowText(
			hWnd,
			pszNewText);

		//���������� ������� �������
		SendMessage(
			hWnd,
			EM_SETSEL,
			(WPARAM)(from + from - to + _tcslen(pszText)),
			(LPARAM)(from + from - to + _tcslen(pszText)));
	}
	//����� ������
	else
		MessageBeep(MB_ICONWARNING);

	//����������� ������
	free(pszNewText);
	free(pszTextInTextBox);
}

VOID
HandlePaste(
	IN HWND hWnd)
{
	//��������� ����� ������
	if (OpenClipboard(hWnd))
	{
		HANDLE hData;                                                //���������� ������ � ������ ������

#if UNICODE                                                          //��� Unicode
		hData = GetClipboardData(                                    //�������� ����� �� ������
			CF_UNICODETEXT);
#else
		hData = GetClipboardData(                                    //��� ANSII
			CF_TEXT);
#endif
		if (hData != NULL)                                           //���� ����� ����
			PasteTextInEditBox(hWnd, static_cast<LPTSTR>(
				GlobalLock(hData)));
		GlobalUnlock(hData);                                         //������� ���������� ������ ������
	}
	CloseClipboard();                                                //��������� ����� ������
}

BOOL
IsValidDouble(
	LPTSTR str)
{
	//�������� ���������, ���� ������� �����
	if (*str == '-')
		str++;
	BOOL wasDelimeter = FALSE;                                       //����������� ��� ���������� � ������
	while (*str != _T('\0'))                                         //���� �� ������ ��� ������
	{
		if (*str == decimalSeparator)                                //������-�����������
		{
			if (wasDelimeter)                                        //���� ����������� ��� ���, �� ������ �� ���������
				return FALSE;
			wasDelimeter = TRUE;
		}
		else if (!_istdigit(*str))                                   //���� ��� �� ����� ����
			return FALSE;
		str++;
	}
	return TRUE;
}

//------------------------------------------------------------------------------------------------------