#ifndef __SCALESDEF_H__
#define __SCALESDEF_H__

// 主窗口类
#define SCALES_WND_MAIN_CLASS			L"ScalesMainClass"
#define SCALES_WND_MAIN_CAPTION			L"电子秤"
#define SCALES_MUTEX_ONLY				L"ScalesMutexOnly"
// 字体索引
extern int g_ifontMSYH11;				// 微软雅黑11
extern int g_ifontMSYH12;				// 微软雅黑12
extern int g_ifontMSYH12i;				// 微软雅黑12 斜体
extern int g_ifontMSYH12u;				// 微软雅黑12 下划线
extern int g_ifontMSYH12b;				// 微软雅黑12 粗体
extern int g_ifontMSYH14;				// 微软雅黑14
extern int g_ifontMSYH15;				// 微软雅黑15
extern int g_ifontMSYH16;				// 微软雅黑16
extern int g_ifontMSYH16b;				// 微软雅黑16 粗体
extern int g_ifontMSYH18;				// 微软雅黑18
extern int g_ifontMSYH18u;				// 微软雅黑18 下划线
extern int g_ifontMSYH22;				// 微软雅黑22
extern int g_ifontMSYH24;				// 微软雅黑24
extern int g_ifontMSYH25;				// 微软雅黑25
extern int g_ifontMSYH28;				// 微软雅黑28
extern int g_ifontMSYH34;				// 微软雅黑34
extern int g_ifontMSYH38;				// 微软雅黑38


// 主窗口
#define MAIN_WND_MIN_CY						350		// 主窗体最小高度
#define MAIN_WND_MIN_CX						650		// 主窗体最小宽度
#define MAIN_WND_ROUNDCORNER_CX				4		// 窗口圆角
#define MAIN_WND_ROUNDCORNER_CY				4		// 窗口圆角
#define MAIN_WND_LINE_CY					1		// 状态区分隔线
#define MAIN_WND_DETECTION_LEFT				6		// 探测区域，左
#define MAIN_WND_DETECTION_RIGHT			6		// 探测区域，右
#define MAIN_WND_DETECTION_TOP				6		// 探测区域，上
#define MAIN_WND_DETECTION_BOTTOM			6		// 探测区域，下
#define MAIN_WND_CAPTION_CY					88		// 标题栏高
#define MAIN_WND_CAPTION_TEXT_CY			25		// 标题栏窗口名的高
#define MAIN_WND_CAPTION_TEXT_LSPACE		13		// 标题栏窗口名的左空隙
#define MAIN_WND_CAPTION_TEXT_RSPACE		0		// 标题栏窗口名的右空隙

#define MAIN_WND_STATUS_CY					42		// 状态栏高

#define MAIN_WND_TASK_TAB_CY				30		// 任务tab高
#define MAIN_WND_TASK_TAB_ITEM_CX			80		// 任务tab子项宽

// 通用控件
#define SCROLLBAR_WIDTH						8		// 滚动条宽
#define COMBO_ITEM_CY						21		// combo控件内容条目高
#define	EDIT_PADDING_LR						6		// EDIT控件左右内缩
#define	EDIT_PADDING_TB						0		// EDIT控件上下内缩
#define LABEL_WIDTH							88		// label的宽
#define LABEL_HEIGHT						24		// label的高
// viewScales试图
#define VIEW_SCALES_INPUT_ITEM_CY			26		// 编辑部分的行高
#define VIEW_SCALES_INPUT_ITEMSPACE_CY		10		// 编辑部分的行高
#define VIEW_SCALES_INPUT_ITEMNAME_CX		100		// 编辑名称的宽
#define VIEW_SCALES_INPUT_ITEMEDIT_CX		150		// 编辑的宽
#define VIEW_SCALES_INPUT_ITEMUNIT_CX		30		// 编辑的单位
#define VIEW_SCALES_LOGS_BSPACE_CY			16		// 日志区距底空隙
//////////////////////////////////////////////////////////////////////////
//	2016-1-4	颜色值
#define ARGB_DEFAULT_TEXT				0xFEFFFFFF	// 默认颜色
#define ARGB_MAIN_TASK_BK				0xFEFF6565	// 主窗口任务栏背景色
// 通用控件
#define ARGB_COMBO_BK					0xFEFFFFFF	// 重要日子编辑视图中combo控件背景色
#define ARGB_COMBO_SELECTED				0xFEEAEAEA	// 重要日子编辑视图中combo控件选中时背景色
#define ARGB_COMBO_TEXT					0xFE5B5B5B	// 重要日子编辑视图中combo控件文本色
#define ARGB_BUTTON_TEXT				0xFE333333	// 按钮文本色
#define ARGB_EDIT_PROMPT_TEXT			0xFEB7B7B7	// 编辑Prompt文本色
#define ARGB_EDIT_TEXT					0xFE333333	// 编辑文本色
#define ARGB_EDIT_BK					0xFEFDFBFB	// EDIT控件背景色
#define ARGB_LABEL_TEXT					0xFE3F3F3F	// label控件文本色

// 秤重视图背景色
#define ARGB_VIEW_SCALES_BK				0xFEFBF7F7	// 
// group
#define GROUP_MAIN_TASK					L"groupMainTask"	// 主窗口任务栏选项组
#endif