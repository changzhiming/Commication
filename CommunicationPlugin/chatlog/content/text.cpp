/*
    Copyright © 2014-2015 by The qTox Project

    This file is part of qTox, a Qt-based graphical interface for Tox.

    qTox is libre software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    qTox is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with qTox.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "text.h"
#include "../documentcache.h"

#include <QFontMetrics>
#include <QPainter>
#include <QPalette>
#include <QDebug>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QDesktopServices>
#include <QTextFragment>
#include <QApplication>
#include <QClipboard>
#include "tool/previewpicture.h"

Text::Text(const QString& txt, QFont font, bool enableElide, const QString &rwText, const QColor c)
    : rawText(rwText)
    , elide(enableElide)
    , defFont(font)
    , color(c)
{
    setText(txt);
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);

}

Text::~Text()
{
    qDebug()<<"delete text";
    if (doc)
        DocumentCache::getInstance().push(doc);
}

void Text::setText(const QString& txt)
{
    text = txt;
    dirty = true;
}

void Text::setWidth(qreal w)
{
    width = w;
    dirty = true;

    if (elide)
    {
        QFontMetrics metrics = QFontMetrics(defFont);         //设置字体的宽度和大小
        elidedText = metrics.elidedText(text, Qt::ElideRight, width); // 太长添加省略号
    }

    regenerate();
}

void Text::selectionMouseMove(QPointF scenePos)
{

    if (!doc)
        return;

    int cur = cursorFromPos(scenePos);
    if (cur >= 0)
    {
        selectionEnd = cur;
        selectedText = extractSanitizedText(getSelectionStart(), getSelectionEnd());
    }

    update();
}

void Text::selectionStarted(QPointF scenePos)
{
    int cur = cursorFromPos(scenePos);
    if (cur >= 0)
    {
        selectionEnd = cur;
        selectionAnchor = cur;
    }
}

void Text::selectionCleared()
{
    selectedText.clear();
    selectedText.squeeze();

    // Do not reset selectionAnchor!
    selectionEnd = -1;

    update();
}

void Text::selectionDoubleClick(QPointF scenePos)
{
    if (!doc)
        return;

    int cur = cursorFromPos(scenePos);

    if (cur >= 0)
    {
        QTextCursor cursor(doc);
        cursor.setPosition(cur);
        cursor.select(QTextCursor::WordUnderCursor);

        selectionAnchor = cursor.selectionStart();
        selectionEnd = cursor.selectionEnd();

        selectedText = extractSanitizedText(getSelectionStart(), getSelectionEnd());
    }

    update();
}

void Text::selectionFocusChanged(bool focusIn)
{
    selectionHasFocus = focusIn;
    update();
}

bool Text::isOverSelection(QPointF scenePos) const
{
    int cur = cursorFromPos(scenePos);
    if (getSelectionStart() < cur && getSelectionEnd() >= cur)
        return true;

    return false;
}

QString Text::getSelectedText() const
{
    return selectedText;
}

QRectF Text::boundingRect() const
{
    return QRectF(QPointF(0, 0), size);
}

void Text::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    if (doc)
    {
        painter->setClipRect(boundingRect());

        // draw selection
        QAbstractTextDocumentLayout::PaintContext ctx;
        QAbstractTextDocumentLayout::Selection sel;

        if (hasSelection())
        {
            sel.cursor = QTextCursor(doc);
            sel.cursor.setPosition(getSelectionStart());
            sel.cursor.setPosition(getSelectionEnd(), QTextCursor::KeepAnchor);
        }

        const QColor selectionColor = QColor::fromRgbF(0.23, 0.68, 0.91);
        sel.format.setBackground(selectionColor.lighter(selectionHasFocus ? 100 : 160));
        sel.format.setForeground(selectionHasFocus ? Qt::white : Qt::black);
        ctx.selections.append(sel);
        ctx.palette.setColor(QPalette::Text, color);

        // draw text
        doc->documentLayout()->draw(painter, ctx);
    }

    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void Text::visibilityChanged(bool visible)
{
    keepInMemory = visible;

    regenerate();
    update();        //重画项区域
}

qreal Text::getAscent() const
{
    return ascent;
}

void Text::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QString picturepath=previewImage(cursorFromPos(event->scenePos(), false));
        /*if(!picturepath.isEmpty())
        {
            previewpicture=new PreviewPicture(picturepath);
            qDebug()<<QCursor::pos()<<"preview pos ";
            int x=QCursor::pos().x()-previewpicture->size().width()/2;
            int y=QCursor::pos().y()-previewpicture->size().height()/2;
            previewpicture->move(x>=0?x:0, y>=0?y:0);
            previewpicture->show();
        }*/
        if(getText()==QObject::tr("查看历史消息"))
        {
            emit  showHistoryMessage();
        }
        event->accept(); // grabber

    }

}

void Text::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!doc)
        return;

    QString anchor = doc->documentLayout()->anchorAt(event->pos());

    // open anchor in browser
    if (!anchor.isEmpty())
        QDesktopServices::openUrl(anchor);
}

void Text::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!doc)
        return;

    QString anchor = doc->documentLayout()->anchorAt(event->pos());

    if (anchor.isEmpty())
    {
        if(getText()==QObject::tr("查看历史消息"))
        {
            setCursor(Qt::PointingHandCursor);
        }
        else
        {
        setCursor(Qt::IBeamCursor);
        }
    }
    else
    {
        setCursor(Qt::PointingHandCursor);
    }

    // tooltip
    if(getText()==QObject::tr("查看历史消息"))
        return;
    setToolTip(extractImgTooltip(cursorFromPos(event->scenePos(), false)));

}

