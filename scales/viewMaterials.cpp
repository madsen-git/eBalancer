#include "StdAfx.h"
#include "viewMaterials.h"

namespace wuc {

viewMaterials::viewMaterials(void)
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
	// �༭��
	const int gpcols = 4;	// ����
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
	for (int idx = 0; idx < 2; ++idx)
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
	phor->setBkColor(0xFE999933);
	phor->setFixedHeight(rowH);

	HContainerArrayType vecCell = wuc::layoutGrid(phor, vecRow, vecCol);// wuc::layoutGrid(phor, rows, cols, VIEW_SCALES_INPUT_ITEM_CY, VIEW_SCALES_INPUT_ITEM_CY, VIEW_SCALES_INPUT_ITEMNAME_CX, VIEW_SCALES_INPUT_ITEMEDIT_CX);
	// ��־
	if (_pdialogMainPtr->supportLogs())
	{
		phor = new wuc::horizontalLayoutUC();
		phor->setBkColor(0xFE9900CC);
		add(phor);
		//phor->setFixedHeight(VIEW_SCALES_LOGS_CY);
	}
	// δ��
	phor = new wuc::horizontalLayoutUC();
	add(phor);
	phor->setFixedHeight(VIEW_SCALES_LOGS_BSPACE_CY);
	phor->setBkColor(0xFE000066);

	//enumSerialPort();
	//// �༭��
	//rowBegin = 1 * cols;	// ����0
	//col = 0;
	//pLable = new labelUC();
	//vecCell[rowBegin + col++]->add(pLable);
	//pLable->applyAttributeList(wsAttrName.c_str());
	//pLable->setText(L"ͨѶ�˿�");
	////pLable->setBkColor(0xFE9966FF);
	//m_comboPort = new comboUC();
	//vecCell[rowBegin + col++]->add(m_comboPort);
	//m_comboPort->applyAttributeList(L"Combo");
	//m_comboPort->setDropBoxAttributeList(L"vscrollbarstyle=\"VScrollBar\"");
	//listLabelElementUC *plstLabel = nullptr;
	//IntSetType setPort = dev::serialPort::enumPort(false);
	//for (IntSetType::const_iterator iter = setPort.cbegin(); iter != setPort.cend(); ++iter)
	//{
	//	std::swprintf(pbuf, _countof(pbuf), L"COM%d", *iter + 1);
	//	plstLabel = new listLabelElementUC();
	//	m_comboPort->add(plstLabel);
	//	plstLabel->setText(pbuf);
	//}
	//++col;	// ������λ
	//// ����ѡ��
	//pLable = new labelUC();
	//vecCell[rowBegin + col++]->add(pLable);
	//pLable->applyAttributeList(wsAttrName.c_str());
	//pLable->setText(L"��������");
	////m_editMaterials = new editUC();
	////vecCell[rowBegin + col++]->add(m_editMaterials);
	////m_editMaterials->applyAttributeList(L"Edit");
	//m_comboMaterials = new comboUC();
	//vecCell[rowBegin + col++]->add(m_comboMaterials);
	//m_comboMaterials->applyAttributeList(L"Combo");
	//m_comboMaterials->setDropBoxAttributeList(L"vscrollbarstyle=\"VScrollBar\"");
	//// ÿ������
	//rowBegin = 3 * cols;
	//col = 0;
	//pLable = new labelUC();
	//vecCell[rowBegin + col++]->add(pLable);
	//pLable->applyAttributeList(wsAttrName.c_str());
	//pLable->setText(L"ÿ������");

	//m_editWeightPack = new editUC();
	//vecCell[rowBegin + col++]->add(m_editWeightPack);
	//m_editWeightPack->applyAttributeList(L"Edit");
	//m_editWeightPack->setTextStyle(DT_SINGLELINE | DT_VCENTER | DT_RIGHT);

	//pLable = new labelUC();
	//vecCell[rowBegin + col++]->add(pLable);
	//pLable->applyAttributeList(wsAttrName.c_str());
	//pLable->setText(L"��");
	//// �������
	//pLable = new labelUC();
	//vecCell[rowBegin + col++]->add(pLable);
	//pLable->applyAttributeList(wsAttrName.c_str());
	//pLable->setText(L"�������");

