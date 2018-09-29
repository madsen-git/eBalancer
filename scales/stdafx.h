// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>
#include <windowsx.h>

#include <Commctrl.h>
#pragma comment(lib, "Comctl32.lib")// SysListView32...

//#include "vld.h"
// gtc
#include "AC/sysupport.h"			// baseEnum.h  debugger.h  basedef.h  baseException.h 
									// singleton.h  stringUtil.h
// debugger.h datetime.h
#include "AC/resourceManager.h"		// image.h
#include "AC/draw/drawer.h"			// drawdef.h  baseobj.h  image.h
//#include "AC/file.h"				// shellExt.h  highLevelAPI.h baseReg.h
#include "AC/SyncObject.h"			// 

// WUC
#include "UC/wucinc.h"
// dev
#include "device/devinc.h"
//#include "device/devdef.h"
//#include "device/device.h"

// db
#include "db/dbtype.h"				// basetype.h  datetime.h  base64.h
#include "db/sqlite/sqlite.h"

#include "scales.h"


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

