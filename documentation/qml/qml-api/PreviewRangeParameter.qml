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

\qmltype PreviewRangeParameter
\inqmlmodule Ubuntu.Unity.Action 1.0
\ingroup action-parameters
\since 1.0

Provides a range preview parameter.
See \l {Preview Actions} for more details.
*/

PreviewParameter {

    /*!
     \qmlproperty real PreviewRangeParameter::maximumValue : 100

     The maximum value of the range.
     */
    property real maximumValue : 100

    /*!
     \qmlproperty real PreviewRangeParameter::minimumValue : 0

     The minimum value of the range.
     */
    property real minimumValue : 0

    /*!
     \qmlproperty string PreviewRangeParameter::text : ""

     The user visible primary label of the parameter.
     */
    property string text : ""

    /*!
     \qmlproperty real PreviewRangeParameter::value : 0

     The current value of the range.
     */
    property real value : 0

}
