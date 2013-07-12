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

#include "tst_action.h"

#include <unity/action/Action>

#include <QtTest/QtTest>

void
TestAction::setName()
{
    unity::action::Action action;

    QSignalSpy spy(&action, SIGNAL(nameChanged(QString)));
    action.setName("myaction");
    QVERIFY(action.name() == "myaction");
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString() == "myaction");

    spy.clear();
    action.setName("myaction");
    QCOMPARE(spy.count(), 0);

    // sets the name once to the generated name
    action.setName("");
    action.setName(QString());
    action.setName(QString(""));
    QCOMPARE(spy.count(), 1);
    QVERIFY(action.name() != "myaction");
}

void
TestAction::setText()
{
    unity::action::Action action;

    QSignalSpy spy(&action, SIGNAL(textChanged(QString)));
    action.setText("Foo");
    QVERIFY(action.text() == "Foo");
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString() == "Foo");

    spy.clear();
    action.setText("Foo");
    QCOMPARE(spy.count(), 0);
}

void
TestAction::setIconName()
{
    unity::action::Action action;

    QSignalSpy spy(&action, SIGNAL(iconNameChanged(QString)));
    action.setIconName("my-icon");
    QVERIFY(action.iconName() == "my-icon");
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString() == "my-icon");

    spy.clear();
    action.setIconName("my-icon");
    QCOMPARE(spy.count(), 0);
}

void
TestAction::setDescription()
{
    unity::action::Action action;

    QSignalSpy spy(&action, SIGNAL(descriptionChanged(QString)));
    action.setDescription("Lorem Ipsum");
    QVERIFY(action.description() == "Lorem Ipsum");
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString() == "Lorem Ipsum");

    spy.clear();
    action.setDescription("Lorem Ipsum");
    QCOMPARE(spy.count(), 0);
}

void
TestAction::setKeywords()
{
    unity::action::Action action;
    QString keywords;

    QSignalSpy spy(&action, SIGNAL(keywordsChanged(QString)));

    keywords = "Foo;Bar";

    action.setKeywords(keywords);
    QVERIFY(action.keywords() == keywords);
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString() == keywords);

    spy.clear();
    action.setKeywords(keywords);
    QCOMPARE(spy.count(), 0);
}

void
TestAction::setEnabled()
{
    unity::action::Action action;

    action.setEnabled(true);
    QSignalSpy spy(&action, SIGNAL(enabledChanged(bool)));
    action.setEnabled(false);
    QVERIFY(action.enabled() == false);
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toBool() == false);

    spy.clear();
    action.setEnabled(false);
    QCOMPARE(spy.count(), 0);

    action.setEnabled(true);
    QSignalSpy trigger_spy(&action, SIGNAL(triggered(QVariant)));
    action.trigger();
    QCOMPARE(trigger_spy.count(), 1);
    trigger_spy.clear();
    action.setEnabled(false);
    action.trigger();
    QCOMPARE(trigger_spy.count(), 0);
}

void
TestAction::setParameterType()
{
    unity::action::Action action;

    QSignalSpy spy(&action, SIGNAL(parameterTypeChanged(unity::action::Action::Type)));
    action.setParameterType(unity::action::Action::String);
    QVERIFY(action.parameterType() == unity::action::Action::String);
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).value<unity::action::Action::Type>() == unity::action::Action::String);

    spy.clear();
    action.setParameterType(unity::action::Action::String);
    QCOMPARE(spy.count(), 0);
}

