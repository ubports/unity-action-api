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

#include <unity/action/PreviewAction>
#include <unity/action/PreviewRangeParameter>

#include <QtTest/QtTest>

// needed for gio includes.
#undef signals
#include <gio/gio.h>

using namespace unity::action;

void
TestActionManager::initTestCase()
{
    manager = new ActionManager(this);
    dbusc = 0;
    action_group = 0;

    dbusc = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, NULL);
    QVERIFY2(dbusc != 0, "Could not get session bus.");
    if (dbusc) {
        action_group = g_dbus_action_group_get(dbusc,
                                               g_dbus_connection_get_unique_name(dbusc),
                                               "/com/canonical/unity/actions");
        QVERIFY2(action_group != 0, "Could not get action group");
    }
}

void
TestActionManager::cleanupTestCase()
{
    g_clear_object(&dbusc);
    g_clear_object(&action_group);
}

void
TestActionManager::cleanup()
{
    // after each test make sure the manager is left in a clean state
    QVERIFY(manager->actions().count() == 1);
    QVERIFY(manager->localContexts().isEmpty());
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
    ActionContext *ctx3 = new ActionContext(manager);

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

    // if new context is added and it's active
    // make it the active local one in the manager.
    ctx3->setActive(true);
    manager->addLocalContext(ctx3);
    QVERIFY(!ctx1->active());
    QVERIFY(!ctx2->active());
    QVERIFY(ctx3->active());


    /*! \todo verify actions from the bus */
    gctx->removeAction(action1);
    gctx->addAction(action1);

    gctx->removeAction(action1);
    manager->removeLocalContext(ctx1);
    manager->removeLocalContext(ctx2);
    manager->removeLocalContext(ctx3);
    QVERIFY(manager->actions().count() == 1);
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
                                   g_variant_new_double(5.5f));
    action5_spy.wait();
    QCOMPARE(action5_spy.count(), 3);
    arguments = action5_spy.takeFirst();
    QCOMPARE(arguments.at(0).toBool(), true);
    arguments = action5_spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 1337);
    arguments = action5_spy.takeFirst();
    QCOMPARE(arguments.at(0).toFloat(), 5.5f);

    action6->setParameterType(Action::String);
    action6->setParameterType(Action::None);

    manager->removeLocalContext(ctx1);
    manager->removeLocalContext(ctx2);
    manager->removeAction(action1);
    manager->removeAction(action2);
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
    QCOMPARE(manager->actions().count(), 6);

    // delete action1 directly from globalContext
    delete action1;
    QCOMPARE(spy.count(), 1);
    QVERIFY(!manager->actions().contains(action1));
    QCOMPARE(manager->actions().count(), 5);
    action1 = 0;

    // delete action2 directly from ctx1
    delete action2;
    QCOMPARE(spy.count(), 2);
    QVERIFY(!manager->actions().contains(action2));
    QCOMPARE(manager->actions().count(), 4);
    action2 = 0;

    // remove action3 indirectly by destroying ctx1
    delete ctx1;
    QCOMPARE(spy.count(), 3);
    QVERIFY(!manager->actions().contains(action3));
    QCOMPARE(manager->actions().count(), 3);
    ctx1 = 0;
    action3 = 0;

    // delete action4 and action5 indirectly by destroying ctx2
    delete ctx2;
    QCOMPARE(spy.count(), 5); // actionsChanged() gets called multiple times (once per each action in a context)
    QVERIFY(!manager->actions().contains(action4));
    QVERIFY(!manager->actions().contains(action5));
    QCOMPARE(manager->actions().count(), 1);
    QVERIFY(manager->localContexts().isEmpty());
    ctx2 = 0;
    action4 = 0;
    action5 = 0;

}

