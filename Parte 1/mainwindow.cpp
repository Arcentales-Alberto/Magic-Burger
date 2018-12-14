#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTextStream>
#include <QDesktopWidget>
#include <QFile>
#include <fstream>
#include <QString>
#include <QDateTime>
#include <sstream>
#include <stdlib.h> // atoi.
#include <QRegExp>

QString fecha = QDateTime::currentDateTime().toString("dd/MM/yyyy");
using namespace std;
int contador = 0;
string rol;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widgetLogin->show();
    ui->widgetGerente->hide();
    ui->widgetGerente1->hide();
    ui->widgetRepositor->hide();
    ui->widgetExpendor->hide();

    ui->widgetLogin->setGeometry(0,0,550,550);
    ui->widgetGerente->setGeometry(0,0,550,550);
    ui->widgetGerente1->setGeometry(0,0,550,550);
    ui->widgetRepositor->setGeometry(0,0,550,550);
    ui->widgetExpendor->setGeometry(0,0,800,800);

    this->setMinimumHeight(750);
    this->setMaximumHeight(750);
    this->setMinimumWidth(600);
    this->setMaximumWidth(600);

}
MainWindow::~MainWindow()
{
    delete ui;

}
void MainWindow::closeEvent(QCloseEvent *event)
{

    event->ignore();

     if(rol == "gerente")
     {

        QMessageBox quitMessageBox;
        quitMessageBox.setWindowTitle("SALIR");
        quitMessageBox.setWindowIcon(QIcon(":/new/img/Salir.gif"));
        quitMessageBox.setIcon(QMessageBox::Question);
        quitMessageBox.setText("Realmente desea salir de la aplicación?");
        quitMessageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        quitMessageBox.setDefaultButton(QMessageBox::No);
        quitMessageBox.button(QMessageBox::Yes)->setText("Sí");
        quitMessageBox.button(QMessageBox::No)->setText("No");

        if (quitMessageBox.exec() == QMessageBox::Yes)
        {
          event->accept();
        }
      }
      else
      {
        QMessageBox::warning(NULL , "  Advertencia  "  ,  "  Unico usuario (Gerente), que puede cerrar el sistema       "  );
      }
}
////////////////////////////////////////////////////////////////
// Botones uados en el widget de la autenticacion.
void MainWindow::on_ingresarButton_clicked()
{
    ui->usuariolineEdit->setFocus();
    QString usuarioEnLineEdit = ui->usuariolineEdit->text();
    QString contrasenaEnLineEdit = ui->contrasenalineEdit->text();

    string usuarioStr = usuarioEnLineEdit.toStdString();
    string contrasenaStr  = contrasenaEnLineEdit.toStdString();
    string renglon , usuario , contrasena , separador;
    bool encontrado = false;


    ifstream entradaF;
    entradaF.open("empleados.txt");

    if(entradaF.good())
    {

        getline(entradaF , renglon);
        while(!entradaF.eof() && !encontrado)
        {
            // Lectura de datos del empleado(cada 4 renglones).
            usuario = renglon;
            getline(entradaF , renglon);
            contrasena = renglon;
            getline(entradaF , renglon);
            rol = renglon;
            getline(entradaF , renglon);
            separador = renglon;

            // Verificacion de la existenia del empleado.
            if(usuarioStr == usuario && contrasenaStr == contrasena)
            {
                encontrado = true;
            }
            else
            {
                getline(entradaF , renglon);
            }

        }

    }

    entradaF.close();

    if(encontrado)
    {
        if(rol == "gerente")
        {
            ui->widgetGerente->show();
            ui->widgetLogin->hide();
        }
        else if(rol == "repositor")
        {
            ui->widgetRepositor->show();
            ui->widgetLogin->hide();
        }
        if(rol == "expendedor")
        {
            ui->widgetExpendor->show();
            ui->widgetLogin->hide();
        }

    }
    else
    {
        QMessageBox::warning(NULL ,"       Advertencia             ", "   Error     ");
        ui->usuariolineEdit->clear();
        ui->contrasenalineEdit->clear();
        ui->usuariolineEdit->setFocus();
    }

}

//////////////////////////////////////////////////////////
// Boton usado en el widget del gerente.
void MainWindow::on_configurarDatos_clicked() //Envia los datos desde el fichero al plainEditText.
{
    string renglon;
    QString QvisorPlain = ui->empleadosplainTextEdit->toPlainText();

    if(QvisorPlain.isEmpty())
    {
        ifstream entradaF; // Etiqueta en modo lectura.
        entradaF.open("empleados.txt");

        if(entradaF.good())
        {

            getline(entradaF , renglon);

            while(!entradaF.eof())
            {
                QString Qrenglon = QString::fromStdString(renglon); // Convierto de un Qstring a string.
                ui->empleadosplainTextEdit->appendPlainText(Qrenglon); // Enviado de datos al planedittext.
                getline(entradaF , renglon);
            }
        }

        entradaF.close();
    }
}

void MainWindow::on_modificarDatosEmp_clicked() // Envia los datos desde el plainEditText al fichero.
{ 
    string visorPlain;
    QString QvisorPlain = ui->empleadosplainTextEdit->toPlainText();

    if(QvisorPlain.isEmpty())
    {

    }
    else
    {

        visorPlain = QvisorPlain.toStdString();

        ofstream salidaF;

        salidaF.open("empleados.txt" , ios::trunc );

        if(salidaF.good())
        {
            salidaF << visorPlain << endl;
        }

        salidaF.close();
    }


}
void MainWindow::on_limpiarDatos_clicked() // Limpia los datos en el plainEditText.
{
    ui->empleadosplainTextEdit->clear();
}
void MainWindow::on_SiguienteGerente_clicked()
{
    ui->empleadosplainTextEdit->clear();
    ui->widgetGerente->hide();
    ui->widgetGerente1->show();
}


void MainWindow::on_volverGerenteLogpushButton_clicked()
{
    ui->empleadosplainTextEdit->clear();
    ui->widgetGerente->hide();
    ui->widgetLogin->show();
    ui->usuariolineEdit->clear();
    ui->contrasenalineEdit->clear();
    ui->usuariolineEdit->setFocus();



}
/////////////////////////////////////////////////////////////
// Boton usado en el widget del gerente1.
void MainWindow::on_fechapushButton_clicked()
{
    // Visor tickets.
    string renglon; // Para obtener los renglones del fichero.
    bool esFechaPosible = true;

    QString fechaEnLineEdit = ui->fechalineEdit->text(); // string a QString.

    if(fechaEnLineEdit.isEmpty())  // Si esta vacio(cuadro de fecha), no realizar accion alguno al hacer click.
    {

    }
    else   // Caso contario.
    {
      // Conversion de posicion de la fecha elegida.
      QString diaElegi = QString::fromStdString("")+ fechaEnLineEdit.at(0) + fechaEnLineEdit.at(1);
      QString mesElegi =  QString::fromStdString("") +fechaEnLineEdit.at(3) + fechaEnLineEdit.at(4);
      QString anioElegi = QString::fromStdString("")+ fechaEnLineEdit.at(6) + fechaEnLineEdit.at(7)+ fechaEnLineEdit.at(8) + fechaEnLineEdit.at(9);
      QString fechaElegi = anioElegi +"/"+ mesElegi +"/"+ diaElegi;

    ifstream entradaF;
    entradaF.open("tickets.txt");

    if(entradaF.good())
    {

        getline(entradaF , renglon);
        while(!entradaF.eof() && esFechaPosible)
        {

            QString Qrenglon = QString::fromStdString(renglon); // string a QString.
            // Conversion de posicion de las fechas del fichero.
            QString diaPost = QString::fromStdString("")+ Qrenglon.at(0) + Qrenglon.at(1);
            QString mesPost =  QString::fromStdString("") +Qrenglon.at(3) + Qrenglon.at(4);
            QString anioPost = QString::fromStdString("")+ Qrenglon.at(6) + Qrenglon.at(7)+ Qrenglon.at(8) + Qrenglon.at(9);
            QString fechaPost = anioPost +"/"+ mesPost +"/"+ diaPost;

            if(fechaElegi < fechaPost)  // Fecha que no existe.
            {
                esFechaPosible = false;
            }

            else if(fechaElegi > fechaPost)  // Fecha que posiblemente exista.
            {
                getline(entradaF , renglon);

                while(renglon != "*")  // Seguimos avanzando hasta posiblemenete encontrar la fecha.
                {
                   getline(entradaF , renglon);
                }
                getline(entradaF , renglon);   // Obtenemos la fecha, para su evaluacion
            }

            else                       // Fecha identica
            {
                while(renglon != "*")
                {
                    QString Qrenglon = QString::fromStdString(renglon); // Convierto de un Qstring a string.
                    ui->ticketsplainTextEdit->appendPlainText(Qrenglon); // Envio de datos al planedittext.
                    getline(entradaF , renglon);
                }

                getline(entradaF , renglon);  // Obtenemos la fecha, para su evaluacion.
            }
            }

        }

    entradaF.close();
  }

  //////////////////////////////////////////////////////////////////////////////////////////
    // Visor ingredientes.
    string renglon2;   // Para obtener los renglones del fichero.
    bool esFechaPosible2 = true;
    // Conversion de posicion de la fecha elegida.
            QString diaElegi2 = QString::fromStdString("")+ fechaEnLineEdit.at(0) + fechaEnLineEdit.at(1);
            QString mesElegi2 =  QString::fromStdString("") +fechaEnLineEdit.at(3) + fechaEnLineEdit.at(4);
            QString anioElegi2 = QString::fromStdString("")+ fechaEnLineEdit.at(6) + fechaEnLineEdit.at(7)+ fechaEnLineEdit.at(8) + fechaEnLineEdit.at(9);
            QString fechaElegi2 = anioElegi2 +"/"+ mesElegi2 +"/"+ diaElegi2;

          ifstream entradaF2;
          entradaF2.open("tickets.txt");

          if(entradaF2.good())
          {

              getline(entradaF2 , renglon2);
              while(!entradaF2.eof() && esFechaPosible2)
              {

                  QString Qrenglon2 = QString::fromStdString(renglon2); // string a QString.
                 // Conversion de posicion de las fechas del fichero.
                  QString diaPost2 = QString::fromStdString("")+ Qrenglon2.at(0) + Qrenglon2.at(1);
                  QString mesPost2 =  QString::fromStdString("") +Qrenglon2.at(3) + Qrenglon2.at(4);
                  QString anioPost2 = QString::fromStdString("")+ Qrenglon2.at(6) + Qrenglon2.at(7)+ Qrenglon2.at(8) + Qrenglon2.at(9);
                  QString fechaPost2 = anioPost2 +"/"+ mesPost2 +"/"+ diaPost2;

                  if(fechaElegi2 < fechaPost2)  // Fecha que no existe.
                  {
                      esFechaPosible2 = false;
                  }

                  else if(fechaElegi2 > fechaPost2)  // Fecha que posiblemente exista.
                  {
                      getline(entradaF2 , renglon2);

                      while(renglon2!= "*")      // Seguimos avanzando hasta posiblemenete encontrar la fecha.
                      {
                         getline(entradaF2 , renglon2);
                      }
                      getline(entradaF2 , renglon2); // Obtenemos la fecha, para su evaluacion
                  }

                  else                              // Fecha identica
                  {
                      getline(entradaF2 ,renglon2);
                      while(renglon2.at(0) != 'T')
                      {
                          QString Qrenglon2 = QString::fromStdString(renglon2); // Convierto de un Qstring a string.
                          ui->ingredientesplainTextEdit->appendPlainText(Qrenglon2); // Envio de datos al planedittext.
                          getline(entradaF2 , renglon2);
                      }

                      getline(entradaF2 , renglon2); // Obtenemos el separador.
                      getline(entradaF2 , renglon2); // Obtenemos la fecha, para su evaluacion.
                  }
                }

              }



          entradaF2.close();

  //////////////////////////////////////////////////////////////////////////////////////////
      // Visor total de ventas.
      string renglon3; // Para obtener los renglones del fichero.
      string totalStr = ""; // Usado dentro de un "if" para obtener numeros string en el renglon.
      QString visorTickets;
      QString visorIngredientes;
      int totalInt = 0; // Usado en el "else" que esta dentro del ciclo "while".
      int sumaTotal = 0; // Usado en el "else" que esta dentro del ciclo "while".
      bool esFechaPosible3 = true;
       // Conversion de posicion de la fecha elegida.
              QString diaElegi3 = QString::fromStdString("")+ fechaEnLineEdit.at(0) + fechaEnLineEdit.at(1);
              QString mesElegi3 =  QString::fromStdString("") +fechaEnLineEdit.at(3) + fechaEnLineEdit.at(4);
              QString anioElegi3 = QString::fromStdString("")+ fechaEnLineEdit.at(6) + fechaEnLineEdit.at(7)+ fechaEnLineEdit.at(8) + fechaEnLineEdit.at(9);
              QString fechaElegi3 = anioElegi3 +"/"+ mesElegi3 +"/"+ diaElegi3;

            ifstream entradaF3;
            entradaF3.open("tickets.txt");

            if(entradaF3.good())
            {

                getline(entradaF3 , renglon3);
                while(!entradaF3.eof() && esFechaPosible3)
                {

                    QString Qrenglon3 = QString::fromStdString(renglon3); // string a QString.
                 // Conversion de posicion de las fechas del fichero.
                    QString diaPost3 = QString::fromStdString("")+ Qrenglon3.at(0) + Qrenglon3.at(1);
                    QString mesPost3 =  QString::fromStdString("") +Qrenglon3.at(3) + Qrenglon3.at(4);
                    QString anioPost3 = QString::fromStdString("")+ Qrenglon3.at(6) + Qrenglon3.at(7)+ Qrenglon3.at(8) + Qrenglon3.at(9);
                    QString fechaPost3 = anioPost3 +"/"+ mesPost3 +"/"+ diaPost3;

                    if(fechaElegi3 < fechaPost3)  // Fecha que no existe.
                    {
                        esFechaPosible3 = false;
                    }

                    else if(fechaElegi3 > fechaPost3) // Fecha que posiblemente exista.
                    {
                        getline(entradaF3 , renglon3);

                        while(renglon3!= "*")  // Seguimos avanzando hasta posiblemenete encontrar la fecha.
                        {
                           getline(entradaF3 , renglon3);
                        }
                        getline(entradaF3 , renglon3); // Obtenemos la fecha, para su evaluacion.
                    }

                    else                              // Fecha identica
                    {
                        getline(entradaF3 ,renglon3);
                        while(renglon3.at(0) != 'T')   // Obtiene el renglon del total.
                        {
                            getline(entradaF3 , renglon3);
                        }

                        for(unsigned int i=0 ; i<renglon3.length() ; i++)  // Para recorrer sus caracteres.
                        {
                            if(renglon3.at(i) >= 48 && renglon3.at(i) <= 57) // Para obtener numeros(tipo string), en el renglon.
                            {
                                totalStr = totalStr + renglon3.at(i);
                            }
                        }

                        totalInt = atoi(totalStr.c_str()); // string a int.
                        totalStr = "";                     // Limpiamos para otros posibles datos.
                        sumaTotal = sumaTotal + totalInt;

                        getline(entradaF3 , renglon3);  // Obtenemos el separador.
                        getline(entradaF3 , renglon3);  // Obtenemos la fecha, para su evaluacion.
                    }

                }

            }

            entradaF3.close();


     visorTickets = ui->ticketsplainTextEdit->toPlainText();
     visorIngredientes = ui->ingredientesplainTextEdit->toPlainText();

    if(visorTickets.isEmpty() && visorIngredientes.isEmpty())
    {
        QMessageBox::warning(NULL ,"  Advertencia  ", " Fecha no existente  ");
    }
    else
    {
        QString QsumaTotal = QString::number(sumaTotal);
        ui->totalLineEdit->setText(QsumaTotal + "$");
        ui->fechapushButton->setEnabled(false);
    }

}

