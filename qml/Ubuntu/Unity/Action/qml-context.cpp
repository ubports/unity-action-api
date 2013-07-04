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

#include "qml-context.h"

using namespace unity::action;

qml::ActionContext::ActionContext(QObject *parent)
    : unity::action::ActionContext(parent)
{

}

qml::ActionContext::~ActionContext()
{
    foreach(Action *action, actions()) {
        removeAction(action);
    }
}

QQmlListProperty<Action>
qml::ActionContext::action_list()
{
    return QQmlListProperty<Action>(this,
                                    0,
                                    qml::ActionContext::append,
                                    qml::ActionContext::count,
                                    0, // at()
                                    qml::ActionContext::clear);
}

void
qml::ActionContext::append(QQmlListProperty<Action> *list,
                           Action *action)
{
    qml::ActionContext *ctx = qobject_cast<qml::ActionContext *>(list->object);
    if (ctx) {
        ctx->addAction(action);
        return;
    }

    Q_ASSERT(0); // should not be reached
}

void
qml::ActionContext::clear(QQmlListProperty<Action> *list)
{
    qml::ActionContext *ctx = qobject_cast<qml::ActionContext *>(list->object);
    if (ctx) {
        foreach(Action *action, ctx->actions()) {
            ctx->removeAction(action);
        }
        return;
    }

    Q_ASSERT(0); // should not be reached
}

int
qml::ActionContext::count(QQmlListProperty<Action> *list)
{
    qml::ActionContext *ctx = qobject_cast<qml::ActionContext *>(list->object);
    if (ctx) {
        return ctx->actions().count();
    }

    Q_ASSERT(0); // should not be reached
    return 0;
}
