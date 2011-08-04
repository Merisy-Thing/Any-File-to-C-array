#include <QtGui/QApplication>
#include "widget.h"
#include <QTextCodec> //解决中文显示需要

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //解决中文显示
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    Widget w;
    w.setWindowFlags(w.windowFlags() & (Qt::WindowMinimizeButtonHint));
    w.show();

    return a.exec();
}
