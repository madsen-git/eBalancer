#include "StdAfx.h"
#include "mainDialog.h"

/*template<> */mainDialog* gtc::singleton<mainDialog>::m_pSingleton = nullptr;

mainDialog::mainDialog(void)
	: m_pdb(nullptr), m_bSupportLogs(true)
	, m_labelCaption(nullptr), m_buttonMinbox(nullptr), m_buttonClose(nullptr)
	, m_tabClient(nullptr), m_viewScales(nullptr), m_viewMaterials(nullptr)
	, m_optionScales(nullptr), m_optionMaterials(nullptr)
{
	
}


mainDialog::~mainDialog(void)
{
}



/*static*/ mainDialog& mainDialog::getSingleton()
{
	assert(m_pSingleton);
	return *m_pSingleton;
}
/*static*/ mainDialog* mainDialog::getSingletonPtr()
{
	return m_pSingleton;
}

LRESULT mainDialog::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	bool bHandled = true;


	switch (uMsg)
	{
	case WME_GOTOMAIN:
		{
			uint8_t *pdata = (uint8_t *)wParam;
			uint32_t len = (uint32_t)lParam;
			if (pdata && len >=5)
			{
				m_funcRecv(pdata, len);
			}
			delete pdata;
		}
		break;
	case WM_DESTROY:
		{

			// 保存主窗口位置
			RECT rcPos = {0};
			GetWindowRect(m_hWnd, &rcPos);
			gtc::app::writeProfile(L"MAIN", L"MAINPOS", rcPos);

			//if(nc_dlgBackgroupWindow.isWindow()) nc_dlgBackgroupWindow.showWindow(SW_HIDE);
			PostQuitMessage(0);
		}
		break;
	case WM_CREATE:
		lRes = onCreate();
		SEND_LOG(L"主窗口线程id：%d", GetCurrentThreadId());
		break;
	case WM_NCHITTEST:
		{
			lRes = onNcHitTest(uMsg, wParam, lParam, bHandled);
		}
		break;
	default:
		bHandled = false;
		break;
	}
	//if(!bHandled && !wuc::windowWnd::isOutMsg(uMsg))
	//SEND_LOG(L"dialogMain::handleMessage msg:%s wp:%08X lp:%08X", wuc::windowWnd::stringMsg(uMsg).c_str(), wParam, lParam);

	if(bHandled) return lRes;
	if(m_drawManager.messageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return windowWnd::handleMessage(uMsg, wParam, lParam);

}
void mainDialog::onFinalMessage(HWND hWnd)
{
	__super::onFinalMessage(hWnd);
}
void mainDialog::notify(wuc::BSNotifyUC& msg)
{
	if(!msg.pSender) return;
	if (msg.wsType == L"click")
	{
		std::wstring wsname = msg.pSender->getName();
		if (msg.pSender == m_buttonClose)
		{
			onClickClose();
		}
		else if (msg.pSender == m_buttonMinbox)
		{
			onClickMinbox();
		}
		else if (msg.pSender == m_optionScales)
		{
			onClickTabScales();
		}
		else if (msg.pSender == m_optionMaterials)
		{
			onClickTabMaterials();
		}
	}
	else if (msg.wsType == L"windowinit")
	{
		onInit();
	}
}
void mainDialog::showWindow(bool bShow /*= true*/, bool bTakeFocus /*= true*/)
{
	__super::showWindow(bShow, bTakeFocus);
	if (bShow)
	{
		m_drawManager.restoreLayerWindowControlAttr();
	}
	else
	{
		m_drawManager.recordLayerWindowControlAttr();
		m_drawManager.showLayerWindowControl(bShow);
	}
}

bool mainDialog::openDatabase()
{
	bool bRet = false;
	try
	{
		m_pdb = new db::sqlite::sqliteDB();
		if(!m_pdb) GTC_EXCEPT_DEFAULT(L"分配内存失败", 0);
		
		std::wstring path = gtc::app::getConfigPath();
		path += L"\\scales.db";
		bRet = m_pdb->open(path, false);	
	}
	catch (gtc::baseException &e)
	{
		bRet = false;
		SEND_LOG(L"ERROR mainDialog::openDatabase %s", e.description().c_str());
	#if !defined(_DEBUG)
		WRITE_LOG(L"throw mainDialog::openDatabase %s", e.description().c_str());
	#endif
	}
	catch (...)
	{
		bRet = false;
		SEND_LASTERROR();
	}

	// 
	if (!bRet && m_pdb)
	{
		delete m_pdb;
		m_pdb = nullptr;
	}
	return bRet;
}

db::database* mainDialog::getDatabase()
{
	assert(m_pdb);
	return m_pdb;
}

