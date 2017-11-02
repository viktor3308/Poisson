#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "plot3d.h"
#include "solvecontroller.h"

#include <QtDataVisualization/Q3DSurface>
#include <QtGui/QScreen>
#include <QThread>
#include <QTextEdit>
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    using namespace QtDataVisualization;

    ui->setupUi(this);
    Q3DSurface* plotSurface = new Q3DSurface();
    QWidget *container = QWidget::createWindowContainer(plotSurface);
    QSize screenSize = plotSurface->screen()->size();

    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    ui->centralWidget->layout()->addWidget(container);
    m_plot = new Plot3D(plotSurface);

    ui->m_iteratorComboBox->insertItem(0, "Метод Якоби");
    ui->m_iteratorComboBox->insertItem(1, "Метод Гаусса-Зейделя");
    ui->m_iteratorComboBox->insertItem(2, "Многосеточный метод");
    ui->m_iteratorComboBox->setCurrentIndex(0);

    m_log = new QTextEdit(this);
    m_log->setReadOnly(true);
    QDockWidget *dockWidget = new QDockWidget(tr("Лог"), this);
    dockWidget->setWidget(m_log);
    dockWidget->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::BottomDockWidgetArea, dockWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSolve()
{
    FunctionalIterator iterator;
    switch(ui->m_iteratorComboBox->currentIndex())
    {
    case 0:
        iterator = JacobiIterator();
        onMessage("\nРешение методом Якоби:\n");
        break;
    case 1:
        iterator = ZeidelIterator();
        onMessage("\nРешение методом Гаусса-Зейделя:\n");
        break;
    case 2:
        iterator = MultigridIterator(2);
        onMessage("\nРешение многосеточным методом:\n");
        break;
    default:
        return;
    }

    SolveController* controller = new SolveController(ui->m_gridSizeSpinBox->value(), ui->m_epsilonSpinBox->value(), iterator);
    QThread* thread = new QThread();
    controller->moveToThread(thread);

    connect(controller, &SolveController::solution, m_plot, &Plot3D::setData);
    connect(controller, &SolveController::message, this, &MainWindow::onMessage);
    connect(thread, SIGNAL(started()), controller, SLOT(solve()));
    connect(controller, SIGNAL(finished()), thread, SLOT(quit()));
    connect(controller, SIGNAL(finished()), controller, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void MainWindow::onMessage(const QString &message)
{
    m_log->insertPlainText(message);
    m_log->ensureCursorVisible();
}
