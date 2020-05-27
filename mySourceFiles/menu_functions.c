/*
 * 文件名: menu_functions.c
 * -------------------------------------
 * 这个文件实现了菜单栏里的所有选项
 */

#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>

#include "graphics.h"

#include "menu_functions.h"
#include "my_display.h"
#include "my_macro.h"
#include "my_resource.h"
#include "my_utilities.h"

extern HWND graphicsWindow;     // GUI窗口句柄，在 libgraphics 中声明
extern HWND consoleWindow;    // 终端窗口句柄，在 libgraphics 中声明
extern DataProperty data;  // 链表相关属性值，在 my_resource.c 中声明
extern epidemic SentinelNode;  // 哨兵节点，在 my_resource.c 中声明
extern MyStatus status;  // 当前状态，在 my_resource.c 中定义
extern int CurrentTheme;  // 当前主题，在 menu_functions.c 

void MenuFileNew()
{
	if (data.BaseDir != nullptr)  // 保证这个分支后，data.BaseDir 一定为空
		MenuFileClose();  // 把旧文件清了
	FileInputList(NEW_FILE_DIR);

	GUIOutputMsg("新建成功");
}

void MenuFileOpen()
{
	if (data.HasModified)
	{
		display();
		const int selection = MessageBox(graphicsWindow,
			TEXT("您有未保存的更改，请问需要保存这些更改吗？"),
			TEXT("提示"), MB_OKCANCEL | MB_ICONWARNING);
		if (selection == IDOK)
		{
			MenuFileSave();
			return;
		}
	}
	/*以下代码的实现部分参考了 StackOverflow 论坛*/
	OPENFILENAME ofn;
	static TCHAR szFile[MAX_PATH] = { 0 };

	ZeroMemory(&ofn, sizeof(ofn));  // 将ofn所在内存区域清零

	/*为 ofn 赋初始值*/
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = graphicsWindow;  // 传入窗口句柄
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "COVID19 FILES\0*.COVID19\0";  // 支持打开的文件类型
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = "..\\myResourceFiles";  // 默认目录
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;  // 目录和文件必须存在，否则弹出警告对话框

	GUIOutputMsg("正在打开");

	if (GetOpenFileName(&ofn) == TRUE)  // ofn.lpstrFile 会被赋上文件的绝对路径，字符串形式
		FileInputList(ofn.lpstrFile);

	GUIOutputMsg("打开成功");
}

void MenuFileSave()
{
	if (data.BaseDir == nullptr)  // 若没有打开任何文件
	{
		GUIOutputMsg("目前未打开文件");
		MessageBox(graphicsWindow, TEXT("您目前没有新建或打开任何文件，无需保存"),
			TEXT("提示"), MB_OK | MB_ICONINFORMATION);
	}
	else if (!strcmp(data.BaseDir, NEW_FILE_DIR))
	{
		MenuFileSaveAs();
		return;
	}
	else if (data.HasModified)  // 如果链表里的数据被修改过
	{
		FileSave(data.BaseDir);
		GUIOutputMsg("保存成功");
	}
	else
	{
		GUIOutputMsg("无需保存");
		MessageBox(graphicsWindow, TEXT("您的文件已与GUI同步，无需保存"),
			TEXT("提示"), MB_OK | MB_ICONINFORMATION);
	}
	data.HasModified = false;
}

void MenuFileSaveAs()
{
	OPENFILENAME ofn;
	static char szFileName[MAX_PATH] = "Untitled.covid19";  // 默认文件名，将存储文件路径

	ZeroMemory(&ofn, sizeof(ofn));  // 将ofn所在内存区域清零

	/*为 ofn 赋初始值*/
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = graphicsWindow;
	ofn.lpstrFilter = "COVID19 FILES\0*.covid19\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = "%UserProfile%\\Documents";  // 默认目录
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = "covid19";

	if (GetSaveFileName(&ofn))  // 拿到文件保存的目录
	{
		FileSave(ofn.lpstrFile);  // 保存文件
		data.BaseDir = ofn.lpstrFile;  // 保存文件路径
		data.HasModified = false;  // 文件已是最新
		GUIOutputMsg("另存成功");
	}
}

void MenuFileClose()
{
	if (data.HasModified)
	{
		display();
		const int selection = MessageBox(graphicsWindow,
			TEXT("您有未保存的更改，请问需要保存这些更改吗？"),
			TEXT("提示"), MB_OKCANCEL | MB_ICONWARNING);
		if (selection == IDOK)
		{
			MenuFileSave();
			return;
		}
	}

	DesHighlight();
	FreeEpidemicList(SentinelNode.next);
	status.DisplayPrediction = false;  // 不显示统计图
	SentinelNode.next = nullptr;
	data.BaseDir = nullptr;  // 清空存储当前文件绝对路径的变量
	GUIOutputMsg("关闭成功");
}

