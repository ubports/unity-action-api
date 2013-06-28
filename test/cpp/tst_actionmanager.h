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

#include <QObject>
#include <unity/action/ActionManager>

typedef struct _GDBusConnection  GDBusConnection;
typedef struct _GDBusActionGroup GDBusActionGroup;

class TestActionManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void cleanup();

    void testGlobalContext();
    void actionOperations();
    void contextOperations();
    void actionPropertyChanges();

    void deletedLocalContext();
    void deletedAction();

    void actionInMultipleContext();
    void localContextOverridesGlobalContext();

    void previewParameters();

    // do this last as it creates a new globalContext in the effort of
    // preventing a crash, but anyway the functionality of the ActionManager
    // is more or less undefined after this.
    void deletedGlobalContext();

private:
    unity::action::ActionManager *manager;
    GDBusConnection *dbusc;
    GDBusActionGroup *action_group;
};

