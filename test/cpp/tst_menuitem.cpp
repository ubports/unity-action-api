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

#include "tst_menuitem.h"

#include <unity/action/MenuItem>
#include <unity/action/Action>

#include <QtTest/QtTest>

void
TestMenuItem::setAction()
{
    unity::action::Action *action = new unity::action::Action(this);
    unity::action::MenuItem item;

    QVERIFY(item.action() == 0);

    QSignalSpy spy(&item, SIGNAL(actionChanged()));
    item.setAction(action);
    QVERIFY(item.action() == action);
    QCOMPARE(spy.count(), 1);

    spy.clear();
    item.setAction(action);
    QCOMPARE(spy.count(), 0);

    item.setAction(0);
    QCOMPARE(spy.count(), 1);
}

void
TestMenuItem::setText()
{
    unity::action::MenuItem *item = new unity::action::MenuItem(this);

    QSignalSpy spy(item, SIGNAL(textChanged(QString)));
    item->setText("Foo");
    QVERIFY(item->text() == "Foo");
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString() == "Foo");

    spy.clear();
    item->setText("Foo");
    QCOMPARE(spy.count(), 0);
}

void
TestMenuItem::setIconName()
{
    unity::action::MenuItem item;

    QSignalSpy spy(&item, SIGNAL(iconNameChanged(QString)));
    item.setIconName("my-icon");
    QVERIFY(item.iconName() == "my-icon");
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString() == "my-icon");

    spy.clear();
    item.setIconName("my-icon");
    QCOMPARE(spy.count(), 0);
}

void
TestMenuItem::setTarget()
{
    unity::action::MenuItem item;
    QVariant var(45);

    QVERIFY(!item.target().isValid());

    QSignalSpy spy(&item, SIGNAL(targetChanged(QVariant)));

    item.setTarget(var);
    QVERIFY(item.target() == var);
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0) == var);

    spy.clear();
    item.setTarget(var);
    QCOMPARE(spy.count(), 0);

    item.setTarget(QVariant());
    QCOMPARE(spy.count(), 1);
    QVERIFY(!item.target().isValid());
}

void
TestMenuItem::setVisible()
{
    unity::action::MenuItem item;

    item.setVisible(true);
    QSignalSpy spy(&item, SIGNAL(visibleChanged(bool)));
    item.setVisible(false);
    QVERIFY(item.visible() == false);
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toBool() == false);

    spy.clear();
    item.setVisible(false);
    QCOMPARE(spy.count(), 0);
}

void
TestMenuItem::setEnabled()
{
    unity::action::MenuItem item;

    item.setEnabled(true);
    QSignalSpy spy(&item, SIGNAL(enabledChanged(bool)));
    item.setEnabled(false);
    QVERIFY(item.enabled() == false);
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toBool() == false);

    spy.clear();
    item.setEnabled(false);
    QCOMPARE(spy.count(), 0);
}

void
TestMenuItem::verifyTargetType()
{
    QSKIP("not implemented yet");
}
