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

#include "chatline.h"
#include "chatlinecontent.h"

#include <QDebug>
#include <QGraphicsScene>
#include "chatlinecontentproxy.h"

ChatLine::ChatLine()
{

}

ChatLine::~ChatLine() //从场景移除
{
    qDebug()<<"delete chatLine";
    for (ChatLineContent* c : content)
    {
        if (c->scene())
            c->scene()->removeItem(c);

        delete c;
    }


}

void ChatLine::setRow(int idx)   //设置行
{
    row = idx;

    for (int c = 0; c < static_cast<int>(content.size()); ++c)
        content[c]->setIndex(row, c);          //设置内容多少行多少列

}

void ChatLine::visibilityChanged(bool visible)    //是否显示
{
    if (isVisible != visible)
    {
        for (ChatLineContent* c : content)
        {

            c->visibilityChanged(visible);

        }
    }

    isVisible = visible;
}

int ChatLine::getRow() const     //返回在第几行
{
    return row;
}

ChatLineContent *ChatLine::getContent(int col) const  //获取列的内容
{
    if (col < static_cast<int>(content.size()) && col >= 0)
        return content[col];

    return nullptr;
}

ChatLineContent *ChatLine::getContent(QPointF scenePos) const //获取场景位置点0的项
{
    for (ChatLineContent* c: content)
    {

        if (c->sceneBoundingRect().contains(scenePos))
            return c;
    }

    return nullptr;
}

void ChatLine::removeFromScene()  //移除一行从场景
{
    for (ChatLineContent* c : content)
    {
        if (c->scene())
            c->scene()->removeItem(c);
    }
}

void ChatLine::addToScene(QGraphicsScene *scene) //增加到场景
{
    if (!scene)
        return;

    for (ChatLineContent* c : content)
        scene->addItem(c);
}

void ChatLine::setVisible(bool visible)  //设置行可见
{
    for (ChatLineContent* c : content)
        c->setVisible(visible);
}

void ChatLine::selectionCleared()     //选择清除
{
    for (ChatLineContent* c : content)
        c->selectionCleared();
}

void ChatLine::selectionFocusChanged(bool focusIn)  //选择焦点改变
{
    for (ChatLineContent* c : content)
        c->selectionFocusChanged(focusIn);
}

int ChatLine::getColumnCount()    //获取列数
{
    return content.size();
}

void ChatLine::update()
{
    for (ChatLineContent* c : content)
        c->update(c->boundingRect());
}

void ChatLine::updateBBox()        //更行BBox
{
    bbox.setHeight(0);
    bbox.setWidth(width);

    for (ChatLineContent* c : content)
    {


        bbox.setHeight(qMax(c->sceneBoundingRect().top() - bbox.top() + c->sceneBoundingRect().height(), bbox.height()));//

    }
}

QRectF ChatLine::sceneBoundingRect() const
{
    return bbox;
}

void ChatLine::addColumn(ChatLineContent* item, ColumnFormat fmt)
{
    if (!item)
        return;
    format.push_back(fmt);
    content.push_back(item);
}


void ChatLine::replaceContent(int col, ChatLineContent *lineContent)
{
    if (col >= 0 && col < static_cast<int>(content.size()) && lineContent)
    {
        QGraphicsScene* scene = content[col]->scene();
        delete content[col];

        content[col] = lineContent;
        lineContent->setIndex(row, col);

        if (scene)
            scene->addItem(content[col]);

        layout(width, bbox.topLeft());
        content[col]->visibilityChanged(isVisible);
        content[col]->update();      //重新绘制
    }
}

void ChatLine::layout(qreal w, QPointF scenePos)  //宽度 起始坐标
{
    if (!content.size())
        return;

    width = w;
    bbox.setTopLeft(scenePos); //

    qreal fixedWidth = (content.size()-1) * columnSpacing;
    qreal varWidth = 0.0; // used for normalisation

    for (size_t i = 0; i < format.size(); ++i)
    {
        if (format[i].policy == ColumnFormat::FixedSize)
            fixedWidth += format[i].size;
        else
            varWidth += format[i].size;
    }

    if (varWidth == 0.0)
        varWidth = 1.0;

    qreal leftover = qMax(0.0, width - fixedWidth);

    qreal maxVOffset = 0.0;
    qreal xOffset = 0.0;
    qreal xPos[content.size()];

    for (size_t i = 0; i < content.size(); ++i)
    {
        // calculate the effective width of the current column
        qreal width;
        if (format[i].policy == ColumnFormat::FixedSize)
            width = format[i].size;
        else
            width = format[i].size / varWidth * leftover;  //平分剩余的列宽

        // set the width of the current column

        content[i]->setWidth(width);            //设置没一列的宽度


        // calculate horizontal alignment
        qreal xAlign = 0.0;

        switch(format[i].hAlign)
        {
            case ColumnFormat::Right:
                xAlign = width - content[i]->boundingRect().width();
                break;
            case ColumnFormat::Center:
                xAlign = (width - content[i]->boundingRect().width()) / 2.0;
                break;
            default:
                break;
        }

        // reposition
        xPos[i] = scenePos.x() + xOffset + xAlign;

        xOffset += width + columnSpacing;
        maxVOffset = qMax(maxVOffset, content[i]->getAscent()); //最大行高

    }

    for (size_t i = 0; i < content.size(); ++i)
    {
        // calculate vertical alignment
        // vertical alignment may depend on width, so we do it in a second pass

        qreal yOffset = maxVOffset - content[i]->getAscent();

        // reposition

        content[i]->setPos(xPos[i], scenePos.y() + yOffset);

    }

    updateBBox();
}

void ChatLine::moveBy(qreal deltaY)  //垂直方向上移动
{
    // reposition only
    for (ChatLineContent* c : content)
        c->moveBy(0, deltaY);

    bbox.moveTop(bbox.top() + deltaY);
}

bool ChatLine::lessThanBSRectTop(const ChatLine::Ptr& lhs, const qreal& rhs)
{
    return lhs->sceneBoundingRect().top() < rhs;
}

bool ChatLine::lessThanBSRectBottom(const ChatLine::Ptr& lhs, const qreal& rhs)
{
    return lhs->sceneBoundingRect().bottom() < rhs;
}

bool ChatLine::lessThanRowIndex(const ChatLine::Ptr& lhs, const ChatLine::Ptr& rhs)
{
    return lhs->getRow() < rhs->getRow();
}
