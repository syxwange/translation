#pragma once

#include <QObject>
#include <QAbstractNativeEventFilter>
#include <qhash.h>
#include <QKeySequence>

class CGlobalShortCut;
class CWinEventFilter : public QAbstractNativeEventFilter
{
public:
	CWinEventFilter(CGlobalShortCut* shortcut) :_shortcut(shortcut) {}
	~CWinEventFilter() {}
	virtual bool nativeEventFilter(const QByteArray& eventType, void* message, long*) override;
private:
	CGlobalShortCut* _shortcut;
};


class QApplication;
class CGlobalShortCut : public QObject
{
	Q_OBJECT

public:
	CGlobalShortCut(QString key, QObject*parent);
	~CGlobalShortCut(){ unregisterHotKey(); }

	void activateShortcut(){ emit activated(); }
	bool registerHotKey();
	bool unregisterHotKey();
	QHash<QPair<quint32, quint32>, CGlobalShortCut*> _shortcuts;
private:	
	CWinEventFilter* _filter;
	QKeySequence _key;
	Qt::Key _qtKey;
	Qt::KeyboardModifiers _mods;
	static quint32 nativeKeycode(Qt::Key keycode);
	static quint32 nativeModifiers(Qt::KeyboardModifiers modifiers);
signals:
	void activated();
};