void MainWindow::on_limpiarVisores_clicked()
{
    ui->ticketsplainTextEdit->clear();
    ui->ingredientesplainTextEdit->clear();
    ui->totalLineEdit->clear();
    ui->fechapushButton->setEnabled(true);
}


void MainWindow::on_volverGerente_clicked()
{
    ui->widgetGerente1->hide();
    ui->widgetGerente->show();
    ui->ticketsplainTextEdit->clear();
    ui->ingredientesplainTextEdit->clear();
    ui->totalLineEdit->clear();
    ui->fechalineEdit->clear();
    ui->fechapushButton->setEnabled(true);

}

//////////////////////////////////////////////////////////////////////////////////////////////
//Botones usados en el widget del repositor.

void MainWindow::on_actualizacionStock_clicked() //Envia los datos desde el fichero al plainEditText.
{
    string renglon;
    QString QvisorPlain = ui->stockplainTextEdit->toPlainText();

    if(QvisorPlain.isEmpty())
    {
        ifstream entradaF; // Etiqueta en modo lectura.
        entradaF.open("stock.txt");

        if(entradaF.good())
        {
            getline(entradaF , renglon);

            while(!entradaF.eof())
            {
                QString Qrenglon = QString::fromStdString(renglon); // Convierto de un Qstring a string.
                ui->stockplainTextEdit->appendPlainText(Qrenglon); // Envio de datos al planedittext.
                getline(entradaF , renglon);
            }
        }

        entradaF.close();
    }

}

void MainWindow::on_modificarDatosStock_clicked()  // Envia los datos desde el plainEditText al fichero.
{
    string visorPlain;
    QString QvisorPlain = ui->stockplainTextEdit->toPlainText();

    if(QvisorPlain.isEmpty())
    {

    }
    else
    {
        visorPlain = QvisorPlain.toStdString();

        ofstream salidaF;

        salidaF.open("stock.txt" , ios::trunc );

        if(salidaF.good())
        {
            salidaF << visorPlain << endl;
        }

        salidaF.close();
    }
}

void MainWindow::on_limpiarDatosStock_clicked() // Limpia los datos en el plainEditText.
{
    ui->stockplainTextEdit->clear();
}

void MainWindow::on_volverRepositorLogpushButton_clicked()
{
   ui->stockplainTextEdit->clear();
   ui->widgetRepositor->hide();
   ui->widgetLogin->show();
   ui->usuariolineEdit->clear();
   ui->contrasenalineEdit->clear();
   ui->usuariolineEdit->setFocus();

}
//////////////////////////////////////////////////////////////////////////
// Botones del widget del expendor.

void MainWindow::on_topeDePan_clicked()
{
    string ingredientes[80];
    string renglon;
    string cantidadStr;
    string posibleCodigoStr;
    int control = 0;
    int posibleCodigoInt;
    int cantidadInt;
    int codigo = 10001; // Codigo del producto.
    int contL = 0;      // contador para iterar el vector en el fichero de modo lectura.
    int a;              // Variable utilizada para convertir de numero a string;
    bool encontrado = false; // Para que rompa el ciclo en caso de ser encontrado en el paso 2.

    // Paso 1
    // Lectura del fichero y guardarlo en el vector.
    ifstream entradaF;
    entradaF.open("stock.txt");
    if(entradaF.good())
    {
        getline(entradaF , renglon);

        while(!entradaF.eof())
        {

            ingredientes[contL] = renglon;
            contL++;
            getline(entradaF , renglon);

        }
    }

    entradaF.close();

    //////////////////////////////////////////////////////////////////////////
    // Paso 2.
    // Modificar la cantidad de stock en el vector.

    for(int i=0 ; i<80 && !encontrado ; i++)
    {
        posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
        posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

        if(posibleCodigoInt == codigo)
        {
            i++;
            cantidadStr = ingredientes[i];
            cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

            if(cantidadInt != 0)
            {
                cantidadInt--;  // Disminucion de la cantidad del ingrediente.
                a = cantidadInt; // Almacenarlo en la variable a.
                stringstream ss;  // Entero a string.
                ss << a;
                string str = ss.str();
                ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
                encontrado = true; // Convertirlo de estado para romper el ciclo.
            }
            else
            {
                control = 1;
                QMessageBox::warning(NULL ,"       Advertencia             ", "   Sin stock: Tope de pan.   ");
            }
        }

    }

    ///////////////////////////////////////////////////////////
    // Paso 3.
    // Pasar los datos del vector al fichero.

    ofstream salidaF;
    salidaF.open("stock.txt");
    if(salidaF.good())
    {
        for(int i=0 ; i<80 ; i++)
        {
            salidaF << ingredientes[i] << endl;
        }
    }

    salidaF.close();

    /////////////////////////////////////////////////////////////////
    // Encontramos el precio del producto.

    contador++;

    string ingrediente , cantidad , codigo2, precio , separador , renglon2;
    ifstream entrada2;
    string posibleCod = "";
    string precioStr;
    int posibleCodInt = 0;
    int precioInt;
    int codigoVerif = 10001;
    bool buscar = false;

    entrada2.open("stock.txt");
    if(entrada2.good())
    {
        getline(entrada2 , renglon2);

        while(!entrada2.eof() && !buscar)
        {
            ingrediente = renglon2;   // Lectura de grupos de 5 renglones.
            getline(entrada2 , renglon2);
            codigo2 = renglon2;
            getline(entrada2 , renglon2);
            cantidad = renglon2;
            getline(entrada2 , renglon2);
            precio = renglon2;
            getline(entrada2 , renglon2);
            separador = renglon2;


            for(unsigned int i=0 ; i<codigo2.length() ; i++) // Eliminar los espacion del string llamado codigo.
            {
                posibleCod = posibleCod + codigo2.at(i);
            }

            posibleCodInt = atoi(posibleCod.c_str()); // Convertir de string a entero.
            posibleCod = ""; // Limpiamos el string para los proximos datos.

            if( posibleCodInt == codigoVerif) // Verificacion si existe el codigo.
            {
                buscar = true;
            }
            else
            {
                getline(entrada2 , renglon2);
            }
        }

    }

    entrada2.close();

    for(unsigned int i=0 ; i<precio.length() ; i++) // Eliminamos el caracter de $
    {
        if(precio.at(i) != '$')
        {
            precioStr = precioStr + precio.at(i);
        }

    }

    precioInt = atoi(precioStr.c_str()); // Convertimos de string a entero.

    int producto = precioInt;

    QString prodqt = QString::number(producto);

    if(control !=1)
    {
        ui->expendedorplainTextEdit->appendPlainText(prodqt +"$ : Tope de pan.");
    }

    ui->limpiarVisorExpendedor->setEnabled(false);
}
void MainWindow::on_discoCentralDePan_clicked()
{
    string ingredientes[80];
        string renglon;
        string cantidadStr;
        string posibleCodigoStr;
        int control = 0;
        int posibleCodigoInt;
        int cantidadInt;
        int codigo = 10002; // Codigo del producto.
        int contL = 0;      // contador para iterar el vector en el fichero de modo lectura.
        int a;              // Variable utilizada para convertir de numero a string;
        bool encontrado = false; // Para que rompa el ciclo en caso de ser encontrado en el paso 2.

        // Paso 1
        // Lectura del fichero y guardarlo en el vector.
        ifstream entradaF;
        entradaF.open("stock.txt");
        if(entradaF.good())
        {
            getline(entradaF , renglon);

            while(!entradaF.eof())
            {

                ingredientes[contL] = renglon;
                contL++;
                getline(entradaF , renglon);

            }
        }

        entradaF.close();

        //////////////////////////////////////////////////////////////////////////
        // Paso 2.
        // Modificar la cantidad de stock en el vector.

        for(int i=0 ; i<80 && !encontrado ; i++)
        {
            posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
            posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

            if(posibleCodigoInt == codigo)
            {
                i++;
                cantidadStr = ingredientes[i];
                cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                if(cantidadInt != 0)
                {
                    cantidadInt--;  // Disminucion de la cantidad del ingrediente.
                    a = cantidadInt; // Almacenarlo en la variable a.
                    stringstream ss;  // Entero a string.
                    ss << a;
                    string str = ss.str();
                    ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
                    encontrado = true; // Convertirlo de estado para romper el ciclo.
                }
                else
                {
                    control = 1;
                    QMessageBox::warning(NULL ,"       Advertencia             ", "   Sin stock: Disco central de pan.    ");
                }
            }

        }

        ///////////////////////////////////////////////////////////
        // Paso 3.
        // Pasar los datos del vector al fichero.

        ofstream salidaF;
        salidaF.open("stock.txt");
        if(salidaF.good())
        {
            for(int i=0 ; i<80 ; i++)
            {
                salidaF << ingredientes[i] << endl;
            }
        }

        salidaF.close();

        /////////////////////////////////////////////////////////////////
        // Encontramos el precio del producto.

        contador++;

        string ingrediente , cantidad , codigo2, precio , separador , renglon2;
        ifstream entrada2;
        string posibleCod = "";
        string precioStr;
        int posibleCodInt = 0;
        int precioInt;
        int codigoVerif = 10002;
        bool buscar = false;

        entrada2.open("stock.txt");
        if(entrada2.good())
        {
            getline(entrada2 , renglon2);

            while(!entrada2.eof() && !buscar)
            {
                ingrediente = renglon2;   // Lectura de grupos de 5 renglones.
                getline(entrada2 , renglon2);
                codigo2 = renglon2;
                getline(entrada2 , renglon2);
                cantidad = renglon2;
                getline(entrada2 , renglon2);
                precio = renglon2;
                getline(entrada2 , renglon2);
                separador = renglon2;


                for(unsigned int i=0 ; i<codigo2.length() ; i++) // Eliminar los espacion del string llamado codigo.
                {
                    posibleCod = posibleCod + codigo2.at(i);
                }

                posibleCodInt = atoi(posibleCod.c_str()); // Convertir de string a entero.
                posibleCod = ""; // Limpiamos el string para los proximos datos.

                if( posibleCodInt == codigoVerif) // Verificacion si existe el codigo.
                {
                    buscar = true;
                }
                else
                {
                    getline(entrada2 , renglon2);
                }
            }

        }

        entrada2.close();

        for(unsigned int i=0 ; i<precio.length() ; i++) // Eliminamos el caracter de $
        {
            if(precio.at(i) != '$')
            {
                precioStr = precioStr + precio.at(i);
            }

        }

        precioInt = atoi(precioStr.c_str()); // Convertimos de string a entero.

        int producto = precioInt;

        QString prodqt = QString::number(producto);

        if(control !=1)
        {
            ui->expendedorplainTextEdit->appendPlainText(prodqt +"$ : Disco central de pan.");
        }

        ui->limpiarVisorExpendedor->setEnabled(false);
}

