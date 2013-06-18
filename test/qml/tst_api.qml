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

import Ubuntu.Unity.Action 1.0
import QtQuick 2.0
import QtTest 1.0

/* This test will make sure there are no unintentional changes to the
 * schemantics of the API (default properties work as expected, etc)
 * so that any files written to the prior release of the library
 * will not bail out when loaded resulting in client applications failing
 * to start.
 */
Item {

    Action {
        id: globalaction
        name: "NewMessage"
        text: "Write New Message"
        iconName: "email-new-message"
        description: "Write a new Message"
        keywords: "Compose;Send"
        enabled: true
        parameterType: Action.String
        onTriggered: {}
    }

    Action {
        id: myaction1
        text: "Foo"
        onTriggered: {}
    }

    PreviewAction {
        id: previewaction
        text: "Color Balance"
        commitLabel: "Apply"

        onStarted: {}
        onResetted: {}
        onCancelled: {}
        onTriggered: {}
    }

    PreviewRangeParameter {
        text: "lorem ipsum"
        value: 0
        minimumValue: -100
        maximumValue: 100
        onValueChanged: {}
    }

    MenuItem {
        action: myaction
        text: "New Message"
        iconName: "menu-new-message"
        target: "user@corporation.tld"
        visible: true
        enabled: false
    }

    ActionManager {
        id: manager

    }

    ActionContext {
        id: ctx1
    }

    ActionContext {
        id: ctx2
    }

    Component.onCompleted: {
        manager.globalContext.actions = [globalaction];
        manager.globalContext.addLocalContext(ctx1);
        manager.globalContext.addLocalContext(ctx2);

        ctx1.addAction(myaction1)
        ctx2.addAction(myaction1)
        ctx2.addAction(previewaction)

        ctx2.active = true
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
