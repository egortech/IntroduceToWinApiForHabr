#include <Windows.h>                                            //Основной заголовочный файл WINAPI               
#include <tchar.h>												//Инвариантные функции для ANSII и Unicode систем
#include "resource.h"                                           //Индификатор элементов управления в конструкторе

#define TXTBOXBUFFERSIZE 1024                                   //Максимальный размер буфера для сохранения из текстового поля

WNDPROC        pfnOldEditProc = NULL;                           //Указатель на стандартную функцию обработки сообщений текстового поля
TCHAR          decimalSeparator;                                //Разделитель целой части

//------------------------------------------Декларация функций------------------------------------------

//--------------------------Функция WinMain--------------------------
//Общая информация:
//		Точка входа в приложения WinAPI
//Параметры:
//		hInstance     - Дескриптор экземпляра приложения
//      hPrevInstance - Предыдуший дискриптор приложения
//      lpCmdLine     - Указатель на строку с аргументами комадной 
//                      строки
//      nShowCmd      - Показывать окно или нет
//Возвращает:
//		Код завершения приложения для операционной системы
INT WINAPI
WinMain(
	IN          HINSTANCE hInstance,
	OPTIONAL IN HINSTANCE hPrevInstance,
	IN          LPSTR     lpCmdLine,
	IN          INT       nShowCmd);

//-------------------------Функция DialogProc-------------------------
//Общая информация:
//		Стандартный цикл обработки сообщений для главного диалогового
//	    окна.
//Параметры:
//		hDlg -   Дескриптор диалогового окна
//      uMsg -   Тип сообщения, которое хочет передать диалоговое окно
//      wParam - Информация о сообщении
//      lParam - Информация о сообщении
//Возвращает:
//		Нужно ли дальше обрабатывать сообщение системе
INT_PTR CALLBACK
DialogProc(
	IN HWND     hDlg,
	IN UINT     uMsg,
	IN WPARAM   wParam,
	IN LPARAM   lParam);

//------------------------Функция OnInitDialog------------------------
//Общая информация:
//		Выполняется при первой инициализации диалогового окна
//Параметры:
//		hDlg - Дескриптор диалогового окна
//Возвращает:
//		Ничего
inline VOID
OnInitDialog(
	IN HWND     hDlg);

//--------------------------Функция OnPaint--------------------------
//Общая информация:
//		Выполняется при необходимости перерисовать окно
//Параметры:
//		hDlg - Дескриптор диалогового окна
//Возвращает:
//		Ничего
inline VOID
OnPaint(
	IN HWND     hDlg);

//----------------------Функция OnEditBoxChange----------------------
//Общая информация:
//		Выполняется при изменении текста в текстовом поле
//Параметры:
//		hDlg - Дескриптор диалогового окна
//Возвращает:
//		Ничего
inline VOID
OnEditBoxChange(
	IN HWND     hDlg);

//------------------------Функция OnCalcClick------------------------
//Общая информация:
//		Выполняется при нажатии кнопки "Выполнить
//Параметры:
//		hDlg - Дескриптор диалогового окна
//Возвращает:
//		Ничего
inline VOID
OnCalcClick(
	IN HWND     hDlg);

//-------------------------Функция OnDestroy-------------------------
//Общая информация:
//		Выполняется при уничтожении диалогового окна
//Параметры:
//		hDlg - Дескриптор диалогового окна
//Возвращает:
//		Ничего
inline VOID
OnDestroy(
	IN HWND     hDlg);

//---------------------Функция DoubleEditBoxProc---------------------
//Общая информация:
//		Цикл обработки сообщений для текстового поля
//Параметры:
//		hDlg -   Дескриптор текстового поля
//      uMsg -   Тип сообщения, которое хочет передать диалоговое окно
//      wParam - Информация о сообщении
//      lParam - Информация о сообщении
//Возвращает:
//		Нужно ли дальше обрабатывать сообщение системе
LRESULT CALLBACK
DoubleEditBoxProc(
	IN HWND     hWnd,
	IN UINT     uMsg,
	IN WPARAM   wParam,
	IN LPARAM   lParam);

