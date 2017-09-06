#include "colorfultextwidget.h"

#include <QPainter>
#include <QXmlStreamReader>
#include <QFileInfo>
#include <QMouseEvent>
#include <QGraphicsEffect>
#include <QDebug>

ColorfulTextWidget::ColorfulTextWidget(QWidget *parent) : QWidget(parent)
    , isDragging(false)
    , lastMousePos(QPoint())
    , m_holderText("")
    , m_text("")
    , m_pen(QPen())
    , alignmentFlag(Qt::AlignCenter)
    , bAutoResize(true)
    , bDragable(true)
{
}

void ColorfulTextWidget::setHolderText(const QString &text)
{
    m_holderText = text;
    update();
}

void ColorfulTextWidget::setText(const QString &text)
{
    m_text = text;
    update();
}

void ColorfulTextWidget::setPen(const QPen &pen)
{
    m_pen = pen;
}

void ColorfulTextWidget::setAlignment(Qt::AlignmentFlag flag)
{
    alignmentFlag = flag;
}

void ColorfulTextWidget::setAutoResize(bool enable)
{
    bAutoResize = enable;
}

void ColorfulTextWidget::setDragable(bool enable)
{
    bDragable = enable;
}

QPixmap ColorfulTextWidget::textToPixmap()
{
    int xOffset = 0;
    int yOffset = 0;

#ifndef QT_NO_GRAPHICSEFFECT
    QGraphicsEffect *effect = graphicsEffect();
    QGraphicsDropShadowEffect *dropShadowEffect = qobject_cast<QGraphicsDropShadowEffect *>(effect);
    if (dropShadowEffect != NULL) {
        xOffset = dropShadowEffect->xOffset();
        yOffset = dropShadowEffect->yOffset();
    }
#endif //QT_NO_GRAPHICSEFFECT

    return grab(QRect(0, 0, rect().width() + xOffset, rect().height() + yOffset));
}

QList<QMap<QString, QString> > ColorfulTextWidget::setColorfulByFrameXml(const QString &path, QString &erro)
{
    QList< QMap<QString, QString> > propertiesList;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        erro = tr("file can't opend: %1").arg(file.errorString());
        return propertiesList;
    }

    QXmlStreamReader xml(&file);
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {
            if (xml.name().toString() == "text") {
                QMap<QString, QString> propertyMap;

                QXmlStreamAttributes attributes = xml.attributes();
                if (attributes.hasAttribute("name")) {
                    propertyMap["name"] = attributes.value("name").toString();
                }

                xml.readNext();

                while ( !(xml.tokenType() == QXmlStreamReader::EndElement
                          && xml.name() == "text") ) {
                    if (xml.tokenType() == QXmlStreamReader::StartElement) {
                        QString elementName = xml.name().toString();
                        xml.readNext();

                        if (xml.tokenType() != QXmlStreamReader::Characters) {
                            break;
                        }

                        propertyMap.insert(elementName, xml.text().toString());
                    }

                    xml.readNext();
                }

                if (!propertyMap.isEmpty())
                    propertiesList.append(propertyMap);
            }  // text xml name
        }  // start element except start document
    }  // iter whole xml

    return propertiesList;
}

void ColorfulTextWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.save();
    painter.setFont(font());
    painter.setPen(m_pen);
    QTextOption option;
    option.setAlignment(alignmentFlag);
    option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    QString text = !m_text.isEmpty() ? m_text : m_holderText;
    if (bAutoResize) {
        QFontMetrics fm(font());
        // calc width
        int fmWidth = fm.width(text);
        int fmBoundingWidth = fm.boundingRect(text).width();
        int painterBoundingWidth = painter.boundingRect(rect(), text, option).width();
        int width = qMax(qMax(fmWidth, fmBoundingWidth), painterBoundingWidth);
//        qDebug() << QString("fmWidth,fmBoundingWidth,painterBoundingWidth:%1,%2,%3").arg(fmWidth).arg(fmBoundingWidth).arg(painterBoundingWidth);

        // calc height
        int ratio = 0;
        if (maximumHeight() != 0) {
            ratio = width / maximumWidth() + ((width % maximumWidth()) != 0);
        }
        int ratioHeight = fm.height() * ratio;
        int painterBoundingHeight = painter.boundingRect(rect(), text, option).height();
        int height = qMax(ratioHeight, painterBoundingHeight);
//        qDebug() << QString("ratioHeight,painterBoundingHeight:%1,%2").arg(ratioHeight).arg(painterBoundingHeight);

        resize(width, height);

        // modify size
        if (this->width() == 0 || this->height() == 0) {
//            qDebug() << "modify size.";
            resize(1, 1);  // ensure visible, so update immediately.
        }
    }

    painter.drawText(rect(), text, option);
    painter.restore();

    QWidget::paintEvent(event);
}

void ColorfulTextWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePos = QPoint(event->globalX() - geometry().x(), event->globalY() - geometry().y());

    if (event->buttons() & Qt::LeftButton) {
        isDragging = true;
    }
}

void ColorfulTextWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (bDragable && isDragging) {
        move(event->globalPos() - lastMousePos);
    }
}

void ColorfulTextWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    isDragging = false;
}