QString Text::getText() const
{

    return rawText;
}

void Text::regenerate()
{
    if (!doc)
    {
        doc = DocumentCache::getInstance().pop();
        dirty = true;
    }

    if (dirty)
    {
        doc->setDefaultFont(defFont);

        if (!elide)
            doc->setHtml(text);
        else
            doc->setPlainText(elidedText);

        // wrap mode
        QTextOption opt;
        opt.setWrapMode(elide ? QTextOption::NoWrap : QTextOption::WrapAtWordBoundaryOrAnywhere);
        doc->setDefaultTextOption(opt);

        // width
        doc->setTextWidth(width);
        doc->documentLayout()->update();
        // update ascent
        if (doc->firstBlock().layout()->lineCount() > 0)
            ascent = doc->firstBlock().layout()->lineAt(0).ascent();

        // let the scene know about our change in size
        if (size != idealSize())
            prepareGeometryChange();    //准备改变位置

        // get the new width and height
        size = idealSize();

        dirty = false;
    }

    // if we are not visible -> free mem
    if (!keepInMemory)
        freeResources();
}

void Text::freeResources()
{
    DocumentCache::getInstance().push(doc);
    doc = nullptr;
}

QSizeF Text::idealSize()
{
    if (doc)
        return QSizeF(qMin(doc->idealWidth(), width), doc->size().height());

    return size;
}

int Text::cursorFromPos(QPointF scenePos, bool fuzzy) const
{
    if (doc)
        return doc->documentLayout()->hitTest(mapFromScene(scenePos), fuzzy ? Qt::FuzzyHit : Qt::ExactHit);

    return -1;
}

int Text::getSelectionEnd() const
{
    return qMax(selectionAnchor, selectionEnd);
}

int Text::getSelectionStart() const
{
    return qMin(selectionAnchor, selectionEnd);
}

bool Text::hasSelection() const
{
    return selectionEnd >= 0;
}

QString Text::extractSanitizedText(int from, int to) const
{
    if (!doc)
        return "";

    //QString txt;
    QTextBlock block = doc->firstBlock();

   /* <p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">
            <span style=" font-family:'幼圆'; font-size:12pt; font-weight:600; font-style:italic; text-decoration: underline;">
            </span><span style=" font-family:'华文行楷'; font-size:12pt;">gfdhgdhgfhgf</span></p></body></html>"
    QString htmlP="<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">")*/
    QString htmlSpanAll;
    for(int i=0; i< doc->blockCount(); i++)
    {
        qDebug()<<doc->blockCount();
    for (QTextBlock::Iterator itr = block.begin(); itr!=block.end(); ++itr)
    {
        qDebug()<<"into block";
        int pos = itr.fragment().position(); //fragment position -> position of the first character in the fragment

        if (itr.fragment().charFormat().isImageFormat())
        {

            QTextImageFormat imgFmt = itr.fragment().charFormat().toImageFormat();
            QString key = imgFmt.name(); //img key (eg. key::D for :D)
            //QString rune = key.mid(4);
            qDebug()<<key<<"image text";
            if (pos >= from && pos < to)
            {

                int width=400, heigth=200;
                if(imgFmt.width()<width)
                    width=imgFmt.width();

                if(imgFmt.height()<heigth)
                    heigth=imgFmt.height();

                htmlSpanAll +=QString("<img width=%3 height=%2 src=%1>").arg(key).arg(heigth).arg(width);
                pos++;
            }
        }
        else
        {
            QFont font;
            for (QChar c : itr.fragment().text())
            {
                QString htmlSpan;
                if (pos >= from && pos < to)
                {
                    QTextCursor cursor(doc);
                    cursor.setPosition(pos);

                        if(font!=(cursor.charFormat().font()))
                        {
                            font=cursor.charFormat().font();
                            htmlSpan=QString("<span style=\" font-family:'%1'; font-size:%2pt; font-weight:%3; font-style:%4; text-decoration: %5;\">")\
                                .arg(font.family()).arg(font.pointSizeF()).arg((font.weight()==75)?600:0).arg(font.italic()?"italic":"").arg(font.underline()?"underline":"");
                        }
                        else
                        {

                        }
                        htmlSpan+=c;
                }
                htmlSpanAll+=htmlSpan;
                pos++;
            }
        }
    }
    block=block.next();
    }

    return htmlSpanAll;
}

QString Text::extractImgTooltip(int pos) const
{

    for (QTextBlock::Iterator itr = doc->firstBlock().begin(); itr!=doc->firstBlock().end(); ++itr)
    {
        if (itr.fragment().contains(pos) && itr.fragment().charFormat().isImageFormat())
        {
            QTextImageFormat imgFmt = itr.fragment().charFormat().toImageFormat();

            return imgFmt.name();
        }
    }
    return "not picture";
}
QString Text::previewImage(int pos)
{

    QString picturepath;
    QTextBlock block=doc->firstBlock();
    for(int i=0; i< doc->blockCount(); i++)
    {
        for (QTextBlock::Iterator itr = block.begin(); itr!=block.end(); ++itr)
        {
            if (itr.fragment().contains(pos) && itr.fragment().charFormat().isImageFormat())
            {
                QTextImageFormat imgFmt = itr.fragment().charFormat().toImageFormat();

                picturepath=imgFmt.name();
            }
        }
        block=block.next();
    }
    return picturepath;
}
