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

#ifndef UNITY_ACTION_CONTEXT
#define UNITY_ACTION_CONTEXT

namespace unity {
namespace action {
    class ActionContext;
    class Action;
}
}

#include <QObject>
#include <QScopedPointer>
#include <QSet>

class Q_DECL_EXPORT unity::action::ActionContext : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ActionContext)

    Q_PROPERTY(bool active
               READ active
               WRITE setActive
               NOTIFY activeChanged)

public:

    explicit ActionContext(QObject *parent = 0);
    virtual ~ActionContext();

    /*! \todo maybe call this includeAction() to emphasize
     *        that actions have to be added to the manager
     */
    void addAction(Action *action);
    void removeAction(Action *action);

    bool active() const;
    void setActive(bool value);

    QSet<Action *> actions();

signals:
    void activeChanged(bool value);
    void actionsChanged();

private:
        class Private;
        QScopedPointer<Private> d;
};
#endif
