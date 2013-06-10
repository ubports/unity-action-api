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

#ifndef UNITY_ACTION_COMMON
#define UNITY_ACTION_COMMON

#ifndef DOXYGEN
#define UNITY_ACTION_PUBLIC_API __attribute__ ((visibility ("default")))
#define UNITY_ACTION_PRIVATE_API __attribute__ ((visibility ("hidden")))
#else
#define UNITY_ACTION_PUBLIC_API
#define UNITY_ACTION_PRIVATE_API
#endif

#endif
