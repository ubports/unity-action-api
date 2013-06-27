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
#include <unity/action/PreviewParameter>
using namespace unity::action;

//! \private
class Q_DECL_HIDDEN unity::action::PreviewAction::Private : public QObject
{
    Q_OBJECT
public:
    PreviewAction *q;

    QString commitLabel;
    QList<PreviewParameter *> parameters;

public slots:
    void parameterDestroyed(QObject *obj);
};

void
PreviewAction::Private::parameterDestroyed(QObject *obj)
{
    /* we can not use qobject_cast() as it will fail for
     * objects about to be destroyed. Instead we can simply cast the
     * pointer directly and use it as long as it's not 0.
     */
    PreviewParameter *parameter = (PreviewParameter *)obj;
    if (parameter == 0) {
        return;
    }
    q->removeParameter(parameter);
}

PreviewAction::PreviewAction(QObject *parent)
    : Action(parent),
      d(new Private())
{
    d->q = this;
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

QList<PreviewParameter *> PreviewAction::parameters()
{
    return d->parameters;
}

void
PreviewAction::addParameter(unity::action::PreviewParameter *parameter)
{
    Q_ASSERT(parameter != 0);
    if (parameter == 0)
        return;
    if (d->parameters.contains(parameter))
        return;
    d->parameters.append(parameter);
    connect(parameter, SIGNAL(destroyed(QObject *)), d.data(), SLOT(parameterDestroyed(QObject *)));
    emit parametersChanged();
}

void
PreviewAction::removeParameter(unity::action::PreviewParameter *parameter)
{
    Q_ASSERT(parameter != 0);
    if (parameter == 0)
        return;
    if (!d->parameters.contains(parameter))
        return;
    parameter->disconnect(d.data());
    d->parameters.removeOne(parameter);
    emit parametersChanged();
}

#include "unity-preview-action.moc"
