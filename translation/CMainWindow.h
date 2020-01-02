#pragma once

#include <QtWidgets/QWidget>
#include "ui_CMainWindow.h"


class QSystemTrayIcon;
class CMainWindow : public QWidget
{
	Q_OBJECT
	QSystemTrayIcon * _systray;			//ϵͳ����
	WId _winId = 0;								//����ID�������ɷǼ���״̬ת����ǰ��ʾʱҪ��
public:
	CMainWindow(QWidget *parent = Q_NULLPTR);
	//��ʾ����������ı�
	void slotShowString(const QString& str);
signals:
	void emitShowFinshed();
private:
	//�¼����ˣ��������ǷǼ���״̬��hide
	bool eventFilter(QObject* o, QEvent* e) override;
	//����ϵͳ����ͼ��
	void setSystray();	

private:
	Ui::CMainWindowClass ui;
};