	//m_editMarginError = new editUC();
	//vecCell[rowBegin + col++]->add(m_editMarginError);
	//m_editMarginError->applyAttributeList(L"Edit");
	//m_editMarginError->setTextStyle(DT_SINGLELINE | DT_VCENTER | DT_RIGHT);

	//pLable = new labelUC();
	//vecCell[rowBegin + col++]->add(pLable);
	//pLable->applyAttributeList(wsAttrName.c_str());
	//pLable->setText(L"��");

	//// �������
	//pLable = new labelUC();
	//vecCell[rowBegin + col++]->add(pLable);
	//pLable->applyAttributeList(wsAttrName.c_str());
	//pLable->setText(L"�������");

	//m_editPackCount = new editUC();
	//vecCell[rowBegin + col++]->add(m_editPackCount);
	//m_editPackCount->applyAttributeList(L"Edit");
	//m_editPackCount->setTextStyle(DT_SINGLELINE | DT_VCENTER | DT_RIGHT);

	////pLable = new labelUC();
	////vecCell[rowBegin + col++]->add(pLable);
	////pLable->applyAttributeList(wsAttrName.c_str());
	////pLable->setText(L"��");

	//// �ϸ����
	//rowBegin = 5 * cols;
	//col = 0;
	//pLable = new labelUC();
	//vecCell[rowBegin + col++]->add(pLable);
	//pLable->applyAttributeList(wsAttrName.c_str());
	//pLable->setText(L"�ϸ����");

	//m_editRealCount = new editUC();
	//vecCell[rowBegin + col++]->add(m_editRealCount);
	//m_editRealCount->applyAttributeList(L"Edit");
	//m_editRealCount->setTextStyle(DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
	//m_editRealCount->setReadOnly(true);
	//++col;	// ������λ
	//// ���ϸ����
	////rowBegin = 7 * cols;
	////col = 0;
	//pLable = new labelUC();
	//vecCell[rowBegin + col++]->add(pLable);
	//pLable->applyAttributeList(wsAttrName.c_str());
	//pLable->setText(L"���ϸ����");
	////pLable->setBkColor(0xFE00ff00);
	//m_editFailedCount = new editUC();
	//vecCell[rowBegin + col++]->add(m_editFailedCount);
	//m_editFailedCount->applyAttributeList(L"Edit");
	//m_editFailedCount->setTextStyle(DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
	//m_editFailedCount->setReadOnly(true);
	//// ���а�ť
	//m_buttonRun = new buttonUC();
	//col += 2;
	//vecCell[rowBegin + col]->add(m_buttonRun);
	//m_buttonRun->applyAttributeList(L"Button");
	//m_buttonRun->setText(L"����");
}


viewMaterials::~viewMaterials(void)
{
}


PCWSTR viewMaterials::getClass() const
{
	return L"viewMaterials";
}
LPVOID viewMaterials::getInterface(PCWSTR pstrName)
{
	if(std::wcscmp(pstrName, L"ViewMaterials") == 0) return static_cast<viewMaterials *>(this);
	return __super::getInterface(pstrName);
}
UINT viewMaterials::getControlFlags() const
{
	return UCFLAG_TABSTOP | (isEnabled() ? UCFLAG_SETCURSOR : 0);
}

void viewMaterials::Event(BSEventUC &event)
{
	if( !isMouseEnabled() && event.Type > BEUCEventMouseBegin && event.Type < BEUCEventMouseEnd ) {
		if( m_pParent != NULL ) m_pParent->Event(event);
		else __super::Event(event);
		return;
	}

	__super::Event(event);
}

void viewMaterials::notify(wuc::BSNotifyUC& msg)
{
	if(!msg.pSender) return;

}
void viewMaterials::onInit()
{

	//assert(m_pManager);
	//if(m_pManager) m_pManager->addNotifier(this);



}
}	// wuc
