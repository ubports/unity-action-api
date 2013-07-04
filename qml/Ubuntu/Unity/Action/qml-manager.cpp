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

#include <qml-manager.h>

#include <QDebug>

using namespace unity::action;

qml::ActionManager::ActionManager(QObject *parent)
    : unity::action::ActionManager(parent)
{

}

qml::ActionManager::~ActionManager()
{
    foreach(Action *action, globalContext()->actions()) {
        removeAction(action);
    }
    foreach(ActionContext *context, localContexts()) {
        removeLocalContext(context);
    }
}


QQmlListProperty<unity::action::ActionContext>
qml::ActionManager::localContexts_list()
{
    return QQmlListProperty<ActionContext>(this,
                                           0,
                                           qml::ActionManager::contextAppend,
                                           qml::ActionManager::contextCount,
                                           0, // at()
                                           qml::ActionManager::contextClear);
}

QQmlListProperty<unity::action::Action>
qml::ActionManager::actions_list()
{
    return QQmlListProperty<Action>(this,
                                    0,
                                    qml::ActionManager::actionAppend,
                                    qml::ActionManager::actionCount,
                                    0, // at
                                    qml::ActionManager::actionClear);
}

void
qml::ActionManager::contextAppend(QQmlListProperty<ActionContext> *list, ActionContext *context)
{
    qml::ActionManager *manager = qobject_cast<qml::ActionManager *>(list->object);
    if (manager) {
        manager->addLocalContext(context);
        return;
    }

    Q_ASSERT(0); // should not be reached
}

void
qml::ActionManager::contextClear(QQmlListProperty<ActionContext> *list)
{
    qml::ActionManager *manager = qobject_cast<qml::ActionManager *>(list->object);
    if (manager) {
        foreach(ActionContext *context, manager->localContexts()) {
            manager->removeLocalContext(context);
        }
        return;
    }

    Q_ASSERT(0); // should not be reached
}

int
qml::ActionManager::contextCount(QQmlListProperty<ActionContext> *list)
{
    qml::ActionManager *manager = qobject_cast<qml::ActionManager *>(list->object);
    if (manager) {
        return manager->localContexts().count();
    }

    Q_ASSERT(0); // should not be reached
    return 0;
}

void
qml::ActionManager::actionAppend(QQmlListProperty<Action> *list, Action *action)
{
    qml::ActionManager *manager = qobject_cast<qml::ActionManager *>(list->object);
    if (manager) {
        manager->globalContext()->addAction(action);
        return;
    }

    Q_ASSERT(0); // should not be reached
}

void
qml::ActionManager::actionClear(QQmlListProperty<Action> *list)
{
    qml::ActionManager *manager = qobject_cast<qml::ActionManager *>(list->object);
    if (manager) {
        foreach(Action *action, manager->globalContext()->actions()) {
            manager->globalContext()->removeAction(action);
        }
        return;
    }

    Q_ASSERT(0); // should not be reached
}

int
qml::ActionManager::actionCount(QQmlListProperty<Action> *list)
{
    qml::ActionManager *manager = qobject_cast<qml::ActionManager *>(list->object);
    if (manager) {
        return manager->globalContext()->actions().count();
    }

    Q_ASSERT(0); // should not be reached
    return 0;
}
