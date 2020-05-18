﻿/*
 * 文件名: menu_functions.c
 * -------------------------------------
 * 这个文件实现了菜单栏里的所有选项
 */

#include <stdbool.h>

#include <Windows.h>

#include "menu_functions.h"
#include "my_display.h"
#include "my_resource.h"


extern HWND graphicsWindow;     // GUI窗口句柄，在 libgraphics 中声明
extern DataProperty data;  // 链表相关属性值，在 my_resource.c 中声明
extern epidemic SentinelNode;  // 哨兵节点，在 my_resource.c 中声明
extern MyStatus status;  // 当前状态，在 my_resource.c 中定义

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
	TCHAR szFile[MAX_PATH] = { 0 };

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
	if (data.BaseDir == nullptr)  // 若没有打开任何文件（新建文件状态）
	{
		if (data.HasModified)
		{
			MenuFileSaveAs();
			return;
		}
		else
		{
			display();
			MessageBox(graphicsWindow, TEXT("您目前没有新建或打开任何文件，无需保存"),
				TEXT("提示"), MB_OK | MB_ICONINFORMATION);
			GUIOutputMsg("目前未打开文件");
		}
	}
	else if (data.HasModified)  // 如果链表里的数据被修改过
	{
		FileSave(data.BaseDir);
		GUIOutputMsg("保存成功");
	}
	else
	{
		display();
		MessageBox(graphicsWindow, TEXT("您的文件已与GUI同步，无需保存"),
			TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		GUIOutputMsg("无需保存");
	}
	data.HasModified = false;
}

void MenuFileSaveAs()
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

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

	if (GetSaveFileName(&ofn))
	{
		FileSave(ofn.lpstrFile);
		data.BaseDir = ofn.lpstrFile;
		data.HasModified = false;
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