void MainWindow::on_baseDePan_clicked()
{
    string ingredientes[80];
        string renglon;
        string cantidadStr;
        string posibleCodigoStr;
        int control = 0;
        int posibleCodigoInt;
        int cantidadInt;
        int codigo = 10003; // Codigo del producto.
        int contL = 0;      // contador para iterar el vector en el fichero de modo lectura.
        int a;              // Variable utilizada para convertir de numero a string;
        bool encontrado = false; // Para que rompa el ciclo en caso de ser encontrado en el paso 2.

        // Paso 1
        // Lectura del fichero y guardarlo en el vector.
        ifstream entradaF;
        entradaF.open("stock.txt");
        if(entradaF.good())
        {
            getline(entradaF , renglon);

            while(!entradaF.eof())
            {

                ingredientes[contL] = renglon;
                contL++;
                getline(entradaF , renglon);

            }
        }

        entradaF.close();

        //////////////////////////////////////////////////////////////////////////
        // Paso 2.
        // Modificar la cantidad de stock en el vector.

        for(int i=0 ; i<80 && !encontrado ; i++)
        {
            posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
            posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

            if(posibleCodigoInt == codigo)
            {
                i++;
                cantidadStr = ingredientes[i];
                cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                if(cantidadInt != 0)
                {
                    cantidadInt--;  // Disminucion de la cantidad del ingrediente.
                    a = cantidadInt; // Almacenarlo en la variable a.
                    stringstream ss;  // Entero a string.
                    ss << a;
                    string str = ss.str();
                    ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
                    encontrado = true; // Convertirlo de estado para romper el ciclo.
                }
                else
                {
                    control = 1;
                    QMessageBox::warning(NULL ,"       Advertencia             ", "   Sin stock: Base de pan.   ");
                }
            }

        }

        ///////////////////////////////////////////////////////////
        // Paso 3.
        // Pasar los datos del vector al fichero.

        ofstream salidaF;
        salidaF.open("stock.txt");
        if(salidaF.good())
        {
            for(int i=0 ; i<80 ; i++)
            {
                salidaF << ingredientes[i] << endl;
            }
        }

        salidaF.close();

        /////////////////////////////////////////////////////////////////
        // Encontramos el precio del producto.

        contador++;

        string ingrediente , cantidad , codigo2, precio , separador , renglon2;
        ifstream entrada2;
        string posibleCod = "";
        string precioStr;
        int posibleCodInt = 0;
        int precioInt;
        int codigoVerif = 10003;
        bool buscar = false;

        entrada2.open("stock.txt");
        if(entrada2.good())
        {
            getline(entrada2 , renglon2);

            while(!entrada2.eof() && !buscar)
            {
                ingrediente = renglon2;   // Lectura de grupos de 5 renglones.
                getline(entrada2 , renglon2);
                codigo2 = renglon2;
                getline(entrada2 , renglon2);
                cantidad = renglon2;
                getline(entrada2 , renglon2);
                precio = renglon2;
                getline(entrada2 , renglon2);
                separador = renglon2;


                for(unsigned int i=0 ; i<codigo2.length() ; i++) // Eliminar los espacion del string llamado codigo.
                {
                    posibleCod = posibleCod + codigo2.at(i);
                }

                posibleCodInt = atoi(posibleCod.c_str()); // Convertir de string a entero.
                posibleCod = ""; // Limpiamos el string para los proximos datos.

                if( posibleCodInt == codigoVerif) // Verificacion si existe el codigo.
                {
                    buscar = true;
                }
                else
                {
                    getline(entrada2 , renglon2);
                }
            }

        }

        entrada2.close();

        for(unsigned int i=0 ; i<precio.length() ; i++) // Eliminamos el caracter de $
        {
            if(precio.at(i) != '$')
            {
                precioStr = precioStr + precio.at(i);
            }

        }

        precioInt = atoi(precioStr.c_str()); // Convertimos de string a entero.

        int producto = precioInt;

        QString prodqt = QString::number(producto);

        if(control !=1)
        {
            ui->expendedorplainTextEdit->appendPlainText(prodqt +"$ : Base de pan.");
        }

        ui->limpiarVisorExpendedor->setEnabled(false);
}
void MainWindow::on_hamburguesaDeCarne_clicked()
{
    string ingredientes[80];
        string renglon;
        string cantidadStr;
        string posibleCodigoStr;
        int control = 0;
        int posibleCodigoInt;
        int cantidadInt;
        int codigo = 10004; // Codigo del producto.
        int contL = 0;      // contador para iterar el vector en el fichero de modo lectura.
        int a;              // Variable utilizada para convertir de numero a string;
        bool encontrado = false; // Para que rompa el ciclo en caso de ser encontrado en el paso 2.

        // Paso 1
        // Lectura del fichero y guardarlo en el vector.
        ifstream entradaF;
        entradaF.open("stock.txt");
        if(entradaF.good())
        {
            getline(entradaF , renglon);

            while(!entradaF.eof())
            {

                ingredientes[contL] = renglon;
                contL++;
                getline(entradaF , renglon);

            }
        }

        entradaF.close();

        //////////////////////////////////////////////////////////////////////////
        // Paso 2.
        // Modificar la cantidad de stock en el vector.

        for(int i=0 ; i<80 && !encontrado ; i++)
        {
            posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
            posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

            if(posibleCodigoInt == codigo)
            {
                i++;
                cantidadStr = ingredientes[i];
                cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                if(cantidadInt != 0)
                {
                    cantidadInt--;  // Disminucion de la cantidad del ingrediente.
                    a = cantidadInt; // Almacenarlo en la variable a.
                    stringstream ss;  // Entero a string.
                    ss << a;
                    string str = ss.str();
                    ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
                    encontrado = true; // Convertirlo de estado para romper el ciclo.
                }
                else
                {
                    control = 1;
                    QMessageBox::warning(NULL ,"       Advertencia             ", "   Sin stock: Hamburguesa de carne.    ");
                }
            }

        }

        ///////////////////////////////////////////////////////////
        // Paso 3.
        // Pasar los datos del vector al fichero.

        ofstream salidaF;
        salidaF.open("stock.txt");
        if(salidaF.good())
        {
            for(int i=0 ; i<80 ; i++)
            {
                salidaF << ingredientes[i] << endl;
            }
        }

        salidaF.close();

        /////////////////////////////////////////////////////////////////
        // Encontramos el precio del producto.

        contador++;

        string ingrediente , cantidad , codigo2, precio , separador , renglon2;
        ifstream entrada2;
        string posibleCod = "";
        string precioStr;
        int posibleCodInt = 0;
        int precioInt;
        int codigoVerif = 10004;
        bool buscar = false;

        entrada2.open("stock.txt");
        if(entrada2.good())
        {
            getline(entrada2 , renglon2);

            while(!entrada2.eof() && !buscar)
            {
                ingrediente = renglon2;   // Lectura de grupos de 5 renglones.
                getline(entrada2 , renglon2);
                codigo2 = renglon2;
                getline(entrada2 , renglon2);
                cantidad = renglon2;
                getline(entrada2 , renglon2);
                precio = renglon2;
                getline(entrada2 , renglon2);
                separador = renglon2;


                for(unsigned int i=0 ; i<codigo2.length() ; i++) // Eliminar los espacion del string llamado codigo.
                {
                    posibleCod = posibleCod + codigo2.at(i);
                }

                posibleCodInt = atoi(posibleCod.c_str()); // Convertir de string a entero.
                posibleCod = ""; // Limpiamos el string para los proximos datos.

                if( posibleCodInt == codigoVerif) // Verificacion si existe el codigo.
                {
                    buscar = true;
                }
                else
                {
                    getline(entrada2 , renglon2);
                }
            }

        }

        entrada2.close();

        for(unsigned int i=0 ; i<precio.length() ; i++) // Eliminamos el caracter de $
        {
            if(precio.at(i) != '$')
            {
                precioStr = precioStr + precio.at(i);
            }

        }

        precioInt = atoi(precioStr.c_str()); // Convertimos de string a entero.

        int producto = precioInt;

        QString prodqt = QString::number(producto);

        if(control !=1)
        {
            ui->expendedorplainTextEdit->appendPlainText(prodqt +"$ : Hamburguesa de carne");
        }

        ui->limpiarVisorExpendedor->setEnabled(false);
}

void MainWindow::on_hamburguesaDePollo_clicked()
{
    string ingredientes[80];
        string renglon;
        string cantidadStr;
        string posibleCodigoStr;
        int control = 0;
        int posibleCodigoInt;
        int cantidadInt;
        int codigo = 10005; // Codigo del producto.
        int contL = 0;      // contador para iterar el vector en el fichero de modo lectura.
        int a;              // Variable utilizada para convertir de numero a string;
        bool encontrado = false; // Para que rompa el ciclo en caso de ser encontrado en el paso 2.

        // Paso 1
        // Lectura del fichero y guardarlo en el vector.
        ifstream entradaF;
        entradaF.open("stock.txt");
        if(entradaF.good())
        {
            getline(entradaF , renglon);

            while(!entradaF.eof())
            {

                ingredientes[contL] = renglon;
                contL++;
                getline(entradaF , renglon);

            }
        }

        entradaF.close();

        //////////////////////////////////////////////////////////////////////////
        // Paso 2.
        // Modificar la cantidad de stock en el vector.

        for(int i=0 ; i<80 && !encontrado ; i++)
        {
            posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
            posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

            if(posibleCodigoInt == codigo)
            {
                i++;
                cantidadStr = ingredientes[i];
                cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                if(cantidadInt != 0)
                {
                    cantidadInt--;  // Disminucion de la cantidad del ingrediente.
                    a = cantidadInt; // Almacenarlo en la variable a.
                    stringstream ss;  // Entero a string.
                    ss << a;
                    string str = ss.str();
                    ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
                    encontrado = true; // Convertirlo de estado para romper el ciclo.
                }
                else
                {
                    control = 1;
                    QMessageBox::warning(NULL ,"       Advertencia             ", "   Sin stock: Hamburguesa de pollo.    ");
                }
            }

        }

        ///////////////////////////////////////////////////////////
        // Paso 3.
        // Pasar los datos del vector al fichero.

        ofstream salidaF;
        salidaF.open("stock.txt");
        if(salidaF.good())
        {
            for(int i=0 ; i<80 ; i++)
            {
                salidaF << ingredientes[i] << endl;
            }
        }

        salidaF.close();

        /////////////////////////////////////////////////////////////////
        // Encontramos el precio del producto.

        contador++;

        string ingrediente , cantidad , codigo2, precio , separador , renglon2;
        ifstream entrada2;
        string posibleCod = "";
        string precioStr;
        int posibleCodInt = 0;
        int precioInt;
        int codigoVerif = 10005;
        bool buscar = false;

        entrada2.open("stock.txt");
        if(entrada2.good())
        {
            getline(entrada2 , renglon2);

            while(!entrada2.eof() && !buscar)
            {
                ingrediente = renglon2;   // Lectura de grupos de 5 renglones.
                getline(entrada2 , renglon2);
                codigo2 = renglon2;
                getline(entrada2 , renglon2);
                cantidad = renglon2;
                getline(entrada2 , renglon2);
                precio = renglon2;
                getline(entrada2 , renglon2);
                separador = renglon2;


                for(unsigned int i=0 ; i<codigo2.length() ; i++) // Eliminar los espacion del string llamado codigo.
                {
                    posibleCod = posibleCod + codigo2.at(i);
                }

                posibleCodInt = atoi(posibleCod.c_str()); // Convertir de string a entero.
                posibleCod = ""; // Limpiamos el string para los proximos datos.

                if( posibleCodInt == codigoVerif) // Verificacion si existe el codigo.
                {
                    buscar = true;
                }
                else
                {
                    getline(entrada2 , renglon2);
                }
            }

        }

        entrada2.close();

        for(unsigned int i=0 ; i<precio.length() ; i++) // Eliminamos el caracter de $
        {
            if(precio.at(i) != '$')
            {
                precioStr = precioStr + precio.at(i);
            }

        }

        precioInt = atoi(precioStr.c_str()); // Convertimos de string a entero.

        int producto = precioInt;

        QString prodqt = QString::number(producto);

        if(control !=1)
        {
            ui->expendedorplainTextEdit->appendPlainText(prodqt +"$ : Hamburguesa de pollo.");
        }

        ui->limpiarVisorExpendedor->setEnabled(false);
}