void MenuFileExit()
{
	if (data.HasModified)
	{
		display();
		const int selection = MessageBox(graphicsWindow,
			TEXT("您有未保存的更改，请问需要保存这些更改吗？"),
			TEXT("提示"), MB_OKCANCEL | MB_ICONWARNING);
		if (selection == IDOK)
		{
			MenuFileSave();
			return;
		}
	}
	display();
	const int selection = MessageBox(graphicsWindow, TEXT("您确定要退出吗？"),
		TEXT("提示"), MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON2);
	if (selection == IDOK)
		exit(0);
}

/***********************************************************************************/

void MenuEditChange()
{
	if (data.BaseDir == nullptr)
	{
		display();
		MessageBox(graphicsWindow,
			TEXT("您尚未打开文件。请先打开文件。"),
			TEXT("提示"), MB_OK | MB_ICONWARNING);
		GUIOutputMsg("修改未完成");
		return;
	}

	InitConsole();

	while (true)
	{
		printf("您当前要更改的是“%s”，未更改前的值是 %d \n",
			PropertyMeaning(status.HighlightProperty),
			status.HighlightNode->properties[status.HighlightProperty]);
		printf("请输入您想更改为的值，要求为不超过%d位的十进制非负数: ", MAX_DIGIT);
		int input = SafeNNegIntInput(MAX_DIGIT);
		if (input == -1)
		{
			printf("您的输入有非法字符，请重新输入。\n");
			//printf("按任意键继续\n");
			//if (_getch() == -32);
			// TODO: 这边的阻塞有点问题，注释掉了，可以考虑看看是为啥
		}
		else if (input == -2)
			printf("您的输入超出了范围，请重新输入。\n");
		else
		{
			data.HasModified = true;
			status.HighlightNode->properties[status.HighlightProperty] = input;
			GetMaxElement();
			display();
			GUIOutputMsg("修改完成");
			break;
		}
	}
	MyExitConsole();  // 退出终端窗口
}

void MenuEditFrontInsert()
{
	if (data.BaseDir == nullptr)
	{
		GUIOutputMsg("文件未打开");
		MessageBox(graphicsWindow,
			TEXT("您尚未打开文件。请先打开文件。"),
			TEXT("提示"), MB_OK | MB_ICONWARNING);
		return;
	}

	/* 欲更改的日期，先赋为初始值 */
	int NewMonth = SentinelNode.next->properties[Month], NewDay = SentinelNode.next->properties[Day];
	if (!DateCalculatePro(&NewMonth, &NewDay, -1))
	{
		MessageBox(graphicsWindow,
			TEXT("您要录入的日期已跨年。"),
			TEXT("错误"), MB_OK | MB_ICONWARNING);
		return;
	}

	/* 节点新建，初始化与连接 */
	epidemic* NewNode = (epidemic*)malloc(sizeof(epidemic));
	NewNode->properties[Month] = NewMonth;
	NewNode->properties[Day] = NewDay;
	NewNode->properties[Current] = 0;
	NewNode->properties[Total] = 0;
	NewNode->properties[Cured] = 0;
	NewNode->properties[Dead] = 0;
	NewNode->prev = nullptr;
	NewNode->next = SentinelNode.next;
	SentinelNode.next->prev = NewNode;
	SentinelNode.next = NewNode;

	data.HasModified = true;
	++status.HighlightNum;
	GetDayNum();
	GetMaxElement();
	GUIOutputMsg("前置录入成功");
}

void MenuEditBackInsert()
{
	if (data.BaseDir == nullptr)
	{
		GUIOutputMsg("文件未打开");
		MessageBox(graphicsWindow,
			TEXT("您尚未打开文件。请先打开文件。"),
			TEXT("提示"), MB_OK | MB_ICONWARNING);
		return;
	}

	/* 欲更改的日期，先赋为初始值 */
	int NewMonth = SentinelNode.next->properties[Month], NewDay = SentinelNode.next->properties[Day];
	if (!DateCalculatePro(&NewMonth, &NewDay, data.TotalDays))
	{
		MessageBox(graphicsWindow,
			TEXT("您要录入的日期已跨年。"),
			TEXT("错误"), MB_OK | MB_ICONWARNING);
		return;
	}

	/* 节点新建，初始化与连接 */
	epidemic* NewNode = (epidemic*)malloc(sizeof(epidemic));
	NewNode->properties[Month] = NewMonth;
	NewNode->properties[Day] = NewDay;
	NewNode->properties[Current] = 0;
	NewNode->properties[Total] = 0;
	NewNode->properties[Cured] = 0;
	NewNode->properties[Dead] = 0;

	epidemic* TempNode = SentinelNode.next;
	while (TempNode->next != nullptr)  // 找到尾节点
		TempNode = TempNode->next;

	TempNode->next = NewNode;
	NewNode->prev = TempNode;
	NewNode->next = nullptr;

	data.HasModified = true;
	GetDayNum();
	GetMaxElement();
	GUIOutputMsg("后置录入成功");
}

