#include "CMainWindow.h"
#include <qicon.h>
#include <qaction.h>
#include <qmenu.h>
#include <QSystemTrayIcon>
#include <qfont.h>
#include <qmessagebox.h>
#include <Windows.h>
CMainWindow::CMainWindow(QWidget *parent)	: QWidget(parent),_systray(new QSystemTrayIcon(this))
{
	setWindowIcon(QIcon(":/CMainWindow/translate.ico"));
	ui.setupUi(this);
	installEventFilter(this);
	_winId = winId();
	ui.textBrowser->setFont(QFont(u8"幼圆",10));
	setWindowFlags(Qt::FramelessWindowHint);
	setWindowOpacity(0.8);
	setSystray();
	setMinimumWidth(900);	
	_systray->show();
}

bool CMainWindow::eventFilter(QObject* o, QEvent* e)
{
	if (QEvent::WindowDeactivate == e->type())
	{
		hide();  //或者关闭窗口,加入自己的操作.
		return true;
	}	
	return false;
}

void CMainWindow::setSystray()
{
	_systray->setIcon(QIcon(":/CMainWindow/translate.ico"));
	//当鼠标移动到托盘上的图标时，会显示此处设置的内容
	_systray->setToolTip(u8"有道翻译");

	auto act1 = new QAction(u8"显示主界面", this);
	connect(act1, &QAction::triggered, this, &CMainWindow::show);
	auto act2 = new QAction(u8"退出", this);
	connect(act2, &QAction::triggered, this, &CMainWindow::close);
	auto mMenu = new QMenu(this);
	mMenu->addAction(act1);
	mMenu->addSeparator();
	mMenu->addAction(act2);
	_systray->setContextMenu(mMenu);
}

void CMainWindow::slotShowString(const QString& str)
{
	ui.textBrowser->setText(str);	
	move(QCursor::pos());	
	show();	
	////以下代码是在windows平台下，让窗口激活并在最前面显示，使用的是winAPI
	::AttachThreadInput(::GetWindowThreadProcessId(::GetForegroundWindow(), NULL), ::GetCurrentThreadId(), true);
	::SetForegroundWindow((HWND)_winId);
	::SetWindowPos((HWND)_winId, HWND_TOPMOST, 0, 0, 900, 600, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	//::SetFocus((HWND)_winId);
	::AttachThreadInput(::GetWindowThreadProcessId(::GetForegroundWindow(), NULL), ::GetCurrentThreadId(), false);	
	emit emitShowFinshed();
}
