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

// needed for gio includes.
#undef signals
#include <gio/gio.h>

using namespace unity::action;

void
TestActionManager::initTestCase()
{
    manager = new ActionManager(this);
}

void
TestActionManager::testGlobalContext()
{
    ActionContext *globalctx = manager->globalContext();
    QVERIFY(globalctx != 0);
}

void
TestActionManager::actionOperations()
{
    Action *action1 = new Action(manager);
    action1->setText("My Global Action");
    action1->setName("Global");

    Action *action2 = new Action(manager);
    action2->setText("Foo");


    QSignalSpy spy(manager, SIGNAL(actionsChanged()));
    manager->addAction(action1);
    QCOMPARE(spy.count(), 1);
    manager->addAction(action2);
    QCOMPARE(spy.count(), 2);
    manager->addAction(action2);
    QCOMPARE(spy.count(), 2);

    QSet<Action *>actions = manager->actions();
    QVERIFY(actions.contains(action1) && actions.contains(action2));

    manager->removeAction(action2);
    QCOMPARE(spy.count(), 3);
    manager->removeAction(action2);
    QCOMPARE(spy.count(), 3);
    actions = manager->actions();
    QVERIFY(actions.contains(action1) && !actions.contains(action2));

    manager->removeAction(action1);

    /*! \todo verify from the bus that actions appear there */
}

void
TestActionManager::contextOperations()
{
    ActionContext *gctx = manager->globalContext();
    ActionContext *ctx1 = new ActionContext(manager);
    ActionContext *ctx2 = new ActionContext(manager);

    Action *action1 = new Action(manager);
    Action *action2 = new Action(manager);
    Action *action3 = new Action(manager);
    Action *action4 = new Action(manager);
    Action *action5 = new Action(manager);

    gctx->addAction(action1);

    QSignalSpy ctxspy(manager, SIGNAL(localContextsChanged()));
    manager->addLocalContext(ctx1);
    QCOMPARE(ctxspy.count(), 1);
    ctx1->addAction(action2);
    ctx1->addAction(action3);


    ctx2->addAction(action4);
    ctx2->addAction(action5);
    manager->addLocalContext(ctx2);
    manager->addLocalContext(ctx2);
    QCOMPARE(ctxspy.count(), 2);

    QVERIFY(manager->localContexts().contains(ctx1) &&
            manager->localContexts().contains(ctx2));

    manager->removeLocalContext(ctx2);
    manager->removeLocalContext(ctx2);
    QVERIFY( manager->localContexts().contains(ctx1) &&
            !manager->localContexts().contains(ctx2));
    QCOMPARE(ctxspy.count(), 3);
    manager->addLocalContext(ctx2);
    QCOMPARE(ctxspy.count(), 4);
    QVERIFY(manager->localContexts().contains(ctx1) &&
            manager->localContexts().contains(ctx2));

    ctx2->setActive(true);
    ctx1->setActive(true);
    QVERIFY(!ctx2->active());

    /*! \todo verify actions from the bus */
    gctx->removeAction(action1);
    gctx->addAction(action1);

    gctx->removeAction(action1);
    manager->removeLocalContext(ctx1);
    manager->removeLocalContext(ctx2);
    QVERIFY(manager->actions().count() == 0);
}

void
TestActionManager::actionPropertyChanges()
{
    ActionContext *ctx1 = new ActionContext(manager);
    ActionContext *ctx2 = new ActionContext(manager);

    Action *action1 = new Action(manager);
    Action *action2 = new Action(manager);
    Action *action3 = new Action(manager);
    Action *action4 = new Action(manager);
    Action *action5 = new Action(manager);
    Action *action6 = new Action(manager);

    GDBusConnection *dbusc = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, NULL);
    const gchar *my_name = g_dbus_connection_get_unique_name(dbusc);
    GDBusActionGroup *action_group = g_dbus_action_group_get(dbusc,
                                                             my_name,
                                                             "/com/canonical/unity/actions");

    QList<QVariant> arguments;


    manager->globalContext()->addAction(action1);
    manager->globalContext()->addAction(action2);
    ctx1->addAction(action3);
    ctx1->addAction(action4);
    ctx2->addAction(action5);
    ctx2->addAction(action6);
    manager->addLocalContext(ctx1);
    manager->addLocalContext(ctx2);

    // force both local context to be disabled
    ctx1->setActive(false);
    ctx2->setActive(false);

    action1->setName("Global");
    action1->setText("My Global Action");
    QSignalSpy action1_spy(action1, SIGNAL(triggered(QVariant)));
    QTest::qWait(100);
    g_action_group_activate_action(G_ACTION_GROUP(action_group), "Global", NULL);
    action1_spy.wait();
    QCOMPARE(action1_spy.count(), 1);
    arguments = action1_spy.takeFirst();
    QVERIFY(arguments.at(0).isNull());

    action2->setText("Action2");
    action2->setEnabled(false);


    ctx1->setActive(true);


    action3->setText("Action3");
    action3->setName("InsideContext");

    action4->setName("CallTo");
    action4->setParameterType(Action::String);
    QSignalSpy action4_spy(action4, SIGNAL(triggered(QVariant)));
    QTest::qWait(100);
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "CallTo",
                                   g_variant_new_string("tel://+0123456789"));
    action4_spy.wait();
    QCOMPARE(action4_spy.count(), 1);
    arguments = action4_spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("tel://+0123456789"));


    ctx2->setActive(true);


    QSignalSpy action5_spy(action5, SIGNAL(triggered(QVariant)));
    action5->setParameterType(Action::Bool);
    QTest::qWait(100);
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   qPrintable(action5->name()),
                                   g_variant_new_boolean(true));
    action5_spy.wait();
    action5->setParameterType(Action::Integer);
    QTest::qWait(100);
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   qPrintable(action5->name()),
                                   g_variant_new_int32(1337));
    action5_spy.wait();
    action5->setParameterType(Action::Real);
    QTest::qWait(100);
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   qPrintable(action5->name()),
                                   g_variant_new_double(5.5));
    action5_spy.wait();
    QCOMPARE(action5_spy.count(), 3);
    arguments = action5_spy.takeFirst();
    QCOMPARE(arguments.at(0).toBool(), true);
    arguments = action5_spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 1337);
    arguments = action5_spy.takeFirst();
    QCOMPARE(arguments.at(0).toReal(), 5.5);

    action6->setParameterType(Action::String);
    action6->setParameterType(Action::None);

    manager->removeLocalContext(ctx1);
    manager->removeLocalContext(ctx2);
    manager->removeAction(action1);
    manager->removeAction(action2);

    g_clear_object(&action_group);
}