void MainWindow::on_hamburguesaDePescado_clicked()
{
    string ingredientes[80];
        string renglon;
        string cantidadStr;
        string posibleCodigoStr;
        int control = 0;
        int posibleCodigoInt;
        int cantidadInt;
        int codigo = 10006; // Codigo del producto.
        int contL = 0;      // contador para iterar el vector en el fichero de modo lectura.
        int a;              // Variable utilizada para convertir de numero a string;
        bool encontrado = false; // Para que rompa el ciclo en caso de ser encontrado en el paso 2.

        // Paso 1
        // Lectura del fichero y guardarlo en el vector.
        ifstream entradaF;
        entradaF.open("stock.txt");
        if(entradaF.good())
        {
            getline(entradaF , renglon);

            while(!entradaF.eof())
            {

                ingredientes[contL] = renglon;
                contL++;
                getline(entradaF , renglon);

            }
        }

        entradaF.close();

        //////////////////////////////////////////////////////////////////////////
        // Paso 2.
        // Modificar la cantidad de stock en el vector.

        for(int i=0 ; i<80 && !encontrado ; i++)
        {
            posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
            posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

            if(posibleCodigoInt == codigo)
            {
                i++;
                cantidadStr = ingredientes[i];
                cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                if(cantidadInt != 0)
                {
                    cantidadInt--;  // Disminucion de la cantidad del ingrediente.
                    a = cantidadInt; // Almacenarlo en la variable a.
                    stringstream ss;  // Entero a string.
                    ss << a;
                    string str = ss.str();
                    ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
                    encontrado = true; // Convertirlo de estado para romper el ciclo.
                }
                else
                {
                    control = 1;
                    QMessageBox::warning(NULL ,"       Advertencia             ", "   Sin stock: Hamburguesa de pescado.    ");
                }
            }

        }

        ///////////////////////////////////////////////////////////
        // Paso 3.
        // Pasar los datos del vector al fichero.

        ofstream salidaF;
        salidaF.open("stock.txt");
        if(salidaF.good())
        {
            for(int i=0 ; i<80 ; i++)
            {
                salidaF << ingredientes[i] << endl;
            }
        }

        salidaF.close();

        /////////////////////////////////////////////////////////////////
        // Encontramos el precio del producto.

        contador++;

        string ingrediente , cantidad , codigo2, precio , separador , renglon2;
        ifstream entrada2;
        string posibleCod = "";
        string precioStr;
        int posibleCodInt = 0;
        int precioInt;
        int codigoVerif = 10006;
        bool buscar = false;

        entrada2.open("stock.txt");
        if(entrada2.good())
        {
            getline(entrada2 , renglon2);

            while(!entrada2.eof() && !buscar)
            {
                ingrediente = renglon2;   // Lectura de grupos de 5 renglones.
                getline(entrada2 , renglon2);
                codigo2 = renglon2;
                getline(entrada2 , renglon2);
                cantidad = renglon2;
                getline(entrada2 , renglon2);
                precio = renglon2;
                getline(entrada2 , renglon2);
                separador = renglon2;


                for(unsigned int i=0 ; i<codigo2.length() ; i++) // Eliminar los espacion del string llamado codigo.
                {
                    posibleCod = posibleCod + codigo2.at(i);
                }

                posibleCodInt = atoi(posibleCod.c_str()); // Convertir de string a entero.
                posibleCod = ""; // Limpiamos el string para los proximos datos.

                if( posibleCodInt == codigoVerif) // Verificacion si existe el codigo.
                {
                    buscar = true;
                }
                else
                {
                    getline(entrada2 , renglon2);
                }
            }

        }

        entrada2.close();

        for(unsigned int i=0 ; i<precio.length() ; i++) // Eliminamos el caracter de $
        {
            if(precio.at(i) != '$')
            {
                precioStr = precioStr + precio.at(i);
            }

        }

        precioInt = atoi(precioStr.c_str()); // Convertimos de string a entero.

        int producto = precioInt;

        QString prodqt = QString::number(producto);

        if(control !=1)
        {
            ui->expendedorplainTextEdit->appendPlainText(prodqt +"$ : Hamburguesa de pescado.");
        }

        ui->limpiarVisorExpendedor->setEnabled(false);
}

void MainWindow::on_huevoFrito_clicked()
{
    string ingredientes[80];
        string renglon;
        string cantidadStr;
        string posibleCodigoStr;
        int control = 0;
        int posibleCodigoInt;
        int cantidadInt;
        int codigo = 10007; // Codigo del producto.
        int contL = 0;      // contador para iterar el vector en el fichero de modo lectura.
        int a;              // Variable utilizada para convertir de numero a string;
        bool encontrado = false; // Para que rompa el ciclo en caso de ser encontrado en el paso 2.

        // Paso 1
        // Lectura del fichero y guardarlo en el vector.
        ifstream entradaF;
        entradaF.open("stock.txt");
        if(entradaF.good())
        {
            getline(entradaF , renglon);

            while(!entradaF.eof())
            {

                ingredientes[contL] = renglon;
                contL++;
                getline(entradaF , renglon);

            }
        }

        entradaF.close();

        //////////////////////////////////////////////////////////////////////////
        // Paso 2.
        // Modificar la cantidad de stock en el vector.

        for(int i=0 ; i<80 && !encontrado ; i++)
        {
            posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
            posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

            if(posibleCodigoInt == codigo)
            {
                i++;
                cantidadStr = ingredientes[i];
                cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                if(cantidadInt != 0)
                {
                    cantidadInt--;  // Disminucion de la cantidad del ingrediente.
                    a = cantidadInt; // Almacenarlo en la variable a.
                    stringstream ss;  // Entero a string.
                    ss << a;
                    string str = ss.str();
                    ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
                    encontrado = true; // Convertirlo de estado para romper el ciclo.
                }
                else
                {
                    control = 1;
                    QMessageBox::warning(NULL ,"       Advertencia             ", "   Sin stock: Huevo frito.    ");
                }
            }

        }

        ///////////////////////////////////////////////////////////
        // Paso 3.
        // Pasar los datos del vector al fichero.

        ofstream salidaF;
        salidaF.open("stock.txt");
        if(salidaF.good())
        {
            for(int i=0 ; i<80 ; i++)
            {
                salidaF << ingredientes[i] << endl;
            }
        }

        salidaF.close();

        /////////////////////////////////////////////////////////////////
        // Encontramos el precio del producto.

        contador++;

        string ingrediente , cantidad , codigo2, precio , separador , renglon2;
        ifstream entrada2;
        string posibleCod = "";
        string precioStr;
        int posibleCodInt = 0;
        int precioInt;
        int codigoVerif = 10007;
        bool buscar = false;

        entrada2.open("stock.txt");
        if(entrada2.good())
        {
            getline(entrada2 , renglon2);

            while(!entrada2.eof() && !buscar)
            {
                ingrediente = renglon2;   // Lectura de grupos de 5 renglones.
                getline(entrada2 , renglon2);
                codigo2 = renglon2;
                getline(entrada2 , renglon2);
                cantidad = renglon2;
                getline(entrada2 , renglon2);
                precio = renglon2;
                getline(entrada2 , renglon2);
                separador = renglon2;


                for(unsigned int i=0 ; i<codigo2.length() ; i++) // Eliminar los espacion del string llamado codigo.
                {
                    posibleCod = posibleCod + codigo2.at(i);
                }

                posibleCodInt = atoi(posibleCod.c_str()); // Convertir de string a entero.
                posibleCod = ""; // Limpiamos el string para los proximos datos.

                if( posibleCodInt == codigoVerif) // Verificacion si existe el codigo.
                {
                    buscar = true;
                }
                else
                {
                    getline(entrada2 , renglon2);
                }
            }

        }

        entrada2.close();

        for(unsigned int i=0 ; i<precio.length() ; i++) // Eliminamos el caracter de $
        {
            if(precio.at(i) != '$')
            {
                precioStr = precioStr + precio.at(i);
            }

        }

        precioInt = atoi(precioStr.c_str()); // Convertimos de string a entero.

        int producto = precioInt;

        QString prodqt = QString::number(producto);

        if(control !=1)
        {
            ui->expendedorplainTextEdit->appendPlainText(prodqt +"$ : Huevo frito");
        }

        ui->limpiarVisorExpendedor->setEnabled(false);
}

void MainWindow::on_fetaDeQueso_clicked()
{
    string ingredientes[80];
        string renglon;
        string cantidadStr;
        string posibleCodigoStr;
        int control = 0;
        int posibleCodigoInt;
        int cantidadInt;
        int codigo = 10008; // Codigo del producto.
        int contL = 0;      // contador para iterar el vector en el fichero de modo lectura.
        int a;              // Variable utilizada para convertir de numero a string;
        bool encontrado = false; // Para que rompa el ciclo en caso de ser encontrado en el paso 2.

        // Paso 1
        // Lectura del fichero y guardarlo en el vector.
        ifstream entradaF;
        entradaF.open("stock.txt");
        if(entradaF.good())
        {
            getline(entradaF , renglon);

            while(!entradaF.eof())
            {

                ingredientes[contL] = renglon;
                contL++;
                getline(entradaF , renglon);

            }
        }

        entradaF.close();

        //////////////////////////////////////////////////////////////////////////
        // Paso 2.
        // Modificar la cantidad de stock en el vector.

        for(int i=0 ; i<80 && !encontrado ; i++)
        {
            posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
            posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

            if(posibleCodigoInt == codigo)
            {
                i++;
                cantidadStr = ingredientes[i];
                cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                if(cantidadInt != 0)
                {
                    cantidadInt--;  // Disminucion de la cantidad del ingrediente.
                    a = cantidadInt; // Almacenarlo en la variable a.
                    stringstream ss;  // Entero a string.
                    ss << a;
                    string str = ss.str();
                    ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
                    encontrado = true; // Convertirlo de estado para romper el ciclo.
                }
                else
                {
                    control = 1;
                    QMessageBox::warning(NULL ,"       Advertencia             ", "   Sin stock: Feta de queso.    ");
                }
            }

        }

        ///////////////////////////////////////////////////////////
        // Paso 3.
        // Pasar los datos del vector al fichero.

        ofstream salidaF;
        salidaF.open("stock.txt");
        if(salidaF.good())
        {
            for(int i=0 ; i<80 ; i++)
            {
                salidaF << ingredientes[i] << endl;
            }
        }

        salidaF.close();

        /////////////////////////////////////////////////////////////////
        // Encontramos el precio del producto.

        contador++;

        string ingrediente , cantidad , codigo2, precio , separador , renglon2;
        ifstream entrada2;
        string posibleCod = "";
        string precioStr;
        int posibleCodInt = 0;
        int precioInt;
        int codigoVerif = 10008;
        bool buscar = false;

        entrada2.open("stock.txt");
        if(entrada2.good())
        {
            getline(entrada2 , renglon2);

            while(!entrada2.eof() && !buscar)
            {
                ingrediente = renglon2;   // Lectura de grupos de 5 renglones.
                getline(entrada2 , renglon2);
                codigo2 = renglon2;
                getline(entrada2 , renglon2);
                cantidad = renglon2;
                getline(entrada2 , renglon2);
                precio = renglon2;
                getline(entrada2 , renglon2);
                separador = renglon2;


                for(unsigned int i=0 ; i<codigo2.length() ; i++) // Eliminar los espacion del string llamado codigo.
                {
                    posibleCod = posibleCod + codigo2.at(i);
                }

                posibleCodInt = atoi(posibleCod.c_str()); // Convertir de string a entero.
                posibleCod = ""; // Limpiamos el string para los proximos datos.

                if( posibleCodInt == codigoVerif) // Verificacion si existe el codigo.
                {
                    buscar = true;
                }
                else
                {
                    getline(entrada2 , renglon2);
                }
            }

        }

        entrada2.close();

        for(unsigned int i=0 ; i<precio.length() ; i++) // Eliminamos el caracter de $
        {
            if(precio.at(i) != '$')
            {
                precioStr = precioStr + precio.at(i);
            }

        }

        precioInt = atoi(precioStr.c_str()); // Convertimos de string a entero.

        int producto = precioInt;

        QString prodqt = QString::number(producto);

        if(control !=1)
        {
            ui->expendedorplainTextEdit->appendPlainText(prodqt +"$ : Feta de queso");
        }

        ui->limpiarVisorExpendedor->setEnabled(false);
}

