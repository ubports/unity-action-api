/* This file is part of libhud-qt
 * Copyright 2013 Canonical Ltd.
 *
 * libhud-qt is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * libhud-qt is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import Ubuntu.Unity.Action 1.0 as Unity
import QtQuick 2.0
import QtTest 1.0

/* This test will make sure there are no unintentional changes to the
 * schemantics of the API (default properties work as expected, etc)
 * so that any files written to the prior release of the library
 * will not bail out when loaded resulting in client applications failing
 * to start.
 */
Item {

    Unity.Action {

    }


    TestCase {
        name: "API Test"
        id: test_api

        function test_api() {
            // just make sure this file can be loaded properly by the QmlEngine.
            verify(1)
        }
    }
}
