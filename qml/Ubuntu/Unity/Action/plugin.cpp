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

#include "plugin.h"

#include <QtQml>

#include "qml-action.h"
#include "qml-preview-action.h"
#include "qml-context.h"
#include "qml-manager.h"

#include <unity/action/PreviewParameter>
#include <unity/action/PreviewRangeParameter>
#include <unity/action/MenuItem>

void
UnityActionQmlPlugin::registerTypes(const char *uri)
{
    // @uri Ubuntu.Unity.Action

    qmlRegisterType<unity::action::Action>                     ();
    qmlRegisterType<unity::action::qml::Action>                (uri, 1, 0, "Action");
    qmlRegisterType<unity::action::PreviewAction>              ();
    qmlRegisterType<unity::action::qml::PreviewAction>         (uri, 1, 0, "PreviewAction");

    qmlRegisterType<unity::action::PreviewParameter>      ();
    qmlRegisterType<unity::action::PreviewRangeParameter> (uri, 1, 0, "PreviewRangeParameter");

    // Don't provide menu item just yet.
    //qmlRegisterType<unity::action::MenuItem> (uri, 1, 0, "MenuItem");

    qmlRegisterType<unity::action::ActionContext>      ();
    qmlRegisterType<unity::action::qml::ActionContext> (uri, 1, 0, "ActionContext");
    qmlRegisterType<unity::action::ActionManager>      ();
    qmlRegisterType<unity::action::qml::ActionManager> (uri, 1, 1, "ActionManager");

}

void
UnityActionQmlPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
  Q_UNUSED(uri);
  Q_UNUSED(engine);
}


