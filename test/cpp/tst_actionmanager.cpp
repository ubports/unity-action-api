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

#include "tst_actionmanager.h"

#include <unity/action/ActionManager>
#include <unity/action/ActionContext>
#include <unity/action/Action>

#include <QtTest/QtTest>

using namespace unity::action;

void
TestActionManager::testGlobalContext()
{

}

void
TestActionManager::actionOperations()
{

}

void
TestActionManager::contextOperations()
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
    action6->setParameterType(Action::String);
    action6->setParameterType(Action::None);
}

