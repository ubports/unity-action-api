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

#include <unity/action/PreviewAction>
using namespace unity::action;

//! \private
class Q_DECL_HIDDEN unity::action::PreviewAction::Private {
public:
    QString commitLabel;
};

PreviewAction::PreviewAction(QObject *parent)
    : Action(parent),
      d(new Private())
{
}

PreviewAction::~PreviewAction()
{
}

QString
PreviewAction::commitLabel() const
{
    return d->commitLabel;
}

void
PreviewAction::setCommitLabel(const QString &value)
{
    if (d->commitLabel == value)
        return;
    d->commitLabel = value;
    emit commitLabelChanged(value);
}
