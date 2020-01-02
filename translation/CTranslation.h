#pragma once

#include <QObject>

#include "CMainWindow.h"


class CYoudaoDict;
class CTranslation : public QObject
{
	Q_OBJECT

	CMainWindow* _win;
	CYoudaoDict* _dict;		
	QString _data;
	QString _tempData;
public:
	CTranslation(QObject *parent=nullptr);
	~CTranslation();

	void show() { _win->show(); }
	void hide() { _win->hide(); }
	void activatedtest();
signals:
	void signTranslate(QString& str);

private:
	void slotDataChanged();
	void slotClearConnect();
};
