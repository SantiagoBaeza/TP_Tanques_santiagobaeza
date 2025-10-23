#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , temporizador(new QTimer(this))
    , volCisterna(0.0)
    , volAux1(0.0)
    , volAux2(0.0)
{
    ui->setupUi(this);

    //conectar diales a lcds para mostrar % seleccionado
    connect(ui->dial_tanque1_entrada, &QDial::valueChanged, ui->lcd_tanque1_entrada, QOverload<int>::of(&QLCDNumber::display));
    connect(ui->dial_tanque1_salida, &QDial::valueChanged, ui->lcd_tanque1_salida, QOverload<int>::of(&QLCDNumber::display));
    connect(ui->dial_tanque2, &QDial::valueChanged, ui->lcd_tanque2, QOverload<int>::of(&QLCDNumber::display));
    connect(ui->dial_tanque3, &QDial::valueChanged, ui->lcd_tanque3, QOverload<int>::of(&QLCDNumber::display));

    // inicializar volúmenes reales desde progressBars
    double capCisterna = ui->capacidad_tanque1->value();
    double capAux1 = ui->capacidad_tanque1->value();
    double capAux2 = ui->capacidad_tanque2->value();

    volCisterna = (ui->tanque1->value() / 100.0) * capCisterna;
    volAux1 = (ui->tanque2->value() / 100.0) * capAux1;
    volAux2 = (ui->tanque3->value() / 100.0) * capAux2;

    // Configurar temporizador (pero no comienza todavia)
    temporizador->setInterval(1000);
    connect(temporizador, &QTimer::timeout, this, &MainWindow::actualizarSimulacion);

    //botones
    connect(ui->boton_iniciar, &QPushButton::clicked, this, &MainWindow::iniciarSimulacion);
    connect(ui->boton_detener, &QPushButton::clicked, this, &MainWindow::detenerSimulacion);

    ui->boton_detener->setEnabled(false);
    ui->boton_iniciar->setEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::iniciarSimulacion()
{
    double capCisterna = ui->capacidad_tanque1->value();
    double capAux1 = ui->capacidad_tanque1->value();
    double capAux2 = ui->capacidad_tanque2->value();

    volCisterna = (ui->tanque1->value() / 100.0) * capCisterna;
    volAux1 = (ui->tanque2->value() / 100.0) * capAux1;
    volAux2 = (ui->tanque3->value() / 100.0) * capAux2;

    temporizador->start();
    ui->boton_iniciar->setEnabled(false);
    ui->boton_detener->setEnabled(true);
}

void MainWindow::detenerSimulacion()
{
    temporizador->stop();
    ui->boton_iniciar->setEnabled(true);
    ui->boton_detener->setEnabled(false);
}

void MainWindow::actualizarSimulacion()
{
    double capCisterna = ui->capacidad_tanque1->value();
    double capAux1 = ui->capacidad_tanque1->value();
    double capAux2 = ui->capacidad_tanque2->value();

    double QmaxEntCisterna = ui->Qmax_tanque1_entrada->value();
    double QmaxSalCisterna = ui->Qmax_tanque1_salida->value();

    double pctEntCisterna = ui->dial_tanque1_entrada->value() / 100.0;
    double pctSalCisterna = ui->dial_tanque1_salida->value() / 100.0;
    double pctSalAux1 = ui->dial_tanque2->value() /100.0;
    double pctSalAux2 = ui->dial_tanque3->value() /100.0;

    double entradaCisterna = (volCisterna >= capCisterna) ? 0.0 : QmaxEntCisterna * pctEntCisterna;

    bool habilitadoAux1 = ui->habilitar_tanque2->isChecked();
    bool habilitadoAux2 = ui->habilitar_tanque3->isChecked();
    bool aux1Disponible = habilitadoAux1 && volAux1 < capAux1;
    bool aux2Disponible = habilitadoAux2 && volAux2 < capAux2;

    double salidaCisterna = QmaxSalCisterna * pctSalCisterna;
    double salidaRealCisterna = (aux1Disponible || aux2Disponible) ? salidaCisterna : 0.0;

    double entradaAux1 = 0.0;
    double entradaAux2 = 0.0;

    if (aux1Disponible && aux2Disponible) {
        entradaAux1 = std::min(salidaRealCisterna / 2.0, capAux1 - volAux1);
        entradaAux2 = std::min(salidaRealCisterna / 2.0, capAux2 - volAux2);
    } else if (aux1Disponible) {
        entradaAux1 = std::min(salidaRealCisterna, capAux1 - volAux1);
    } else if (aux2Disponible) {
        entradaAux2 = std::min(salidaRealCisterna, capAux2 - volAux2);
    }

    double salidaAux1 = (volAux1 >= 0.1 * capAux1) ? QmaxSalCisterna * pctSalAux1 : 0.0;
    double salidaAux2 = (volAux2 >= 0.1 * capAux2) ? QmaxSalCisterna * pctSalAux2 : 0.0;

    volCisterna += entradaCisterna - (entradaAux1 + entradaAux2);
    volAux1 += entradaAux1 - salidaAux1;
    volAux2 += entradaAux2 - salidaAux2;

    volCisterna = std::clamp(volCisterna, 0.0, capCisterna);
    volAux1 = std::clamp(volAux1, 0.0, capAux1);
    volAux2 = std::clamp(volAux2, 0.0, capAux2);

    ui->tanque1->setValue(static_cast<int>((volCisterna / capCisterna)* 100));
    ui->tanque2->setValue(static_cast<int>((volAux1 / capAux1) *100));
    ui->tanque3->setValue(static_cast<int>((volAux2 / capAux2) *100));

    ui->lcd_tanque1_entrada->display(QString::number(entradaCisterna, 'f', 2));
    ui->lcd_tanque1_salida->display(QString::number(entradaAux1 + entradaAux2, 'f',2));
    ui->lcd_tanque2->display(QString::number(salidaAux1, 'f', 2));
    ui->lcd_tanque3->display(QString::number(salidaAux2, 'f', 2));
}