void
TestActionManager::actionInMultipleContext()
{
    /* If Action is added to multiple contexts
     * the manager has to be aware of the action
     * as long as it's part of any of the contexts.
     */

    ActionContext *gctx = manager->globalContext();
    ActionContext *ctx1 = new ActionContext(manager);
    ActionContext *ctx2 = new ActionContext(manager);

    Action *action1 = new Action(manager);
    Action *action2 = new Action(manager);
    Action *action3 = new Action(manager);

    QSignalSpy spy(manager, SIGNAL(actionsChanged()));

    gctx->addAction(action1);

    ctx1->addAction(action1);
    ctx1->addAction(action2);

    ctx2->addAction(action1);
    ctx2->addAction(action3);

    manager->addLocalContext(ctx1);
    manager->addLocalContext(ctx2);

    // 3 actions have been added in total
    QCOMPARE(spy.count(), 3);
    QCOMPARE(manager->actions().count(), 4);

    spy.clear();
    manager->removeLocalContext(ctx2); // removes only action3, action1 is shared.
    QCOMPARE(spy.count(), 1);
    QCOMPARE(manager->actions().count(), 3);
    // and now add it back
    manager->addLocalContext(ctx2);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(manager->actions().count(), 4);

    spy.clear();
    gctx->removeAction(action1);
    QCOMPARE(spy.count(), 0);
    QCOMPARE(manager->actions().count(), 4);
    QVERIFY(manager->actions().contains(action1));

    ctx1->removeAction(action1);
    QCOMPARE(spy.count(), 0);
    QCOMPARE(manager->actions().count(), 4);
    QVERIFY(manager->actions().contains(action1));

    // remove the action from the last context
    ctx2->removeAction(action1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(manager->actions().count(), 3);
    QVERIFY(!manager->actions().contains(action1));

    manager->removeLocalContext(ctx1);
    manager->removeLocalContext(ctx2);
}

void
TestActionManager::localContextOverridesGlobalContext()
{
    /* If local context defines an action with the same
     * name with an action in the global context the
     * local context action must override the global
     * one.
     */

    ActionContext *gctx = manager->globalContext();
    ActionContext *ctx1 = new ActionContext(manager);
    ActionContext *ctx2 = new ActionContext(manager);

    Action *action1 = new Action(manager);
    Action *action2 = new Action(manager);
    Action *action3 = new Action(manager);

    QSignalSpy spy1(action1, SIGNAL(triggered(QVariant)));
    QSignalSpy spy2(action2, SIGNAL(triggered(QVariant)));
    QSignalSpy spy3(action3, SIGNAL(triggered(QVariant)));

    action1->setName("MyAction");
    action2->setName("MyAction");
    action3->setName("MyAction");

    ctx1->addAction(action2);
    ctx2->addAction(action3);

    gctx->addAction(action1);
    manager->addLocalContext(ctx1);
    manager->addLocalContext(ctx2);


    /* no local context has been activated.
     * triggering Myaction must now trigger action1
     */
    QTest::qWait(100);
    g_action_group_activate_action(G_ACTION_GROUP(action_group), "MyAction", NULL);
    spy1.wait();
    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    spy1.clear();


    ctx1->setActive(true);
    /* ctx1 is the active one.
     * triggering Myaction must now trigger action2
     */
    QTest::qWait(100);
    g_action_group_activate_action(G_ACTION_GROUP(action_group), "MyAction", NULL);
    spy2.wait();
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 1);
    QCOMPARE(spy3.count(), 0);
    spy2.clear();


    ctx2->setActive(true);
    /* ctx2 is the active one.
     * triggering Myaction must now trigger action3
     */
    QTest::qWait(100);
    g_action_group_activate_action(G_ACTION_GROUP(action_group), "MyAction", NULL);
    spy3.wait();
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 1);
    spy3.clear();


    manager->removeLocalContext(ctx2);
    /* removed the current active context.
     * triggering Myaction must now trigger action1 again from global context
     */
    QTest::qWait(100);
    g_action_group_activate_action(G_ACTION_GROUP(action_group), "MyAction", NULL);
    spy1.wait();
    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    spy1.clear();


    gctx->removeAction(action1);
    manager->removeLocalContext(ctx1);

}

