#pragma once

namespace wuc {

class viewMaterials : public verticalLayoutUC
{
public:
	viewMaterials(void);
	~viewMaterials(void);

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual UINT getControlFlags() const;

	virtual void Event(BSEventUC &event);
	virtual void notify(wuc::BSNotifyUC& msg);
	virtual void onInit();
};

}	// wuc
