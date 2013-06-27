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

#include "tst_previewaction.h"

#include <unity/action/PreviewAction>
#include <unity/action/PreviewRangeParameter>

#include <QtTest/QtTest>

using namespace unity::action;

void
TestPreviewAction::setCommitLabel()
{
    PreviewAction action;

    QSignalSpy spy(&action, SIGNAL(commitLabelChanged(QString)));
    action.setCommitLabel("Crop");
    QVERIFY(action.commitLabel() == "Crop");
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString() == "Crop");

    spy.clear();
    action.setCommitLabel("Crop");
    QCOMPARE(spy.count(), 0);
}

void
TestPreviewAction::parameterOperations()
{
    PreviewAction action;
    PreviewRangeParameter param1;
    PreviewRangeParameter param2;

    QSignalSpy spy(&action, SIGNAL(parametersChanged()));

    QVERIFY(action.parameters().isEmpty());

    action.addParameter(&param1);
    QCOMPARE(spy.count(), 1);

    action.addParameter(&param2);
    QCOMPARE(spy.count(), 2);

    QVERIFY(action.parameters().contains(&param1));
    QVERIFY(action.parameters().contains(&param2));


    spy.clear();
    action.addParameter(&param1);
    action.addParameter(&param2);
    QCOMPARE(spy.count(), 0);
    QCOMPARE(action.parameters().count(), 2);

    spy.clear();
    action.removeParameter(&param1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(action.parameters().count(), 1);
    QVERIFY(!action.parameters().contains(&param1));

    spy.clear();
    action.removeParameter(&param1);
    QCOMPARE(spy.count(), 0);
    QVERIFY(action.parameters().contains(&param2));

    action.removeParameter(&param2);
    QCOMPARE(spy.count(), 1);
    QVERIFY(action.parameters().isEmpty());
}

void
TestPreviewAction::testSignals()
{
    PreviewAction action;
    // started, cancelled, resetted, triggered
}

void
TestPreviewAction::deletedParameter()
{
    // if parameter is deleted we must detect this and not crash

    PreviewAction action;
    PreviewRangeParameter *param1 = new PreviewRangeParameter();
    PreviewRangeParameter *param2 = new PreviewRangeParameter();

    action.addParameter(param1);
    action.addParameter(param2);

    QSignalSpy spy(&action, SIGNAL(parametersChanged()));
    delete param1;
    QCOMPARE(spy.count(), 1);
    QVERIFY(!action.parameters().contains(param1));
    param1 = 0;

    delete param2;
    QCOMPARE(spy.count(), 2);
    QVERIFY(!action.parameters().contains(param2));
    param2 = 0;

    QVERIFY(action.parameters().isEmpty());
}
