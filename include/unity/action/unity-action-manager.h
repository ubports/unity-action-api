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

#ifndef UNITY_ACTION_MANAGER
#define UNITY_ACTION_MANAGER

namespace unity {
namespace action {
    class ActionManager;
    class ActionContext;
    class Action;
}
}

#include <QObject>
#include <QScopedPointer>

class Q_DECL_EXPORT unity::action::ActionManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ActionManager)

    Q_PROPERTY(unity::action::ActionContext *globalContext
               READ globalContext)

public:

    explicit ActionManager(QObject *parent = 0);
    virtual ~ActionManager();

    Q_INVOKABLE void addAction(unity::action::Action *action);
    Q_INVOKABLE void removeAction(unity::action::Action *action);

    Q_INVOKABLE unity::action::ActionContext *globalContext();

    Q_INVOKABLE void addLocalContext(unity::action::ActionContext *context);

    Q_INVOKABLE void removeLocalContext(unity::action::ActionContext *context);
    QSet<ActionContext *> localContexts() const;

    QSet<Action *> actions() const;

signals:
    void localContextsChanged();
    void actionsChanged();

private:
        class Private;
        QScopedPointer<Private> d;
};
#endif
