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

#ifndef UNITY_ACTION_QML_ACTION_CONTEXT
#define UNITY_ACTION_QML_ACTION_CONTEXT

namespace unity {
namespace action {
namespace qml {
    class ActionContext;
}
}
}

#include <QQmlListProperty>

#include <unity/action/ActionContext>
#include <unity/action/Action>

class Q_DECL_EXPORT unity::action::qml::ActionContext : public unity::action::ActionContext
{
    Q_OBJECT
    Q_DISABLE_COPY(ActionContext)

    Q_PROPERTY(QQmlListProperty<unity::action::Action> actions
               READ action_list)

    Q_CLASSINFO("DefaultProperty", "actions")

public:
        explicit ActionContext(QObject *parent = 0);
        virtual ~ActionContext();

        QQmlListProperty<unity::action::Action> action_list();

private:

    static void append(QQmlListProperty<unity::action::Action> *list,unity::action::Action *action);
    static void clear(QQmlListProperty<unity::action::Action> *list);
    static int count(QQmlListProperty<unity::action::Action> *list);

};
#endif
