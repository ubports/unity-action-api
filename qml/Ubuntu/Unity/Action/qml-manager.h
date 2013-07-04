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

#ifndef UNITY_ACTION_QML_ACTION_MANAGER
#define UNITY_ACTION_QML_ACTION_MANAGER

#include <QQmlListProperty>
#include <unity/action/ActionManager>
#include <unity/action/Action>
#include <unity/action/ActionContext>

namespace unity {
namespace action {
namespace qml {
    class ActionManager;
}
}
}

class unity::action::qml::ActionManager : public unity::action::ActionManager
{
    Q_OBJECT
    Q_DISABLE_COPY(ActionManager)

    Q_PROPERTY(QQmlListProperty<unity::action::ActionContext> localContexts
               READ localContexts_list)
    Q_PROPERTY(QQmlListProperty<unity::action::Action> actions
               READ actions_list)

    Q_CLASSINFO("DefaultProperty", "actions")

public:

    ActionManager(QObject *parent = 0);
    virtual ~ActionManager();

    QQmlListProperty<unity::action::ActionContext> localContexts_list();
    QQmlListProperty<unity::action::Action> actions_list();

private:
    static void contextAppend(QQmlListProperty<ActionContext> *list, ActionContext *context);
    static void contextClear(QQmlListProperty<ActionContext> *list);
    static int contextCount(QQmlListProperty<ActionContext> *list);

    static void actionAppend(QQmlListProperty<Action> *list, Action *action);
    static void actionClear(QQmlListProperty<Action> *list);
    static int actionCount(QQmlListProperty<Action> *list);
};
#endif









