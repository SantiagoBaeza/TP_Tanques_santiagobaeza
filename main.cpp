
#include <QApplication>
#include <QTimer>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    //Inicialización de la aplicación Qt
    QApplication app(argc, argv);
    app.setApplicationName("SimuladorCisterna");
    app.setApplicationVersion("1.0");


    MainWindow *mainWin = new MainWindow;
    mainWin->setWindowTitle("Simulador de Cisterna y Tanques Auxiliares");
    mainWin->show();


    //ejecuta el bucle principal de la aplicación
    int ret = app.exec();

    return ret;
}

