#pragma once
#include <functional>
typedef std::function<void (uint8_t *, uint32_t)>	FUNC_SCALS;
class mainDialog : public wuc::windowWnd, public wuc::INotifyUC, public gtc::singleton<mainDialog>
{
public:
	enum _BETabFlag{
		tabFlagScales		= 0,
		tabFlagMaterials
	};

public:
	mainDialog(void);
	~mainDialog(void);

	static mainDialog& getSingleton();
	static mainDialog* getSingletonPtr();

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	窗口样式	2015-1-19
	virtual PCWSTR getWindowClassName() const {		return SCALES_WND_MAIN_CLASS;					}
	virtual PCWSTR getWindowCaption() const {		return SCALES_WND_MAIN_CAPTION;					}
	virtual UINT getClassStyle() const {			return CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS;		}
	virtual UINT getWindowStyle() const {			return WS_POPUP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN;}
	//virtual UINT getWindowExStyle() const {			return WS_EX_TOOLWINDOW|WS_EX_TOPMOST;			}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-1-20	相对WUC::handleMessage多了bHandled参数，其用来判断用户是否接收了对消息处理
	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void onFinalMessage(HWND hWnd);
	virtual void notify(wuc::BSNotifyUC& msg);
	virtual void showWindow(bool bShow = true, bool bTakeFocus = true);

public:
	// 消息
	LRESULT onCreate();
	LRESULT onNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	void onInit();
	void onClickMinbox();
	void onClickClose();
	void onClickTabScales();
	void onClickTabMaterials();
public:
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-7-24 创建本窗体
	bool create();
	void setCaption(PCWSTR pText);
	void switchToView(_BETabFlag tabFlag);

	// 
	bool openDatabase();
	db::database* getDatabase();
public:
	bool supportLogs() const {							return m_bSupportLogs;						}
private:
	wuc::drawManager		m_drawManager;
	db::sqlite::sqliteDB	*m_pdb;				// 
	bool					m_bSupportLogs;		// 是否支持日志输出
	// 标题区
	wuc::labelUC			*m_labelCaption;	// 标题
	wuc::buttonUC			*m_buttonMinbox;	// 最小化
	wuc::buttonUC			*m_buttonClose;		// 关闭
	// 客户区
	wuc::optionUC			*m_optionScales;	// 秤重
	wuc::optionUC			*m_optionMaterials;	// 物料

	wuc::tabLayoutUC		*m_tabClient;		// 客户视图容器
	wuc::viewScales			*m_viewScales;		// 视图 秤重
	wuc::viewMaterials		*m_viewMaterials;	// 视图 物料
	// 状态区

	FUNC_SCALS m_funcRecv;
};