//------------------------Функция HandlePaste------------------------
//Общая информация:
//		Прозводит вставку в текстовое поле из буфера обмена,
//      проверив что действительно вставляется 
//      вещественное число
//Параметры:
//		hDlg - Дескриптор диалогового окна
//Возвращает:
//		Ничего
VOID
HandlePaste(
	IN HWND     hWnd);

//---------------------Функция PasteTextInEditBox---------------------
//Общая информация:
//		Прозводит вставку текста в текстовое поле, проверив что 
//      действительно получается вещественное число
//Параметры:
//		hWnd    - Дескриптор элемента управления
//      pszText - Вставляемый текст
//Возвращает:
//		Ничего
VOID PasteTextInEditBox(
	IN HWND     hWnd,
	IN LPTSTR   pszText);

//-----------------------Функция IsValidDouble-----------------------
//Общая информация:
//		Проверяет, что в строке вещественное число
//Параметры:
//      psz - Текст для проверки
//Возвращает:
//		TRUE  -> В строке вещественное число
//      FALSE -> В строке не только вещественное число
BOOL IsValidDouble(
	IN LPTSTR   psz);
//------------------------------------------------------------------------------------------------------

//------------------------------------------Реализация функций------------------------------------------

INT WINAPI
WinMain(
	IN          HINSTANCE hInstance,
	OPTIONAL IN HINSTANCE hPrevInstance,
	IN          LPSTR     lpCmdLine,
	IN          INT       nShowCmd)
{
	MSG  msg;                                                        //Сообщение
	BOOL ret;                                                        //Значение для возврата в ОС

	//Получаем разделитель дробной части
	GetLocaleInfo(
		LOCALE_USER_DEFAULT,                                         //Стандартная локаль
		LOCALE_SDECIMAL,                                             //Разделитель дробной части
		&decimalSeparator,                                           //Куда копируем
		sizeof decimalSeparator);                                    //Размер для копирования

	//Создание диалогового окна
	HWND hDlg = CreateDialogParam(
		hInstance,                                                   //Экзепляр приложения
		MAKEINTRESOURCE(IDD_MAINDIALOG),                             //Ресурс созданный редактором с видом окна
		NULL,													     //Родительское окно
		DialogProc,                                                  //Указатель на цикл обработки сообщений
		0);                                                          //Дополнительные параметры для создания окна

	//Открываем окно
	ShowWindow(
		hDlg,                                                        //Дескриптор окна
		nShowCmd);

	while (true)
	{
		//Получаем очередное сообщение
		ret = GetMessage(
			&msg,                                                    //Сохраняем сообщение
			NULL,                                                    //Дескриптор источника сообщения
			0,                                                       //Нижняя граница получения сообщения
			0);                                                      //Верхняя граница получения сообщения

		//Сообщения о завершении приложения
		switch (ret)
		{
		case 0:
			return 0;
		case -1:
			return -1;
		}

		//Проверяем предназначено ли сообщение не нашему диалоговому окну
		if (!IsDialogMessage(
			hDlg,                                                    //Дескриптор окна
			&msg))                                                   //Сообщение
		{
			TranslateMessage(&msg);                                  //Сохраняет сообщение   
			DispatchMessage(&msg);                                   //Распределяет сообщение оконной процедуре
		}
	}
}

