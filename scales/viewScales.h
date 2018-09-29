#pragma once
#include "device/port.h"
#include "scalestype.h"

namespace wuc {
class logItemElementUC : public listContainerElementUC
{
public:
	logItemElementUC();
	~logItemElementUC();

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual UINT getControlFlags() const;
	virtual void Event(BSEventUC& event);
	virtual SIZE estimateSize(SIZE szAvailable);

public:
	// IListItemUC
	virtual void drawItem(HDC hdc, const RECT& rcItem);
	virtual void drawItemBk(HDC hdc, const RECT& rcItem);
public:
	//void setOwner(listUC *pList);
	void setFlag(std::wstring flag) {		m_flag = flag;						}
	void setMSV(std::wstring msv) {			m_msv = msv;						}
	void setInfo(std::wstring info) {		m_info = info;						}
protected:
	gtc::datetime	m_dt;
	std::wstring	m_flag;
	std::wstring	m_msv;
	std::wstring	m_info;
};


class viewScales : public verticalLayoutUC, public INotifyUC
{
public:
	viewScales(void);
	~viewScales(void);

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual UINT getControlFlags() const;

	virtual void Event(BSEventUC &event);
	virtual void notify(wuc::BSNotifyUC& msg);
	virtual void onInit();

public:
	// 启动或停止秤重
	void onClickRunOrStop();
	void onClickClearLog();
public:
	void refreshMaterials();
	void onRecvData(const uint8_t *pdata, uint32_t dataLen);
	void addLog(std::wstring doflag, std::wstring msv, std::wstring info);
private:
	static unsigned int WINAPI doMeasureValue(LPVOID pParam);
private:
	void write();
	bool read();
private:
	comboUC		*m_comboPort;
	comboUC		*m_comboMaterials;
	editUC		*m_editWeightPack;		// 
	editUC		*m_editMarginError;		// 
	editUC		*m_editPackCount;		// 

	editUC		*m_editPower0;			// 调功板 初始输出（0x0最大  0xFF关闭）  格式：OUT1(上);OUT2(下)
	editUC		*m_editPower1;			// 50%目标重时的输出
	editUC		*m_editPower2;			// 80%目标重时的输出

	buttonUC	*m_buttonRun;			// 执行
	listUC		*m_lstLog;

	buttonUC	*m_buttonClear;

	dev::serialPort	m_port;
	volatile LONG	m_lSwitch;

	BSScaleWork		m_swork;
};

}	// wuc
