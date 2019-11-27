#include <QApplication>

#include "gui/MainWindow.h"



int
main( int argc, char *argv[] )
{
    Q_INIT_RESOURCE( images );

    QApplication app( argc, argv );

    MainWindow window;
    window.show();

    return app.exec();
}
