#pragma once

#include <QtWidgets/QWidget>
#include "ui_CMainWindow.h"


class QSystemTrayIcon;
class CMainWindow : public QWidget
{
	Q_OBJECT
	QSystemTrayIcon * _systray;			//系统托盘
	WId _winId = 0;								//窗口ID，窗口由非激活状态转到最前显示时要用
public:
	CMainWindow(QWidget *parent = Q_NULLPTR);
	//显示翻译出来的文本
	void slotShowString(const QString& str);
signals:
	void emitShowFinshed();
private:
	//事件过滤，当窗口是非激活状态下hide
	bool eventFilter(QObject* o, QEvent* e) override;
	//设置系统托盘图标
	void setSystray();	

private:
	Ui::CMainWindowClass ui;
};

