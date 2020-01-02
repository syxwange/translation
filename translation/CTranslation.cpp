#include "CTranslation.h"
#include <qclipboard.h>
#include "CYoudaoDict.h"
#include <qdebug.h>
#include <qmessagebox.h>
#include <QKeyEvent>
#include <Windows.h>

CTranslation::CTranslation(QObject *parent)	: QObject(parent),_win(new CMainWindow),_dict(new CYoudaoDict(this))
{
	
	connect(this, &CTranslation::signTranslate, _dict, &CYoudaoDict::slotTranslate);
	connect(_dict, &CYoudaoDict::signSendString, _win, &CMainWindow::slotShowString);
	connect(_win, &CMainWindow::emitShowFinshed, this, &CTranslation::slotClearConnect);
}

CTranslation::~CTranslation()
{
	if (_win)
	{
		delete _win;
		_win = nullptr;
	}		
}

void CTranslation::activatedtest()
{
	_data = qApp->clipboard()->text();
	connect(qApp->clipboard(), &QClipboard::dataChanged, this, &CTranslation::slotDataChanged);
	/*QKeyEvent tabKey(QEvent::KeyPress, Qt::Key_C, Qt::ControlModifier);
	QCoreApplication::sendEvent(this, &tabKey);*/
	keybd_event(VK_CONTROL, 0, 0, 0);
	keybd_event('C', 0, 0, 0);
	//_sleep(10);
	keybd_event('C', 0, 2, 0);
	keybd_event(VK_CONTROL, 0, 2, 0);
	//_sleep(10);
}


void CTranslation::slotDataChanged()
{			
	if (_tempData == qApp->clipboard()->text())
		return;
	_tempData = qApp->clipboard()->text();
	emit signTranslate( qApp->clipboard()->text());
}

void CTranslation::slotClearConnect()
{
	disconnect(qApp->clipboard(), &QClipboard::dataChanged, this, &CTranslation::slotDataChanged);
	qApp->clipboard()->setText(_data);
}
