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

#ifndef UNITY_ACTION_QML_ACTION
#define UNITY_ACTION_QML_ACTION

namespace unity {
namespace action {
namespace qml {
    class Action;
}
}
}

#include <QObject>
#include <QVariant>
#include <QScopedPointer>

#include <unity/action/Action>

class Q_DECL_EXPORT unity::action::qml::Action : public unity::action::Action
{
    Q_OBJECT
    Q_DISABLE_COPY(Action)

public:
        explicit Action(QObject *parent = 0);
        virtual ~Action();
};
#endif