INT_PTR CALLBACK DialogProc(
	IN HWND   hDlg,
	IN UINT   uMsg,
	IN WPARAM wParam,
	IN LPARAM lParam)
{
	// О чем пришло сообщение
	if (HIWORD(wParam) == EN_CHANGE)                                 //Изменение текста в текстовом поле
	{
		OnEditBoxChange(hDlg);
		return TRUE;
	}

	switch (uMsg)
	{
	case WM_INITDIALOG:                                              //Инициализация окна
		OnInitDialog(hDlg);
		return TRUE;

	case WM_PAINT:                                                   //Отрисовка окна
		OnPaint(hDlg);
		return FALSE;

	case WM_COMMAND:                                                 //Пришла какая-то команда
		switch (LOWORD(wParam))
		{
		case IDC_CLEARBUTTON:                                        //Нажатие на кнопку "Очистка"
																	 //Скрытие текста о результате из окна
			ShowWindow(
				GetDlgItem(                                          //Получает дескриптор элемента по его ID
					hDlg,                                            //Декскриптор окна
					IDC_RESULTSTATIC),                               //ID элемента для скрытия
				SW_HIDE);                                            //Команда скрыть
			return TRUE;

		case IDC_CALCBUTTON:                                         //Нажатие кнопки "Сосчитать"
			OnCalcClick(hDlg);
			return TRUE;

		default:
			return FALSE;
		}

	case WM_CLOSE:                                                   //Закрытие окна
		DestroyWindow(hDlg);                                         //Его уничтожение
		return TRUE;

	case WM_DESTROY:                                                 //Уничтожение окна
		OnDestroy(hDlg);
		PostQuitMessage(0);                                          //Передаёт код завершения приложения ОС
		return TRUE;

	default:
		return FALSE;
	}
}


VOID OnInitDialog(
	IN HWND hDlg)
{
	//Получаем стандартную процедуру для текстового поля и подменяем её
	pfnOldEditProc = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(                                            //Получаем инфорацию о элементе управления
			GetDlgItem(                                              //Дескриптор для поля Y
				hDlg,
				IDC_XEDIT),
			GWLP_WNDPROC,                                            //Команда для смены оконной процедуры
			reinterpret_cast<INT_PTR>(
				DoubleEditBoxProc)));                                //Новая процедура

	//Текстовое поле для X
	SetWindowLongPtr(
		GetDlgItem(hDlg, IDC_YEDIT),
		GWLP_WNDPROC,
		reinterpret_cast<INT_PTR>(
			DoubleEditBoxProc));

	//Выключаем кнопку расчёта
	EnableWindow(
		GetDlgItem(
			hDlg,
			IDC_CALCBUTTON),
		FALSE);                                                      //Активен элемент?
}

VOID OnPaint(
	IN HWND hDlg)
{
	//Устанавливаем текст для поле отображения результата
	SendMessage(
		GetDlgItem(
			hDlg,
			IDC_RESULTSTATIC),
		WM_SETFONT,                                                  //Команда смены шрифта
		reinterpret_cast<WPARAM>(CreateFont(                         //Создаём нужный тип шрифта
			16,                                                      //Кегль
			0,                                                       //Высота символа
			0,                                                       //Ширина символа
			0,                                                       //Угол наклона
			FW_BOLD,                                                 //Толщина шрифта (Толстый)
			FALSE,                                                   //Курсив?
			FALSE,                                                   //Нужно подчеркивание?
			FALSE,                                                   //Нужно зачеркивание?
			DEFAULT_CHARSET,                                         //Используемая кодировка (Стандартная)
			OUT_DEFAULT_PRECIS,                                      //Устанавливает точность вывода данных
			CLIP_DEFAULT_PRECIS,                                     //Устанавливает точность отсечения по границам.
			DEFAULT_QUALITY,                                         //Устанавливает качество вывода данных.
			DEFAULT_PITCH,                                           //Устанавливает шаг между символами и семейство шрифта.
			_T("MS Shell Dlg"))),                                    //Название шрифта
		TRUE);                                                       //Вызвать перерисовку
}

