/*
 * Copyright 2013 Canonical Ltd.
 *
 * libhud-qt is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * libhud-qt is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tst_action.h"

#include "../../src/unity-action.h"

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
TestAction::setIcon()
{
    unity::action::Action action;

    QSignalSpy spy(&action, SIGNAL(iconChanged(QString)));
    action.setIcon("my-icon");
    QVERIFY(action.icon() == "my-icon");
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString() == "my-icon");

    spy.clear();
    action.setIcon("my-icon");
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
    unity::action::Action *action = new unity::action::Action();

    QSignalSpy spy(action, SIGNAL(triggered(QVariant)));
    action->trigger();
    QCOMPARE(spy.count(), 1);
    delete action;
}
