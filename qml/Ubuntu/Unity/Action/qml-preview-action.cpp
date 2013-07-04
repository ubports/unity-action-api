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

#include "qml-preview-action.h"

using namespace unity::action;

qml::PreviewAction::PreviewAction(QObject *parent)
    : unity::action::PreviewAction(parent)
{

}

qml::PreviewAction::~PreviewAction()
{
    foreach(PreviewParameter *parameter, parameters()) {
        removeParameter(parameter);
    }
}


QQmlListProperty<unity::action::PreviewParameter>
qml::PreviewAction::parameters_list()
{
    return QQmlListProperty<unity::action::PreviewParameter>(this,
                                                             0,
                                                             qml::PreviewAction::append,
                                                             qml::PreviewAction::count,
                                                             qml::PreviewAction::at,
                                                             qml::PreviewAction::clear);
}

void
qml::PreviewAction::append(QQmlListProperty<unity::action::PreviewParameter> *list,
                           unity::action::PreviewParameter *parameter)
{
    qml::PreviewAction *action = qobject_cast<qml::PreviewAction *>(list->object);
    if (action) {
        action->addParameter(parameter);
        return;
    }

    Q_ASSERT(0); // should not be reached
}

PreviewParameter *
qml::PreviewAction::at(QQmlListProperty<unity::action::PreviewParameter> *list, int index)
{
    qml::PreviewAction *action = qobject_cast<qml::PreviewAction *>(list->object);
    if (action)
        return action->parameters().at(index);

    Q_ASSERT(0); // should not be reached
    return 0;
}

void
qml::PreviewAction::clear(QQmlListProperty<unity::action::PreviewParameter> *list)
{
    qml::PreviewAction *action = qobject_cast<qml::PreviewAction *>(list->object);
    if (action) {
        foreach(PreviewParameter *parameter, action->parameters()) {
            action->removeParameter(parameter);
        }
        return;
    }

    Q_ASSERT(0); // should not be reached
}

int
qml::PreviewAction::count(QQmlListProperty<unity::action::PreviewParameter> *list)
{
    qml::PreviewAction *action = qobject_cast<qml::PreviewAction *>(list->object);
    if (action) {
        return action->parameters().count();
    }

    Q_ASSERT(0); // should not be reached
    return 0;
}