void MainWindow::on_porcionDeTomatePicado_clicked()
{
        string ingredientes[80];
        string renglon;
        string cantidadStr;
        string posibleCodigoStr;
        int control = 0;
        int posibleCodigoInt;
        int cantidadInt;
        int codigo = 10009; // Codigo del producto.
        int contL = 0;      // contador para iterar el vector en el fichero de modo lectura.
        int a;              // Variable utilizada para convertir de numero a string;
        bool encontrado = false; // Para que rompa el ciclo en caso de ser encontrado en el paso 2.

        // Paso 1
        // Lectura del fichero y guardarlo en el vector.
        ifstream entradaF;
        entradaF.open("stock.txt");
        if(entradaF.good())
        {
            getline(entradaF , renglon);

            while(!entradaF.eof())
            {

                ingredientes[contL] = renglon;
                contL++;
                getline(entradaF , renglon);

            }
        }

        entradaF.close();

        //////////////////////////////////////////////////////////////////////////
        // Paso 2.
        // Modificar la cantidad de stock en el vector.

        for(int i=0 ; i<80 && !encontrado ; i++)
        {
            posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
            posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

            if(posibleCodigoInt == codigo)
            {
                i++;
                cantidadStr = ingredientes[i];
                cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                if(cantidadInt != 0)
                {
                    cantidadInt--;  // Disminucion de la cantidad del ingrediente.
                    a = cantidadInt; // Almacenarlo en la variable a.
                    stringstream ss;  // Entero a string.
                    ss << a;
                    string str = ss.str();
                    ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
                    encontrado = true; // Convertirlo de estado para romper el ciclo.
                }
                else
                {
                    control = 1;
                    QMessageBox::warning(NULL ,"       Advertencia             ", "   Sin stock: Porcion de tomate picado.    ");
                }
            }

        }

        ///////////////////////////////////////////////////////////
        // Paso 3.
        // Pasar los datos del vector al fichero.

        ofstream salidaF;
        salidaF.open("stock.txt");
        if(salidaF.good())
        {
            for(int i=0 ; i<80 ; i++)
            {
                salidaF << ingredientes[i] << endl;
            }
        }

        salidaF.close();

        /////////////////////////////////////////////////////////////////
        // Encontramos el precio del producto.

        contador++;

        string ingrediente , cantidad , codigo2, precio , separador , renglon2;
        ifstream entrada2;
        string posibleCod = "";
        string precioStr;
        int posibleCodInt = 0;
        int precioInt;
        int codigoVerif = 10009;
        bool buscar = false;

        entrada2.open("stock.txt");
        if(entrada2.good())
        {
            getline(entrada2 , renglon2);

            while(!entrada2.eof() && !buscar)
            {
                ingrediente = renglon2;   // Lectura de grupos de 5 renglones.
                getline(entrada2 , renglon2);
                codigo2 = renglon2;
                getline(entrada2 , renglon2);
                cantidad = renglon2;
                getline(entrada2 , renglon2);
                precio = renglon2;
                getline(entrada2 , renglon2);
                separador = renglon2;


                for(unsigned int i=0 ; i<codigo2.length() ; i++) // Eliminar los espacion del string llamado codigo.
                {
                    posibleCod = posibleCod + codigo2.at(i);
                }

                posibleCodInt = atoi(posibleCod.c_str()); // Convertir de string a entero.
                posibleCod = ""; // Limpiamos el string para los proximos datos.

                if( posibleCodInt == codigoVerif) // Verificacion si existe el codigo.
                {
                    buscar = true;
                }
                else
                {
                    getline(entrada2 , renglon2);
                }
            }

        }

        entrada2.close();

        for(unsigned int i=0 ; i<precio.length() ; i++) // Eliminamos el caracter de $
        {
            if(precio.at(i) != '$')
            {
                precioStr = precioStr + precio.at(i);
            }

        }

        precioInt = atoi(precioStr.c_str()); // Convertimos de string a entero.

        int producto = precioInt;

        QString prodqt = QString::number(producto);

        if(control !=1)
        {
            ui->expendedorplainTextEdit->appendPlainText(prodqt +"$ : Porcion de tomate picado");
        }

        ui->limpiarVisorExpendedor->setEnabled(false);
}

void MainWindow::on_porcionDeLechugaPicada_clicked()
{
        string ingredientes[80];
        string renglon;
        string cantidadStr;
        string posibleCodigoStr;
        int control = 0;
        int posibleCodigoInt;
        int cantidadInt;
        int codigo = 10010; // Codigo del producto.
        int contL = 0;      // contador para iterar el vector en el fichero de modo lectura.
        int a;              // Variable utilizada para convertir de numero a string;
        bool encontrado = false; // Para que rompa el ciclo en caso de ser encontrado en el paso 2.

        // Paso 1
        // Lectura del fichero y guardarlo en el vector.
        ifstream entradaF;
        entradaF.open("stock.txt");
        if(entradaF.good())
        {
            getline(entradaF , renglon);

            while(!entradaF.eof())
            {

                ingredientes[contL] = renglon;
                contL++;
                getline(entradaF , renglon);

            }
        }

        entradaF.close();

        //////////////////////////////////////////////////////////////////////////
        // Paso 2.
        // Modificar la cantidad de stock en el vector.

        for(int i=0 ; i<80 && !encontrado ; i++)
        {
            posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
            posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

            if(posibleCodigoInt == codigo)
            {
                i++;
                cantidadStr = ingredientes[i];
                cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                if(cantidadInt != 0)
                {
                    cantidadInt--;  // Disminucion de la cantidad del ingrediente.
                    a = cantidadInt; // Almacenarlo en la variable a.
                    stringstream ss;  // Entero a string.
                    ss << a;
                    string str = ss.str();
                    ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
                    encontrado = true; // Convertirlo de estado para romper el ciclo.
                }
                else
                {
                    control = 1;
                    QMessageBox::warning(NULL ,"       Advertencia             ", "   Sin stock: Porcion de lechuga picada.    ");
                }
            }

        }

        ///////////////////////////////////////////////////////////
        // Paso 3.
        // Pasar los datos del vector al fichero.

        ofstream salidaF;
        salidaF.open("stock.txt");
        if(salidaF.good())
        {
            for(int i=0 ; i<80 ; i++)
            {
                salidaF << ingredientes[i] << endl;
            }
        }

        salidaF.close();

        /////////////////////////////////////////////////////////////////
        // Encontramos el precio del producto.

        contador++;

        string ingrediente , cantidad , codigo2, precio , separador , renglon2;
        ifstream entrada2;
        string posibleCod = "";
        string precioStr;
        int posibleCodInt = 0;
        int precioInt;
        int codigoVerif = 10010;
        bool buscar = false;

        entrada2.open("stock.txt");
        if(entrada2.good())
        {
            getline(entrada2 , renglon2);

            while(!entrada2.eof() && !buscar)
            {
                ingrediente = renglon2;   // Lectura de grupos de 5 renglones.
                getline(entrada2 , renglon2);
                codigo2 = renglon2;
                getline(entrada2 , renglon2);
                cantidad = renglon2;
                getline(entrada2 , renglon2);
                precio = renglon2;
                getline(entrada2 , renglon2);
                separador = renglon2;


                for(unsigned int i=0 ; i<codigo2.length() ; i++) // Eliminar los espacion del string llamado codigo.
                {
                    posibleCod = posibleCod + codigo2.at(i);
                }

                posibleCodInt = atoi(posibleCod.c_str()); // Convertir de string a entero.
                posibleCod = ""; // Limpiamos el string para los proximos datos.

                if( posibleCodInt == codigoVerif) // Verificacion si existe el codigo.
                {
                    buscar = true;
                }
                else
                {
                    getline(entrada2 , renglon2);
                }
            }

        }

        entrada2.close();

        for(unsigned int i=0 ; i<precio.length() ; i++) // Eliminamos el caracter de $
        {
            if(precio.at(i) != '$')
            {
                precioStr = precioStr + precio.at(i);
            }

        }

        precioInt = atoi(precioStr.c_str()); // Convertimos de string a entero.

        int producto = precioInt;

        QString prodqt = QString::number(producto);

        if(control !=1)
        {
            ui->expendedorplainTextEdit->appendPlainText(prodqt +"$ : Porcion de lechuga picada ");
        }

        ui->limpiarVisorExpendedor->setEnabled(false);
}

void MainWindow::on_PorcionDeCebollaPicada_clicked()
{
    string ingredientes[80];
        string renglon;
        string cantidadStr;
        string posibleCodigoStr;
        int control = 0;
        int posibleCodigoInt;
        int cantidadInt;
        int codigo = 10011; // Codigo del producto.
        int contL = 0;      // contador para iterar el vector en el fichero de modo lectura.
        int a;              // Variable utilizada para convertir de numero a string;
        bool encontrado = false; // Para que rompa el ciclo en caso de ser encontrado en el paso 2.

        // Paso 1
        // Lectura del fichero y guardarlo en el vector.
        ifstream entradaF;
        entradaF.open("stock.txt");
        if(entradaF.good())
        {
            getline(entradaF , renglon);

            while(!entradaF.eof())
            {

                ingredientes[contL] = renglon;
                contL++;
                getline(entradaF , renglon);

            }
        }

        entradaF.close();

        //////////////////////////////////////////////////////////////////////////
        // Paso 2.
        // Modificar la cantidad de stock en el vector.

        for(int i=0 ; i<80 && !encontrado ; i++)
        {
            posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
            posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

            if(posibleCodigoInt == codigo)
            {
                i++;
                cantidadStr = ingredientes[i];
                cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                if(cantidadInt != 0)
                {
                    cantidadInt--;  // Disminucion de la cantidad del ingrediente.
                    a = cantidadInt; // Almacenarlo en la variable a.
                    stringstream ss;  // Entero a string.
                    ss << a;
                    string str = ss.str();
                    ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
                    encontrado = true; // Convertirlo de estado para romper el ciclo.
                }
                else
                {
                    control = 1;
                    QMessageBox::warning(NULL ,"       Advertencia             ", "   Sin stock: Porcion de cebolla picada.    ");
                }
            }

        }

        ///////////////////////////////////////////////////////////
        // Paso 3.
        // Pasar los datos del vector al fichero.

        ofstream salidaF;
        salidaF.open("stock.txt");
        if(salidaF.good())
        {
            for(int i=0 ; i<80 ; i++)
            {
                salidaF << ingredientes[i] << endl;
            }
        }

        salidaF.close();

        /////////////////////////////////////////////////////////////////
        // Encontramos el precio del producto.

        contador++;

        string ingrediente , cantidad , codigo2, precio , separador , renglon2;
        ifstream entrada2;
        string posibleCod = "";
        string precioStr;
        int posibleCodInt = 0;
        int precioInt;
        int codigoVerif = 10011;
        bool buscar = false;

        entrada2.open("stock.txt");
        if(entrada2.good())
        {
            getline(entrada2 , renglon2);

            while(!entrada2.eof() && !buscar)
            {
                ingrediente = renglon2;   // Lectura de grupos de 5 renglones.
                getline(entrada2 , renglon2);
                codigo2 = renglon2;
                getline(entrada2 , renglon2);
                cantidad = renglon2;
                getline(entrada2 , renglon2);
                precio = renglon2;
                getline(entrada2 , renglon2);
                separador = renglon2;


                for(unsigned int i=0 ; i<codigo2.length() ; i++) // Eliminar los espacion del string llamado codigo.
                {
                    posibleCod = posibleCod + codigo2.at(i);
                }

                posibleCodInt = atoi(posibleCod.c_str()); // Convertir de string a entero.
                posibleCod = ""; // Limpiamos el string para los proximos datos.

                if( posibleCodInt == codigoVerif) // Verificacion si existe el codigo.
                {
                    buscar = true;
                }
                else
                {
                    getline(entrada2 , renglon2);
                }
            }

        }

        entrada2.close();

        for(unsigned int i=0 ; i<precio.length() ; i++) // Eliminamos el caracter de $
        {
            if(precio.at(i) != '$')
            {
                precioStr = precioStr + precio.at(i);
            }

        }

        precioInt = atoi(precioStr.c_str()); // Convertimos de string a entero.

        int producto = precioInt;

        QString prodqt = QString::number(producto);

        if(control !=1)
        {
            ui->expendedorplainTextEdit->appendPlainText(prodqt +"$ : Porcion de cebolla picada");
        }

        ui->limpiarVisorExpendedor->setEnabled(false);
}