void
TestAction::trigger()
{
    unity::action::Action *action = new unity::action::Action(this);

    QSignalSpy spy(action, SIGNAL(triggered(QVariant)));


    /*** parameterType = None ***/

    action->trigger();
    QCOMPARE(spy.count(), 1);

    action->trigger(QVariant());
    QCOMPARE(spy.count(), 2);


    /* expected failures */

    spy.clear();
    action->trigger("foo");
    QCOMPARE(spy.count(), 0);

    action->trigger(-50);
    QCOMPARE(spy.count(), 0);

    action->trigger(false);
    QCOMPARE(spy.count(), 0);

    action->trigger(5.5f);
    QCOMPARE(spy.count(), 0);

    action->trigger(QRect());
    QCOMPARE(spy.count(), 0);



    /*** parameterType = String ***/

    action->setParameterType(unity::action::Action::String);
    action->trigger("hello");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toString(), QString("hello"));

    // these work because of the automatic type conversion QVariant does
    action->trigger(-50);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toString(), QString("-50"));

    action->trigger(false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toString(), QString("false"));

    action->trigger(true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toString(), QString("true"));

    action->trigger(5.5f);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toString(), QString("5.5"));


    /* expected failures */

    spy.clear();
    action->trigger();
    QCOMPARE(spy.count(), 0);

    action->trigger(QRect());
    QCOMPARE(spy.count(), 0);



    /*** parameterType = Integer ***/

    action->setParameterType(unity::action::Action::Integer);

    action->trigger(-50);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toInt(), -50);

    // these work because of the automatic type conversion QVariant does
    action->trigger("-50");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toInt(), -50);

    action->trigger(false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toInt(), 0);

    action->trigger(true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toInt(), 1);

    action->trigger(5.5f);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toInt(), 6);


    /* expected failures */

    spy.clear();
    action->trigger();
    QCOMPARE(spy.count(), 0);

    action->trigger("hello");
    QCOMPARE(spy.count(), 0);

    action->trigger("true");  // QVariant does not do QString -> Bool -> Int
    QCOMPARE(spy.count(), 0);

    action->trigger("false");
    QCOMPARE(spy.count(), 0);

    action->trigger("5.5");  // QVariant does not do QString -> Double -> Int
    QCOMPARE(spy.count(), 0);

    action->trigger(QRect());
    QCOMPARE(spy.count(), 0);



    /*** parameterType = Bool ***/

    action->setParameterType(unity::action::Action::Bool);

    action->trigger(false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);

    action->trigger(true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);


    // these work because of the automatic type conversion QVariant does

    action->trigger(-50);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);

    action->trigger(0);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);

    action->trigger(1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);

    action->trigger(0.0f);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);

    action->trigger(5.5f);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);


    /* When converting strings to bools qvariant uses to following logic:
     *
     * "true" -> true
     * "false" -> false

     * "0" -> false
     * "<other integer> -> true
     *
     * "" -> false
     * <any other string> -> true
     */

    action->trigger("true");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);

    action->trigger("false");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);

    action->trigger("0");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);

    action->trigger("1");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);

    action->trigger("-50");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);

    action->trigger("");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);

    action->trigger("0.0");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);


    action->trigger("hello");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);


    /* expected failures */

    spy.clear();
    action->trigger();
    QCOMPARE(spy.count(), 0);

    action->trigger(QRect());
    QCOMPARE(spy.count(), 0);



    /*** parameterType = Real ***/

    action->setParameterType(unity::action::Action::Real);

    action->trigger(0.0f);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toFloat(), 0.0f);

    action->trigger(-1.0f);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toFloat(), -1.0f);


    // these work because of the automatic type conversion QVariant does

    action->trigger(false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toFloat(), 0.0f);

    action->trigger(true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toFloat(), 1.0f);

    action->trigger("0.0");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toFloat(), 0.0f);

    action->trigger(-50);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toFloat(), -50.0f);


    /* expected failures */

    spy.clear();
    action->trigger();
    QCOMPARE(spy.count(), 0);

    action->trigger(QRect());
    QCOMPARE(spy.count(), 0);

    action->trigger("");
    QCOMPARE(spy.count(), 0);

    action->trigger("false");
    QCOMPARE(spy.count(), 0);

    action->trigger("hello");
    QCOMPARE(spy.count(), 0);
}
