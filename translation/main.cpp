#include "CTranslation.h"
#include <QtWidgets/QApplication>
#include <qtimer.h>
#include "CGlobalShortCut.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CTranslation w;
	CGlobalShortCut* shortcut = new CGlobalShortCut("Ctrl+d", &a);
	QObject::connect(shortcut, &CGlobalShortCut::activated, &w, &CTranslation::activatedtest);
	
	w.show();
	QTimer::singleShot(1000, &w, &CTranslation::hide);
	return a.exec();
}
