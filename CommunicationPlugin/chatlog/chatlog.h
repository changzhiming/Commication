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

#ifndef CHATLOG_H
#define CHATLOG_H

#include <QGraphicsView>
#include <QDateTime>
#include <QMargins>
#include <QMimeData>
#include <QDebug>
#include <QMoveEvent>

#include "core/core.h"
#include "chatline.h"
#include "chatmessage.h"

class QGraphicsScene;
class QGraphicsRectItem;   //正方形项
class QMouseEvent;
class QTimer;
class ChatLineContent;     //单行项

class ChatLog : public QGraphicsView
{
    Q_OBJECT
public:

    explicit ChatLog(QWidget* parent = 0);

    virtual ~ChatLog();

    void insertChatlineAtBottom(ChatLine::Ptr l);
    void insertChatlineOnTop(ChatLine::Ptr l);
    void insertChatlineOnTop(const QList<ChatLine::Ptr>& newLines);
    void removeFirst();
    void clearSelection();
    void clear();
    void copySelectedText(bool toSelectionBuffer = false);
    void setBusyNotification(ChatLine::Ptr notification);
    void setTypingNotification(ChatLine::Ptr notification);
    void setTypingNotificationVisible(bool visible);
    void scrollToLine(ChatLine::Ptr line);

    void selectAll();
    void forceRelayout();
    //
    QString getSelectedText() const;

    bool isEmpty() const;
    bool hasTextToBeCopied() const;     //是否有文本拷贝

    ChatLine::Ptr getTypingNotification() const;
    QVector<ChatLine::Ptr> getLines();
    ChatLine::Ptr getLatestLine() const;
    // repetition interval sender name (sec)
    const uint repNameAfter = 5*60;

signals:
    void selectionChanged();

protected:
    QRectF calculateSceneRect() const;   //计算场景矩形
    QRect getVisibleRect() const;
    ChatLineContent* getContentFromPos(QPointF scenePos) const;

    void layout(int start, int end, qreal width);
    bool isOverSelection(QPointF scenePos) const;
    bool stickToBottom() const;

    qreal useableWidth() const;

    void reposition(int start, int end, qreal deltaY);
    void updateSceneRect();
    void checkVisibility();
    void scrollToBottom();
    void startResizeWorker();
    void contextMenuEvent(QContextMenuEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent* ev) final override;
    virtual void mousePressEvent(QMouseEvent* ev) final override;
    virtual void mouseReleaseEvent(QMouseEvent* ev) final override;
    virtual void mouseMoveEvent(QMouseEvent* ev) final override;
    virtual void scrollContentsBy(int dx, int dy) final override;
    virtual void resizeEvent(QResizeEvent* ev) final override;
   // virtual void showEvent(QShowEvent*) final override;
    virtual void focusInEvent(QFocusEvent* ev) final override;
    virtual void focusOutEvent(QFocusEvent* ev) final override;

    void updateMultiSelectionRect();
    void updateTypingNotification();
    void updateBusyNotification();

    ChatLine::Ptr findLineByPosY(qreal yPos) const;

private slots:
    void onSelectionTimerTimeout();
    void onWorkerTimeout();

private:
    void retranslateUi();

private:
    enum SelectionMode {
        None,
        Precise,
        Multi,
    };

    enum AutoScrollDirection {
        NoDirection,
        Up,
        Down,
    };

    QAction* copyAction = nullptr;
    QAction* selectAllAction = nullptr;
    QGraphicsScene* scene = nullptr;             //场景
    QGraphicsScene* busyScene = nullptr;
    QVector<ChatLine::Ptr> lines;           //所有的聊天行
    QList<ChatLine::Ptr> visibleLines;      //可见的聊天行

    ChatLine::Ptr typingNotification;
    ChatLine::Ptr busyNotification;

    // selection
    int selClickedRow = -1; //These 4 are only valid while selectionMode != None
    int selClickedCol = -1;
    int selFirstRow = -1;
    int selLastRow = -1;
    QColor selectionRectColor = QColor::fromRgbF(0.23, 0.68, 0.91).lighter(150);
    SelectionMode selectionMode = None;
    QPointF clickPos; //点击坐标
    QGraphicsRectItem* selGraphItem = nullptr;
    QTimer* selectionTimer = nullptr;        //选择定时器
    QTimer* workerTimer = nullptr;          //工作定时器
    AutoScrollDirection selectionScrollDir = NoDirection;   //滚动方向

    //worker vars
    int workerLastIndex = 0;
    bool workerStb = false;
    ChatLine::Ptr workerAnchorLine;   //

    // layout
    QMargins margins = QMargins(3,5,3,5);  //长方形的四个边缘
    qreal lineSpacing = 0;        //现款

    //ChatLog::Ptr mimeData;
    QMimeData *mimeData;
    QString cliptext;
};

#endif // CHATLOG_H
