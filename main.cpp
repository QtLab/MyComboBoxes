#include "cbwnd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CBWnd w;
    w.show();

    return a.exec();
}
