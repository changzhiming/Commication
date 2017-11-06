#include "mainmenustackedwidget.h"
#include "ui_mainmenustackedwidget.h"
#include <QDebug>
#include <QPainter>

MainMenuStackedWidget::MainMenuStackedWidget(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::MainMenuStackedWidget)
{
    this->setStyleSheet("QPushButton{color: blue;font: italic  \"Times New Roman\"; text-decoration: underline}\
                         QToolButton{text-align: bottom }");
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(changedPage()));

    ui->pushButton->installEventFilter(this);
    currentgridLayout=ui->gridLayout;
    for(int i=0; i<10; i++)
    {
        this->addToolButton(new MainFunctionButton(MainFunctionButton::TEXT));
    }

}

MainMenuStackedWidget::~MainMenuStackedWidget()
{
    delete ui;
}

void MainMenuStackedWidget::changedPage()
{
    qDebug()<<this->count()<<this->currentIndex();
    if(this->count()==this->currentIndex()+1)
    {
        this->setCurrentIndex(0);
    }
     else
    {
        this->setCurrentIndex(this->currentIndex()+1);
    }
}

bool MainMenuStackedWidget::eventFilter(QObject *obj, QEvent *ev)
{
    QList<QPushButton *> list=this->findChildren<QPushButton *>();
    foreach (QPushButton *button, list) {
        if(obj==button)
        {
            if(ev->type()==QEvent::Enter)
            {
                this->setCursor(Qt::PointingHandCursor);
            }
            else if(ev->type()==QEvent::Leave)
            {
                this->setCursor(Qt::ArrowCursor);
            }
        }

    }

    return QStackedWidget::eventFilter(obj, ev);
}

void MainMenuStackedWidget::addToolButton(MainFunctionButton *button)
{
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    button->setSizePolicy(sizePolicy);
    button->setMinimumSize(QSize(100, 120));

    button->setIconSize(QSize(90, 70));
    button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    if(this->count()>=1)
    {
        QWidget *curwidget=this->widget(this->count()-1);
        QList<MainFunctionButton *> widgetList=curwidget->findChildren<MainFunctionButton *>();
        if(widgetList.size()<9)
        {
            currentgridLayout->addWidget(button, widgetList.size()/3, widgetList.size()%3);
            return;
        }
    }
    //新建一页
        QWidget *nextWidget=new QWidget();
        QPushButton *pushButton = new QPushButton(nextWidget);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setFlat(true);
        pushButton->setText(tr("下一页"));
        connect(pushButton, SIGNAL(clicked()), this, SLOT(changedPage()));
        pushButton->installEventFilter(this);

        QVBoxLayout *vboxLayout=new QVBoxLayout(nextWidget);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setSpacing(60);

        this->addWidget(nextWidget);
        QGridLayout *gridLayout=new QGridLayout();
        currentgridLayout=gridLayout;
        vboxLayout->addLayout(gridLayout);
        vboxLayout->addWidget(pushButton);
        currentgridLayout->addWidget(button, 0, 0);
}