// TODO: 在做删除时，一定要注意删除后链表的长度，以及高亮光标所在的节点会不会被删掉

void MenuEditFrontDelete()
{
	if (data.BaseDir == nullptr)
	{
		GUIOutputMsg("文件未打开");
		MessageBox(graphicsWindow,
			TEXT("您尚未打开文件。请先打开文件。"),
			TEXT("提示"), MB_OK | MB_ICONWARNING);
		return;
	}
	if (data.TotalDays <= MIN_LIST_LENGTH)
	{
		GUIOutputMsg("删除未完成");
		MessageBox(graphicsWindow,
			TEXT("您的链表已达到最短长度，无法进行删除。"),
			TEXT("提示"), MB_OK | MB_ICONWARNING);
		return;
	}

	if (status.HighlightNode != SentinelNode.next)  // 如果不是首个数据节点
		--status.HighlightNum;
	else
		status.HighlightNode = SentinelNode.next->next;

	epidemic* TempNode = SentinelNode.next;
	SentinelNode.next = SentinelNode.next->next;
	SentinelNode.next->prev = nullptr;
	free(TempNode);

	data.HasModified = true;
	GetDayNum();
	GetMaxElement();
	GUIOutputMsg("前置删除成功");
}

void MenuEditBackDelete()
{
	if (data.BaseDir == nullptr)
	{
		GUIOutputMsg("文件未打开");
		MessageBox(graphicsWindow,
			TEXT("您尚未打开文件。请先打开文件。"),
			TEXT("提示"), MB_OK | MB_ICONWARNING);
		return;
	}
	if (data.TotalDays <= MIN_LIST_LENGTH)
	{
		GUIOutputMsg("删除未完成");
		MessageBox(graphicsWindow,
			TEXT("您的链表已达到最短长度，无法进行删除。"),
			TEXT("提示"), MB_OK | MB_ICONWARNING);
		return;
	}

	epidemic* TempNode = SentinelNode.next;
	while (TempNode->next != nullptr)  // 找到尾节点
		TempNode = TempNode->next;

	if (status.HighlightNode == TempNode)  // 如果是尾节点
	{
		--status.HighlightNum;
		status.HighlightNode = TempNode->prev;
	}

	TempNode->prev->next = nullptr;
	free(TempNode);

	data.HasModified = true;
	GetDayNum();
	GetMaxElement();
	GUIOutputMsg("后置删除成功");
}

/****************************************************************************/

void MenuDrawGraph()
{
	extern _Bool DisplayLineChart, DisplayFanChart, DisplayBarChart;
	if (data.BaseDir == nullptr)
	{
		GUIOutputMsg("图标未绘制");
		MessageBox(graphicsWindow,
			TEXT("您尚未打开文件。请先打开文件。"),
			TEXT("提示"), MB_OK | MB_ICONWARNING);
		return;
	}
	DisplayLineChart = DisplayFanChart = DisplayBarChart = true;
}

void MenuDrawPrediction()
{
	extern char MenuDrawPredictionString[20];
	if (data.BaseDir == nullptr)
	{
		GUIOutputMsg("预测未显示");
		MessageBox(graphicsWindow,
			TEXT("您尚未打开文件。请先打开文件。"),
			TEXT("提示"), MB_OK | MB_ICONWARNING);
		return;
	}

	status.DisplayPrediction = !(status.DisplayPrediction);

	if (status.DisplayPrediction)
	{
		sprintf(MenuDrawPredictionString, "隐藏预测");
		GUIOutputMsg("已显示预测");
	}

	else
	{
		sprintf(MenuDrawPredictionString, "显示预测");
		GUIOutputMsg("已隐藏预测");
	}
}

void MenuDrawHighlight()
{
	extern char Highlight[20];
	if (data.BaseDir == nullptr)
	{
		GUIOutputMsg("高亮光标未显示");
		MessageBox(graphicsWindow,
			TEXT("您尚未打开文件。请先打开文件。"),
			TEXT("提示"), MB_OK | MB_ICONWARNING);
		return;
	}

	if (!status.HighlightVisible)
	{
		sprintf(Highlight, "隐藏高亮光标");
		status.HighlightVisible = true;
	}
	else if (status.HighlightVisible)
	{
		sprintf(Highlight, "显示高亮光标");
		status.HighlightVisible = false;
	}
}

void MenuDrawChangeTheme()
{
	CurrentTheme = (CurrentTheme + 1) % THEME_NUM;
	GUIOutputMsg("切换主题成功");
}
