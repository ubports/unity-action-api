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

#include <unity/action/MenuItem>
#include <unity/action/Action>

using namespace unity::action;


class Q_DECL_HIDDEN unity::action::MenuItem::Private : public QObject
{
    Q_OBJECT
public:
    MenuItem *q;

    Action *action;
    QString text;
    QString iconName;
    QVariant target;
    bool visible;
    bool enabled;

public slots:
    void actionDestroyed(QObject *obj);
};

void
MenuItem::Private::actionDestroyed(QObject *obj)
{
    /* we can not use qobject_cast() as it will fail for
     * objects about to be destroyed. Instead we can simply cast the
     * pointer directly and use it as long as it's not 0.
     */
    Action *tmp = (Action *)obj;
    if (tmp == 0) {
        return;
    }
    // simply clear the action.
    q->setAction(0);
}

MenuItem::MenuItem(QObject *parent)
    : QObject(parent),
      d(new Private)
{
    d->q = this;
    d->action = 0;
    d->visible = true;
    d->enabled = true;
}

MenuItem::~MenuItem()
{
}

/*!
 * \brief MenuItem::action
 * \return pointer to a unity::action::Action
 * \return or 0 if no action has been set
 */
Action *
MenuItem::action() const
{
    return d->action;
}

void
MenuItem::setAction(Action *value)
{
    if (d->action == value)
        return;

    if (value == 0) {
        d->action->disconnect(d.data());
    } else {
        d->action->disconnect(d.data());
        connect(value, SIGNAL(destroyed(QObject*)), d.data(), SLOT(actionDestroyed(QObject *)));
    }
    d->action = value;
    emit actionChanged();
}

QString
MenuItem::text() const
{
    return d->text;
}

void
MenuItem::setText(const QString &value)
{
    if (d->text == value)
        return;
    d->text = value;
    emit textChanged(value);
}

QString
MenuItem::iconName() const
{
    return d->iconName;
}

void
MenuItem::setIconName(const QString &value)
{
    if (d->iconName == value)
        return;
    d->iconName = value;
    emit iconNameChanged(value);
}

QVariant
MenuItem::target() const
{
    return d->target;
}

void
MenuItem::setTarget(const QVariant &value)
{
    if (d->target == value)
        return;
    d->target = value;
    emit targetChanged(value);
}

bool
MenuItem::visible() const
{
    return d->visible;
}

void
MenuItem::setVisible(bool value)
{
    if (d->visible == value)
        return;
    d->visible = value;
    emit visibleChanged(value);
}

bool
MenuItem::enabled() const
{
    return d->enabled;
}

void
MenuItem::setEnabled(bool value)
{
     if (d->enabled == value)
         return;
     d->enabled = value;
     emit enabledChanged(value);
}

#include "unity-menu-item.moc"
