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

#include "../../../../src/unity-action.h"


void
UnityActionQmlPlugin::registerTypes(const char *uri)
{
    // @uri Ubuntu.Unity.Action

  qmlRegisterType<unity::action::Action>         (uri, 1, 0, "Action");
}

void
UnityActionQmlPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
  Q_UNUSED(uri);
  Q_UNUSED(engine);
}