void MainWindow::on_porcionDePepinosEnConserva_clicked()
{
    string ingredientes[80];
        string renglon;
        string cantidadStr;
        string posibleCodigoStr;
        int control = 0;
        int posibleCodigoInt;
        int cantidadInt;
        int codigo = 10012; // Codigo del producto.
        int contL = 0;      // contador para iterar el vector en el fichero de modo lectura.
        int a;              // Variable utilizada para convertir de numero a string;
        bool encontrado = false; // Para que rompa el ciclo en caso de ser encontrado en el paso 2.

        // Paso 1
        // Lectura del fichero y guardarlo en el vector.
        ifstream entradaF;
        entradaF.open("stock.txt");
        if(entradaF.good())
        {
            getline(entradaF , renglon);

            while(!entradaF.eof())
            {

                ingredientes[contL] = renglon;
                contL++;
                getline(entradaF , renglon);

            }
        }

        entradaF.close();

        //////////////////////////////////////////////////////////////////////////
        // Paso 2.
        // Modificar la cantidad de stock en el vector.

        for(int i=0 ; i<80 && !encontrado ; i++)
        {
            posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
            posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

            if(posibleCodigoInt == codigo)
            {
                i++;
                cantidadStr = ingredientes[i];
                cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                if(cantidadInt != 0)
                {
                    cantidadInt--;  // Disminucion de la cantidad del ingrediente.
                    a = cantidadInt; // Almacenarlo en la variable a.
                    stringstream ss;  // Entero a string.
                    ss << a;
                    string str = ss.str();
                    ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
                    encontrado = true; // Convertirlo de estado para romper el ciclo.
                }
                else
                {
                    control = 1;
                    QMessageBox::warning(NULL ,"       Advertencia             ", "   Sin stock: Porcion de pepinos en conserva    ");
                }
            }

        }

        ///////////////////////////////////////////////////////////
        // Paso 3.
        // Pasar los datos del vector al fichero.

        ofstream salidaF;
        salidaF.open("stock.txt");
        if(salidaF.good())
        {
            for(int i=0 ; i<80 ; i++)
            {
                salidaF << ingredientes[i] << endl;
            }
        }

        salidaF.close();

        /////////////////////////////////////////////////////////////////
        // Encontramos el precio del producto.

        contador++;

        string ingrediente , cantidad , codigo2, precio , separador , renglon2;
        ifstream entrada2;
        string posibleCod = "";
        string precioStr;
        int posibleCodInt = 0;
        int precioInt;
        int codigoVerif = 10012;
        bool buscar = false;

        entrada2.open("stock.txt");
        if(entrada2.good())
        {
            getline(entrada2 , renglon2);

            while(!entrada2.eof() && !buscar)
            {
                ingrediente = renglon2;   // Lectura de grupos de 5 renglones.
                getline(entrada2 , renglon2);
                codigo2 = renglon2;
                getline(entrada2 , renglon2);
                cantidad = renglon2;
                getline(entrada2 , renglon2);
                precio = renglon2;
                getline(entrada2 , renglon2);
                separador = renglon2;


                for(unsigned int i=0 ; i<codigo2.length() ; i++) // Eliminar los espacion del string llamado codigo.
                {
                    posibleCod = posibleCod + codigo2.at(i);
                }

                posibleCodInt = atoi(posibleCod.c_str()); // Convertir de string a entero.
                posibleCod = ""; // Limpiamos el string para los proximos datos.

                if( posibleCodInt == codigoVerif) // Verificacion si existe el codigo.
                {
                    buscar = true;
                }
                else
                {
                    getline(entrada2 , renglon2);
                }
            }

        }

        entrada2.close();

        for(unsigned int i=0 ; i<precio.length() ; i++) // Eliminamos el caracter de $
        {
            if(precio.at(i) != '$')
            {
                precioStr = precioStr + precio.at(i);
            }

        }

        precioInt = atoi(precioStr.c_str()); // Convertimos de string a entero.

        int producto = precioInt;

        QString prodqt = QString::number(producto);

        if(control !=1)
        {
            ui->expendedorplainTextEdit->appendPlainText(prodqt +"$ : Porcion de pepinos en conserva");
        }

        ui->limpiarVisorExpendedor->setEnabled(false);
}

void MainWindow::on_salsaDeMayonesa_clicked()
{
    string ingredientes[80];
        string renglon;
        string cantidadStr;
        string posibleCodigoStr;
        int control = 0;
        int posibleCodigoInt;
        int cantidadInt;
        int codigo = 10013; // Codigo del producto.
        int contL = 0;      // contador para iterar el vector en el fichero de modo lectura.
        int a;              // Variable utilizada para convertir de numero a string;
        bool encontrado = false; // Para que rompa el ciclo en caso de ser encontrado en el paso 2.

        // Paso 1
        // Lectura del fichero y guardarlo en el vector.
        ifstream entradaF;
        entradaF.open("stock.txt");
        if(entradaF.good())
        {
            getline(entradaF , renglon);

            while(!entradaF.eof())
            {

                ingredientes[contL] = renglon;
                contL++;
                getline(entradaF , renglon);

            }
        }

        entradaF.close();

        //////////////////////////////////////////////////////////////////////////
        // Paso 2.
        // Modificar la cantidad de stock en el vector.

        for(int i=0 ; i<80 && !encontrado ; i++)
        {
            posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
            posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

            if(posibleCodigoInt == codigo)
            {
                i++;
                cantidadStr = ingredientes[i];
                cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                if(cantidadInt != 0)
                {
                    cantidadInt--;  // Disminucion de la cantidad del ingrediente.
                    a = cantidadInt; // Almacenarlo en la variable a.
                    stringstream ss;  // Entero a string.
                    ss << a;
                    string str = ss.str();
                    ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
                    encontrado = true; // Convertirlo de estado para romper el ciclo.
                }
                else
                {
                    control = 1;
                    QMessageBox::warning(NULL ,"       Advertencia             ", "   Sin stock: Porcion de salsa de mayonesa.    ");
                }
            }

        }

        ///////////////////////////////////////////////////////////
        // Paso 3.
        // Pasar los datos del vector al fichero.

        ofstream salidaF;
        salidaF.open("stock.txt");
        if(salidaF.good())
        {
            for(int i=0 ; i<80 ; i++)
            {
                salidaF << ingredientes[i] << endl;
            }
        }

        salidaF.close();

        /////////////////////////////////////////////////////////////////
        // Encontramos el precio del producto.

        contador++;

        string ingrediente , cantidad , codigo2, precio , separador , renglon2;
        ifstream entrada2;
        string posibleCod = "";
        string precioStr;
        int posibleCodInt = 0;
        int precioInt;
        int codigoVerif = 10013;
        bool buscar = false;

        entrada2.open("stock.txt");
        if(entrada2.good())
        {
            getline(entrada2 , renglon2);

            while(!entrada2.eof() && !buscar)
            {
                ingrediente = renglon2;   // Lectura de grupos de 5 renglones.
                getline(entrada2 , renglon2);
                codigo2 = renglon2;
                getline(entrada2 , renglon2);
                cantidad = renglon2;
                getline(entrada2 , renglon2);
                precio = renglon2;
                getline(entrada2 , renglon2);
                separador = renglon2;


                for(unsigned int i=0 ; i<codigo2.length() ; i++) // Eliminar los espacion del string llamado codigo.
                {
                    posibleCod = posibleCod + codigo2.at(i);
                }

                posibleCodInt = atoi(posibleCod.c_str()); // Convertir de string a entero.
                posibleCod = ""; // Limpiamos el string para los proximos datos.

                if( posibleCodInt == codigoVerif) // Verificacion si existe el codigo.
                {
                    buscar = true;
                }
                else
                {
                    getline(entrada2 , renglon2);
                }
            }

        }

        entrada2.close();

        for(unsigned int i=0 ; i<precio.length() ; i++) // Eliminamos el caracter de $
        {
            if(precio.at(i) != '$')
            {
                precioStr = precioStr + precio.at(i);
            }

        }

        precioInt = atoi(precioStr.c_str()); // Convertimos de string a entero.

        int producto = precioInt;

        QString prodqt = QString::number(producto);

        if(control !=1)
        {
            ui->expendedorplainTextEdit->appendPlainText(prodqt +"$ : Porcion de salsa mayonesa");
        }

        ui->limpiarVisorExpendedor->setEnabled(false);
}

void MainWindow::on_porcionDeSalsaCatsup_clicked()
{
    string ingredientes[80];
        string renglon;
        string cantidadStr;
        string posibleCodigoStr;
        int control = 0;
        int posibleCodigoInt;
        int cantidadInt;
        int codigo = 10014; // Codigo del producto.
        int contL = 0;      // contador para iterar el vector en el fichero de modo lectura.
        int a;              // Variable utilizada para convertir de numero a string;
        bool encontrado = false; // Para que rompa el ciclo en caso de ser encontrado en el paso 2.

        // Paso 1
        // Lectura del fichero y guardarlo en el vector.
        ifstream entradaF;
        entradaF.open("stock.txt");
        if(entradaF.good())
        {
            getline(entradaF , renglon);

            while(!entradaF.eof())
            {

                ingredientes[contL] = renglon;
                contL++;
                getline(entradaF , renglon);

            }
        }

        entradaF.close();

        //////////////////////////////////////////////////////////////////////////
        // Paso 2.
        // Modificar la cantidad de stock en el vector.

        for(int i=0 ; i<80 && !encontrado ; i++)
        {
            posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
            posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

            if(posibleCodigoInt == codigo)
            {
                i++;
                cantidadStr = ingredientes[i];
                cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                if(cantidadInt != 0)
                {
                    cantidadInt--;  // Disminucion de la cantidad del ingrediente.
                    a = cantidadInt; // Almacenarlo en la variable a.
                    stringstream ss;  // Entero a string.
                    ss << a;
                    string str = ss.str();
                    ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
                    encontrado = true; // Convertirlo de estado para romper el ciclo.
                }
                else
                {
                    control = 1;
                    QMessageBox::warning(NULL ,"       Advertencia             ", "   Sin stock: Porcion salsa catsup.    ");
                }
            }

        }

        ///////////////////////////////////////////////////////////
        // Paso 3.
        // Pasar los datos del vector al fichero.

        ofstream salidaF;
        salidaF.open("stock.txt");
        if(salidaF.good())
        {
            for(int i=0 ; i<80 ; i++)
            {
                salidaF << ingredientes[i] << endl;
            }
        }

        salidaF.close();

        /////////////////////////////////////////////////////////////////
        // Encontramos el precio del producto.

        contador++;

        string ingrediente , cantidad , codigo2, precio , separador , renglon2;
        ifstream entrada2;
        string posibleCod = "";
        string precioStr;
        int posibleCodInt = 0;
        int precioInt;
        int codigoVerif = 10014;
        bool buscar = false;

        entrada2.open("stock.txt");
        if(entrada2.good())
        {
            getline(entrada2 , renglon2);

            while(!entrada2.eof() && !buscar)
            {
                ingrediente = renglon2;   // Lectura de grupos de 5 renglones.
                getline(entrada2 , renglon2);
                codigo2 = renglon2;
                getline(entrada2 , renglon2);
                cantidad = renglon2;
                getline(entrada2 , renglon2);
                precio = renglon2;
                getline(entrada2 , renglon2);
                separador = renglon2;


                for(unsigned int i=0 ; i<codigo2.length() ; i++) // Eliminar los espacion del string llamado codigo.
                {
                    posibleCod = posibleCod + codigo2.at(i);
                }

                posibleCodInt = atoi(posibleCod.c_str()); // Convertir de string a entero.
                posibleCod = ""; // Limpiamos el string para los proximos datos.

                if( posibleCodInt == codigoVerif) // Verificacion si existe el codigo.
                {
                    buscar = true;
                }
                else
                {
                    getline(entrada2 , renglon2);
                }
            }

        }

        entrada2.close();

        for(unsigned int i=0 ; i<precio.length() ; i++) // Eliminamos el caracter de $
        {
            if(precio.at(i) != '$')
            {
                precioStr = precioStr + precio.at(i);
            }

        }

        precioInt = atoi(precioStr.c_str()); // Convertimos de string a entero.

        int producto = precioInt;

        QString prodqt = QString::number(producto);

        if(control !=1)
        {
            ui->expendedorplainTextEdit->appendPlainText(prodqt +"$ : Porcion de salsa catsup.");
        }

        ui->limpiarVisorExpendedor->setEnabled(false);
}