VOID OnEditBoxChange(
	IN HWND hDlg)
{
	DWORD xTextBoxTextSize;                                          //Количество символов в поле для x
	DWORD yTextBoxTextSize;										     //В поле для y

	xTextBoxTextSize = SendMessage(
		GetDlgItem(
			hDlg,
			IDC_XEDIT),
		EM_LINELENGTH,                                               //Команда для возврата кол-ва символов в строке
		-1,                                                          //Номер строки (-1 => Текущая строка)
		0);
	yTextBoxTextSize = SendMessage(
		GetDlgItem(
			hDlg,
			IDC_YEDIT),
		EM_LINELENGTH,
		-1,
		0);

	//Включаем кнопку расчёта, если было введены значения в оба поля
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
	LPTSTR szTextValueX = new TCHAR[TXTBOXBUFFERSIZE];                //Текст в поле для X
	LPTSTR szTextValueY = new TCHAR[TXTBOXBUFFERSIZE];                //Текст в поле для Y
	LPTSTR szTextResult = new TCHAR[TXTBOXBUFFERSIZE];                //Текст для вывода
	DOUBLE x, y;                                                      //Числа из текста из полей

	szTextResult[0] = '\0';                                           //Инициализация строки для результатов

	//Получаем текст из поля X
	GetDlgItemText(
		hDlg,                                                         //Декскриптор окна
		IDC_XEDIT,                                                    //ID поля
		szTextValueX,                                                 //Назначение для копирования
		TXTBOXBUFFERSIZE);                                            //Максимальное число символов для копирования

    //Получаем текст из поля Y
	GetDlgItemText(
		hDlg,
		IDC_YEDIT,
		szTextValueY,
		TXTBOXBUFFERSIZE);

	//Получаем числовые значения из текста
	x = _tcstod(szTextValueX, NULL);
	y = _tcstod(szTextValueY, NULL);

	//Построение строки с результами
	//Контатация строк
	_tcscat_s(
		szTextResult,                                                //Назначение
		TXTBOXBUFFERSIZE,                                            //Максимальное число символов для копирования
		_T("Результат: "));                                          //Источник
	LPTSTR res;
	if (x == 0 && y == 0)
		res = _T("Начало координат");
	else if (x == 0)
		res = _T("Ось ординат");
	else if (y == 0)
		res = _T("Ось абцисс");
	else if (x > 0 && y > 0)
		res = _T("I четверть");
	else if (x < 0 && y > 0)
		res = _T("II четверть");
	else if (x < 0 && y < 0)
		res = _T("III четверть");
	else
		res = _T("IV четверть");

	_tcscat_s(
		szTextResult,
		TXTBOXBUFFERSIZE,
		res);

	//Отображем текст в окне
	SetDlgItemText(
		hDlg,                                                        //Дескриптор окна
		IDC_RESULTSTATIC,                                            //ID элемента для отображения
		szTextResult);                                               //Текст для отображения

	//Отображаем скрытую область с результами
	ShowWindow(
		GetDlgItem(
			hDlg,
			IDC_RESULTSTATIC),
		SW_SHOW);

	//Очистка буферов
	free(szTextValueX);
	free(szTextValueY);
	free(szTextResult);
}

