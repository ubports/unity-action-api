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

#include <QtTest/QtTest>
#include <QCoreApplication>

#include "tst_action.h"
#include "tst_previewaction.h"
#include "tst_previewrangeparameter.h"
#include "tst_menuitem.h"
#include "tst_actioncontext.h"
#include "tst_actionmanager.h"

int main(int argc, char *argv[])
{
    bool fail = false;

    // needed for QTest::qWait
    QCoreApplication app(argc, argv);

    TestAction tst_action;
    TestPreviewAction tst_previewaction;
    TestPreviewRangeParameter tst_previewrangeparameter;
    TestMenuItem tst_menuitem;
    TestActionContext tst_actioncontext;
    TestActionManager tst_actionmanager;

    if (QTest::qExec(&tst_action, argc, argv) != 0)
        fail = true;
    if (QTest::qExec(&tst_previewaction, argc, argv) != 0)
        fail = true;
    if (QTest::qExec(&tst_previewrangeparameter, argc, argv) != 0)
        fail = true;
    if (QTest::qExec(&tst_menuitem, argc, argv) != 0)
        fail = true;
    if (QTest::qExec(&tst_actioncontext, argc, argv) != 0)
        fail = true;
    if (QTest::qExec(&tst_actionmanager, argc, argv) != 0)
        fail = true;
    if (fail)
        return 1;

    return 0;
}