void MainWindow::on_porcionDePapasFritas_clicked()
{
    string ingredientes[80];
        string renglon;
        string cantidadStr;
        string posibleCodigoStr;
        int control = 0;
        int posibleCodigoInt;
        int cantidadInt;
        int codigo = 10015; // Codigo del producto.
        int contL = 0;      // contador para iterar el vector en el fichero de modo lectura.
        int a;              // Variable utilizada para convertir de numero a string;
        bool encontrado = false; // Para que rompa el ciclo en caso de ser encontrado en el paso 2.

        // Paso 1
        // Lectura del fichero y guardarlo en el vector.
        ifstream entradaF;
        entradaF.open("stock.txt");
        if(entradaF.good())
        {
            getline(entradaF , renglon);

            while(!entradaF.eof())
            {

                ingredientes[contL] = renglon;
                contL++;
                getline(entradaF , renglon);

            }
        }

        entradaF.close();

        //////////////////////////////////////////////////////////////////////////
        // Paso 2.
        // Modificar la cantidad de stock en el vector.

        for(int i=0 ; i<80 && !encontrado ; i++)
        {
            posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
            posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

            if(posibleCodigoInt == codigo)
            {
                i++;
                cantidadStr = ingredientes[i];
                cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                if(cantidadInt != 0)
                {
                    cantidadInt--;  // Disminucion de la cantidad del ingrediente.
                    a = cantidadInt; // Almacenarlo en la variable a.
                    stringstream ss;  // Entero a string.
                    ss << a;
                    string str = ss.str();
                    ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
                    encontrado = true; // Convertirlo de estado para romper el ciclo.
                }
                else
                {
                    control = 1;
                    QMessageBox::warning(NULL ,"       Advertencia             ", "   Sin stock: Porcion de papas fritas.    ");
                }
            }

        }

        ///////////////////////////////////////////////////////////
        // Paso 3.
        // Pasar los datos del vector al fichero.

        ofstream salidaF;
        salidaF.open("stock.txt");
        if(salidaF.good())
        {
            for(int i=0 ; i<80 ; i++)
            {
                salidaF << ingredientes[i] << endl;
            }
        }

        salidaF.close();

        /////////////////////////////////////////////////////////////////
        // Encontramos el precio del producto.

        contador++;

        string ingrediente , cantidad , codigo2, precio , separador , renglon2;
        ifstream entrada2;
        string posibleCod = "";
        string precioStr;
        int posibleCodInt = 0;
        int precioInt;
        int codigoVerif = 10015;
        bool buscar = false;

        entrada2.open("stock.txt");
        if(entrada2.good())
        {
            getline(entrada2 , renglon2);

            while(!entrada2.eof() && !buscar)
            {
                ingrediente = renglon2;   // Lectura de grupos de 5 renglones.
                getline(entrada2 , renglon2);
                codigo2 = renglon2;
                getline(entrada2 , renglon2);
                cantidad = renglon2;
                getline(entrada2 , renglon2);
                precio = renglon2;
                getline(entrada2 , renglon2);
                separador = renglon2;


                for(unsigned int i=0 ; i<codigo2.length() ; i++) // Eliminar los espacion del string llamado codigo.
                {
                    posibleCod = posibleCod + codigo2.at(i);
                }

                posibleCodInt = atoi(posibleCod.c_str()); // Convertir de string a entero.
                posibleCod = ""; // Limpiamos el string para los proximos datos.

                if( posibleCodInt == codigoVerif) // Verificacion si existe el codigo.
                {
                    buscar = true;
                }
                else
                {
                    getline(entrada2 , renglon2);
                }
            }

        }

        entrada2.close();

        for(unsigned int i=0 ; i<precio.length() ; i++) // Eliminamos el caracter de $
        {
            if(precio.at(i) != '$')
            {
                precioStr = precioStr + precio.at(i);
            }

        }

        precioInt = atoi(precioStr.c_str()); // Convertimos de string a entero.

        int producto = precioInt;

        QString prodqt = QString::number(producto);

        if(control !=1)
        {
            ui->expendedorplainTextEdit->appendPlainText(prodqt +"$ : Porcion de papas fritas.");
        }

        ui->limpiarVisorExpendedor->setEnabled(false);
}

void MainWindow::on_vasoConGaseosa_clicked()
{
    string ingredientes[80];
        string renglon;
        string cantidadStr;
        string posibleCodigoStr;
        int control = 0;
        int posibleCodigoInt;
        int cantidadInt;
        int codigo = 10016; // Codigo del producto.
        int contL = 0;      // contador para iterar el vector en el fichero de modo lectura.
        int a;              // Variable utilizada para convertir de numero a string;
        bool encontrado = false; // Para que rompa el ciclo en caso de ser encontrado en el paso 2.

        // Paso 1
        // Lectura del fichero y guardarlo en el vector.
        ifstream entradaF;
        entradaF.open("stock.txt");
        if(entradaF.good())
        {
            getline(entradaF , renglon);

            while(!entradaF.eof())
            {

                ingredientes[contL] = renglon;
                contL++;
                getline(entradaF , renglon);
            }
        }

        entradaF.close();

        //////////////////////////////////////////////////////////////////////////
        // Paso 2.
        // Modificar la cantidad de stock en el vector.

        for(int i=0 ; i<80 && !encontrado ; i++)
        {
            posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
            posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

            if(posibleCodigoInt == codigo)
            {
                i++;
                cantidadStr = ingredientes[i];
                cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                if(cantidadInt != 0)
                {
                    cantidadInt--;  // Disminucion de la cantidad del ingrediente.
                    a = cantidadInt; // Almacenarlo en la variable a.
                    stringstream ss;  // Entero a string.
                    ss << a;
                    string str = ss.str();
                    ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
                    encontrado = true; // Convertirlo de estado para romper el ciclo.
                }
                else
                {
                    control = 1;
                    QMessageBox::warning(NULL ,"       Advertencia             ", "   Sin stock: Vaso con gaseosa.   ");
                }
            }

        }

        ///////////////////////////////////////////////////////////
        // Paso 3.
        // Pasar los datos del vector al fichero.

        ofstream salidaF;
        salidaF.open("stock.txt");
        if(salidaF.good())
        {
            for(int i=0 ; i<80 ; i++)
            {
                salidaF << ingredientes[i] << endl;
            }
        }

        salidaF.close();

        /////////////////////////////////////////////////////////////////
        // Encontramos el precio del producto.

        contador++;

        string ingrediente , cantidad , codigo2, precio , separador , renglon2;
        ifstream entrada2;
        string posibleCod = "";
        string precioStr;
        int posibleCodInt = 0;
        int precioInt;
        int codigoVerif = 10016;
        bool buscar = false;

        entrada2.open("stock.txt");
        if(entrada2.good())
        {
            getline(entrada2 , renglon2);

            while(!entrada2.eof() && !buscar)
            {
                ingrediente = renglon2;   // Lectura de grupos de 5 renglones.
                getline(entrada2 , renglon2);
                codigo2 = renglon2;
                getline(entrada2 , renglon2);
                cantidad = renglon2;
                getline(entrada2 , renglon2);
                precio = renglon2;
                getline(entrada2 , renglon2);
                separador = renglon2;


                for(unsigned int i=0 ; i<codigo2.length() ; i++) // Eliminar los espacion del string llamado codigo.
                {
                    posibleCod = posibleCod + codigo2.at(i);
                }

                posibleCodInt = atoi(posibleCod.c_str()); // Convertir de string a entero.
                posibleCod = ""; // Limpiamos el string para los proximos datos.

                if( posibleCodInt == codigoVerif) // Verificacion si existe el codigo.
                {
                    buscar = true;
                }
                else
                {
                    getline(entrada2 , renglon2);
                }
            }

        }

        entrada2.close();

        for(unsigned int i=0 ; i<precio.length() ; i++) // Eliminamos el caracter de $
        {
            if(precio.at(i) != '$')
            {
                precioStr = precioStr + precio.at(i);
            }

        }

        precioInt = atoi(precioStr.c_str()); // Convertimos de string a entero.

        int producto = precioInt;

        QString prodqt = QString::number(producto);

        if(control !=1)
        {
            ui->expendedorplainTextEdit->appendPlainText(prodqt +"$ : Vaso con gaseosa.");
        }

        ui->limpiarVisorExpendedor->setEnabled(false);
}

void MainWindow::on_comboCarnepushButton_clicked()
{
   MainWindow::on_topeDePan_clicked();
   MainWindow::on_hamburguesaDeCarne_clicked();
   MainWindow::on_salsaDeMayonesa_clicked();
   MainWindow::on_porcionDeTomatePicado_clicked();
   MainWindow::on_PorcionDeCebollaPicada_clicked();
   MainWindow::on_porcionDePepinosEnConserva_clicked();
   MainWindow::on_huevoFrito_clicked();
   MainWindow::on_fetaDeQueso_clicked();
   MainWindow::on_porcionDePapasFritas_clicked();
   MainWindow::on_vasoConGaseosa_clicked();
}

void MainWindow::on_comboPollopushButton_clicked()
{
    MainWindow::on_topeDePan_clicked();
    MainWindow::on_hamburguesaDePollo_clicked();
    MainWindow::on_porcionDeTomatePicado_clicked();
    MainWindow::on_PorcionDeCebollaPicada_clicked();
    MainWindow::on_huevoFrito_clicked();
    MainWindow::on_porcionDePapasFritas_clicked();
    MainWindow::on_porcionDeSalsaCatsup_clicked();
    MainWindow::on_vasoConGaseosa_clicked();
}

void MainWindow::on_comboPescadopushButton_clicked()
{
    MainWindow::on_topeDePan_clicked();
    MainWindow::on_hamburguesaDePescado_clicked();
    MainWindow::on_porcionDeTomatePicado_clicked();
    MainWindow::on_PorcionDeCebollaPicada_clicked();
    MainWindow::on_porcionDePapasFritas_clicked();
    MainWindow::on_vasoConGaseosa_clicked();
}

void MainWindow::on_confirmarCompra_clicked()
{
    string totalstr;
    int total = 0;
    int cint = 0;
    string c = "";
    string fechaStr;
    string renglon = "";
    QString totalqt;
    string textoStr;

    QString textoqt = ui->expendedorplainTextEdit->toPlainText(); //PASO LO QUE TENGO EN EL TEXT A TEXTOQSTRING
    textoStr = textoqt.toStdString(); // QString a string
    // Modo escritura
    // Enviamos los datos del visor al fichero auxiliar.
    ofstream salidaF;
       salidaF.open("auxiliarConfirmar.txt");
       if(salidaF.good())
       {
         salidaF << textoStr << endl;
       }

       salidaF.close();

    // Modo lectura.
       // Conseguimos el precio total.
       ifstream entradaF;

       entradaF.open("auxiliarConfirmar.txt");

       if(entradaF.good())
       {
             getline(entradaF , renglon);

             while(!entradaF.eof())
             {
               for(unsigned int i=0 ; i<renglon.length() ; i++)
                  {
                    if(renglon.at(i)!='$')
                    {
                       c = c + renglon.at(i);
                    }
                  }
                   cint = atoi(c.c_str());
                   c = "";
                   total = total + cint;
                   getline(entradaF , renglon);
             }
       }

       entradaF.close();

    // Entero a string y, luego string a QString para enviarlo al visor.
    stringstream ss;
    ss<<total;
    totalstr = ss.str();
    totalqt = QString::fromStdString(totalstr);
    ui->expendedorplainTextEdit->appendPlainText("Total: "+totalqt+" $\n*");

   // Modo escritura
    // Enviamos los datos del visor al fichero ticket.
    ofstream salidaF2;
    string tex = textoqt.toStdString();

    salidaF2.open("tickets.txt",ofstream::app);

    if(salidaF2.good())
    {
        fechaStr = fecha.toStdString();
        salidaF2 << fechaStr << endl;
        salidaF2 << tex << endl;
        salidaF2 << "Total: "<<totalstr<<"$"<<endl;
        salidaF2 << "*" <<endl;
    }
    salidaF.close();

    ui->confirmarCompra->setEnabled(false);
    ui->limpiarVisorExpendedor->setEnabled(true);
}

