#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsProxyWidget>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(5);
    effect->setXOffset(5);
    effect->setYOffset(5);
    effect->setColor(Qt::black);

    ui->lineEdit->setGraphicsEffect(effect);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    QFont font = qApp->font();
    font.setFamily(QStringLiteral("Microsoft YaHei"));
    font.setPixelSize(100);
    font.setWeight(5);
    font.setItalic(true);
    font.setUnderline(true);
    ui->widget->setFont(font);

    QPen pen;
    QColor color("#00ff00");
    color.setAlpha(100);
    pen.setColor(color);
    ui->widget->setPen(pen);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;

    effect->setBlurRadius(20);
    effect->setXOffset(2);
    effect->setYOffset(20);
    effect->setColor(QColor("#a32749"));
    ui->widget->setGraphicsEffect(effect);

    ui->widget->setAlignment(Qt::AlignCenter);
    ui->widget->setText(arg1);

    ui->widget->setHolderText("Hammer");
}

void MainWindow::on_pushButton_clicked()
{
    ColorfulTextWidget *wgt = new ColorfulTextWidget(this);

    QList< QMap<QString, QString> > list;
    QString error;
    list = ColorfulTextWidget::setColorfulByFrameXml(":/resources/frame.xml", error);
    if (!list.isEmpty()) {
        QMap<QString, QString> map = list.at(0);
        QPen pen;
        QColor color(map.value("color"));
        color.setAlpha(100);
        pen.setColor(color);
        wgt->setPen(pen);
    }

    wgt->setHolderText("Qt");
    wgt->show();
}