LRESULT mainDialog::onCreate()
{
	openDatabase();
	// 设置本窗口
	const SIZE szRoundCorner = {MAIN_WND_DETECTION_LEFT, MAIN_WND_DETECTION_TOP};
	m_drawManager.init(m_hWnd);
	m_drawManager.setBackgroundTransparent(true);
	m_drawManager.setLayerWindowTransparent(true);
	m_drawManager.setRoundCorner(MAIN_WND_ROUNDCORNER_CX, MAIN_WND_ROUNDCORNER_CY);
	m_drawManager.setMinInfo(MAIN_WND_MIN_CX, MAIN_WND_MIN_CY);
	m_drawManager.setCaptionRect(MAIN_WND_CAPTION_CY+MAIN_WND_DETECTION_TOP);
	RECT rcSizeBox = {MAIN_WND_DETECTION_LEFT, MAIN_WND_DETECTION_TOP, MAIN_WND_DETECTION_RIGHT, MAIN_WND_DETECTION_BOTTOM};
	m_drawManager.setSizeBox(rcSizeBox);
	m_drawManager.setDefaultFontColor(ARGB_DEFAULT_TEXT);
	// 加载控件
	wuc::verticalLayoutUC *pRoot = new wuc::verticalLayoutUC();
	assert(pRoot);
	m_drawManager.attachDialog(pRoot);
	m_drawManager.addNotifier(this);

	pRoot->setName(L"rootBk");

	SIZE sz = {0};

	pRoot->setImage(L"main_bk");
	pRoot->setBkImage(0,0, 52, 183);
	// 预留4个阴影边
	pRoot->setInset(MAIN_WND_DETECTION_LEFT, MAIN_WND_DETECTION_TOP, MAIN_WND_DETECTION_RIGHT, MAIN_WND_DETECTION_BOTTOM);

	wuc::verticalLayoutUC *pver = nullptr;
	wuc::horizontalLayoutUC *phor = nullptr;
	wuc::containerUC *pContainer = nullptr;
	wuc::containerUC *pContainer2 = nullptr;
	wuc::containerUC *pTitle = nullptr, *pCaption = nullptr;
	wuc::containerUC *pClient = nullptr;
	wuc::labelUC *pLabel = nullptr;
	// 标题栏
	phor = new wuc::horizontalLayoutUC();
	pRoot->add(phor);
	phor->setFixedHeight(MAIN_WND_CAPTION_CY);
	pver = new wuc::verticalLayoutUC();
	//pver->setBkColor(0xFE33CC66);
	phor->add(pver);
	pTitle = pver;
	// 标题栏 名称
	phor = new wuc::horizontalLayoutUC();
	pTitle->add(phor);
	pCaption = phor;
	phor->setFixedHeight(MAIN_WND_CAPTION_TEXT_CY);
	// 标题栏 名称，左空隙
	pver = new wuc::verticalLayoutUC();
	pCaption->add(pver);
	pver->setFixedWidth(MAIN_WND_CAPTION_TEXT_LSPACE);
	// 标题栏 名称，caption
	pver = new wuc::verticalLayoutUC();
	pCaption->add(pver);
	phor = new wuc::horizontalLayoutUC();
	pver->add(phor);
	m_labelCaption = new wuc::labelUC();
	phor->add(m_labelCaption);
	//m_labelCaption->setBkColor(0xFE0066CC);
	m_labelCaption->setFont(g_ifontMSYH16);
	//m_labelCaption->setTextStyle(DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	m_labelCaption->setText(L"电子秤");
	// 标题栏 名称，最小化、关闭按钮
	sz.cx = 24;	// 最小化
	sz.cy = 25;	// = MAIN_WND_CAPTION_TEXT_CY
	pver = new wuc::verticalLayoutUC();
	pCaption->add(pver);
	pver->setFixedWidth(sz.cx);
	phor = new wuc::horizontalLayoutUC();
	pver->add(phor);
	m_buttonMinbox = new wuc::buttonUC();
	phor->add(m_buttonMinbox);
	m_buttonMinbox->setImage(L"button_minbox");
	m_buttonMinbox->setNormalImage(sliceAreaCell(0, sz, false));
	m_buttonMinbox->setHotImage(sliceAreaCell(1, sz, false));
	m_buttonMinbox->setPushedImage(sliceAreaCell(2, sz, false));
	sz.cx = 29;	// 关闭
	pver = new wuc::verticalLayoutUC();
	pCaption->add(pver);
	pver->setFixedWidth(sz.cx);
	phor = new wuc::horizontalLayoutUC();
	pver->add(phor);
	m_buttonClose = new wuc::buttonUC();
	phor->add(m_buttonClose);
	m_buttonClose->setImage(L"button_close");
	m_buttonClose->setNormalImage(sliceAreaCell(0, sz, false));
	m_buttonClose->setHotImage(sliceAreaCell(1, sz, false));
	m_buttonClose->setPushedImage(sliceAreaCell(2, sz, false));
	// 标题栏 名称，右空隙
	if (MAIN_WND_CAPTION_TEXT_RSPACE > 0)
	{
		pver = new wuc::verticalLayoutUC();
		pCaption->add(pver);
		pver->setFixedWidth(MAIN_WND_CAPTION_TEXT_RSPACE);
	}
	// 标题栏 命令区
	phor = new wuc::horizontalLayoutUC();
	pTitle->add(phor);
	//phor->setBkColor(0xFE9966FF);
	// 客户区
	phor = new wuc::horizontalLayoutUC();
	pRoot->add(phor);
	pver = new wuc::verticalLayoutUC();
	phor->add(pver);
	pClient = pver;
	// 客户区	任务栏
	phor = new wuc::horizontalLayoutUC();
	pClient->add(phor);
	phor->setFixedHeight(MAIN_WND_TASK_TAB_CY);
	phor->setBkColor(0xFEFF6565);
	pContainer = phor;
	//			
	pver = new wuc::verticalLayoutUC();
	pContainer->add(pver);
	pver->setFixedWidth(MAIN_WND_TASK_TAB_ITEM_CX);
	phor = new wuc::horizontalLayoutUC();
	pver->add(phor);
	m_optionScales = new wuc::optionUC();
	phor->add(m_optionScales);
	m_optionScales->setGroup(GROUP_MAIN_TASK);
	m_optionScales->setText(L"秤重");
	m_optionScales->setFont(g_ifontMSYH14);
	m_optionScales->setImage(L"main_option_bk");
	m_optionScales->setHotImage(sliceAreaCell(0, 20, false, 2));
	m_optionScales->setSelectedImage(sliceAreaCell(1, 20, false, 2));
	
	//			
	pver = new wuc::verticalLayoutUC();
	pContainer->add(pver);
	pver->setFixedWidth(MAIN_WND_TASK_TAB_ITEM_CX);
	phor = new wuc::horizontalLayoutUC();
	pver->add(phor);
	m_optionMaterials = new wuc::optionUC();
	phor->add(m_optionMaterials);
	m_optionMaterials->setGroup(GROUP_MAIN_TASK);
	m_optionMaterials->setText(L"物料");
	m_optionMaterials->setFont(g_ifontMSYH14);
	m_optionMaterials->setImage(L"main_option_bk");
	m_optionMaterials->setHotImage(sliceAreaCell(0, 20, false, 2));
	m_optionMaterials->setSelectedImage(sliceAreaCell(1, 20, false, 2));
	// 客户区	任务栏
	phor = new wuc::horizontalLayoutUC();
	pClient->add(phor);
	m_tabClient = new wuc::tabLayoutUC();
	phor->add(m_tabClient);
	//m_tabClient->setBkColor(0xFE009999);
	// 客户区	视图 秤重
	m_viewScales = new wuc::viewScales();
	m_funcRecv = std::bind(&wuc::viewScales::onRecvData, m_viewScales, std::placeholders::_1, std::placeholders::_2);
	m_tabClient->add(m_viewScales);
	m_viewScales->setBkColor(ARGB_VIEW_SCALES_BK);
	m_viewMaterials = new wuc::viewMaterials();
	m_tabClient->add(m_viewMaterials);
	m_viewMaterials->setBkColor(0xFE66CCFF);
	// 状态栏
	phor = new wuc::horizontalLayoutUC();
	pRoot->add(phor);
	phor->setFixedHeight(MAIN_WND_STATUS_CY);
	//phor->setBkColor(0xFECC6699);

	// 
	switchToView(tabFlagScales);
	// 
	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW/*|SWP_DRAWFRAME*/);
	InvalidateRect(m_hWnd, NULL, TRUE);
	return 0;
}
LRESULT mainDialog::onNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
	m_drawManager.screenToClient(pt);
	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	if( !::IsZoomed(*this) ) {
		RECT rcSizeBox = m_drawManager.getSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}
	RECT rcCaption = m_drawManager.getCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			wuc::controlUC* pControl = m_drawManager.findControl(pt);
			if( pControl 
				&& _tcscmp(pControl->getClass(), _T("buttonUC")) != 0 
				&& _tcscmp(pControl->getClass(), _T("usericonUC")) != 0)
			{
				return HTCAPTION;
			}
	}
	return HTCLIENT;
}
void mainDialog::onInit()
{
	IntSetType set = dev::serialPort::enumPort(false);
	int s;
	s = 0; 
	
}

