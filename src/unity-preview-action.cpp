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


namespace unity {
namespace action {
/*!
 * \class PreviewAction
 * \brief previewable action
 *
 * The preview action is an action that allows the application to
 * generate a preview of the action before the action is applied.
 * The preview is controlled by the HUD UI.
 *
 * PreviewActions contain one or more parameters which form the preview
 * parameters of the action.
 *
 * For more details see \ref page_preview-actions
 *
 * \note Even though PreviewAction is subclass of Action not all of the properties
 * of Action base class are supported.
 *
 * \note Action::parameterType must be Action::None
 */

// properties

/*!
 * \property QString PreviewAction::commitLabel
 *
 * User visible label shown in the HUD parameter view.
 *
 * This is the label shown in the HUD UI on the button that applies or commits the action
 * after the user is happy with the parameters.
 *
 * If set to empty string the HUD UI will use a default commit label.
 *
 * \initvalue ""
 *
 * \accessors commitLabel(), setCommitLabel()
 *
 * \notify commitLabelChanged()
 */


// signals

/*!
 * \fn void PreviewAction::started()
 *
 * Signal to inform that a action is selected in the HUD UI.
 * The application should set the values of the parameters to their initial state.
 */

/*!
 * \fn void PreviewAction::cancelled()
 *
 * Signal to inform that the user has cancelled the action from the
 * HUD UI. No modifications should be done on the application and the program should
 * return to the state it was before the action was first started.
 */

/*!
 * \fn void PreviewAction::resetted()
 *
 * Signal to inform that the user has clicked the "reset" button from the
 * HUD UI. The action is still active on the HUD UI but the application
 * should reset the values of the parameters to the same values they where when the
 * action was started.
 */

/*!
 * \fn void PreviewAction::parametersChanged()
 *
 * Notifies that a parameter was either added or removed.
 */
}
}

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

/*!
 * \fn PreviewAction::PreviewAction(QObject *parent = 0)
 * \param parent parent QObject or 0
 *
 * Constructs a new PreviewAction. See the property documentation for
 * default values.
 */
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

/*!
 * \returns The list of parameters added to the action.
 */
QList<PreviewParameter *> PreviewAction::parameters()
{
    return d->parameters;
}

/*!
 * \param parameter parameter to be added
 *
 * Adds a parameter.
 *
 * Calling this function multiple times with the same parameter
 * does not have any side effects; the parameter gets added only once.
 *
 * PreviewAction monitors if the parameter is deleted and does the appropriate
 * cleanup when necessary, so it is not mandatory to call removeParameter()
 * before the action is destroyed.
 *
 * \note parameter must not be 0.
 */
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

/*!
 * \param parameter parameter to be removed
 *
 * Remove a parameter.
 *
 * Calling this function multiple times with the same parameter
 * does not have any side effects; the parameter gets removed only if
 * it was previously added with addParameter().
 *
 * \note parameter must not be 0
 */
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
