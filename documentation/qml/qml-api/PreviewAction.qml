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

import QtQuick 2.0

/*!

\qmltype PreviewAction
\inqmlmodule Ubuntu.Unity.Action 1.0
\ingroup action-base
\since 1.0
\brief previewable action

The preview action is an action that allows the application to
generate a preview of the action before the action is applied.
The preview is controlled by the HUD UI.

PreviewActions contain one or more parameters which form the preview
parameters of the action.

For more details see \l {Preview Actions}

\note Even though PreviewAction is subclass of Action not all of the properties
of Action base class are supported.

\note Action::parameterType must be Action::None
*/


Action {

    /*!
     \qmlproperty QString PreviewAction::commitLabel : ""

     User visible label shown in the HUD parameter view.

     This is the label shown in the HUD UI on the button that applies or commits the action
     after the user is happy with the parameters.

     If set to empty string the HUD UI will use a default commit label.
     */
    property string commitLabel


    /*!
     \qmlsignal PreviewAction::started()

     Signal to inform that a action is selected in the HUD UI.
     The application should set the values of the parameters to their initial state.
     */
    signal started()

    /*!
     \qmlsignal PreviewAction::cancelled()

     Signal to inform that the user has cancelled the action from the
     HUD UI. No modifications should be done on the application and the program should
     return to the state it was before the action was first started.
     */
    signal cancelled()

    /*!
     \qmlsignal PreviewAction::resetted()

     Signal to inform that the user has clicked the "reset" button from the
     HUD UI. The action is still active on the HUD UI but the application
     should reset the values of the parameters to the same values they where when the
     action was started.
     */
    signal resetted()
}