bool mainDialog::create()
{
	//if(!nc_dlgBackgroupWindow.create())
	//{
	//	return false;
	//}
	// 宽度固定，高度可变，但不能小于KPSGLW_WND_MAIN_HEIGHT
	RECT rcWork = {0};  
	SystemParametersInfo(SPI_GETWORKAREA, NULL, (PVOID)&rcWork, 0);  
	//SIZE sss = {GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN)};
	SIZE szWall = {rectWidth(rcWork), rectHeight(rcWork)};
	RECT rcPos = {-1,-1,-1,-1};
	bool bl = gtc::app::readProfileRect(L"MAIN", L"MAINPOS", rcPos);
	if (bl)
	{
		if(rectWidth(rcPos) < MAIN_WND_MIN_CX) rcPos.right = rcPos.left + MAIN_WND_MIN_CX;
		if(rectHeight(rcPos) < MAIN_WND_MIN_CY) rcPos.bottom = rcPos.top + MAIN_WND_MIN_CY;

		// 
		if (rcPos.top < 0)
		{
			::OffsetRect(&rcPos, 0, - rcPos.bottom);
		}
		if (rcPos.left < 0)
		{
			::OffsetRect(&rcPos, -rcPos.left, 0);
		}
		if (rcPos.right >= rcWork.right)
		{
			::OffsetRect(&rcPos, rcWork.right - rcPos.right, 0);
		}
	}
	else
	{
		SIZE szWnd = {MAIN_WND_MIN_CX*1.3, MAIN_WND_MIN_CY*1.6};
		if (rcPos.left < 0 || rcPos.left >= szWall.cx)
		{
			rcPos.left = (szWall.cx - szWnd.cx)/2;
			rcPos.right = rcPos.left + szWnd.cx;
		}
		if(rectWidth(rcPos) < szWnd.cx) rcPos.right = rcPos.left + szWnd.cx;
		if (rcPos.right > szWall.cx)
		{
			::OffsetRect(&rcPos, szWall.cx-rcPos.right, 0);
		}
		if (rcPos.top < 0)
		{
			rcPos.top = (szWall.cy - szWnd.cy)/2;
			rcPos.bottom = rcPos.top + szWnd.cy;
		}
		if(rectHeight(rcPos) < szWnd.cy) rcPos.bottom = rcPos.top + szWnd.cy;
		if (rcPos.bottom > szWall.cy)
		{
			::OffsetRect(&rcPos, 0, szWall.cy - rcPos.bottom);
		}

	}
	HWND hWnd = __super::create(NULL/*nc_dlgBackgroupWindow*/, rcPos, SCALES_WND_MAIN_CAPTION);
	if (!hWnd)
	{
		//nc_dlgBackgroupWindow.close();

	}
	else
	{

		// 低权限app往高权限app发送消息要注册
		//if(!gtc::app::addFilterWindowMessage(m_hWnd, gc_uiKPDesktopExit))
		//{
		//	SEND_LOG(L"ERROR dialogMain::create 过滤自定义消息<gc_uiKPDesktopExit> lasterr:%u", GetLastError());
		//}
		//if(!gtc::app::addFilterWindowMessage(m_hWnd, gc_uiKPDesktopRun))
		//{
		//	SEND_LOG(L"ERROR dialogMain::create 过滤自定义消息<gc_uiKPDesktopRun> lasterr:%u", GetLastError());
		//}
		//gtc::app::addFilterWindowMessage(m_hWnd, KPDESK_WMME_WALLPAPER);
		//gtc::app::addFilterWindowMessage(m_hWnd, KPDESK_WMME_DESK_CHANGENOTIFY);
		//gtc::app::addFilterWindowMessage(m_hWnd, KPDESK_WMME_ADDNODE);
		//gtc::app::addFilterWindowMessage(m_hWnd, KPDESK_WMME_DELETENODE);
		//gtc::app::addFilterWindowMessage(m_hWnd, KPDESK_WMME_UPDATENODE);


	}
	assert(hWnd);

	return hWnd != NULL;
}

void mainDialog::onClickMinbox()
{

}
void mainDialog::onClickClose()
{
	close(wuc::BEAnswerStateOK);
}
void mainDialog::onClickTabScales()
{
	assert(m_tabClient);
	switchToView(tabFlagScales);
}
void mainDialog::onClickTabMaterials()
{
	assert(m_tabClient);
	switchToView(tabFlagMaterials);
}
void mainDialog::setCaption(PCWSTR pText)
{
	assert(pText);
	assert(m_labelCaption);
	if(!pText || !m_labelCaption) return;
	m_labelCaption->setText(pText);
}

void mainDialog::switchToView(_BETabFlag tabFlag)
{
	switch (tabFlag)
	{
	case tabFlagScales:
		m_optionScales->selected(true);
		m_tabClient->selectItem(0);
		break;
	case tabFlagMaterials:
		m_optionMaterials->selected(true);
		m_tabClient->selectItem(1);
		break;
	}
}