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

\qmltype ActionContext
\inqmlmodule Ubuntu.Unity.Action 1.0
\ingroup action-base
\since 1.0

ActionContext groups actions together and by providing multiple contexts
the developer is able to control the visibility of the actions. The ActionManager then exposes the actions from
these different contexts. See \l {Action Contexts} for more details.
*/

Item {

    /*!
     \qmlproperty bool ActionContext::active : false

     If true the context is active. If false the context is inactive.

     When context has been added to the ActionManager setting this value controls
     whether or not the actions in a context are available to external components.

     The ActionManager monitors the active property of each of the local contexts
     that has been added to it. There can be only one active local context at a time.
     When one of the local contexts sets itself active the manager will notice this,
     export the actions from that given context and set the previously active local
     context as inactive. This way setting active to true on a local context is
     sufficient to manage the active local context of the manager and no additional
     calls are necessary to manually inactivate the other contexts.
    */
    property bool active : false


   /*!
     \qmlproperty list<Action> ActionContext::actions
     \default
     List of Actions in this ActionContext.

     This is the default property of ActionContext.
    */
     property list<Action> actions


    /*!
     Adds an action to the context.

     Calling this function multiple times with the same action
     does not have any side effects; the action gets added only once.
    */
    function addAction(action) {}

    /*!
     Removes an action from the context.

     Calling this function multiple times with the same action
     does not have any side effects; the action gets removed only if
     it was first added to the context with addAction().
     */
    function removeAction(action) {}


}
