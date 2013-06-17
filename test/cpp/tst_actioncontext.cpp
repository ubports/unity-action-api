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

#include "tst_actioncontext.h"

#include <unity/action/ActionContext>
#include <unity/action/ActionManager>
#include <unity/action/Action>

#include <QtTest/QtTest>

void
TestActionContext::setActive()
{
    unity::action::ActionContext ctx;

    ctx.setActive(false);
    QSignalSpy spy(&ctx, SIGNAL(activeChanged(bool)));
    ctx.setActive(true);
    QVERIFY(ctx.active() == true);
    QCOMPARE(spy.count(), 1);
    ctx.setActive(false);
    QVERIFY(ctx.active() == false);
    QCOMPARE(spy.count(), 2);

    QList<QVariant> arguments;
    arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toBool() == true);
    arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toBool() == false);

    spy.clear();
    ctx.setActive(false);
    QCOMPARE(spy.count(), 0);
}

void
TestActionContext::actionOperations()
{
    unity::action::ActionContext *ctx = new unity::action::ActionContext(this);
    unity::action::Action *action1 = new unity::action::Action(this);
    unity::action::Action *action2 = new unity::action::Action(this);

    QSignalSpy spy(ctx, SIGNAL(actionsChanged()));
    ctx->addAction(action1);
    ctx->addAction(action2);
    ctx->addAction(action2);
    QVERIFY(spy.count() == 2);
    QVERIFY(ctx->actions().contains(action1) &&
            ctx->actions().contains(action2));

    ctx->removeAction(action1);
    ctx->removeAction(action1);
    QVERIFY(spy.count() == 3);
    QVERIFY(!ctx->actions().contains(action1) &&
             ctx->actions().contains(action2));

    ctx->addAction(action1);
    QVERIFY(spy.count() == 4);
    QVERIFY(ctx->actions().contains(action1) &&
            ctx->actions().contains(action2));
}

