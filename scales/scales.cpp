// scales.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "scales.h"

mainDialog*	_pdialogMainPtr		= mainDialog::getSingletonPtr();

std::wstring g_wsAPPVersion		= L"1.0";		// ��Ӧ�ó���䱾

int g_ifontMSYH11 = 0;				// ΢���ź�11
int g_ifontMSYH12 = 0;				// ΢���ź�12
int g_ifontMSYH12i = 0;				// ΢���ź�12 б��
int g_ifontMSYH12u = 0;				// ΢���ź�12 �»���
int g_ifontMSYH12b = 0;				// ΢���ź�12 ����
int g_ifontMSYH14 = 0;				// ΢���ź�14
int g_ifontMSYH15 = 0;				// ΢���ź�15
int g_ifontMSYH16 = 0;				// ΢���ź�16
int g_ifontMSYH16b = 0;				// ΢���ź�16 ����
int g_ifontMSYH18 = 0;				// ΢���ź�18
int g_ifontMSYH18u = 0;				// ΢���ź�18 �»���
int g_ifontMSYH22 = 0;				// ΢���ź�22
int g_ifontMSYH24 = 0;				// ΢���ź�24
int g_ifontMSYH25 = 0;				// ΢���ź�25
int g_ifontMSYH28 = 0;				// ΢���ź�28
int g_ifontMSYH34 = 0;				// ΢���ź�34
int g_ifontMSYH38 = 0;				// ΢���ź�38



