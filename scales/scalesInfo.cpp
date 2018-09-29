#include "StdAfx.h"
#include "scalesInfo.h"
#include "Winspool.h"


void enumSerialPort()
{
	LPBYTE pbuf = NULL;
	DWORD pcbNeeded, pcReturned;

	if (!EnumPorts(NULL, 2, pbuf, 0, &pcbNeeded, &pcReturned))
	{
		pbuf = new BYTE[pcbNeeded];
	}
	if (EnumPorts(NULL, 2, pbuf, pcbNeeded, &pcbNeeded, &pcReturned))
	{
		PORT_INFO_2 *pPorts = (PORT_INFO_2 *)pbuf;
		for (int idx = 0; idx < pcReturned; ++idx)
		{
			std::wstring wstr = pPorts[idx].pPortName;
			wstr.clear();
		}
	}


}