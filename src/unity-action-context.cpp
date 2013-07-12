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

#include <unity/action/ActionContext>
#include <unity/action/Action>

using namespace unity::action;

namespace unity {
namespace action {
/*!
 * \class ActionContext
 *
 * ActionContext groups actions together and by providing multiple contexts
 * the developer is able to control the visibility of the actions. The ActionManager then exposes the actions from
 * these different contexts. See \ref page_contexts for more details.
 */

// property documentation

/*!
 * \property bool ActionContext::active
 *
 * If true the context is active. If false the context is inactive.
 *
 * When context has been added to the ActionManager setting this value controls
 * whether or not the actions in a context are available to external components.
 *
 * The ActionManager monitors the active property of each of the local contexts
 * that has been added to it. There can be only one active local context at a time.
 * When one of the local contexts sets itself active the manager will notice this,
 * export the actions from that given context and set the previously active local
 * context as inactive. This way a call to setActive() on a local context is
 * sufficient to manage the active local context of the manager and no additional
 * calls are necessary to manually inactivate the other contexts.
 *
 * \initvalue false
 *
 * \accessors active(), setActive()
 *
 * \notify activeChanged()
 */

/*!
 * \fn void ActionContext::actionsChanged()
 * Notifies that the actions inside a context have changed from a call to
 * addAction() or removeAction().
 */
}
}

//! \private
class Q_DECL_HIDDEN unity::action::ActionContext::Private : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Private)

public:
    ActionContext *q;

    QSet<Action *> actions;
    bool active;

    Private(ActionContext *ctx)
        : q(ctx)
    {}

public slots:
    void actionDestroyed(QObject *obj);

};

void
ActionContext::Private::actionDestroyed(QObject *obj)
{
    /* we can not use qobject_cast() as it will fail for
     * objects about to be destroyed. Instead we can simply cast the
     * pointer directly and use it as long as it's not 0.
     */
    Action *action = (Action *)obj;
    if (action == 0) {
        return;
    }
    q->removeAction(action);
}

/*!
 * \fn ActionContext::ActionContext(QObject *parent = 0)
 *
 * Creates a new ActionContext.
 * ActionContext has to be added to the ActionManager and set active to
 * make it's actions vailable for external components.
 *
 * \param parent parent QObject or 0
 */
ActionContext::ActionContext(QObject *parent)
    : QObject(parent),
      d(new Private(this))
{
    d->active = false;
}

ActionContext::~ActionContext()
{

}

/*!
 * Adds an action to the context.
 *
 * \param action Action to be added to the context
 *
 * Calling this function multiple times with the same action
 * does not have any side effects; the action gets added only once.
 *
 * ActionContext monitors if the action is deleted and does the appropriate
 * cleanup when necessary, so it is not mandatory to call removeAction()
 * before the action is destroyed.
 *
 * \note action must not be 0
 */
void
ActionContext::addAction(Action *action)
{
    Q_ASSERT(action != 0);
    if (action == 0)
        return;
    if (d->actions.contains(action))
        return;
    d->actions.insert(action);
    connect(action, SIGNAL(destroyed(QObject*)), d.data(), SLOT(actionDestroyed(QObject*)));
    emit actionsChanged();
}

/*!
 * Removes an action from the context.
 *
 * \param action Action to be removed to the context
 *
 * Calling this function multiple times with the same action
 * does not have any side effects; the action gets removed only if
 * it was first added to the context with addAction().
 *
 * \note action must not be 0
 */
void
ActionContext::removeAction(Action *action)
{
    Q_ASSERT(action != 0);
    if (action == 0)
        return;
    if (!d->actions.contains(action))
        return;
    action->disconnect(d.data());
    d->actions.remove(action);
    emit actionsChanged();
}

bool
ActionContext::active() const
{
    return d->active;
}

void
ActionContext::setActive(bool value)
{
    if (d->active == value)
        return;
    d->active = value;
    emit activeChanged(value);
}

/*!
 * \returns The set of actions in the context.
 */
QSet<Action *>
ActionContext::actions() const
{
    return d->actions;
}

#include "unity-action-context.moc"