void MainWindow::on_anularComprapushButton_clicked()
{
    string nomIngre = "";
    string textoStr;

    QString textoqt = ui->expendedorplainTextEdit->toPlainText(); //PASO LO QUE TENGO EN EL TEXT A TEXTOQSTRING
    textoStr = textoqt.toStdString(); // QString a string
     // Paso 1.
    // Modo escritura
    // Enviamos los datos del visor al fichero auxiliarAnular.
    ofstream salidaF;
       salidaF.open("auxiliarAnular.txt" , ios::trunc);
       if(salidaF.good())
       {
         salidaF << textoStr << endl;
       }

       salidaF.close();
//////////////////////////////////////////////////////////////////////////////////////
       // Paso 2.
       // Modo lectura.
       // Verificar que ingredientes hay que restaurar.
       string renglon;
       int contIngre1 = 0;
       int contIngre2 = 0;
       int contIngre3 = 0;
       int contIngre4 = 0;
       int contIngre5 = 0;
       int contIngre6 = 0;
       int contIngre7 = 0;
       int contIngre8 = 0;
       int contIngre9 = 0;
       int contIngre10 = 0;
       int contIngre11 = 0;
       int contIngre12 = 0;
       int contIngre13 = 0;
       int contIngre14 = 0;
       int contIngre15 = 0;
       int contIngre16 = 0;
       ifstream entradaF;

       entradaF.open("auxiliarAnular.txt");

       if(entradaF.good())
       {
             getline(entradaF , renglon);

             while(!entradaF.eof())
             {
               for(unsigned int i=0 ; i<renglon.length() ; i++)
                  {
                    if((renglon.at(i) >= 65 && renglon.at(i) <= 90) || (renglon.at(i) >=97 && renglon.at(i) <= 122))
                    {
                       nomIngre = nomIngre + renglon.at(i);
                    }
                  }

                 if(nomIngre == "Topedepan")
                 {
                     contIngre1++;
                 }
                 else if (nomIngre == "Discocentraldepan")
                 {
                     contIngre2++;
                 }
                 else if (nomIngre == "Basedepan")
                 {
                     contIngre3++;
                 }
                 else if (nomIngre == "Hamburguesadecarne")
                 {
                     contIngre4++;
                 }
                 else if (nomIngre == "Hamburguesadepollo")
                 {
                     contIngre5++;
                 }
                 else if (nomIngre == "Hamburguesadepescado")
                 {
                     contIngre6++;
                 }
                 else if (nomIngre == "Huevofrito")
                 {
                     contIngre7++;
                 }
                 else if (nomIngre == "Fetadequeso")
                 {
                     contIngre8++;
                 }
                 else if (nomIngre == "Porciondetomatepicado")
                 {
                     contIngre9++;
                 }
                 else if (nomIngre == "Porciondelechugapicada")
                 {
                     contIngre10++;
                 }
                 else if (nomIngre == "Porciondecebollapicada")
                 {
                     contIngre11++;
                 }
                 else if (nomIngre == "Porciondepepinosenconserva")
                 {
                     contIngre12++;
                 }
                 else if (nomIngre == "Porciondesalsamayonesa")
                 {
                     contIngre13++;
                 }
                 else if (nomIngre == "Porciondesalsacatsup")
                 {
                     contIngre14++;
                 }
                 else if (nomIngre == "Porciondepapasfritas")
                 {
                     contIngre15++;
                 }
                 else if (nomIngre == "Vasocongaseosa")
                 {
                     contIngre16++;
                 }

                 nomIngre = "";
                 getline(entradaF , renglon);
             }
       }

       entradaF.close();

////////////////////////////////////////////////////////////////////////////////////////////////
       // Lectura del fichero y guardarlo en el vector.
       int contL = 0;
       string ingredientes [80];
       string renglon2;

       ifstream entradaF2;
       entradaF2.open("stock.txt");
       if(entradaF2.good())
       {
           getline(entradaF2 , renglon2);

           while(!entradaF2.eof())
           {

               ingredientes[contL] = renglon2;
               contL++;
               getline(entradaF2 , renglon2);
           }
       }

       entradaF2.close();

 ///////////////////////////////////////////////////////////////////
 // Modificar la cantidad de stock en el vector.
       string cantidadStr = "";
       string posibleCodigoStr = "";
       int posibleCodigoInt = 0;
       int cantidadInt = 0;
       int codigo1 = 10001;
       int codigo2 = 10002;
       int codigo3 = 10003;
       int codigo4 = 10004;
       int codigo5 = 10005;
       int codigo6 = 10006;
       int codigo7 = 10007;
       int codigo8 = 10008;
       int codigo9 = 10009;
       int codigo10 = 10010;
       int codigo11 = 10011;
       int codigo12 = 10012;
       int codigo13 = 10013;
       int codigo14 = 10014;
       int codigo15 = 10015;
       int codigo16 = 10016;


       for(int i=0 ; i<80 ; i++)
       {
           posibleCodigoStr = ingredientes[i];  // Almacenar el datos de esa posicion, a la variable.
           posibleCodigoInt = atoi(posibleCodigoStr.c_str());  // String a entero.

           if(posibleCodigoInt == codigo1)
           {
               i++;
               cantidadStr = ingredientes[i];
               cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                   cantidadInt = cantidadInt + contIngre1;
                   stringstream ss;  // Entero a string.
                   ss << cantidadInt;
                   string str = ss.str();
                   ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
            }
           else if(posibleCodigoInt == codigo2)
           {
               i++;
               cantidadStr = ingredientes[i];
               cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                   cantidadInt = cantidadInt + contIngre2;
                   stringstream ss;  // Entero a string.
                   ss << cantidadInt;
                   string str = ss.str();
                   ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
            }
           else if(posibleCodigoInt == codigo3)
           {
               i++;
               cantidadStr = ingredientes[i];
               cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                   cantidadInt = cantidadInt + contIngre3;
                   stringstream ss;  // Entero a string.
                   ss << cantidadInt;
                   string str = ss.str();
                   ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
            }
           else if(posibleCodigoInt == codigo4)
           {
               i++;
               cantidadStr = ingredientes[i];
               cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                   cantidadInt = cantidadInt + contIngre4;
                   stringstream ss;  // Entero a string.
                   ss << cantidadInt;
                   string str = ss.str();
                   ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
            }
           else if(posibleCodigoInt == codigo5)
           {
               i++;
               cantidadStr = ingredientes[i];
               cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                   cantidadInt = cantidadInt + contIngre5;
                   stringstream ss;  // Entero a string.
                   ss << cantidadInt;
                   string str = ss.str();
                   ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
            }
           else if(posibleCodigoInt == codigo6)
           {
               i++;
               cantidadStr = ingredientes[i];
               cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                   cantidadInt = cantidadInt + contIngre6;
                   stringstream ss;  // Entero a string.
                   ss << cantidadInt;
                   string str = ss.str();
                   ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
            }
           else if(posibleCodigoInt == codigo7)
           {
               i++;
               cantidadStr = ingredientes[i];
               cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                   cantidadInt = cantidadInt + contIngre7;
                   stringstream ss;  // Entero a string.
                   ss << cantidadInt;
                   string str = ss.str();
                   ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
            }
           else if(posibleCodigoInt == codigo8)
           {
               i++;
               cantidadStr = ingredientes[i];
               cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                   cantidadInt = cantidadInt + contIngre8;
                   stringstream ss;  // Entero a string.
                   ss << cantidadInt;
                   string str = ss.str();
                   ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
            }
           else if(posibleCodigoInt == codigo9)
           {
               i++;
               cantidadStr = ingredientes[i];
               cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                   cantidadInt = cantidadInt + contIngre9;
                   stringstream ss;  // Entero a string.
                   ss << cantidadInt;
                   string str = ss.str();
                   ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
            }
           else if(posibleCodigoInt == codigo10)
           {
               i++;
               cantidadStr = ingredientes[i];
               cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                   cantidadInt = cantidadInt + contIngre10;
                   stringstream ss;  // Entero a string.
                   ss << cantidadInt;
                   string str = ss.str();
                   ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
            }
           else if(posibleCodigoInt == codigo11)
           {
               i++;
               cantidadStr = ingredientes[i];
               cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                   cantidadInt = cantidadInt + contIngre11;
                   stringstream ss;  // Entero a string.
                   ss << cantidadInt;
                   string str = ss.str();
                   ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
            }
           else if(posibleCodigoInt == codigo12)
           {
               i++;
               cantidadStr = ingredientes[i];
               cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                   cantidadInt = cantidadInt + contIngre12;
                   stringstream ss;  // Entero a string.
                   ss << cantidadInt;
                   string str = ss.str();
                   ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
            }
           else if(posibleCodigoInt == codigo13)
           {
               i++;
               cantidadStr = ingredientes[i];
               cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                   cantidadInt = cantidadInt + contIngre13;
                   stringstream ss;  // Entero a string.
                   ss << cantidadInt;
                   string str = ss.str();
                   ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
            }
           else if(posibleCodigoInt == codigo14)
           {
               i++;
               cantidadStr = ingredientes[i];
               cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                   cantidadInt = cantidadInt + contIngre14;
                   stringstream ss;  // Entero a string.
                   ss << cantidadInt;
                   string str = ss.str();
                   ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
            }
           else if(posibleCodigoInt == codigo15)
           {
               i++;
               cantidadStr = ingredientes[i];
               cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                   cantidadInt = cantidadInt + contIngre15;
                   stringstream ss;  // Entero a string.
                   ss << cantidadInt;
                   string str = ss.str();
                   ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
            }
           else if(posibleCodigoInt == codigo16)
           {
               i++;
               cantidadStr = ingredientes[i];
               cantidadInt = atoi(cantidadStr.c_str()); // String a entero.

                   cantidadInt = cantidadInt + contIngre16;
                   stringstream ss;  // Entero a string.
                   ss << cantidadInt;
                   string str = ss.str();
                   ingredientes[i] = str; // Almacenarlo en la misma posicion, pero con el dato actualizado.
            }

        }

   ///////////////////////////////////////////////////////////////////////////////////
       // Datos del vector enviados al fichero stock.
       ofstream salidaF2;
               salidaF2.open("stock.txt");
               if(salidaF2.good())
               {
                   for(int i=0 ; i<80 ; i++)
                   {
                       salidaF2 << ingredientes[i] << endl;
                   }
               }

               salidaF2.close();


       ui->anularComprapushButton->setEnabled(false);
       ui->confirmarCompra->setEnabled(false);
       ui->limpiarVisorExpendedor->setEnabled(true);

}

void MainWindow::on_limpiarVisorExpendedor_clicked()
{
    ui->expendedorplainTextEdit->clear();
    ui->confirmarCompra->setEnabled(true);
    ui->anularComprapushButton->setEnabled(true);
}

void MainWindow::on_volverExpendedorLogpushButton_clicked()
{
    QString visorExpendedor = ui->expendedorplainTextEdit->toPlainText();

    if(visorExpendedor.isEmpty())
    {
       MainWindow::on_anularComprapushButton_clicked();
      //MainWindow::on_limpiarVisorExpendedor_clicked();

      ui->limpiarVisorExpendedor->setEnabled(true);
      ui->widgetExpendor->hide();
      ui->widgetLogin->show();
      ui->usuariolineEdit->clear();
      ui->contrasenalineEdit->clear();
      ui->usuariolineEdit->setFocus();
    }
    else
    {
        QMessageBox::warning( NULL ,  "  Advertencia   " ,   "  Limpiar el visor para poder volver al login   ");
        ui->limpiarVisorExpendedor->setEnabled(true);
    }

}