void
TestActionManager::deletedGlobalContext()
{
    // detect if the client accidentally calls delete on global context
    // and make sure we don't crash
    delete manager->globalContext();
}

void
TestActionManager::deletedLocalContext()
{
    // if local context is added to the manager and delete is called for it
    // before it's removed we must detect this and not crash

    ActionContext *ctx1 = new ActionContext();
    ActionContext *ctx2 = new ActionContext();

    Action *action1 = new Action(ctx1);
    Action *action2 = new Action(ctx1);
    Action *action3 = new Action(ctx1);
    Action *action4 = new Action(ctx2);
    Action *action5 = new Action(ctx2);


    ctx1->addAction(action1);
    ctx1->addAction(action2);
    ctx1->addAction(action3);
    manager->addLocalContext(ctx1);

    ctx2->addAction(action4);
    ctx2->addAction(action5);
    manager->addLocalContext(ctx2);

    QSignalSpy ctxspy(manager, SIGNAL(localContextsChanged()));

    delete ctx1;
    QCOMPARE(ctxspy.count(), 1);
    QVERIFY(!manager->localContexts().contains(ctx1));
    ctx1 = 0;

    delete ctx2;
    QCOMPARE(ctxspy.count(), 2);
    QVERIFY(!manager->localContexts().contains(ctx2));
    ctx2 = 0;
}

void
TestActionManager::deletedAction()
{
    // if action is added to the manager and delete is called for it
    // before it's removed we must detect this and not crash

    ActionContext *gctx = manager->globalContext();
    ActionContext *ctx1 = new ActionContext();
    ActionContext *ctx2 = new ActionContext();

    Action *action1 = new Action(gctx);
    Action *action2 = new Action(ctx1);
    Action *action3 = new Action(ctx1);
    Action *action4 = new Action(ctx2);
    Action *action5 = new Action(ctx2);


    gctx->addAction(action1);

    ctx1->addAction(action2);
    ctx1->addAction(action3);
    manager->addLocalContext(ctx1);

    ctx2->addAction(action4);
    ctx2->addAction(action5);
    manager->addLocalContext(ctx2);

    QSignalSpy spy(manager, SIGNAL(actionsChanged()));
    QCOMPARE(manager->actions().count(), 5);

    // delete action1 directly from globalContext
    delete action1;
    QCOMPARE(spy.count(), 1);
    QVERIFY(!manager->actions().contains(action1));
    QCOMPARE(manager->actions().count(), 4);
    action1 = 0;

    // delete action2 directly from ctx1
    delete action2;
    QCOMPARE(spy.count(), 2);
    QVERIFY(!manager->actions().contains(action2));
    QCOMPARE(manager->actions().count(), 3);
    action2 = 0;

    // remove action3 indirectly by destroying ctx1
    delete ctx1;
    QCOMPARE(spy.count(), 3);
    QVERIFY(!manager->actions().contains(action3));
    QCOMPARE(manager->actions().count(), 2);
    ctx1 = 0;
    action3 = 0;

    // delete action4 and action5 indirectly by destroying ctx2
    delete ctx2;
    QCOMPARE(spy.count(), 5); // actionsChanged() gets called multiple times (once per each action in a context)
    QVERIFY(!manager->actions().contains(action4));
    QVERIFY(!manager->actions().contains(action5));
    QCOMPARE(manager->actions().count(), 0);
    QVERIFY(manager->localContexts().isEmpty());
    ctx2 = 0;
    action4 = 0;
    action5 = 0;

}

