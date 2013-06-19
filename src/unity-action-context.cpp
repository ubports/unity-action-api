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

//! \private
class Q_DECL_HIDDEN unity::action::ActionContext::Private {
public:
    QSet<Action *> actions;
    bool active;
};

ActionContext::ActionContext(QObject *parent)
    : QObject(parent),
      d(new Private)
{
    d->active = false;
}

ActionContext::~ActionContext()
{

}

void
ActionContext::addAction(Action *action)
{
    if (d->actions.contains(action))
        return;
    d->actions.insert(action);
    emit actionsChanged();
}

void
ActionContext::removeAction(Action *action)
{
    if (!d->actions.contains(action))
        return;
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

QSet<Action *>
ActionContext::actions() const
{
    return d->actions;
}