int Run(int nCmdShow = SW_SHOWDEFAULT)
{
	if (_pdialogMainPtr == NULL)
	{
		_pdialogMainPtr = new mainDialog();
		if(!_pdialogMainPtr->create())
		{
			delete _pdialogMainPtr;
			_pdialogMainPtr = NULL;
			return - 1;
		}
	}
	assert(_pdialogMainPtr);

	gtc::app::MAIN_WINDOW = *_pdialogMainPtr;
	_pdialogMainPtr->showWindow();

	wuc::drawManager::messageLoop();


	delete _pdialogMainPtr;
	return 0;
}
bool loadResource()
{
	int fails = 0;
	gtc::resourceManager &resManager = wuc::drawManager::getResource();
	gtc::image *pImage = nullptr;
	// ����ͼƬ
	// �����屳��ͼ
	if(!(pImage = resManager.addImage(L"main_bk", IDB_PNG_MAIN_BK, L"PNG")))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<main_bk>��");
	}
	else
	{
		pImage->setPatch(MAIN_WND_ROUNDCORNER_CX + MAIN_WND_DETECTION_LEFT
			, MAIN_WND_ROUNDCORNER_CY + MAIN_WND_DETECTION_TOP + MAIN_WND_CAPTION_CY + MAIN_WND_LINE_CY
			, MAIN_WND_ROUNDCORNER_CX + MAIN_WND_DETECTION_RIGHT
			, MAIN_WND_ROUNDCORNER_CY + MAIN_WND_DETECTION_BOTTOM + MAIN_WND_STATUS_CY + MAIN_WND_LINE_CY);
	}
	if(!resManager.addImage(L"button_minbox", IDB_PNG_BUTTON_MINBOX, L"PNG"))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<button_minbox>��");
	}	
	if(!resManager.addImage(L"button_close", IDB_PNG_BUTTON_CLOSE, L"PNG"))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<button_close>��");
	}
	if(!resManager.addImage(L"main_option_bk", IDB_PNG_MAIN_OPTION_BK, L"PNG"))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<main_option_bk>��");
	}
	// ��ͨ�Ի���
	if(!(pImage = resManager.addImage(L"dialog_bk", IDB_PNG_DIALOG_BK, L"PNG")))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<dialog_bk>��");
	}
	else
	{
		pImage->setPatch(10, 32, 10, 10);
	}
	// ͨ�ÿؼ�
	if(!resManager.addImage(L"scrollbar_v", IDB_PNG_SCROLLBAR_V, L"PNG"))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<scrollbar_v>��");
	}
	if(!resManager.addImage(L"combo_bk", IDB_PNG_COMBO_BK, L"PNG"))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<combo_bk>��");
	}
	if(!(pImage = resManager.addImage(L"button_bk", IDB_PNG_BUTTON_BK, L"PNG")))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<button_bk>��");
	}
	else
	{
		pImage->setPatch(4,4,4,4);
	}
	if(!(pImage = resManager.addImage(L"button_bk_green", IDB_PNG_BUTTON_BK_GREEN, L"PNG")))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<button_bk_green>��");
	}
	else
	{
		pImage->setPatch(4,4,4,4);
	}
	if(!(pImage = resManager.addImage(L"button_bk_icon", IDB_PNG_BUTTON_BK_ICON, L"PNG")))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<button_bk_icon>��");
	}
	else
	{
		pImage->setPatch(4,4,4,4);
	}
	if(!(pImage = resManager.addImage(L"button_cancel_bk", IDB_PNG_BUTTON_CANCEL_BK, L"PNG")))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<button_cancel_bk>��");
	}
	else
	{
		pImage->setPatch(4,4,4,4);
	}
	if(!(pImage = resManager.addImage(L"edit_rim", IDB_PNG_EDIT_RIM, L"PNG")))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<edit_rim>��");
	}
	else
	{
		pImage->setPatch(4,4,4,4);
	}
	// �˵�
	if(!resManager.addImage(L"menu_bk", IDB_PNG_MENU_BK, L"PNG"))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<menu_bk>��");
	}
	if(!resManager.addImage(L"menu_item_hot", IDB_PNG_MENU_ITEM_HOT, L"PNG"))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<menu_item_hot>��");
	}
	// ����
	if ((g_ifontMSYH11 = resManager.addFont(L"΢���ź�", 11/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�11>��");
	}
	if ((g_ifontMSYH12 = resManager.addFont(L"΢���ź�", 12/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�12>��");
	}
	if ((g_ifontMSYH12i = resManager.addFont(L"΢���ź�", 12, false, false, true)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�12>��");
	}
	if ((g_ifontMSYH12u = resManager.addFont(L"΢���ź�", 12, false, true)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�12>��");
	}
	if ((g_ifontMSYH12b = resManager.addFont(L"΢���ź�", 12, true)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�12b>��");
	}
	if ((g_ifontMSYH14 = resManager.addFont(L"΢���ź�", 14/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�14>��");
	}
	if ((g_ifontMSYH15 = resManager.addFont(L"΢���ź�", 15/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�15>��");
	}
	if ((g_ifontMSYH16 = resManager.addFont(L"΢���ź�", 16/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�16>��");
	}
	if ((g_ifontMSYH16b = resManager.addFont(L"΢���ź�", 16, true)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�16b>��");
	}
	if ((g_ifontMSYH18 = resManager.addFont(L"΢���ź�", 18/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�18>��");
	}
	if ((g_ifontMSYH18u = resManager.addFont(L"΢���ź�", 18, false, true)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�18u>��");
	}
	if ((g_ifontMSYH24 = resManager.addFont(L"΢���ź�", 24/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�24>��");
	}
	if ((g_ifontMSYH22 = resManager.addFont(L"΢���ź�", 22/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�22>��");
	}
	if ((g_ifontMSYH25 = resManager.addFont(L"΢���ź�", 25/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�25>��");
	}
	if ((g_ifontMSYH28 = resManager.addFont(L"΢���ź�", 28/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�28>��");
	}
	if ((g_ifontMSYH34 = resManager.addFont(L"΢���ź�", 34/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�34>��");
	}
	if ((g_ifontMSYH38 = resManager.addFont(L"΢���ź�", 38/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�38>��");
	}
	// �����ؼ�����
	//wchar_t pbuf[80] = {0};
	std::wstring wsAttribute;
	wchar_t pbuf[MAX_PATH] = {0};

	wsAttribute = L"image=\"name='scrollbar_v' corner='0,4,0,4'\"";
	std::swprintf(pbuf, _countof(pbuf), L" width=\"%d\"", SCROLLBAR_WIDTH);
	wsAttribute += pbuf;
	SIZE szCell = {8, 8};
	std::swprintf(pbuf, _countof(pbuf), L" button1normalimage=\"%s\"", sliceAreaCellString(0, szCell, false, 8).c_str());
	wsAttribute += pbuf;
	RECT rc = sliceAreaCell(5, szCell, false, 8);
	rc.bottom = rc.top + szCell.cy*2;
	std::swprintf(pbuf, _countof(pbuf), L" thumbnormalimage=\"%s\"", gtc::format::rect(rc).c_str());
	wsAttribute += pbuf;
	rc = sliceAreaCell(1, szCell, false, 8);
	rc.bottom = rc.top + szCell.cy*2;
	std::swprintf(pbuf, _countof(pbuf), L" thumbhotimage=\"%s\"", gtc::format::rect(rc).c_str());
	wsAttribute += pbuf;
	rc = sliceAreaCell(3, szCell, false, 8);
	rc.bottom = rc.top + szCell.cy*2;
	std::swprintf(pbuf, _countof(pbuf), L" thumbpushedimage=\"%s\"", gtc::format::rect(rc).c_str());
	wsAttribute += pbuf;

	std::swprintf(pbuf, _countof(pbuf), L" button2normalimage=\"%s\"", sliceAreaCellString(7, szCell, false, 8).c_str());
	wsAttribute += pbuf;

	resManager.addAttributeList(L"VScrollBar", wsAttribute);
	// ����combo����
	wsAttribute = L"image=\"name='combo_bk' corner='3,0,31,0'\"";
	//SIZE szCell = {51, KPSGLW_HOLIDAY_EDIT_ITEM_DAY_COMBO_CY};
	szCell.cx = 51;
	szCell.cy = 21;
	std::swprintf(pbuf, _countof(pbuf), L" normalimage=\"%s\"", gtc::format::rect(sliceAreaCell(0, szCell, false)).c_str());
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" hotimage=\"%s\"", gtc::format::rect(sliceAreaCell(1, szCell, false)).c_str());
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" pushedimage=\"%s\"", gtc::format::rect(sliceAreaCell(2, szCell, false)).c_str());
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" height=\"%d\"", COMBO_ITEM_CY);
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" itemtextcolor=\"#%08X\"", ARGB_COMBO_TEXT);
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" itemhottextcolor=\"#%08X\"", ARGB_COMBO_TEXT);
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" itemselectedtextcolor=\"#%08X\"", ARGB_COMBO_TEXT);
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" itembkcolor=\"#%08X\"", ARGB_COMBO_BK);
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" itemselectedbkcolor=\"#%08X\"", ARGB_COMBO_SELECTED);
	wsAttribute += pbuf;
	wsAttribute += L" textpadding=\"2,0,0,0\"";
	wsAttribute += L" itemtextpadding=\"2,0,0,0\"";
	std::swprintf(pbuf, _countof(pbuf), L" itemfont=\"%d\"", g_ifontMSYH12);
	wsAttribute += pbuf;
	resManager.addAttributeList(L"Combo", wsAttribute);
	// �˵����� 28����ߴ�Ҫ��ͼƬһ��Ŷ
	wsAttribute = L"image=\"name='menu_bk' corner='2,2,2,2'\" defaultitemheight=\"28\" separatorcolor=\"#FEF7F7F7\"" \
		L" bkimage=\"0,0,28,28\"" \
		L" itemimage=\"menu_item_hot\" itemhotimage=\"0,0,28,28\"";
	std::swprintf(pbuf, _countof(pbuf), L" itemfont=\"%d\" itemtextcolor=\"#FE5E5E5E\" itemhottextcolor=\"#FE5E5E5E\" itemselectedtextcolor=\"#FE5E5E5E\"", g_ifontMSYH11);
	wsAttribute += pbuf;
	resManager.addAttributeList(L"Menu", wsAttribute);
	// ͨ�ð�ť
	wsAttribute = L"image=\"button_bk\"";
	std::swprintf(pbuf, _countof(pbuf), L" textcolor=\"#%08X\"", ARGB_BUTTON_TEXT);
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" font=\"%d\"", g_ifontMSYH12);
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" normalimage=\"%s\" hotimage=\"%s\" pushedimage=\"%s\""
		, sliceAreaCellString(0, 16, false).c_str()
		, sliceAreaCellString(1, 16, false).c_str()
		, sliceAreaCellString(2, 16, false).c_str());
	wsAttribute += pbuf;
	resManager.addAttributeList(L"Button", wsAttribute);
	// ͨ��EDITBOX
	wsAttribute = L"image=\"edit_rim\" align=\"left\"";
	std::swprintf(pbuf, _countof(pbuf), L" prompttextcolor=\"#%08X\"", ARGB_EDIT_PROMPT_TEXT);
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" textcolor=\"#%08X\"", ARGB_EDIT_TEXT);
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" nativebkcolor=\"#%08X\"", ARGB_EDIT_BK);
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" font=\"%d\"", g_ifontMSYH14);
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" textpadding=\"%d,%d,%d,%d\"", EDIT_PADDING_LR,EDIT_PADDING_TB,EDIT_PADDING_LR,EDIT_PADDING_TB);
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" normalimage=\"%s\"", sliceAreaCellString(0, 20, false).c_str());
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" hotimage=\"%s\"", sliceAreaCellString(1, 20, false).c_str());
	wsAttribute += pbuf;
	resManager.addAttributeList(L"Edit", wsAttribute);
	// ͨ��Label
	wsAttribute = L"align=\"right\"";
	std::swprintf(pbuf, _countof(pbuf), L" font=\"%d\"", g_ifontMSYH14);
	wsAttribute += pbuf;
	std::swprintf(pbuf, _countof(pbuf), L" textcolor=\"#%08X\"", ARGB_LABEL_TEXT);
	wsAttribute += pbuf;
	resManager.addAttributeList(L"Label", wsAttribute);
	std::swprintf(pbuf, _countof(pbuf), L" width=\"%d\" height=\"%d\"", LABEL_WIDTH, LABEL_HEIGHT);
	wsAttribute += pbuf;
	resManager.addAttributeList(L"LabelFixed", wsAttribute);
	return fails == 0;
}

void freeResource()
{
	__try
	{
		wuc::drawManager::getResource().free();
	}
	__except(gtc::app::sehUnhandleFilterDump(GetExceptionInformation()))
	{
		//bSEH = true;
	}
}
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	HRESULT hRes = S_FALSE;
	//hRes = ::CoInitialize(NULL);
	hRes = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	// If you are running on NT 4.0 or higher you can use the following call instead to 
	// make the EXE free threaded. This means that calls come in on a random RPC thread.
	//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hRes));
	hRes = ::OleInitialize(NULL);
	assert(SUCCEEDED(hRes));
	gtc::app::MAIN_THREAD_ID = GetCurrentThreadId();
	SEND_LOG(L"�����߳�id��%d", GetCurrentThreadId());
	gtc::app::init(L"scales.ini", L"sclg", L"scdm", L"data", true);
	::SetUnhandledExceptionFilter(gtc::app::sehUnhandleFilterDump);

	HANDLE hMutex = CreateMutex(NULL, FALSE, SCALES_MUTEX_ONLY);
	if(hMutex && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		// ��������ʾ����
		HWND hwnd = FindWindow(SCALES_WND_MAIN_CLASS, SCALES_WND_MAIN_CAPTION);
		RECT rcPos = {0};
		GetWindowRect(hwnd, &rcPos);
		int x = rcPos.left;
		int y = rcPos.top;
		if(x < 0) x = 0;
		if(y < 0) y = 0;
		SetWindowPos(hwnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW);
		SEND_LOG(L"KPSglwEx ��������ͻ����Ѿ������У�mutex:%08X wnd:%08X", hMutex, hwnd);
		CloseHandle(hMutex);
		::OleUninitialize();
		::CoUninitialize();
		return 0;
	}


	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	//::DefWindowProc(NULL, 0, 0, 0L);

	INITCOMMONCONTROLSEX iccx = { sizeof(INITCOMMONCONTROLSEX), ICC_WIN95_CLASSES };
	BOOL bRet = ::InitCommonControlsEx(&iccx);

	// 
	wuc::drawManager::setInstance(hInstance);
	// GDI+��ʼ��
	if (!gtc::resourceManager::initEnvironment((HMODULE)hInstance))
	{
		SEND_LOG(L"ERROR KPSglwEx GDI+ ��ʼ��ʧ�ܣ�");
		return 0;
	}
	// 
	if (!loadResource())
	{
		SEND_LOG(L"ERROR KPSglwEx ������Դʧ�ܣ�");
		return 0;
	}


	//wuc::wkeWebkitUC::wkeWebkit_Init();
	int nRet = Run(nCmdShow);

	//wuc::wkeWebkitUC::wkeWebkit_Shutdown();
	// �ͷ�ͼƬ����Դ
	freeResource();
	// �˳�GDI+
	gtc::resourceManager::releaseEnvironment();

	::OleUninitialize();
	::CoUninitialize();
	SEND_LOG(L"WARNING scales.exe �˳�����������������������������");
	return nRet;
}

