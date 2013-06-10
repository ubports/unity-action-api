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

#include "unity-action.h"

using namespace unity::action;

//! \private
class Q_DECL_HIDDEN unity::action::Action::Private {
public:
    QString name;
    QString text;
    QString iconName;
    QString description;
    QString keywords;
    bool enabled;
    Action::Type parameterType;
};

Action::Action(QObject *parent)
    : QObject(parent),
      d(new Private())
{
    qRegisterMetaType<unity::action::Action::Type>();
}

Action::~Action()
{
}

QString
Action::name() const
{
    return d->name;
}

void
Action::setName(const QString &value)
{
    if (d->name == value)
        return;
    d->name = value;
    emit nameChanged(value);
}

QString
Action::text() const
{
    return d->text;
}

void
Action::setText(const QString &value)
{
    if (d->text == value)
        return;
    d->text = value;
    emit textChanged(value);
}

QString
Action::iconName() const
{
    return d->iconName;
}

void
Action::setIconName(const QString &value)
{
    if (d->iconName == value)
        return;
    d->iconName = value;
    emit iconNameChanged(value);
}

QString
Action::description() const
{
    return d->description;
}

void
Action::setDescription(const QString &value)
{
    if (d->description == value)
        return;
    d->description = value;
    emit descriptionChanged(value);
}

QString
Action::keywords() const
{
    return d->keywords;
}

void
Action::setKeywords(const QString &value)
{
    if (d->keywords == value)
        return;
    d->keywords = value;
    emit keywordsChanged(value);
}

bool
Action::enabled() const
{
    return d->enabled;
}

void
Action::setEnabled(bool value)
{
    if (d->enabled == value)
        return;
    d->enabled = value;
    emit enabledChanged(value);
}

Action::Type
Action::parameterType() const
{
    return d->parameterType;
}

void
Action::setParameterType(Type value)
{
    if (d->parameterType == value)
        return;
    d->parameterType = value;
    emit parameterTypeChanged(value);
}

void
Action::trigger(QVariant value)
{
    //! \todo validate value type
    emit triggered(value);
}
