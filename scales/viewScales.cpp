#include "StdAfx.h"
#include "viewScales.h"
#include "AC/draw/drawer.h"
#include "device/devdef.h"

#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

namespace wuc {

#define SCALES_WORK_WEIGHT		1	// 称重
#define SCALES_WORK_MOTOR		2	// 电机测试
#define SCALES_WORK_EMPTY		3	// 空载测量值
#define SCALES_WORK_FULL		4	// 满载测量值
#define SCALES_WORK_POWER1		5	// 调功板1通道
#define SCALES_WORK_POWER2		6	// 调功板2通道
#define SCALES_WORK_POWER3		7	// 调功板3通道
#define SCALES_WORK_POWER4		8	// 调功板4通道

logItemElementUC::logItemElementUC()
{

}
logItemElementUC::~logItemElementUC()
{

}

PCWSTR logItemElementUC::getClass() const
{
	return L"logItemElementUC";
}
LPVOID logItemElementUC::getInterface(PCWSTR pstrName)
{
	if( _tcscmp(pstrName, L"RemindItemElement") == 0 ) return static_cast<logItemElementUC*>(this);
	return __super::getInterface(pstrName);
}
UINT logItemElementUC::getControlFlags() const
{
	return UCFLAG_TABSTOP | (isEnabled() ? UCFLAG_SETCURSOR : 0);
}
void logItemElementUC::Event(BSEventUC& event)
{
	if( !isMouseEnabled() && event.Type > BEUCEventMouseBegin && event.Type < BEUCEventMouseEnd ) {
		if( m_pParent != NULL ) m_pParent->Event(event);
		else __super::Event(event);
		return;
	}

	if (event.Type == BEUCEventMouseHover)
	{
		SEND_LOG(L"NOTE holidayItemElementUC::Event BEUCEventMouseHover");
		//RECT rc = m_rcItem;
		//_pdialogMainPtr->getRemindHoverUC()->show(m_pholidayInfo, m_rcItem);
	}
	else if (event.Type == BEUCEventMouseLeave )
	{
		SEND_LOG(L"NOTE holidayItemElementUC::Event BEUCEventMouseLeave");
		//wuc::remindHoverUC *pHover = _pdialogMainPtr->getRemindHoverUC();
		//if (pHover && pHover->isVisible() && !pHover->ptIn(event.ptMouse))
		//{
		//	_pdialogMainPtr->getRemindHoverUC()->setVisible(false);
		//}
	}
	__super::Event(event);
}
SIZE logItemElementUC::estimateSize(SIZE szAvailable)
{
	return m_szxyFixed;
}

void logItemElementUC::drawItem(HDC hdc, const RECT& rcItem)
{
	assert(hdc);
	Gdiplus::Graphics grap(hdc);
	RECT rcBk = rcItem;

}

void logItemElementUC::drawItemBk(HDC hdc, const RECT& rcItem)
{
	assert(hdc);
	if(!hdc) return;
	__super::drawItemBk(hdc, rcItem);
	Gdiplus::Graphics grap(hdc);
	gtc::rect rcBk = rcItem;
	// 背景
	//gtc::drawer::fillRectangle(grap, m_dwBackColor, rcBk);

	//int offx = 0, width = 0;
	BSListItemInfo *pItems = m_pOwner->getListInfo();
	if (pItems)
	{
		wchar_t pinfo[40] = {0};
		gtc::rect rcCell;
		// 序号
		std::swprintf(pinfo, _countof(pinfo), L"%d", m_iIndex+1);
		rcCell = pItems->rcColumn[0];
		rcCell.height = rcBk.height;
		rcCell.y = rcBk.y;
		gtc::drawer::drawString(grap, pinfo, rcCell, m_pManager->getFont(g_ifontMSYH12), BEAlignModeAllLeft);
		// 时间
		std::swprintf(pinfo, _countof(pinfo), L"%02d:%02d:%02d", m_dt.hour(), m_dt.minute(), m_dt.second());
		rcCell = pItems->rcColumn[1];
		rcCell.height = rcBk.height;
		rcCell.y = rcBk.y;
		gtc::drawer::drawString(grap, pinfo, rcCell, m_pManager->getFont(g_ifontMSYH12), BEAlignModeAllLeft);
		// 操作
		rcCell = pItems->rcColumn[2];
		rcCell.height = rcBk.height;
		rcCell.y = rcBk.y;
		gtc::drawer::drawString(grap, m_flag.c_str(), rcCell, m_pManager->getFont(g_ifontMSYH12), BEAlignModeAllLeft);
		// 测量
		rcCell = pItems->rcColumn[3];
		rcCell.height = rcBk.height;
		rcCell.y = rcBk.y;
		gtc::drawer::drawString(grap, m_msv.c_str(), rcCell, m_pManager->getFont(g_ifontMSYH12), BEAlignModeAllLeft);
		// 信息
		rcCell = pItems->rcColumn[4];
		rcCell.height = rcBk.height;
		rcCell.y = rcBk.y;
		gtc::drawer::drawString(grap, m_info.c_str(), rcCell, m_pManager->getFont(g_ifontMSYH12), BEAlignModeAllLeft);




		//for (int idx = 0; idx < pItems->iColumns; ++idx)
		//{
		//	gtc::rect rcCell = pItems->rcColumn[idx];
		//	rcCell.height = rcBk.height;
		//	//rcCell.x = offx;
		//	rcCell.y = rcBk.y;
		//	//offx += rcCell.width;

		//	uint8_t rgb = std::rand()%255;
		//	DWORD clr = 0xFE000000 | rgb;
		//	rgb = std::rand()%255;
		//	clr |= rgb << 8;
		//	rgb = std::rand()%255;
		//	clr |= rgb << 16;
		//	//gtc::drawer::fillRectangle(grap, clr, rcCell);

		//	wchar_t pinfo[40] = {0};
		//	std::swprintf(pinfo, 40, L"item%d", idx);
		//	////HFONT hfont = /*getFont()*/;
		//	gtc::drawer::drawString(grap, pinfo, rcCell, m_pManager->getFont(g_ifontMSYH12), BEAlignModeAllLeft);
		//}
	}
	
}

//void logItemElementUC::setOwner(listUC *pList)
//{
//	m_pOwner = pList;
//}

//////////////////////////////////////////////////////////////////////////////////////////
//   类    2016-12-12    madsen
// 
// 
//////////////////////////////////////////////////////////////////////////////////////////
viewScales::viewScales(void)
	: m_comboPort(nullptr), m_comboMaterials(nullptr)
	, m_editWeightPack(nullptr), m_editMarginError(nullptr), m_editPackCount(nullptr)
	, m_editPower0(nullptr), m_editPower1(nullptr)
	, m_buttonRun(nullptr)
	, m_lSwitch(0)
{
	wuc::verticalLayoutUC *pver = nullptr;
	wuc::horizontalLayoutUC *phor = nullptr;
	wuc::containerUC *pContainer = nullptr;
	wuc::containerUC *pContainer2 = nullptr;
	labelUC *pLable = nullptr;

	std::wstring wsAttrName;
	wsAttrName = m_pManager->getDefaultAttributeList(L"LabelFixed");
	wchar_t pbuf[128] = {0};
	std::swprintf(pbuf, _countof(pbuf), L" float=\"true\" fixedxy=\"-10,%d\"", (VIEW_SCALES_INPUT_ITEM_CY-LABEL_HEIGHT)/2-4);
	wsAttrName += pbuf;
	// 编辑区
	const int gpcols = 3;	// 组列
	const int cols = gpcols * 3;
	//const int rows = 9;
	int rowBegin, col;

	IntArrayType vecRow;
	IntArrayType vecCol;
	int rowH = 0;
	vecRow.push_back(VIEW_SCALES_INPUT_ITEM_CY);
	vecRow.push_back(VIEW_SCALES_INPUT_ITEM_CY);
	vecRow.push_back(VIEW_SCALES_INPUT_ITEM_CY);
	rowH += VIEW_SCALES_INPUT_ITEM_CY * 3;
	for (int idx = 0; idx < 3; ++idx)
	{
		vecRow.push_back(VIEW_SCALES_INPUT_ITEM_CY);
		vecRow.push_back(VIEW_SCALES_INPUT_ITEMSPACE_CY);
		rowH += VIEW_SCALES_INPUT_ITEM_CY + VIEW_SCALES_INPUT_ITEMSPACE_CY;
	}
	rowH += 20;
	for (int idx = 0; idx < gpcols; ++idx)
	{
		vecCol.push_back(VIEW_SCALES_INPUT_ITEMNAME_CX);
		vecCol.push_back(VIEW_SCALES_INPUT_ITEMEDIT_CX);
		vecCol.push_back(VIEW_SCALES_INPUT_ITEMUNIT_CX);
	}
	phor = new wuc::horizontalLayoutUC();
	add(phor);
	//phor->setBkColor(0xFE999933);
	phor->setFixedHeight(rowH);

	HContainerArrayType vecCell = wuc::layoutGrid(phor, vecRow, vecCol);// wuc::layoutGrid(phor, rows, cols, VIEW_SCALES_INPUT_ITEM_CY, VIEW_SCALES_INPUT_ITEM_CY, VIEW_SCALES_INPUT_ITEMNAME_CX, VIEW_SCALES_INPUT_ITEMEDIT_CX);
	// 日志
	if (_pdialogMainPtr->supportLogs())
	{
		//phor = new wuc::horizontalLayoutUC();
		//add(phor);
		//pver = new wuc::verticalLayoutUC();
		//phor->add(pver);
		m_lstLog = new listUC();
		//m_lstLog->setBkColor(0xFE00ffff);
		m_lstLog->setAttribute(L"vscrollbar", L"true");
		m_lstLog->setAttribute(L"vscrollbarstyle", L"VScrollBar");
		m_lstLog->getList()->setDriftScroll(true);
		add(m_lstLog);
		m_lstLog->setPadding(20, 0, 20, 0);

		// 列表框列数
		// 只能有1个列标题
		listHeaderUC *pHeader = m_lstLog->getHeader();
		assert(pHeader);
		listHeaderItemUC *pHeaderItem = new wuc::listHeaderItemUC();
		pHeaderItem->setBkColor(0xFEff0000);
		pHeaderItem->setFixedWidth(50);
		m_lstLog->add(pHeaderItem);
		pHeaderItem = new wuc::listHeaderItemUC();
		pHeaderItem->setBkColor(0xFEff0000);
		pHeaderItem->setFixedWidth(80);
		m_lstLog->add(pHeaderItem);
		pHeaderItem = new wuc::listHeaderItemUC();
		pHeaderItem->setBkColor(0xFE00ff00);
		pHeaderItem->setFixedWidth(60);
		m_lstLog->add(pHeaderItem);
		pHeaderItem = new wuc::listHeaderItemUC();
		pHeaderItem->setBkColor(0xFE0000ff);
		pHeaderItem->setFixedWidth(60);
		m_lstLog->add(pHeaderItem);
		pHeaderItem = new wuc::listHeaderItemUC();
		pHeaderItem->setBkColor(0xFE00ffff);
		pHeaderItem->setFixedWidth(560);
		m_lstLog->add(pHeaderItem);
		pHeader->setVisible(false);
		//phor->add(m_lstLog);
		//m_lstLog->setBkColor(0x0ff0000);
		//phor->setFixedHeight(VIEW_SCALES_LOGS_CY);

	}
	// 未行
	phor = new wuc::horizontalLayoutUC();
	add(phor);
	phor->setFixedHeight(VIEW_SCALES_LOGS_BSPACE_CY);
	//phor->setBkColor(0xFECCCC66);
	
	enumSerialPort();
	// 编辑区
	rowBegin = 1 * cols;	// 基于0
	col = 0;
	pLable = new labelUC();
	vecCell[rowBegin + col++]->add(pLable);
	pLable->applyAttributeList(wsAttrName.c_str());
	pLable->setText(L"通讯端口");
	//pLable->setBkColor(0xFE9966FF);
	m_comboPort = new comboUC();
	vecCell[rowBegin + col++]->add(m_comboPort);
	m_comboPort->applyAttributeList(L"Combo");
	m_comboPort->setDropBoxAttributeList(L"vscrollbarstyle=\"VScrollBar\"");
	listLabelElementUC *plstLabel = nullptr;
	IntSetType setPort = dev::serialPort::enumPort(false);
	for (IntSetType::const_iterator iter = setPort.cbegin(); iter != setPort.cend(); ++iter)
	{
		std::swprintf(pbuf, _countof(pbuf), L"COM%d", *iter + 1);
		plstLabel = new listLabelElementUC();
		m_comboPort->add(plstLabel);
		plstLabel->setText(pbuf);
		plstLabel->setTag(*iter);
	}
	++col;	// 跳过单位
	// 物料选择
	pLable = new labelUC();
	vecCell[rowBegin + col++]->add(pLable);
	pLable->applyAttributeList(wsAttrName.c_str());
	pLable->setText(L"工作模式");
	//m_editMaterials = new editUC();
	//vecCell[rowBegin + col++]->add(m_editMaterials);
	//m_editMaterials->applyAttributeList(L"Edit");
	m_comboMaterials = new comboUC();
	vecCell[rowBegin + col++]->add(m_comboMaterials);
	m_comboMaterials->applyAttributeList(L"Combo");
	m_comboMaterials->setDropBoxAttributeList(L"vscrollbarstyle=\"VScrollBar\"");
	//
	plstLabel = new listLabelElementUC();
	m_comboMaterials->add(plstLabel);
	plstLabel->setText(L"称重");
	plstLabel->setTag(SCALES_WORK_WEIGHT);
	plstLabel = new listLabelElementUC();
	m_comboMaterials->add(plstLabel);
	plstLabel->setText(L"电机测试");
	plstLabel->setTag(SCALES_WORK_MOTOR);
	plstLabel = new listLabelElementUC();
	m_comboMaterials->add(plstLabel);
	plstLabel->setText(L"空载值");
	plstLabel->setTag(SCALES_WORK_EMPTY);
	plstLabel = new listLabelElementUC();
	m_comboMaterials->add(plstLabel);
	plstLabel->setText(L"满载值（1kg）");
	plstLabel->setTag(SCALES_WORK_FULL);
	//
	plstLabel = new listLabelElementUC();
	m_comboMaterials->add(plstLabel);
	plstLabel->setText(L"调功板1通道");
	plstLabel->setTag(SCALES_WORK_POWER1);
	plstLabel = new listLabelElementUC();
	m_comboMaterials->add(plstLabel);
	plstLabel->setText(L"调功板2通道");
	plstLabel->setTag(SCALES_WORK_POWER2);
	plstLabel = new listLabelElementUC();
	m_comboMaterials->add(plstLabel);
	plstLabel->setText(L"调功板3通道");
	plstLabel->setTag(SCALES_WORK_POWER3);
	plstLabel = new listLabelElementUC();
	m_comboMaterials->add(plstLabel);
	plstLabel->setText(L"调功板4通道");
	plstLabel->setTag(SCALES_WORK_POWER4);
	// 每包重量
	rowBegin = 3 * cols;
	col = 0;
	pLable = new labelUC();
	vecCell[rowBegin + col++]->add(pLable);
	pLable->applyAttributeList(wsAttrName.c_str());
	pLable->setText(L"每包重量");

	m_editWeightPack = new editUC();
	vecCell[rowBegin + col++]->add(m_editWeightPack);
	m_editWeightPack->applyAttributeList(L"Edit");
	m_editWeightPack->setTextStyle(DT_SINGLELINE | DT_VCENTER | DT_RIGHT);

	pLable = new labelUC();
	vecCell[rowBegin + col++]->add(pLable);
	pLable->applyAttributeList(wsAttrName.c_str());
	pLable->setText(L"克");
	// 允许误差
	pLable = new labelUC();
	vecCell[rowBegin + col++]->add(pLable);
	pLable->applyAttributeList(wsAttrName.c_str());
	pLable->setText(L"允许误差");

	m_editMarginError = new editUC();
	vecCell[rowBegin + col++]->add(m_editMarginError);
	m_editMarginError->applyAttributeList(L"Edit");
	m_editMarginError->setTextStyle(DT_SINGLELINE | DT_VCENTER | DT_RIGHT);

	pLable = new labelUC();
	vecCell[rowBegin + col++]->add(pLable);
	pLable->applyAttributeList(wsAttrName.c_str());
	pLable->setText(L"克");

	// 打包数量
	pLable = new labelUC();
	vecCell[rowBegin + col++]->add(pLable);
	pLable->applyAttributeList(wsAttrName.c_str());
	pLable->setText(L"打包数量");

	m_editPackCount = new editUC();
	vecCell[rowBegin + col++]->add(m_editPackCount);
	m_editPackCount->applyAttributeList(L"Edit");
	m_editPackCount->setTextStyle(DT_SINGLELINE | DT_VCENTER | DT_RIGHT);

	//pLable = new labelUC();
	//vecCell[rowBegin + col++]->add(pLable);
	//pLable->applyAttributeList(wsAttrName.c_str());
	//pLable->setText(L"克");

	// 合格包数
	rowBegin = 5 * cols;
	col = 0;
	pLable = new labelUC();
	vecCell[rowBegin + col++]->add(pLable);
	pLable->applyAttributeList(wsAttrName.c_str());
	//pLable->setText(L"合格包数");
	pLable->setText(L"初始功率");
	m_editPower0 = new editUC();
	vecCell[rowBegin + col++]->add(m_editPower0);
	m_editPower0->applyAttributeList(L"Edit");
	m_editPower0->setTextStyle(DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
	m_editPower0->setText(L"185");
	++col;	// 跳过单位
	// 不合格包数
	//rowBegin = 7 * cols;
	//col = 0;
	pLable = new labelUC();
	vecCell[rowBegin + col++]->add(pLable);
	pLable->applyAttributeList(wsAttrName.c_str());
	//pLable->setText(L"不合格包数");
	pLable->setText(L"第一变功");
	//pLable->setBkColor(0xFE00ff00);
	m_editPower1 = new editUC();
	vecCell[rowBegin + col++]->add(m_editPower1);
	m_editPower1->applyAttributeList(L"Edit");
	m_editPower1->setTextStyle(DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
	m_editPower1->setText(L"205");
	//m_editPower1->setReadOnly(true);
	// 
	++col;
	pLable = new labelUC();
	vecCell[rowBegin + col++]->add(pLable);
	pLable->applyAttributeList(wsAttrName.c_str());
	pLable->setText(L"第二变功");
	//pLable->setBkColor(0xFE00ff00);
	m_editPower2 = new editUC();
	vecCell[rowBegin + col++]->add(m_editPower2);
	m_editPower2->applyAttributeList(L"Edit");
	m_editPower2->setTextStyle(DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
	m_editPower2->setText(L"215");


	// 合格包数
	rowBegin = 7 * cols;
	col = 0;
	m_buttonClear = new buttonUC();
	vecCell[rowBegin + 1]->add(m_buttonClear);
	m_buttonClear->applyAttributeList(L"Button");
	m_buttonClear->setText(L"清空");
	// 运行按钮
	m_buttonRun = new buttonUC();
	col += 7;
	vecCell[rowBegin + col]->add(m_buttonRun);
	m_buttonRun->applyAttributeList(L"Button");
	m_buttonRun->setText(L"启动");
	
	// 读取参数
	if (read())
	{
		wchar_t buf[32] = {0};
		std::swprintf(buf, _countof(buf), L"%d.%d", m_swork.pack.weightPackInteger, m_swork.pack.weightPackDecimal);
		m_editWeightPack->setText(buf);
		std::swprintf(buf, _countof(buf), L"%d.%d", m_swork.pack.marginErrorInteger, m_swork.pack.marginErrorDecimal);
		m_editMarginError->setText(buf);
		std::swprintf(buf, _countof(buf), L"%d", m_swork.pack.packCount);
		m_editPackCount->setText(buf);

	}
	else
	{
		m_swork.scaler.msvHigh = 29082;
		m_swork.scaler.msvLow = 14657;
		m_swork.scaler.logicHigh = 1000;	// 克
		m_swork.scaler.logicLow = 0;
		m_swork.scaler.busface.addressSCR = 2;
		m_swork.scaler.busface.upChannel = 1;
		m_swork.scaler.busface.downChannel = 2;
		m_swork.scaler.busface.addressWeigher = 1;
		// 
		m_swork.pack.weightPackInteger = 10;	// 克
		m_swork.pack.weightPackDecimal = 0;
		m_swork.pack.marginErrorInteger = 0;
		m_swork.pack.marginErrorDecimal = 500;	// 0.25克
		m_swork.pack.packCount = 50;
		m_swork.pack.realCount = 0;
		m_swork.pack.failedCount = 0;
		//
		m_swork.power.feederUpPower0 = 200;
		m_swork.power.feederUpPower1 = 200;
		m_swork.power.feederUpPower2 = 0xFF;		// 0x0最大  0xFF关闭
		m_swork.power.feederUpPer1 = 50;
		m_swork.power.feederUpPer2 = 80;
		m_swork.power.feederDownPower0 = 190;
		m_swork.power.feederDownPower1 = 195;
		m_swork.power.feederDownPower2 = 200;
		m_swork.power.feederDownPer1 = 50;
		m_swork.power.feederDownPer2 = 90;
	}

	//
	wchar_t buf[32] = {0};
	swprintf(buf, sizeof(buf), L"%d;%d", m_swork.power.feederUpPower0, m_swork.power.feederDownPower0);
	m_editPower0->setText(buf);
	swprintf(buf, sizeof(buf), L"%d;%d", m_swork.power.feederUpPower1, m_swork.power.feederDownPower1);
	m_editPower1->setText(buf);
	swprintf(buf, sizeof(buf), L"%d;%d", m_swork.power.feederUpPower2, m_swork.power.feederDownPower2);
	m_editPower2->setText(buf);
}


viewScales::~viewScales(void)
{
}


PCWSTR viewScales::getClass() const
{
	return L"viewScales";
}
LPVOID viewScales::getInterface(PCWSTR pstrName)
{
	if(std::wcscmp(pstrName, L"ViewScales") == 0) return static_cast<viewScales *>(this);
	return __super::getInterface(pstrName);
}
UINT viewScales::getControlFlags() const
{
	return UCFLAG_TABSTOP | (isEnabled() ? UCFLAG_SETCURSOR : 0);
}

void viewScales::Event(BSEventUC &event)
{
	if( !isMouseEnabled() && event.Type > BEUCEventMouseBegin && event.Type < BEUCEventMouseEnd ) {
		if( m_pParent != NULL ) m_pParent->Event(event);
		else __super::Event(event);
		return;
	}

	__super::Event(event);
}

void viewScales::notify(wuc::BSNotifyUC& msg)
{
	if(!msg.pSender) return;
	if (msg.wsType == L"click")
	{
		if (msg.pSender == m_buttonRun)
		{
			onClickRunOrStop();
		}
		else if (msg.pSender == m_buttonClear)
		{
			onClickClearLog();
		}
	}
}
void viewScales::onInit()
{
	//refreshMaterials();
	// m_editPower0,m_editPower1,m_editPower2
	
	// 
	m_buttonRun->setTag(0);
	//m_editPackCount->setText(L"10");
	//m_editWeightPack->setText(L"10");
	//m_editMarginError->setText(L"0.5");
	if (m_pManager)
	{
		m_pManager->addNotifier(this);
	}

}

void viewScales::refreshMaterials()
{
	assert(m_comboMaterials);
	m_comboMaterials->removeAll();

	std::unique_ptr<db::command> pcmd(_pdialogMainPtr->getDatabase()->createCommand());
	std::unique_ptr<db::dataSet> pdata(pcmd->executeReader(L"SELECT FID,FName,FModel FROM tbWMaterial ORDER BY FSort"));
	int32_t id;
	std::wstring wsname, wsmodel;
	wchar_t pbuf[MAX_PATH];
	listLabelElementUC *plstLabel = nullptr;
	if (pdata && !pdata->eof())
	{
		while (!pdata->eof())
		{
			id = pdata->getInt(L"FID");
			wsname = pdata->getString(L"FName");
			wsmodel = pdata->getString(L"FModel");

			assert(id > 0 && !wsname.empty());
			if (id > 0 && !wsname.empty())
			{
				plstLabel = new listLabelElementUC();
				m_comboMaterials->add(plstLabel);
				if (!wsmodel.empty())
				{
					std::swprintf(pbuf, _countof(pbuf), L"%s(%s)", wsname.c_str(), wsmodel.c_str());
					wsname = pbuf;
				}
				plstLabel->setText(wsname.c_str());
				plstLabel->setTag(id);
			}
			pdata->nextRow();
		}
	}
	//listLabelElementUC *plstLabel = nullptr;
	//IntSetType setPort = dev::serialPort::enumPort(false);
	//for (IntSetType::const_iterator iter = setPort.cbegin(); iter != setPort.cend(); ++iter)
	//{
	//	std::swprintf(pbuf, _countof(pbuf), L"COM%d", *iter + 1);
	//	plstLabel = new listLabelElementUC();
	//	m_comboPort->add(plstLabel);
	//	plstLabel->setText(pbuf);
	//}
}

void viewScales::onRecvData(const uint8_t *pdata, uint32_t dataLen)
{
	if (pdata && dataLen >= 5)
	{
		wchar_t msv[16] = {0}, info[64] = {0};
		if (pdata[0] == CMD_RSP_SCALE_ITEM_A && pdata[1] == CMD_RSP_SCALE_ITEM_B)
		{
			if (pdata[2] == sizeof(BSScaleItem))
			{
				BSScaleItem *pitem = (BSScaleItem *)&pdata[3];
			
				std::swprintf(msv, _countof(msv), L"%.3f", pitem->weightInteger + pitem->weightDecimal/1000.0f);
				std::swprintf(info, _countof(info), L"%s", pitem->bOk ? L"成功" : L"失败");
				addLog(L"测量", msv, info);
				//addLog(L"测量", )
				SEND_LOG(L"秤重结果反馈 ok:%d %d.%d", pitem->bOk, pitem->weightInteger, pitem->weightDecimal);
			}
		}
		else if (pdata[0] == CMD_RSP_MOTOR_STOP_A && pdata[1] == CMD_RSP_MOTOR_STOP_B)
		{
			uint32_t *ploops = (uint32_t *)&pdata[3];
			std::swprintf(info, _countof(info), L"%u", *ploops);
			addLog(L"电机测试", L"", info);
		}
		else if (pdata[0] == CMD_RSP_SCALE_EMPTY_A && pdata[1] == CMD_RSP_SCALE_EMPTY_B)
		{
			int32_t *pmsv = (int32_t *)&pdata[3];
			m_swork.scaler.msvLow = *pmsv;
			std::swprintf(info, _countof(info), L"%d", m_swork.scaler.msvLow);
			addLog(L"空载测量值", L"", info);
		}
		else if (pdata[0] == CMD_RSP_SCALE_FULL_A && pdata[1] == CMD_RSP_SCALE_FULL_B)
		{
			int32_t *pmsv = (int32_t *)&pdata[3];
			m_swork.scaler.msvHigh = *pmsv;
			std::swprintf(info, _countof(info), L"%d", m_swork.scaler.msvHigh);
			addLog(L"满载测量值", L"", info);
		}
		else if (pdata[0] == CMD_RSP_SCALE_STATE_A && pdata[1] == CMD_RSP_SCALE_STATE_B)
		{
			char info[255] = {'-',0};
			if (pdata[2] > 0)
			{
				memcpy_s(info, sizeof(info), &pdata[3], pdata[2]);
				info[pdata[2]-1] = '\0';
			}
			addLog(L"状态", msv, gtc::stringUtil::toWChar(info));

			SEND_LOGA("秤重状态反馈 info:%s", info);
		}
		SEND_LOG(L"viewScales::onRecvData..");
	}
}

void viewScales::addLog(std::wstring doflag, std::wstring msv, std::wstring info)
{
	logItemElementUC *log = new logItemElementUC();
	if (m_lstLog->getCount()%2)
	{
		log->setBkColor(0xFEECECEC);
	}
	else
	{
		log->setBkColor(0xFEC0C0C0);
	}
	log->setFixedHeight(22);
	log->setFlag(doflag);
	log->setMSV(msv);
	log->setInfo(info);
	m_lstLog->add(log);
	RECT rc = m_lstLog->getList()->getPosition();
	m_lstLog->getList()->setPosition(rc);
	m_lstLog->endDown();
}

unsigned int WINAPI viewScales::doMeasureValue(LPVOID pParam)
{
	viewScales *scales = (viewScales *)pParam;

	assert(scales);
	uint8_t pbuf[32] = {0};
	int rc = -1;
	while(scales->m_lSwitch == 1)
	{
		rc = scales->m_port.read(pbuf, sizeof(pbuf));
	}
	return 0;
}

void viewScales::write()
{
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<> *root = doc.allocate_node(rapidxml::node_pi, "xml version='1.0' encoding='utf-8'");
	doc.append_node(root);
	rapidxml::xml_node<> *worker = doc.allocate_node(rapidxml::node_element, "work");
	doc.append_node(worker);
	//rapidxml::xml_node<> *materials = doc.allocate_node(rapidxml::node_element, "materials");
	rapidxml::xml_node<> *pack = doc.allocate_node(rapidxml::node_element, "pack");
	rapidxml::xml_node<> *power = doc.allocate_node(rapidxml::node_element, "power");
	rapidxml::xml_node<> *scaler = doc.allocate_node(rapidxml::node_element, "scaler");
	//worker->append_node(materials);
	worker->append_node(pack);
	worker->append_node(power);
	worker->append_node(scaler);
	//
	char buf[64] = {0};
	rapidxml::xml_node<> *node = nullptr;
	//// materials
	//node = materials; //doc.allocate_node(rapidxml::node_element, "weight");
	////materials->append_node(node);
	////std::sprintf(buf, "%d", m_swork.pack.weightPackInteger);
	//node->append_attribute(doc.allocate_attribute("weight", doc.allocate_string(gtc::stringUtil::toChar(m_editWeightPack->getText()).data())));
	//node->append_attribute(doc.allocate_attribute("margin", doc.allocate_string(gtc::stringUtil::toChar(m_editMarginError->getText()).data())));
	//node->append_attribute(doc.allocate_attribute("count", doc.allocate_string(gtc::stringUtil::toChar(m_editPackCount->getText()).data())));
	// weight
	node = doc.allocate_node(rapidxml::node_element, "weight");
	pack->append_node(node);
	std::sprintf(buf, "%d", m_swork.pack.weightPackInteger);
	node->append_attribute(doc.allocate_attribute("integer", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.pack.weightPackDecimal);
	node->append_attribute(doc.allocate_attribute("decimal", doc.allocate_string(buf)));
	//
	node = doc.allocate_node(rapidxml::node_element, "margin");
	pack->append_node(node);
	std::sprintf(buf, "%d", m_swork.pack.marginErrorInteger);
	node->append_attribute(doc.allocate_attribute("integer", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.pack.marginErrorDecimal);
	node->append_attribute(doc.allocate_attribute("decimal", doc.allocate_string(buf)));
	//
	node = doc.allocate_node(rapidxml::node_element, "count");
	pack->append_node(node);
	std::sprintf(buf, "%d", m_swork.pack.packCount);
	node->append_attribute(doc.allocate_attribute("pack", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.pack.realCount);
	node->append_attribute(doc.allocate_attribute("real", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.pack.failedCount);
	node->append_attribute(doc.allocate_attribute("failed", doc.allocate_string(buf)));
	// power
	node = doc.allocate_node(rapidxml::node_element, "up");
	power->append_node(node);
	std::sprintf(buf, "%d", m_swork.power.feederUpPower0);
	node->append_attribute(doc.allocate_attribute("power0", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.power.feederUpPer1);
	node->append_attribute(doc.allocate_attribute("per1", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.power.feederUpPower1);
	node->append_attribute(doc.allocate_attribute("power1", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.power.feederUpPer2);
	node->append_attribute(doc.allocate_attribute("per2", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.power.feederUpPower2);
	node->append_attribute(doc.allocate_attribute("power2", doc.allocate_string(buf)));
	// 
	node = doc.allocate_node(rapidxml::node_element, "down");
	power->append_node(node);
	std::sprintf(buf, "%d", m_swork.power.feederDownPower0);
	node->append_attribute(doc.allocate_attribute("power0", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.power.feederDownPer1);
	node->append_attribute(doc.allocate_attribute("per1", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.power.feederDownPower1);
	node->append_attribute(doc.allocate_attribute("power1", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.power.feederDownPer2);
	node->append_attribute(doc.allocate_attribute("per2", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.power.feederDownPower2);
	node->append_attribute(doc.allocate_attribute("power2", doc.allocate_string(buf)));
	// scaler
	node = doc.allocate_node(rapidxml::node_element, "logic");
	scaler->append_node(node);
	std::sprintf(buf, "%d", m_swork.scaler.logicHigh);
	node->append_attribute(doc.allocate_attribute("high", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.scaler.logicLow);
	node->append_attribute(doc.allocate_attribute("low", doc.allocate_string(buf)));
	//
	node = doc.allocate_node(rapidxml::node_element, "msv");
	scaler->append_node(node);
	std::sprintf(buf, "%d", m_swork.scaler.msvHigh);
	node->append_attribute(doc.allocate_attribute("high", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.scaler.msvLow);
	node->append_attribute(doc.allocate_attribute("low", doc.allocate_string(buf)));
	//
	node = doc.allocate_node(rapidxml::node_element, "busface");
	scaler->append_node(node);
	std::sprintf(buf, "%d", m_swork.scaler.busface.addressSCR);
	if(m_swork.scaler.busface.addressSCR == 0)
	{
		int s = 0;
	}
	node->append_attribute(doc.allocate_attribute("addr_scr", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.scaler.busface.addressWeigher);
	node->append_attribute(doc.allocate_attribute("addr_weigher", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.scaler.busface.upChannel);
	node->append_attribute(doc.allocate_attribute("channel_up", doc.allocate_string(buf)));
	std::sprintf(buf, "%d", m_swork.scaler.busface.downChannel);
	node->append_attribute(doc.allocate_attribute("channel_down", doc.allocate_string(buf)));
	std::ofstream out("worker.xml");
	out << doc;
}

static char* getAttribute(rapidxml::xml_node<> *node, char *attrName)
{
	assert(node && attrName);
	rapidxml::xml_attribute<> *attr = node->first_attribute();
	while(attr)
	{
		if (std::strcmp(attr->name(), attrName) == 0)
		{
			return attr->value();
		}
		attr = attr->next_attribute();
	}
	return nullptr;
}
bool viewScales::read()
{
	bool bret = false;
	try
	{
		rapidxml::xml_document<> doc;
		rapidxml::file<> fdoc("worker.xml");
		doc.parse<0>(fdoc.data());

		std::string str = doc.name();
		rapidxml::xml_node<> *subnode = nullptr;
		rapidxml::xml_attribute<> *attr;

		rapidxml::xml_node<> *node = doc.first_node()->first_node();
		std::memset(&m_swork, 0, sizeof(m_swork));
		while(node)
		{
			str = node->name();
			if (str == "pack")
			{
				subnode = node->first_node();
				while(subnode)
				{
					str = subnode->name();
					if (str == "weight")
					{
						m_swork.pack.weightPackInteger = std::atoi(getAttribute(subnode, "integer"));
						m_swork.pack.weightPackDecimal = std::atoi(getAttribute(subnode, "decimal"));
					}
					else if (str == "margin")
					{
						m_swork.pack.marginErrorInteger = std::atoi(getAttribute(subnode, "integer"));
						m_swork.pack.marginErrorDecimal = std::atoi(getAttribute(subnode, "decimal"));
					}
					else if (str == "count")
					{
						m_swork.pack.packCount = std::atoi(getAttribute(subnode, "pack"));
						m_swork.pack.realCount = std::atoi(getAttribute(subnode, "real"));
						m_swork.pack.failedCount = std::atoi(getAttribute(subnode, "failed"));
					}
					subnode = subnode->next_sibling();
				}
			}
			else if (str == "power")
			{
				subnode = node->first_node();
				while(subnode)
				{
					str = subnode->name();
					if (str == "up")
					{
						m_swork.power.feederUpPer1 = std::atoi(getAttribute(subnode, "per1"));
						m_swork.power.feederUpPer2 = std::atoi(getAttribute(subnode, "per2"));
						m_swork.power.feederUpPower0 = std::atoi(getAttribute(subnode, "power0"));
						m_swork.power.feederUpPower1 = std::atoi(getAttribute(subnode, "power1"));
						m_swork.power.feederUpPower2 = std::atoi(getAttribute(subnode, "power2"));
					}
					else if (str == "down")
					{
						m_swork.power.feederDownPer1 = std::atoi(getAttribute(subnode, "per1"));
						m_swork.power.feederDownPer2 = std::atoi(getAttribute(subnode, "per2"));
						m_swork.power.feederDownPower0 = std::atoi(getAttribute(subnode, "power0"));
						m_swork.power.feederDownPower1 = std::atoi(getAttribute(subnode, "power1"));
						m_swork.power.feederDownPower2 = std::atoi(getAttribute(subnode, "power2"));
					}
					subnode = subnode->next_sibling();
				}
			}
			else if (str == "scaler")
			{
				subnode = node->first_node();
				while(subnode)
				{
					str = subnode->name();
					if (str == "logic")
					{
						m_swork.scaler.logicHigh = std::atoi(getAttribute(subnode, "high"));
						m_swork.scaler.logicLow = std::atoi(getAttribute(subnode, "low"));
					}
					else if (str == "msv")
					{
						m_swork.scaler.msvHigh = std::atoi(getAttribute(subnode, "high"));
						m_swork.scaler.msvLow = std::atoi(getAttribute(subnode, "low"));
					}
					else if (str == "busface")
					{
						m_swork.scaler.busface.addressSCR = std::atoi(getAttribute(subnode, "addr_scr"));
						m_swork.scaler.busface.addressWeigher = std::atoi(getAttribute(subnode, "addr_weigher"));
						m_swork.scaler.busface.upChannel = std::atoi(getAttribute(subnode, "channel_up"));
						m_swork.scaler.busface.downChannel = std::atoi(getAttribute(subnode, "channel_down"));
					}
					subnode = subnode->next_sibling();
				}
			}
			node = node->next_sibling();
		}
		bret = true;
	}
	catch (...)
	{
		bret = false;
	}	
	return bret;
}

void viewScales::onClickRunOrStop()
{
	if (m_comboMaterials->getCurSel() < 0)
	{
		gtc::app::message(L"请选择合适的工作模式！");
		m_comboMaterials->setFocus();
		return;
	}
	int workflag = m_comboMaterials->getItemAt(m_comboMaterials->getCurSel())->getTag();

	float fltWeightPack = 0.0f;
	float fltMarginError = 0.0f;
	int packCount = std::wcstol(m_editPackCount->getText(), nullptr, 10);
	{
		std::wistringstream istr(m_editWeightPack->getText());
		istr >> fltWeightPack;
	}
	{
		std::wistringstream istr(m_editMarginError->getText());
		istr >> fltMarginError;
	}

	if (m_buttonRun->getTag() == 0)
	{
		if (!m_port.isOpened())
		{
			if (m_comboPort->getCurSel() >= 0)
			{
				int port = m_comboPort->getItemAt(m_comboPort->getCurSel())->getTag();

				m_port.setPort(port);
				m_port.setBaudrate(19200);
				m_port.open();
			}
			else
			{
				gtc::app::message(L"请选择合适的通信端口！");
				m_comboPort->setFocus();
				return;
			}
		}
		if (m_port.isOpened())
		{
			if (workflag == SCALES_WORK_WEIGHT)
			{
				std::wstring wstr = m_editWeightPack->getText();
				m_swork.pack.weightPackInteger = std::wcstol(wstr.c_str(), nullptr, 10);
				m_swork.pack.weightPackDecimal = 0;
				double margin = std::wcstod(m_editMarginError->getText(), nullptr);
				m_swork.pack.marginErrorInteger = margin;
				m_swork.pack.marginErrorDecimal = (margin - int(margin)) * 1000;

				int val1, val2;
				std::swscanf(m_editPower0->getText(), L"%d;%d", &val1, &val2);
				m_swork.power.feederUpPower0 = val1;
				m_swork.power.feederDownPower0 = val2;
				std::swscanf(m_editPower1->getText(), L"%d;%d", &val1, &val2);
				m_swork.power.feederUpPower1 = val1;
				m_swork.power.feederDownPower1 = val2;
				std::swscanf(m_editPower2->getText(), L"%d;%d", &val1, &val2);
				m_swork.power.feederUpPower2 = val1;
				m_swork.power.feederDownPower2 = val2;
				//
				uint32_t len = 3 + sizeof(m_swork) + 2;
				uint8_t *pcmd = new uint8_t[len];
				pcmd[0] = CMD_MASTER_START_A;
				pcmd[1] = CMD_MASTER_START_B;
				pcmd[2] = sizeof(m_swork);
				memcpy_s(&pcmd[3], sizeof(m_swork), &m_swork, sizeof(m_swork));
				pcmd[len - 2] = CMD_CONTROL_END_A;
				pcmd[len - 1] = CMD_CONTROL_END_B;
				m_port.write(pcmd, len);
				//
				delete[] pcmd;

				//
				m_buttonRun->setTag(1);
				m_buttonRun->setText(L"停止");
			}
			else if (workflag == SCALES_WORK_MOTOR)
			{
				uint8_t cmd[5] = {CMD_MOTOR_START_A, CMD_MOTOR_START_B, 0, CMD_CONTROL_END_A, CMD_CONTROL_END_B};
				m_port.write(cmd, sizeof(cmd));
			}
			else if (workflag == SCALES_WORK_EMPTY)
			{
				uint8_t cmd[5] = {CMD_SCALE_EMPTY_A, CMD_SCALE_EMPTY_B, 0, CMD_CONTROL_END_A, CMD_CONTROL_END_B};
				m_port.write(cmd, sizeof(cmd));
			}
			else if (workflag == SCALES_WORK_FULL)
			{
				uint8_t cmd[5] = {CMD_SCALE_FULL_A, CMD_SCALE_FULL_B, 0, CMD_CONTROL_END_A, CMD_CONTROL_END_B};
				m_port.write(cmd, sizeof(cmd));
			}
			else if (workflag == SCALES_WORK_POWER1)
			{	// 0x0最大  0xFF关闭
				static bool _c_run1 = true;
				uint8_t cmd[7] = {CMD_POWER_START_A, CMD_POWER_START_B, 2, 1, _c_run1 ? 200 : 0xFF, CMD_CONTROL_END_A, CMD_CONTROL_END_B};
				_c_run1 = !_c_run1;
				m_port.write(cmd, sizeof(cmd));
			}
			else if (workflag == SCALES_WORK_POWER2)
			{
				static bool _c_run2 = true;
				uint8_t cmd[7] = {CMD_POWER_START_A, CMD_POWER_START_B, 2, 2, _c_run2 ? 200 : 0xFF, CMD_CONTROL_END_A, CMD_CONTROL_END_B};
				_c_run2 = !_c_run2;
				m_port.write(cmd, sizeof(cmd));
			}
			else if (workflag == SCALES_WORK_POWER3)
			{
				static bool _c_run3 = true;
				uint8_t cmd[7] = {CMD_POWER_START_A, CMD_POWER_START_B, 2, 3, _c_run3 ? 200 : 0xFF, CMD_CONTROL_END_A, CMD_CONTROL_END_B};
				_c_run3 = !_c_run3;
				m_port.write(cmd, sizeof(cmd));
			}
			else if (workflag == SCALES_WORK_POWER4)
			{
				static bool _c_run4 = true;
				uint8_t cmd[7] = {CMD_POWER_START_A, CMD_POWER_START_B, 2, 4, _c_run4 ? 200 : 0xFF, CMD_CONTROL_END_A, CMD_CONTROL_END_B};
				_c_run4 = !_c_run4;
				m_port.write(cmd, sizeof(cmd));
			}
		}
	}
	else
	{
		if (m_port.isOpened())
		{
			uint32_t len = 4;
			uint8_t *pcmd = new uint8_t[len];
			pcmd[0] = CMD_MASTER_STOP_A;
			pcmd[1] = CMD_MASTER_STOP_B;
			pcmd[len - 2] = CMD_CONTROL_END_A;
			pcmd[len - 1] = CMD_CONTROL_END_B;
			m_port.write(pcmd, len);
			m_port.close();
			//
			delete[] pcmd;


			m_buttonRun->setTag(0);
			m_buttonRun->setText(L"启动");

			// 保持参数
			write();
		}
	}
}


void viewScales::onClickClearLog()
{
	m_lstLog->removeAll();
}


}	// wuc
