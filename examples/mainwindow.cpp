#include "mainwindow.h"

#include <unity/action/ActionManager>
#include <unity/action/ActionContext>
#include <unity/action/Action>

#include <QDebug>

using namespace unity::action;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ActionManager *manager = new ActionManager(this);
    ActionContext *ctx1 = new ActionContext(this);
    ActionContext *ctx2 = new ActionContext(this);

    Action *action1 = new Action(this);
    Action *action2 = new Action(this);
    Action *action3 = new Action(this);
    Action *action4 = new Action(this);
    Action *action5 = new Action(this);
    Action *action6 = new Action(this);

    action4->setParameterType(Action::String);

    manager->addAction(action1);
    manager->addAction(action2);
    manager->addAction(action3);
    manager->addAction(action4);
    manager->addAction(action5);
    manager->addAction(action6);

    manager->globalContext()->addAction(action1);
    manager->addLocalContext(ctx1);

    ctx1->addAction(action2);
    ctx1->addAction(action5);
    ctx2->addAction(action3);
    ctx2->addAction(action6);

    ctx2->setActive(true);

    manager->addLocalContext(ctx2);

    //ctx2->removeAction(action3);
    //manager->removeAction(action3);

    action1->setName("Global");
    action1->setText("My Global Action");

    action2->setText("Action2");
    action2->setEnabled(false);

    action3->setText("Action3");
    action3->setName("InsideContext");

    action4->setName("CallTo");

    action5->setParameterType(Action::Bool);
    action5->setText("Action5");

    action6->setParameterType(Action::String);
    action6->setParameterType(Action::None);
    action6->setText("Action6");

    connect(action1, &Action::triggered, [=](const QVariant &arg) {
        qDebug() << "Action1 triggered.";
    });
    connect(action2, &Action::triggered, [=](const QVariant &arg) {
        qDebug() << "Action2 triggered.";
    });
    connect(action3, &Action::triggered, [=](const QVariant &arg) {
        qDebug() << "Action3 triggered.";
    });
    connect(action4, &Action::triggered, [=](const QVariant &arg) {
        qDebug() << "Action4 triggered: " << arg.toString();
    });

    connect(action6, &Action::triggered, [=](const QVariant &arg) {
        qDebug() << "Action6 triggered.";
    });
}

MainWindow::~MainWindow()
{
    
}
