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

#include <QtTest/QtTest>

void
TestPreviewAction::setCommitLabel()
{
    unity::action::PreviewAction action;

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
TestPreviewAction::testSignals()
{
    unity::action::PreviewAction action;
    //! \todo implement signal testing
    QSKIP("signal testing not implemented yet.");
}
