#pragma once

#include <QObject>

class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;

class CYoudaoDict : public QObject
{
	Q_OBJECT

	QNetworkAccessManager* _http;			//网络管理
	QString _strSrc{};										//要翻译的源文本，因为有道AI翻译有1000字限制，
	QString _strTran{};									//翻译后发送的文本
public:
	CYoudaoDict(QObject *parent=nullptr);
	~CYoudaoDict() {}

	void slotTranslate(QString& str);
signals:
	void signSendString(QString& str);	
private:
	//因为有道AI翻译有1000字限制，做了一些处理
	void slotFinished(QNetworkReply * );
	//通过GET请求发送要翻译的文本，该方法对单调翻译好。
	void translateByGet(QString&);
	//通过post请求发送要翻译的文本，该方法对大段的文字翻译好，用的是有道的AI翻译。
	void translateByPost(QString&);
	//解析用get方法翻译返回来的数据
	QString parsingByGet(QByteArray& );
	//解析用post方法翻译返回来的数据
	QString parsingByPost(QByteArray&);
	//解压返回的Gzip格式压缩数据
	QByteArray unCompress(QByteArray& src);
};
