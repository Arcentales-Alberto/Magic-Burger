#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
     // Widget de ingreso.
    void on_ingresarButton_clicked();
////////////////////////////////////////////////
    // Widget de gerente.
    void on_configurarDatos_clicked();

    void on_modificarDatosEmp_clicked();

    void on_limpiarDatos_clicked();

    void on_SiguienteGerente_clicked();
 /////////////////////////////////////////////////////////////////
    // Widget de gerente 1.
    void on_fechapushButton_clicked();

    void on_limpiarVisores_clicked();

    void on_volverGerente_clicked();
////////////////////////////////////////////////////////////////
   // Widget del repositor.
    void on_actualizacionStock_clicked();

    void on_modificarDatosStock_clicked();

    void on_limpiarDatosStock_clicked(); 
    /////////////////////////////////////////////////////////////////
    // Widget del expendedor.
    void on_topeDePan_clicked();

    void on_discoCentralDePan_clicked();

    void on_baseDePan_clicked();

    void on_hamburguesaDeCarne_clicked();

    void on_hamburguesaDePollo_clicked();

    void on_hamburguesaDePescado_clicked();

    void on_huevoFrito_clicked();

    void on_fetaDeQueso_clicked();

    void on_porcionDeTomatePicado_clicked();

    void on_porcionDeLechugaPicada_clicked();

    void on_PorcionDeCebollaPicada_clicked();

    void on_porcionDePepinosEnConserva_clicked();

    void on_salsaDeMayonesa_clicked();

    void on_porcionDeSalsaCatsup_clicked();

    void on_porcionDePapasFritas_clicked();

    void on_vasoConGaseosa_clicked();

    void on_comboCarnepushButton_clicked();

    void on_comboPollopushButton_clicked();

    void on_comboPescadopushButton_clicked();

    void on_confirmarCompra_clicked();

    void on_anularComprapushButton_clicked();

    void on_limpiarVisorExpendedor_clicked();


    void on_volverGerenteLogpushButton_clicked();

    void on_volverRepositorLogpushButton_clicked();

    void on_volverExpendedorLogpushButton_clicked();

private:
   void closeEvent(QCloseEvent *);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
