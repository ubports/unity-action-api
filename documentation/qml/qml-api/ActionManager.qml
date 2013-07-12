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

\qmltype ActionManager
\inqmlmodule Ubuntu.Unity.Action 1.0
\ingroup action-base
\since 1.0
\brief

ActionManager exports the application actions to the external components.
See \l {Platform Integration} and \l {Action Contexts} for more details.

*/

Item {

    /*!
       \qmlproperty ActionContext  ActionManager::globalContext

       The globalContext of the Application.

       \note Setting the ActionContext::active on the global context has no effect;
     */
    property ActionContext globalContext

    /*!
      \qmlproperty list<Action> ActionManager::actions
      \default
      List of Actions in manager's globalContext.

      This is the default property of ActionManager.
     */
    property list<Action> actions

    /*!
      \qmlproperty list<ActionContext> ActionManager::localContexts

      List of localContexts.
     */
    property list<ActionContext> localContexts


    /*!
     this is a shorthand for
     \qml
        manager.globalContext.addAction(action);
     \endqml

     \sa ActionContext::addAction()
     */
    function addAction(action) {}

    /*!
     this is a shorthand for
     \qml
        manager.globalContext.removeAction(action);
     \endqml

     \sa ActionContext::removeAction()
     */
    function removeAction(action) {}

    /*!
     Adds a local context.

     Calling this function multiple times with the same context
     does not have any side effects; the context gets added only once.
    */
    function addLocalContext(context) {}

    /*!
     Removes a local context.

     Calling this function multiple times with the same context
     does not have any side effects; the context gets removed only if
     it was previously added with addLocalContext().
    */
    function removeLocalContext(context) {}

}
