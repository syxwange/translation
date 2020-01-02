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
	ui.textBrowser->setFont(QFont(u8"��Բ",10));
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
		hide();  //���߹رմ���,�����Լ��Ĳ���.
		return true;
	}	
	return false;
}

void CMainWindow::setSystray()
{
	_systray->setIcon(QIcon(":/CMainWindow/translate.ico"));
	//������ƶ��������ϵ�ͼ��ʱ������ʾ�˴����õ�����
	_systray->setToolTip(u8"�е�����");

	auto act1 = new QAction(u8"��ʾ������", this);
	connect(act1, &QAction::triggered, this, &CMainWindow::show);
	auto act2 = new QAction(u8"�˳�", this);
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
	////���´�������windowsƽ̨�£��ô��ڼ������ǰ����ʾ��ʹ�õ���winAPI
	::AttachThreadInput(::GetWindowThreadProcessId(::GetForegroundWindow(), NULL), ::GetCurrentThreadId(), true);
	::SetForegroundWindow((HWND)_winId);
	::SetWindowPos((HWND)_winId, HWND_TOPMOST, 0, 0, 900, 600, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	//::SetFocus((HWND)_winId);
	::AttachThreadInput(::GetWindowThreadProcessId(::GetForegroundWindow(), NULL), ::GetCurrentThreadId(), false);	
	emit emitShowFinshed();
}
