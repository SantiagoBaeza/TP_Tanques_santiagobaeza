
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "ui_mainwindow.h" // Interfaz generada por Qt Designer

class MainWindow : public QMainWindow
{
    Q_OBJECT  // Macro obligatoria para usar señales y slots

public:
    // Constructor y destructor
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void iniciarSimulacion();

    // Detiene la simulación (detiene el temporizador)
    void detenerSimulacion();

public slots:

    void actualizarSimulacion();

private:

    Ui::MainWindow *ui;


    QTimer *temporizador;

    double volCisterna;  //volumen actual de la cisterna
    double volAux1;      //volumen actual del tanque auxiliar 1
    double volAux2;      //volumen actual del tanque auxiliar 2
};

#endif

