/* This file is part of unity-action-api
 * Copyright 2013 Canonical Ltd.
 *
 * unity-action-api is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * unity-action-api is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include <QMainWindow>

#include <unity/action/ActionManager>
#include <unity/action/Action>
#include <unity/action/ActionContext>

#include <QDebug>

using namespace unity::action;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
//! [create global action]
    Action *globalAction = new Action(this);
    globalAction->setText("My Global");
//! [create global action]

//! [create local actions]
    Action *ctx1Action1 = new Action(this);
    ctx1Action1->setText("Context1, Action 1");
    Action *ctx1Action2 = new Action(this);
    ctx1Action2->setText("Context1, Action 2");

    Action *ctx2Action1 = new Action(this);
    ctx2Action1->setText("Context2, Action 1");
    Action *ctx2Action2 = new Action(this);
    ctx2Action2->setText("Context2, Action 2");
//! [create local actions]

//! [create contexts]
    ActionContext *ctx1 = new ActionContext(this);
    ActionContext *ctx2 = new ActionContext(this);
//! [create contexts]


//! [add context actions]
    ctx1->addAction(ctx1Action1);
    ctx1->addAction(ctx1Action2);

    ctx2->addAction(ctx2Action1);
    ctx2->addAction(ctx2Action2);
//! [add context actions]

//! [add global to manager]
    ActionManager *manager = new ActionManager(this);
    manager->addAction(globalAction);
//! [add global to manager]

//! [add local ctx to manager]
    manager->addLocalContext(ctx1);
    manager->addLocalContext(ctx2);
//! [add local ctx to manager]

//! [set context active]
    ctx2->setActive(true);
    // now the ActionManager has exported globalAction, ctx2Action1 and ctx2Action2
//! [set context active]

}

MainWindow::~MainWindow()
{}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

#include "action-context.moc"
