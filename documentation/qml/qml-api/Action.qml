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

\qmltype Action
\inqmlmodule Ubuntu.Unity.Action 1.0
\ingroup action-base
\since 1.0
Defines a Unity Action.

\qml
import Ubuntu.Unity.Action 1.0

Action {
    id: myaction1
    name: "myaction1"
    text: i18n.tr("My Action")
    description: i18n.tr("Perform foo.")
    onTriggered: {
        console.debug ("myaction1 is triggered.");
    }
}

\endqml

*/


Item {
    /*!
      \qmlproperty string Action::name
      Name for the action. The action is available in D-Bus with this name.
    */
    property string name

    /*!
      \qmlproperty string Action::text
      User visible label for the action.

      Text is the primary string shown by the HUD in it's results.
    */
    property string text

    /*!
      \qmlproperty string Action::description
      User visible description for the action.

      Description is more verbose than the label and should describe the Action with
      couple of words.
    */
    property string description

    /*!
      \qmlproperty string Action::keywords
      Additional keywords for the action.

      Keywords improve the search results when the user tries to search for an action
      with a synonym of the label. For example if we the application has an action "Crop"
      but the user tries to "Trim" then without the keywords the HUD would not try to offer
      the "Crop" action.

      The format of the keywords string is "Keyword 1;Keyword 2;Keyword 3" to allow translators
      to define different number of keywords per language.

      The keywords are separated by ; and they may contain spaces.

      \qml
      Action {
        text: i18n.tr("Crop")
        description: i18n.tr("Crop the image")
        keywords: i18n.tr("Trim;Cut")
      }
      \endqml
    */
    property string keywords

    /*!
      \qmlproperty bool Action::enabled : true

      If set to false the Action is removed from the search results of the HUD.
    */
    property bool enabled

    /*!
     \qmlsignal Action::triggered(var value)
     Signal to commit or apply the action.

    */
    signal triggered(var value)

    /*!
      \qmlmethod void Action::trigger(var value)

      Causes the triggered() signal to be activated.
    */
    function trigger(value) {}
}