void
TestActionManager::previewParameters()
{
    ActionContext *ctx1 = new ActionContext(manager);

    PreviewAction *action1 = new PreviewAction(manager);
    PreviewAction *action2 = new PreviewAction(ctx1);

    PreviewRangeParameter *param1 = new PreviewRangeParameter(action1);

    PreviewRangeParameter *param2 = new PreviewRangeParameter(action2);
    PreviewRangeParameter *param3 = new PreviewRangeParameter(action2);

    action1->setName("MyPreviewAction1");
    action2->setName("MyPreviewAction2");


    // goes in to the global context
    manager->addAction(action1);
    action1->addParameter(param1);

    ctx1->addAction(action2);
    action2->addParameter(param2);
    action2->addParameter(param3);
    manager->addLocalContext(ctx1);

    QSignalSpy spy_action1_started(action1, SIGNAL(started()));
    QSignalSpy spy_action1_triggered(action1, SIGNAL(triggered(QVariant)));
    QSignalSpy spy_action1_cancelled(action1, SIGNAL(cancelled()));
    QSignalSpy spy_action1_resetted(action1, SIGNAL(resetted()));

    QSignalSpy spy_action2_started(action2, SIGNAL(started()));
    QSignalSpy spy_action2_triggered(action2, SIGNAL(triggered(QVariant)));
    QSignalSpy spy_action2_cancelled(action2, SIGNAL(cancelled()));
    QSignalSpy spy_action2_resetted(action2, SIGNAL(resetted()));

    QSignalSpy spy_param1_value(param1, SIGNAL(valueChanged(float)));
    QSignalSpy spy_param2_value(param2, SIGNAL(valueChanged(float)));
    QSignalSpy spy_param3_value(param3, SIGNAL(valueChanged(float)));




    // no context is active, we only have MyPreviewAction1 from globalContext
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction1",
                                   g_variant_new_string("start"));
    QTest::qWait(100);
    QCOMPARE(spy_action1_started.count(),   1);
    QCOMPARE(spy_action1_triggered.count(), 0);
    QCOMPARE(spy_action1_cancelled.count(), 0);
    QCOMPARE(spy_action1_resetted.count(),  0);
    spy_action1_started.clear();

    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction1",
                                   g_variant_new_string("reset"));
    QTest::qWait(100);
    QCOMPARE(spy_action1_started.count(),   0);
    QCOMPARE(spy_action1_triggered.count(), 0);
    QCOMPARE(spy_action1_cancelled.count(), 0);
    QCOMPARE(spy_action1_resetted.count(),  1);
    spy_action1_resetted.clear();

    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction1",
                                   g_variant_new_string("commit"));
    QTest::qWait(100);
    QCOMPARE(spy_action1_started.count(),   0);
    QCOMPARE(spy_action1_triggered.count(), 1);
    QCOMPARE(spy_action1_cancelled.count(), 0);
    QCOMPARE(spy_action1_resetted.count(),  0);
    spy_action1_triggered.clear();

    /* we don't actually have a corresponging signal for this state,
     * but HUD sends it anyway when the HUD UI parameterized view is
     * closed, so let's send it
     */
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction1",
                                   g_variant_new_string("end"));
    QTest::qWait(100);
    QCOMPARE(spy_action1_started.count(),   0);
    QCOMPARE(spy_action1_triggered.count(), 0);
    QCOMPARE(spy_action1_cancelled.count(), 0);
    QCOMPARE(spy_action1_resetted.count(),  0);



    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction1",
                                   g_variant_new_string("start"));
    QTest::qWait(100);
    QCOMPARE(spy_action1_started.count(),   1);
    QCOMPARE(spy_action1_triggered.count(), 0);
    QCOMPARE(spy_action1_cancelled.count(), 0);
    QCOMPARE(spy_action1_resetted.count(),  0);
    spy_action1_started.clear();

    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction1",
                                   g_variant_new_string("cancel"));
    QTest::qWait(100);
    QCOMPARE(spy_action1_started.count(),   0);
    QCOMPARE(spy_action1_triggered.count(), 0);
    QCOMPARE(spy_action1_cancelled.count(), 1);
    QCOMPARE(spy_action1_resetted.count(),  0);
    spy_action1_cancelled.clear();

    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction1",
                                   g_variant_new_string("end"));
    QTest::qWait(100);
    QCOMPARE(spy_action1_started.count(),   0);
    QCOMPARE(spy_action1_triggered.count(), 0);
    QCOMPARE(spy_action1_cancelled.count(), 0);
    QCOMPARE(spy_action1_resetted.count(),  0);



    // activate ctx1
    ctx1->setActive(true);

    QTest::qWait(100);
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction2",
                                   g_variant_new_string("start"));
    QTest::qWait(100);
    QCOMPARE(spy_action2_started.count(),   1);
    QCOMPARE(spy_action2_triggered.count(), 0);
    QCOMPARE(spy_action2_cancelled.count(), 0);
    QCOMPARE(spy_action2_resetted.count(),  0);
    QCOMPARE(spy_action1_started.count(),   0);
    QCOMPARE(spy_action1_triggered.count(), 0);
    QCOMPARE(spy_action1_cancelled.count(), 0);
    QCOMPARE(spy_action1_resetted.count(),  0);
    spy_action2_started.clear();

    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction2",
                                   g_variant_new_string("reset"));
    QTest::qWait(100);
    QCOMPARE(spy_action2_started.count(),   0);
    QCOMPARE(spy_action2_triggered.count(), 0);
    QCOMPARE(spy_action2_cancelled.count(), 0);
    QCOMPARE(spy_action2_resetted.count(),  1);
    QCOMPARE(spy_action1_started.count(),   0);
    QCOMPARE(spy_action1_triggered.count(), 0);
    QCOMPARE(spy_action1_cancelled.count(), 0);
    QCOMPARE(spy_action1_resetted.count(),  0);
    spy_action2_resetted.clear();

    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction2",
                                   g_variant_new_string("commit"));
    QTest::qWait(100);
    QCOMPARE(spy_action2_started.count(),   0);
    QCOMPARE(spy_action2_triggered.count(), 1);
    QCOMPARE(spy_action2_cancelled.count(), 0);
    QCOMPARE(spy_action2_resetted.count(),  0);
    QCOMPARE(spy_action1_started.count(),   0);
    QCOMPARE(spy_action1_triggered.count(), 0);
    QCOMPARE(spy_action1_cancelled.count(), 0);
    QCOMPARE(spy_action1_resetted.count(),  0);
    spy_action2_triggered.clear();

    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction2",
                                   g_variant_new_string("end"));
    QTest::qWait(100);
    QCOMPARE(spy_action2_started.count(),   0);
    QCOMPARE(spy_action2_triggered.count(), 0);
    QCOMPARE(spy_action2_cancelled.count(), 0);
    QCOMPARE(spy_action2_resetted.count(),  0);
    QCOMPARE(spy_action1_started.count(),   0);
    QCOMPARE(spy_action1_triggered.count(), 0);
    QCOMPARE(spy_action1_cancelled.count(), 0);
    QCOMPARE(spy_action1_resetted.count(),  0);



    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction2",
                                   g_variant_new_string("start"));
    QTest::qWait(100);
    QCOMPARE(spy_action2_started.count(),   1);
    QCOMPARE(spy_action2_triggered.count(), 0);
    QCOMPARE(spy_action2_cancelled.count(), 0);
    QCOMPARE(spy_action2_resetted.count(),  0);
    QCOMPARE(spy_action1_started.count(),   0);
    QCOMPARE(spy_action1_triggered.count(), 0);
    QCOMPARE(spy_action1_cancelled.count(), 0);
    QCOMPARE(spy_action1_resetted.count(),  0);
    spy_action2_started.clear();

    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction2",
                                   g_variant_new_string("cancel"));
    QTest::qWait(100);
    QCOMPARE(spy_action2_started.count(),   0);
    QCOMPARE(spy_action2_triggered.count(), 0);
    QCOMPARE(spy_action2_cancelled.count(), 1);
    QCOMPARE(spy_action2_resetted.count(),  0);
    QCOMPARE(spy_action1_started.count(),   0);
    QCOMPARE(spy_action1_triggered.count(), 0);
    QCOMPARE(spy_action1_cancelled.count(), 0);
    QCOMPARE(spy_action1_resetted.count(),  0);
    spy_action2_cancelled.clear();

    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction2",
                                   g_variant_new_string("end"));
    QTest::qWait(100);
    QCOMPARE(spy_action2_started.count(),   0);
    QCOMPARE(spy_action2_triggered.count(), 0);
    QCOMPARE(spy_action2_cancelled.count(), 0);
    QCOMPARE(spy_action2_resetted.count(),  0);
    QCOMPARE(spy_action1_started.count(),   0);
    QCOMPARE(spy_action1_triggered.count(), 0);
    QCOMPARE(spy_action1_cancelled.count(), 0);
    QCOMPARE(spy_action1_resetted.count(),  0);



    // now check some range value updates

    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction1",
                                   g_variant_new_string("start"));
    QTest::qWait(100);
    spy_action1_started.clear();

    // param1
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "unity-action-range-param-0",
                                   g_variant_new_double(1.0f));
    QTest::qWait(100);
    QCOMPARE(spy_param1_value.count(), 1);
    QCOMPARE(spy_param2_value.count(), 0);
    QCOMPARE(spy_param3_value.count(), 0);
    QCOMPARE(spy_param1_value.takeFirst().at(0).toFloat(), 1.0f);

    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "unity-action-range-param-0",
                                   g_variant_new_double(1.1f));
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "unity-action-range-param-0",
                                   g_variant_new_double(1.2f));
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "unity-action-range-param-0",
                                   g_variant_new_double(1.3f));
    QTest::qWait(100);
    QCOMPARE(spy_param1_value.count(), 3);
    QCOMPARE(spy_param2_value.count(), 0);
    QCOMPARE(spy_param3_value.count(), 0);
    QCOMPARE(spy_param1_value.takeFirst().at(0).toFloat(), 1.1f);
    QCOMPARE(spy_param1_value.takeFirst().at(0).toFloat(), 1.2f);
    QCOMPARE(spy_param1_value.takeFirst().at(0).toFloat(), 1.3f);

    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction1",
                                   g_variant_new_string("cancel"));
    QTest::qWait(100);
    spy_action1_cancelled.clear();

    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction1",
                                   g_variant_new_string("end"));
    QTest::qWait(100);


    // then the others
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction2",
                                   g_variant_new_string("start"));
    QTest::qWait(100);
    spy_action2_started.clear();

    // param2
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "unity-action-range-param-1",
                                   g_variant_new_double(2.0f));
    // param3
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "unity-action-range-param-2",
                                   g_variant_new_double(3.0f));
    QTest::qWait(100);
    QCOMPARE(spy_param1_value.count(), 0);
    QCOMPARE(spy_param2_value.count(), 1);
    QCOMPARE(spy_param3_value.count(), 1);
    QCOMPARE(spy_param2_value.takeFirst().at(0).toFloat(), 2.0f);
    QCOMPARE(spy_param3_value.takeFirst().at(0).toFloat(), 3.0f);

    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "unity-action-range-param-1",
                                   g_variant_new_double(2.1f));
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "unity-action-range-param-2",
                                   g_variant_new_double(3.1f));
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "unity-action-range-param-1",
                                   g_variant_new_double(2.2f));
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "unity-action-range-param-2",
                                   g_variant_new_double(3.2f));
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "unity-action-range-param-1",
                                   g_variant_new_double(2.3f));
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "unity-action-range-param-2",
                                   g_variant_new_double(3.3f));
    QTest::qWait(100);
    QCOMPARE(spy_param1_value.count(), 0);
    QCOMPARE(spy_param2_value.count(), 3);
    QCOMPARE(spy_param3_value.count(), 3);
    QCOMPARE(spy_param2_value.takeFirst().at(0).toFloat(), 2.1f);
    QCOMPARE(spy_param2_value.takeFirst().at(0).toFloat(), 2.2f);
    QCOMPARE(spy_param2_value.takeFirst().at(0).toFloat(), 2.3f);
    QCOMPARE(spy_param3_value.takeFirst().at(0).toFloat(), 3.1f);
    QCOMPARE(spy_param3_value.takeFirst().at(0).toFloat(), 3.2f);
    QCOMPARE(spy_param3_value.takeFirst().at(0).toFloat(), 3.3f);

    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction2",
                                   g_variant_new_string("cancel"));
    QTest::qWait(100);
    spy_action2_cancelled.clear();
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "MyPreviewAction2",
                                   g_variant_new_string("end"));
    QTest::qWait(100);


    // update some parameters
    action1->setCommitLabel("Do Stuff");
    param1->setMinimumValue(-50.0f);
    param1->setMaximumValue(50.0f);
    param1->setValue(25.0f);
    /*! \todo there is actually no way of verifying these right now without accessing them from
     *        HUD menumodels or creating a HUD query..
     */


    // remove parameter
    action2->removeParameter(param3);
    g_action_group_activate_action(G_ACTION_GROUP(action_group),
                                   "unity-action-range-param-2",
                                   g_variant_new_double(100.0f));
    QTest::qWait(100);
    spy_param3_value.wait(100);
    QCOMPARE(spy_param3_value.count(), 0); // removed parameter should not receive updates.


    manager->removeAction(action1);
    manager->removeLocalContext(ctx1);
}
