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

#include "tst_previewrangeparameter.h"

#include <unity/action/PreviewRangeParameter>

#include <QtTest/QtTest>

void
TestPreviewRangeParameter::setText()
{
    unity::action::PreviewRangeParameter param;

    QSignalSpy spy(&param, SIGNAL(textChanged(QString)));
    param.setText("Red");
    QVERIFY(param.text() == "Red");
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString() == "Red");

    spy.clear();
    param.setText("Red");
    QCOMPARE(spy.count(), 0);
}

void
TestPreviewRangeParameter::setValue()
{
    unity::action::PreviewRangeParameter *param;
    param = new unity::action::PreviewRangeParameter(this);

    QSignalSpy spy(param, SIGNAL(valueChanged(float)));
    param->setValue(100);
    QVERIFY(qFuzzyCompare(param->value(), 100.0f));
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(qFuzzyCompare(arguments.at(0).toFloat(), 100.0f));

    spy.clear();
    param->setValue(100);
    QCOMPARE(spy.count(), 0);
}

void
TestPreviewRangeParameter::setMinimumValue()
{
    unity::action::PreviewRangeParameter *param;
    param = new unity::action::PreviewRangeParameter(this);

    param->setMinimumValue(-100);
    param->setValue(-100);

    QSignalSpy spy(param, SIGNAL(minimumValueChanged(float)));
    param->setMinimumValue(0);
    QVERIFY(qFuzzyCompare(param->minimumValue(), 0));
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(qFuzzyCompare(arguments.at(0).toFloat(), 0.0f));
    // verify that the value changed to allowed range
    QVERIFY(qFuzzyCompare(param->value(), 0.0f));

    spy.clear();
    param->setMinimumValue(0);
    QCOMPARE(spy.count(), 0);

    // verify that value can not be changed lower than minimum value
    param->setValue(-100);
    QVERIFY(qFuzzyCompare(param->value(), param->minimumValue()));

    // verify that minimum value can't be bigger than maximum value
    param->setMaximumValue(100);
    param->setMinimumValue(101);
    QVERIFY(qFuzzyCompare(param->minimumValue(), param->maximumValue()));
}

void
TestPreviewRangeParameter::setMaximumValue()
{
    unity::action::PreviewRangeParameter *param;
    param = new unity::action::PreviewRangeParameter(this);

    param->setMaximumValue(500);
    param->setValue(500);

    QSignalSpy spy(param, SIGNAL(maximumValueChanged(float)));
    param->setMaximumValue(200);
    QVERIFY(qFuzzyCompare(param->maximumValue(), 200));
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(qFuzzyCompare(arguments.at(0).toFloat(), 200.0f));
    // verify that the value changed to allowed range
    QVERIFY(qFuzzyCompare(param->value(), 200.0f));

    spy.clear();
    param->setMaximumValue(200);
    QCOMPARE(spy.count(), 0);

    // verify that value can not be changed higher than maximum value
    param->setValue(400);
    QVERIFY(qFuzzyCompare(param->value(), param->maximumValue()));

    // verify that maximum value can't be lower than minimum value
    param->setMinimumValue(100);
    param->setMaximumValue(99);
    QVERIFY(qFuzzyCompare(param->minimumValue(), param->maximumValue()));
}
