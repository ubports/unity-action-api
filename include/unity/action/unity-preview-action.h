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

#ifndef UNITY_ACTION_PREVIEW_ACTION
#define UNITY_ACTION_PREVIEW_ACTION

namespace unity {
namespace action {
    class PreviewAction;
    class PreviewParameter;
}
}

#include "unity-action.h"

/*! even though PreviewAction is subclass of Action not all of the properties
 *  of Action base class are supported. parameterType must be Action::None
 */
class Q_DECL_EXPORT unity::action::PreviewAction : public unity::action::Action
{
    Q_OBJECT
    Q_DISABLE_COPY(PreviewAction)
    Q_PROPERTY(QString commitLabel
               READ commitLabel
               WRITE setCommitLabel
               NOTIFY commitLabelChanged)

public:

    explicit PreviewAction(QObject *parent = 0);
    virtual ~PreviewAction();

    QString commitLabel() const;
    void setCommitLabel(const QString &value);

    QList<PreviewParameter *> parameters();
    Q_INVOKABLE void addParameter(unity::action::PreviewParameter *parameter);
    Q_INVOKABLE void removeParameter(unity::action::PreviewParameter *parameter);

signals:
    void started();
    void cancelled();
    void resetted();

    void commitLabelChanged(const QString &value);

    void parametersChanged();

private:
    class Private;
    QScopedPointer<Private> d;
};

#endif
