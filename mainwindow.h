#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QLineEdit>

class Plot3D;
class QTextEdit;

namespace Ui {
class MainWindow;
}

class Power2Spinbox : public QSpinBox
{
    Q_OBJECT
public:
    Power2Spinbox(QWidget* parent = Q_NULLPTR):
        QSpinBox(parent)
    {
        setMaximum(4096);
        setMinimum(16);
        setValue(256);
        findChild<QLineEdit*>()->setReadOnly(true);
    }

    void stepBy(int steps)
    {
        int newValue = value();
        if(steps > 0)
        {
            while(steps > 0)
            {
                newValue *= 2;
                --steps;
            }
            setValue(std::min(newValue, maximum()));
            return;
        }
        while(steps < 0)
        {
            newValue /= 2;
            ++steps;
        }
        setValue(std::max(newValue, minimum()));
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onSolve();
    void onMessage(const QString& message);

private:
    Ui::MainWindow *ui;
    Plot3D* m_plot;
    QTextEdit* m_log;
};

#endif // MAINWINDOW_H
