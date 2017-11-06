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

#ifndef CHATLINE_H
#define CHATLINE_H

#include <memory>
#include <vector>
#include <QPointF>
#include <QRectF>

class ChatLog;
class ChatLineContent;
class QGraphicsScene;
class QStyleOptionGraphicsItem;
class QGraphicsItem;
class ChatLineContentProxy;

struct ColumnFormat   //列内容填充格式
{
    enum Policy {
        FixedSize,
        VariableSize,
    };

    enum Align {       //对齐方式
        Left,
        Center,
        Right,
    };

    ColumnFormat() {}
    ColumnFormat(qreal s, Policy p, Align halign = Left)
        : size(s)
        , policy(p)
        , hAlign(halign)
    {}

    qreal size = 1.0;   //宽度
    Policy policy = VariableSize;
    Align hAlign = Left;
};

using ColumnFormats = QVector<ColumnFormat>;

class ChatLine
{

public:
    using Ptr = std::shared_ptr<ChatLine>;

    ChatLine();
    virtual ~ChatLine();

    QRectF sceneBoundingRect() const;  //项目在场景的矩形

    void replaceContent(int col, ChatLineContent* lineContent);
    void layout(qreal width, QPointF scenePos);
    void moveBy(qreal deltaY);
    void removeFromScene();
    void addToScene(QGraphicsScene* scene);
    void setVisible(bool visible);
    void selectionCleared();
    void selectionFocusChanged(bool focusIn);

    void update();

    int getColumnCount();
    int getRow() const;

    ChatLineContent* getContent(int col) const;
    ChatLineContent* getContent(QPointF scenePos) const;

    bool isOverSelection(QPointF scenePos);

    //comparators
    static bool lessThanBSRectTop(const ChatLine::Ptr& lhs, const qreal& rhs);
    static bool lessThanBSRectBottom(const ChatLine::Ptr& lhs, const qreal& rhs);
    static bool lessThanRowIndex(const ChatLine::Ptr& lhs, const ChatLine::Ptr& rhs);

protected:
    friend class ChatLog;

    QPointF mapToContent(ChatLineContent* c, QPointF pos);

    void addColumn(ChatLineContent* item, ColumnFormat fmt);

    void updateBBox();
    void setRow(int idx);
    void visibilityChanged(bool visible);

private:
    int row = -1;                           //初始行
    std::vector<ChatLineContent*> content;  //行内容列表
    std::vector<ColumnFormat> format;     //格式列表
    qreal width = 100.0;                 //宽度
    qreal columnSpacing = 5.0;          // 列间距
    QRectF bbox;
    bool isVisible = false;             //不可见

};

#endif // CHATLINE_H
