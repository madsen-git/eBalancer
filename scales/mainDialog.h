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
	//	������ʽ	2015-1-19
	virtual PCWSTR getWindowClassName() const {		return SCALES_WND_MAIN_CLASS;					}
	virtual PCWSTR getWindowCaption() const {		return SCALES_WND_MAIN_CAPTION;					}
	virtual UINT getClassStyle() const {			return CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS;		}
	virtual UINT getWindowStyle() const {			return WS_POPUP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN;}
	//virtual UINT getWindowExStyle() const {			return WS_EX_TOOLWINDOW|WS_EX_TOPMOST;			}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-1-20	���WUC::handleMessage����bHandled�������������ж��û��Ƿ�����˶���Ϣ����
	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void onFinalMessage(HWND hWnd);
	virtual void notify(wuc::BSNotifyUC& msg);
	virtual void showWindow(bool bShow = true, bool bTakeFocus = true);

public:
	// ��Ϣ
	LRESULT onCreate();
	LRESULT onNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	void onInit();
	void onClickMinbox();
	void onClickClose();
	void onClickTabScales();
	void onClickTabMaterials();
public:
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-7-24 ����������
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
	bool					m_bSupportLogs;		// �Ƿ�֧����־���
	// ������
	wuc::labelUC			*m_labelCaption;	// ����
	wuc::buttonUC			*m_buttonMinbox;	// ��С��
	wuc::buttonUC			*m_buttonClose;		// �ر�
	// �ͻ���
	wuc::optionUC			*m_optionScales;	// ����
	wuc::optionUC			*m_optionMaterials;	// ����

	wuc::tabLayoutUC		*m_tabClient;		// �ͻ���ͼ����
	wuc::viewScales			*m_viewScales;		// ��ͼ ����
	wuc::viewMaterials		*m_viewMaterials;	// ��ͼ ����
	// ״̬��

	FUNC_SCALS m_funcRecv;
};

