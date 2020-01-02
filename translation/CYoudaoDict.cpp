#include "CYoudaoDict.h"

#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qdebug.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QtZlib/zlib.h>
#include <qmessagebox.h>

CYoudaoDict::CYoudaoDict(QObject *parent) : QObject(parent),_http(new QNetworkAccessManager(this))
{
    connect(_http, &QNetworkAccessManager::finished, this, &CYoudaoDict::slotFinished);
}

void CYoudaoDict::slotTranslate(QString& str)
{   
    _strTran.clear();   
    _strSrc.clear();
    if (str.size() < 1000)
    {        
        translateByPost(str);
        return;
    }   
        int pos = str.indexOf(".", 900);
        if (pos<899 || pos>str.size())
        {
            QMessageBox::warning(nullptr, "", u8"在1000单词内没有找到句号");
            return;
        }
        auto n = str.size() - pos - 1;
        _strSrc = str.right(n);
        translateByPost(str.left(pos));
}

void CYoudaoDict::slotFinished(QNetworkReply* reply)
{
    QByteArray replData = reply->readAll();
   
     _strTran += parsingByPost(replData);// parsingByGet(replData);
     if (_strSrc.isEmpty())
     {
         emit signSendString(_strTran);
         reply->deleteLater();
         return;
     }
    if(_strSrc.size()<1000)
    {
        translateByPost(_strSrc);
        _strSrc.clear();
        reply->deleteLater();       
        return;
    } 
    else
    {
        int pos = _strSrc.indexOf(".", 900);
        auto rtemp = _strSrc.left(pos);
        qDebug() << rtemp;
        auto n = _strSrc.size() - pos - 1;
        _strSrc = _strSrc.right(n);
        translateByPost(rtemp);
    }  
    reply->deleteLater();
}

void CYoudaoDict::translateByGet(QString& str)
{
    QString urlstr = QString("http://dict.youdao.com/w/%1").arg(str);
    QNetworkRequest request;
    request.setUrl(QUrl(urlstr));
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Charset", "GBK,utf-8;q=0.7,*;q=0.3");
    //request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
    request.setRawHeader("Accept-Language", "zh-CN,zh;q=0.8");
    request.setRawHeader("Cache-Control", "max-age=0");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/535.7 (KHTML, like Gecko) Chrome/16.0.912.63 Safari/535.7");
    _http->get(request);
}

void CYoudaoDict::translateByPost(QString& str)
{
    auto bytes = str.toLocal8Bit().toPercentEncoding();
    bytes.replace("%20", "+");
    bytes.replace("%28", "(");
    bytes.replace("%29", ")");
    //bytes.replace("%3D", "=");

////    POST https ://aidemo.youdao.com/trans HTTP/1.1
    QNetworkRequest request(QUrl("https://aidemo.youdao.com/trans"));
////Host: aidemo.youdao.com
    request.setRawHeader("Host", "aidemo.youdao.com");
////Connection : keep - alive
    request.setRawHeader("Connection", "keep-alive");
////Content - Length : 24
////Accept : application/json, text/javascript, */*; q=0.01
    request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
////Origin: http://ai.youdao.com
    request.setRawHeader("Origin", "http://ai.youdao.com");
////User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.88 Safari/537.36
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.88 Safari/537.36");
////Content-Type: application/x-www-form-urlencoded; charset=UTF-8
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
////Sec-Fetch-Site: cross-site
    request.setRawHeader("Sec-Fetch-Site", "cross-site");
////Sec-Fetch-Mode: cors
    request.setRawHeader("Sec-Fetch-Mode", "cors");
////Referer: http://ai.youdao.com/product-fanyi.s
    request.setRawHeader("Referer", "http://ai.youdao.com/product-fanyi.s");
////Accept-Encoding: gzip, deflate, br
    request.setRawHeader("Accept-Encoding", "gzip,deflate,br");
////Accept-Language: zh-CN,zh;q=0.9
    request.setRawHeader("Accept-Language", "zh-CN,zh;q=0.9");
////
    auto mybytes = QByteArray("q=")+bytes+QByteArray("&from=Auto&to=Auto");

    _http->post(request, mybytes);

}

QString CYoudaoDict::parsingByGet(QByteArray& replData)
{
    QString str;
    int pos1 = replData.indexOf("<div class=\"trans-container\">");
    if (pos1 > 0)
    {
        int pos2 = replData.indexOf("</div>", pos1) + 6;
        auto ret = replData.mid(pos1, pos2 - pos1).replace("class=\"additional\"", "style=\"color:gray\"");
        pos1 = ret.indexOf("<p>");
        if (pos1 > 0)
        {
            pos2 = ret.indexOf("</p>", pos1);
            str = ret.remove(pos1, pos2 - pos1);
        }
        else
        {
            str = ret;
        }
    }
    else
    {
        str.clear();
    }
    str.remove(u8"</p>\n <p>以上为机器翻译结果，长、整句建议使用 <a class=\"viaInner\" href=\"http://f.youdao.com?keyfrom=dict.result\" target=_blank>人工翻译</a> 。</p>\n ");
    return str;
}


QString CYoudaoDict::parsingByPost(QByteArray& bytes)
{
    QString str;
    auto data = unCompress(bytes);
    QJsonParseError jsonError;//Qt5新类
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);//Qt5新类
    if (jsonError.error == QJsonParseError::NoError)
    {
        auto jsonObj = jsonDoc.object();       
        if (jsonObj.contains("translation"))
        {
            auto translation = jsonObj.value("translation").toArray();
            for (auto& i : translation)
            {
                 str += i.toString();
            }
            return str;
        }
    } 
    return QString(u8"json格式错误");
}

QByteArray CYoudaoDict::unCompress(QByteArray& src)
{
    QByteArray outBuffer;
    z_stream strm;
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    strm.avail_in = src.size();
    strm.next_in = (Bytef*)src.data();
    int err = -1;
    err = inflateInit2(&strm, MAX_WBITS + 16);
    if (err == Z_OK) 
    {
        while (true)
        {
            char buffer[4096] = {};
            strm.avail_out = 4096;
            strm.next_out = (Bytef*)buffer;
            int code = inflate(&strm, Z_FINISH);
            outBuffer.append(buffer, 4096 - strm.avail_out);
            if (Z_STREAM_END == code)// || Z_OK != code) 防止截取半头           
                break;    
        }
    }
    inflateEnd(&strm);
    return outBuffer;
}
