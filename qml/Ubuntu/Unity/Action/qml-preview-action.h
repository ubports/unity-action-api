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

#ifndef UNITY_ACTION_QML_PREVIEW_ACTION
#define UNITY_ACTION_QML_PREVIEW_ACTION

namespace unity {
namespace action {
namespace qml {
    class PreviewAction;
}
}
}

#include <QQmlListProperty>
#include <unity/action/PreviewAction>

class Q_DECL_EXPORT unity::action::qml::PreviewAction : public unity::action::PreviewAction
{
    Q_OBJECT
    Q_DISABLE_COPY(PreviewAction)

    Q_PROPERTY(QQmlListProperty<unity::action::PreviewParameter> parameters
               READ parameters_list)

    Q_CLASSINFO("DefaultProperty", "parameters")

public:

    explicit PreviewAction(QObject *parent = 0);
    virtual ~PreviewAction();

    QQmlListProperty<unity::action::PreviewParameter> parameters_list();

private:
    static void append(QQmlListProperty<unity::action::PreviewParameter> *list, unity::action::PreviewParameter *parameter);
    static unity::action::PreviewParameter *at(QQmlListProperty<unity::action::PreviewParameter> *list, int index);
    static void clear(QQmlListProperty<unity::action::PreviewParameter> *list);
    static int count(QQmlListProperty<unity::action::PreviewParameter> *list);

signals:

};

#endif
