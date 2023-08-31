#include "mainwindow.h"
#include <QApplication>
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QMessageBox>
#include <QPixmap>

void displaySpecifications(MainWindow *window);
QString getSpecifications(const QString &device);

// Created a function that connects button's clicked signal to displaySpecifications function

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet("QWidget { color: blue; }"
                    "QMainWindow { background-color: grey; }"); // Setting the application's stylesheet
    MainWindow w;
    QIcon icon("/home/khoi/Apple Project/AppleComparisonProject/icons/icons8-button-50.png");
    w.ui->pushButton->setIcon(icon); // Setting the icon for the push button
    w.ui->pushButton->setIconSize(QSize(50, 50)); // Setting the size of the icon
    w.show();
    QObject::connect(w.ui->pushButton, &QPushButton::clicked, &w, [&w](){
        w.ui->textEdit->clear(); // Clearing text boxes immediately when the "compare" button is pressed
        w.ui->textEdit_2->clear(); // Clearing text boxes immediately when the "compare" button is pressed
        displaySpecifications(&w);
    });
    return a.exec();
}

void displaySpecifications(MainWindow *window)
{
    window->ui->label_2->setText("Comparing...");
    window->ui->label_2->setVisible(true);

    QTimer::singleShot(1000, window, [window]()
    {
        QString device1 = window -> ui -> comboBox -> currentText();
        QString device2 = window -> ui -> comboBox_2 -> currentText();

        QString specs1 = getSpecifications(device1);
        QString specs2 = getSpecifications(device2);

        // Clearing the text boxes before the error message box appears

        window->ui->textEdit->clear();
        window->ui->textEdit_2->clear();

        // Error message box if device specs not found

        if (specs1 == "Specifications not found.") {
            QMessageBox::critical(window, "Error", "Specifications not found for " + device1 + ".");
        } else {
            window->ui->textEdit->setText("Specifications for " + device1 + ":\n" + specs1);
        }

        if (specs2 == "Specifications not found.") {
            QMessageBox::critical(window, "Error", "Specifications not found for " + device2 + ".");
        } else {
            window->ui->textEdit_2->setText("Specifications for " + device2 + ":\n" + specs2);
        }

        window -> ui -> textEdit -> setText("Specifications for " + device1 + ":\n" + specs1);
        window -> ui -> textEdit_2 -> setText("Specifications for " + device2 + ":\n" + specs2);

        window->ui->label_2->setVisible(false);
    });
}

// Function that fetches specifications for the selected devices

QString getSpecifications(const QString &device)
{
    QFile file("/home/khoi/Apple Project/AppleComparisonProject/AppleDeviceComparisonTool/device-specs-data");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "Error reading specifications.";
    }

    QTextStream in(&file);
    QString line;
    QString specs;
    bool found = false;

    while (!in.atEnd()) {
        line = in.readLine();
        if (line == device) {
            found = true;
            specs = line + "\n";
            while (!in.atEnd()) {
                line = in.readLine();
                if (line.isEmpty()) {
                    break;
                }
                specs += line + "\n";
            }
            break;
        }
    }

    file.close();

    if (found) {
        return specs;
    } else {
        return "Specifications not found.";
    }
}


