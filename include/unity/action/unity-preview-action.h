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

/*!
 *
 * The preview action is an action that performs a preview of the action. The preview is controlled by the HUD UI.
 * PreviewActions contain one or more parameters which can form the preview parameters of the action.
 *
 * The PreviewAction has additional extra requirements compared to other Actions:
 * - It can have multiple parameters whose values can be seen and changed over the bus
 * - The parameters’ ordering can be specified by client code
 * - It adds new signals to control the preview mode: started, cancelled, resetted
 * - It has properties to control how the preview is generated
 *
 * The ordering of the parameters is specified by the order in which the actions are defined in the parameters list.
 *
 * PreviewAction does not have a value of it’s own. Instead the values of the parameters provide the information
 * on which the preview is generated.
 *
 * Diagram describing when the different signals are emitted:
 *
 * <diagram>
 *
 * even though PreviewAction is subclass of Action not all of the properties
 * of Action base class are supported. parameterType must be Action::None
 *
 * PreviewAction {
    PreviewActionParameterRange {
        id: range1
        min: 0.0
        max: 1.0
onValueChanged: console.log(“range1 = “ + arg)
}
    PreviewActionParameterRange {
        id: range2
min: -5.0
        max: 5.0
onValueChanged: console.log(“range2 = “ + arg)
}
PreviewActionParameterRange {
    id: range3
min: 0.0
        max: 100.0
    onValueChanged: console.log(“range3 = “ + arg)
}
PreviewActionParameterString {
    id: string1
    onValueChanged: console.log(“string 1 = “ + value)
}
onStarted: console.log(“set the initial values”)
onResetted: console.log(“reset the values”)
onCancelled: console.log(“user decided to cancel”)
onTriggered: {
console.log(“apply all the changes with:”)
console.log(“range1: “ + range1.value)
console.log(“range2: “ + range2.value)
console.log(“range3: “ + range1.value)
console.log(“string1: “ + string1.value)
}
}
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