VOID OnDestroy(
	IN HWND hDlg)
{
	//Возращаем стандартные процедуры текстовым полям
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
	case WM_PASTE:                                                   //Вставка
		HandlePaste(hWnd);
		return FALSE;
	case WM_CHAR:                                                    //В текстовом поле ввели один символ
																	 //Нажата клавиша Ctrl
		if (0x8000
			& GetKeyState(VK_CONTROL))
		{
			if (wParam == 0x16)                                      //Ctrl+V
			{
				HandlePaste(hWnd);                                   //Обработка вставки
				return 0;
			}
		}
		else if (wParam == VK_BACK);                                 //Стандартный обработчик для BackSpace
		else
		{
			//Пакуем символ в нуль-завершённую строку
			LPTSTR s = new TCHAR[2];
			s[0] = wParam;
			s[1] = '\0';
			//Вставляем её в строку ввода
			PasteTextInEditBox(
				hWnd,
				s);
			free(s);
			return 0;
		}
	}

	//Даём возможность текстовому полю самому обработать нефильтрованные сообщения
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
	LPTSTR pszNewText = new TCHAR[TXTBOXBUFFERSIZE];                 //Новый текст для поля ввода текста
	LPTSTR pszTextInTextBox = new TCHAR[TXTBOXBUFFERSIZE];           //Текст в поле ввода

	DWORD  from, to;                                                 //Позиция выделения в текстовом поле

																	 //Получаем текст в текстовом поле
	GetWindowText(
		hWnd,
		pszTextInTextBox,
		TXTBOXBUFFERSIZE);

	//Получаем область выделения
	SendMessage(
		hWnd,
		EM_GETSEL,                                                   //Команда для получения выделения
		reinterpret_cast<WPARAM>(&from),                             //Начало выделения
		reinterpret_cast<WPARAM>(&to));                              //Конец выделения

																	 //Копироваение начало текста без выделения
	_tcsncpy_s(
		pszNewText,                                                  //Назначение
		TXTBOXBUFFERSIZE,                                            //Максимальный размер для копирования
		pszTextInTextBox,                                            //Источник
		from);                                                       //Сколько символов из источника нужно скопировать

	//Вставляемый фрагмент
	_tcscpy_s(
		pszNewText + from,
		TXTBOXBUFFERSIZE - from,
		pszText);

	//Отстаток текст без выделения
	_tcscpy_s(
		pszNewText + from + _tcslen(pszText),
		TXTBOXBUFFERSIZE - from - _tcslen(pszText),
		pszTextInTextBox + to);

	//Проверяем строку на содержание вещественного числа
	if (IsValidDouble(pszNewText))
	{
		//Отображаем текст в поле для ввода текста
		SetWindowText(
			hWnd,
			pszNewText);

		//Возвращаем позицию курсора
		SendMessage(
			hWnd,
			EM_SETSEL,
			(WPARAM)(from + from - to + _tcslen(pszText)),
			(LPARAM)(from + from - to + _tcslen(pszText)));
	}
	//Иначе ошибка
	else
		MessageBeep(MB_ICONWARNING);

	//Освобождаем память
	free(pszNewText);
	free(pszTextInTextBox);
}

VOID
HandlePaste(
	IN HWND hWnd)
{
	//Открываем буфер обмена
	if (OpenClipboard(hWnd))
	{
		HANDLE hData;                                                //Дескриптор данных в буфере обмена

#if UNICODE                                                          //Для Unicode
		hData = GetClipboardData(                                    //Получаем текст из буфера
			CF_UNICODETEXT);
#else
		hData = GetClipboardData(                                    //Для ANSII
			CF_TEXT);
#endif
		if (hData != NULL)                                           //Если текст есть
			PasteTextInEditBox(hWnd, static_cast<LPTSTR>(
				GlobalLock(hData)));
		GlobalUnlock(hData);                                         //Снимаем блокировку буфера обмена
	}
	CloseClipboard();                                                //Закрываем буфер обмена
}

BOOL
IsValidDouble(
	LPTSTR str)
{
	//Сдвигаем указатель, если впереди минус
	if (*str == '-')
		str++;
	BOOL wasDelimeter = FALSE;                                       //Разделитель уже встретился в строке
	while (*str != _T('\0'))                                         //Пока не прочтём всю строку
	{
		if (*str == decimalSeparator)                                //Символ-разделитель
		{
			if (wasDelimeter)                                        //Если разделитель уже был, то строка не корректна
				return FALSE;
			wasDelimeter = TRUE;
		}
		else if (!_istdigit(*str))                                   //Если это не цифра тоже
			return FALSE;
		str++;
	}
	return TRUE;
}

//------------------------------------------------------------------------------------------------------