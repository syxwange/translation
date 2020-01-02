#pragma once

#include <QObject>

class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;

class CYoudaoDict : public QObject
{
	Q_OBJECT

	QNetworkAccessManager* _http;			//�������
	QString _strSrc{};										//Ҫ�����Դ�ı�����Ϊ�е�AI������1000�����ƣ�
	QString _strTran{};									//������͵��ı�
public:
	CYoudaoDict(QObject *parent=nullptr);
	~CYoudaoDict() {}

	void slotTranslate(QString& str);
signals:
	void signSendString(QString& str);	
private:
	//��Ϊ�е�AI������1000�����ƣ�����һЩ����
	void slotFinished(QNetworkReply * );
	//ͨ��GET������Ҫ������ı����÷����Ե�������á�
	void translateByGet(QString&);
	//ͨ��post������Ҫ������ı����÷����Դ�ε����ַ���ã��õ����е���AI���롣
	void translateByPost(QString&);
	//������get�������뷵����������
	QString parsingByGet(QByteArray& );
	//������post�������뷵����������
	QString parsingByPost(QByteArray&);
	//��ѹ���ص�Gzip��ʽѹ������
	QByteArray unCompress(QByteArray& src);
};
