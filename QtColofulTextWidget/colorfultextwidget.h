#ifndef COLORFULWIDGET_H
#define COLORFULWIDGET_H

#include <QWidget>
#include <QPen>
#include <QGraphicsDropShadowEffect>

class ColorfulTextWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColorfulTextWidget(QWidget *parent = nullptr);

    void setHolderText(const QString &text);
    void setText(const QString &text);

    void setPen(const QPen &pen);
    void setAlignment(Qt::AlignmentFlag flag);

    void setAutoResize(bool enable);
    void setDragable(bool enable);

    QPixmap textToPixmap();

    static QList< QMap<QString, QString> > setColorfulByFrameXml(const QString &path, QString &erro);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool isDragging;
    QPoint lastMousePos;

    QString m_holderText;
    QString m_text;

    QPen m_pen;
    Qt::AlignmentFlag alignmentFlag;

    bool bAutoResize;
    bool bDragable;
};

#endif // COLORFULWIDGET_H
