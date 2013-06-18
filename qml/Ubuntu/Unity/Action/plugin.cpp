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

#include <unity/action/Action>
#include <unity/action/PreviewAction>
#include <unity/action/PreviewParameter>
#include <unity/action/PreviewRangeParameter>
#include <unity/action/MenuItem>
#include <unity/action/ActionContext>
#include <unity/action/ActionManager>

void
UnityActionQmlPlugin::registerTypes(const char *uri)
{
    // @uri Ubuntu.Unity.Action

  qmlRegisterType<unity::action::Action>                (uri, 1, 0, "Action");
  qmlRegisterType<unity::action::PreviewAction>         (uri, 1, 0, "PreviewAction");

  qmlRegisterInterface<unity::action::PreviewParameter> ("PreviewParameter");
  qmlRegisterType<unity::action::PreviewRangeParameter> (uri, 1, 0, "PreviewRangeParameter");

  qmlRegisterType<unity::action::MenuItem> (uri, 1, 0, "MenuItem");

  qmlRegisterType<unity::action::ActionContext> (uri, 1, 0, "ActionContext");
  qmlRegisterType<unity::action::ActionManager> (uri, 1, 0, "ActionManager");
}

void
UnityActionQmlPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
  Q_UNUSED(uri);
  Q_UNUSED(engine);
}


