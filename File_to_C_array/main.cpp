#include <QtGui/QApplication>
#include "widget.h"
#include <QTextCodec> //���������ʾ��Ҫ

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //���������ʾ
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    Widget w;
    w.setWindowFlags(w.windowFlags() & (Qt::WindowMinimizeButtonHint));
    w.show();

    return a.exec();
}
