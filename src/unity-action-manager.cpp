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

#include <unity/action/ActionManager>
#include <unity/action/ActionContext>
#include <unity/action/Action>
#include <unity/action/PreviewAction>

#include <QSet>
#include <QMutex>

#include <QDebug>

// needed for gio includes.
#undef signals
#include <libhud-2/hud.h>

using namespace unity::action;

//! \private
struct Q_DECL_HIDDEN HudContextData
{
    HudActionPublisher *publisher;
    GSimpleActionGroup *group;
    QString path;
    guint exportid;
    QSet<Action *> actions;

    HudContextData() {
        publisher = 0;
        group     = 0;
    }
    HudContextData(const HudContextData &other) {
        publisher = (HudActionPublisher*)  g_object_ref(other.publisher);
        group     = (GSimpleActionGroup *) g_object_ref(other.group);
        path      = other.path;
        exportid  = other.exportid;
        actions   = other.actions;
    }
    ~HudContextData() {
        g_clear_object(&publisher);
        g_clear_object(&group);
    }
};

//! \private
struct Q_DECL_HIDDEN HudActionData
{

    HudActionDescription *desc;
    GSimpleAction        *gaction;

    HudActionData() {
        desc    = 0;
        gaction = 0;
    }
    HudActionData(const HudActionData &other) {
        desc    = (HudActionDescription*)  g_object_ref(other.desc);
        gaction = (GSimpleAction *) g_object_ref(other.gaction);
    }
    ~HudActionData() {
        g_clear_object(&desc);
        g_clear_object(&gaction);
    }
};

//! \private
class Q_DECL_HIDDEN unity::action::ActionManager::Private : public QObject
{
    Q_OBJECT

public:
    QSet<Action *> actions;

    ActionContext *globalContext;
    QSet<ActionContext *> localContexts;

    ActionContext *activeLocalContext;

    /* THE HUD */
    QMap<ActionContext *, HudContextData>       hudContexts;
    QMap<Action *, HudActionData>               hudActions;
    HudManager *hudManager;

    // HUD does not support removing actions at this point
    // for now just simply make them disabled in the eyes of HUD
    // this needs to be fixed properly when libhud gets support for
    // removing action descriptions.
    QList<HudActionData>                         hudRemovedActions;

    /* Action Group */
    GSimpleActionGroup *mainActionGroup;
    QMap<Action *, GSimpleAction *> mainActions;
    guint mainExportId;

    Private() {
        globalContext = new ActionContext();
        hudManager = 0;
    }
    ~Private() {
        delete globalContext;
        g_clear_object(&hudManager);
    }

    static void hud_action_activated(GSimpleAction *action,
                                     GVariant      *parameter,
                                     gpointer       user_data);

    static void action_activated(GSimpleAction *action,
                                 GVariant      *parameter,
                                 gpointer       user_data);


    void updateContext(ActionContext *context);
    void updateHudContext(ActionContext *context, QSet<Action *> globalActions);
    void updateActionDescription(Action *action, HudActionDescription *desc);

    GSimpleAction *createNewSimpleAction(Action *action);

    HudActionData createHudAction(Action *action);

    void updateActionsWhenNameOrTypeHaveChanged(Action *action);

public slots:
    void contextActiveChanged(bool value);
    void contextActionsChanged();
    void actionNameChanged();
    void actionParameterTypeChanged();
    void actionEnabledChanged();
    void actionPropertiesChanged();
};

void
ActionManager::Private::hud_action_activated(GSimpleAction *simpleaction,
                                            GVariant      *parameter,
                                            gpointer       user_data)
{
    Action *action = qobject_cast<Action *>((QObject *)user_data);
    Q_ASSERT(action != 0);
    if (action == 0) {
        return;
    }

    if (g_action_get_parameter_type(G_ACTION(simpleaction)) == NULL) {
        action->trigger();
        return;
    }

    if (g_variant_is_of_type(parameter, G_VARIANT_TYPE_STRING)) {
        QString state(g_variant_get_string(parameter, NULL));

        PreviewAction *previewaction = qobject_cast<PreviewAction *>(action);
        Q_ASSERT(previewaction != 0);

        if (state == "start") {
            emit previewaction->started();
            return;
        } else if (state == "end") {
            // just skip for now
            return;
        } else if (state == "commit") {
            emit previewaction->trigger();
            return;
        } else if (state == "reset") {
            emit previewaction->resetted();
            return;
        } else if (state == "cancel") {
            emit previewaction->cancelled();
            return;
        } else {
            qWarning("Unknown PreviewAction state: %s", qPrintable(state));
        }
    }

    qWarning("Tried to activate HUD action with incorrect parameter type.");
}

void
ActionManager::Private::action_activated(GSimpleAction *simpleaction,
                                         GVariant      *parameter,
                                         gpointer       user_data)
{
    Action *action = qobject_cast<Action *>((QObject *)user_data);
    Q_ASSERT(action != 0);
    if (action == 0) {
        return;
    }

    if (g_action_get_parameter_type(G_ACTION(simpleaction)) == NULL) {
        action->trigger();
        return;

    } else if (g_variant_is_of_type(parameter, G_VARIANT_TYPE_STRING)) {
        Q_ASSERT(action->parameterType() == Action::String);
        QString arg(g_variant_get_string(parameter, NULL));
        action->trigger(QVariant(arg));

    } else if (g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32)) {
        Q_ASSERT(action->parameterType() == Action::Integer);
        int arg = g_variant_get_int32(parameter);
        action->trigger(QVariant(arg));

    } else if (g_variant_is_of_type(parameter, G_VARIANT_TYPE_BOOLEAN)) {
        Q_ASSERT(action->parameterType() == Action::Bool);
        bool arg = g_variant_get_boolean(parameter);
        action->trigger(QVariant(arg));

    } else if (g_variant_is_of_type(parameter, G_VARIANT_TYPE_DOUBLE)) {
        Q_ASSERT(action->parameterType() == Action::Real);
        float arg = g_variant_get_double(parameter);
        action->trigger(QVariant(arg));

    } else {
        qWarning("Tried to activate gaction with incorrect parameter type.");
    }
}


void
ActionManager::Private::contextActiveChanged(bool value)
{
    ActionContext *context = qobject_cast<ActionContext *>(sender());
    Q_ASSERT(context != 0);

    if (context == globalContext) {
        // global context is always active.
        // ignore the possible change
        return;
    }

    if (value == true) {
        if (activeLocalContext == 0) {
            activeLocalContext = context;
            Q_ASSERT(hudContexts.contains(context));
            hud_manager_switch_window_context(hudManager,
                                              hudContexts[context].publisher);
        } else if (activeLocalContext == context) {
            // already active one.
            return;
        } else {
            // deactivate the old active one.
            activeLocalContext->setActive(false);
            activeLocalContext = context;
            Q_ASSERT(hudContexts.contains(context));
            hud_manager_switch_window_context(hudManager,
                                              hudContexts[context].publisher);
        }
    } else {
        if (activeLocalContext == context) {
            activeLocalContext = 0;
            /*! \todo this allows that we don't have an active context..
             *  maybe add yet another hud context that contains only the
             *  global actions?
             */
        }
    }

}

void
ActionManager::Private::updateHudContext(ActionContext *context,
                                         QSet<Action *> globalActions)
{
    if (!hudContexts.contains(context)) {

        /* create a new HUD context */

        static QMutex mutex;
        QMutexLocker locker(&mutex);
        static int id = 0;

        HudContextData hudctx;
        hudctx.path = QString("/com/canonical/hud/action_context_%1").arg(id++);
        hudctx.group = g_simple_action_group_new();
        hudctx.publisher = hud_action_publisher_new(HUD_ACTION_PUBLISHER_ALL_WINDOWS,
                                                    qPrintable(hudctx.path));
        hud_action_publisher_add_action_group(hudctx.publisher,
                                              "hud",
                                              qPrintable(hudctx.path));
        hudctx.exportid = g_dbus_connection_export_action_group(g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, NULL),
                                                                qPrintable(hudctx.path),
                                                                G_ACTION_GROUP(hudctx.group),
                                                                NULL);
        hud_manager_add_actions(hudManager, hudctx.publisher);
        hudContexts.insert(context, hudctx);
    }

    HudContextData &hudctx = hudContexts[context];
    QSet<Action *> old_actions = hudctx.actions;
    QSet<Action *> new_actions = context->actions();
    new_actions += globalActions;
    QSet<Action *> removed_actions = old_actions - new_actions;
    foreach (Action *i, new_actions) {
        /*! \todo what if not all actions in the context are not added with addAction() ?*/
        Q_ASSERT(hudActions.contains(i));
        HudActionData &hudactdata = hudActions[i];
        hud_action_publisher_add_description(hudctx.publisher, hudactdata.desc);
        hudctx.actions.insert(i);
        g_simple_action_group_insert(hudctx.group, G_ACTION(hudactdata.gaction));
    }
    foreach (Action *i, removed_actions) {
        qDebug() << "removed action " << i;
        hudctx.actions.remove(i);
        /*! \todo handle removal of actions from HUD */
        qWarning("%s: removing actions from the HUD is not yet supported.",
                 __PRETTY_FUNCTION__);
    }
}

void
ActionManager::Private::updateContext(ActionContext *context)
{
    if (context == globalContext) {
        foreach(ActionContext *localContext, localContexts) {
            updateHudContext(localContext, globalContext->actions());
        }
        return;
    }
    updateHudContext(context, globalContext->actions());
}

void
ActionManager::Private::updateActionDescription(Action *action, HudActionDescription *desc)
{
    Q_ASSERT(action != 0);
    Q_ASSERT(desc   != 0);

    hud_action_description_set_attribute_value(desc,
                                               G_MENU_ATTRIBUTE_LABEL,
                                               g_variant_new_string(qPrintable(action->text())));
    hud_action_description_set_attribute_value(desc,
                                               "description",
                                               g_variant_new_string(qPrintable(action->description())));
    hud_action_description_set_attribute_value(desc,
                                               "keywords",
                                               g_variant_new_string(qPrintable(action->keywords())));
/*! \todo preview action... */
#if 0
    hud_action_description_set_attribute_value(desc,
                                               "commitLabel",
                                               g_variant_new_string(qPrintable(d->commitLabel)));
#endif
}

GSimpleAction *
ActionManager::Private::createNewSimpleAction(Action *action)
{
    const GVariantType *paramType;
    switch(action->parameterType()) {
    case Action::None:
        paramType = NULL;
        break;
    case Action::String:
        paramType = G_VARIANT_TYPE_STRING;
        break;
    case Action::Integer:
        paramType = G_VARIANT_TYPE_INT32;
        break;
    case Action::Bool:
        paramType = G_VARIANT_TYPE_BOOLEAN;
        break;
    case Action::Real:
        paramType = G_VARIANT_TYPE_DOUBLE;
        break;
    }

    GSimpleAction *gaction = g_simple_action_new(qPrintable(action->name()),
                                                 paramType);
    g_signal_connect(G_OBJECT(gaction),
                     "activate",
                     G_CALLBACK(ActionManager::Private::action_activated),
                     action);
    g_simple_action_set_enabled(gaction, action->enabled());
    return gaction;
}

HudActionData
ActionManager::Private::createHudAction(Action *action)
{
    HudActionData data;

    QString actionid = action->name();

    GSimpleAction *gaction;
    gaction = g_simple_action_new(qPrintable(actionid), NULL);
    g_signal_connect(G_OBJECT(gaction), "activate", G_CALLBACK(ActionManager::Private::hud_action_activated), action);
    g_simple_action_set_enabled(gaction, action->enabled());
    data.gaction = gaction;

    HudActionDescription *desc;
    desc = hud_action_description_new(qPrintable(QString("hud.%1").arg(actionid)), NULL);
    updateActionDescription(action, desc);
    data.desc = desc;

    return data;
}

void
ActionManager::Private::contextActionsChanged()
{
    ActionContext *context = qobject_cast<ActionContext *>(sender());
    Q_ASSERT(context != 0);
    if (context == 0)
        return;
    updateContext(context);
}

void
ActionManager::Private::updateActionsWhenNameOrTypeHaveChanged(Action *action)
{
    Q_ASSERT(mainActions.contains(action));

    // create a new GSimpleAction with the name
    GSimpleAction *new_gaction = createNewSimpleAction(action);

    // remove the old one
    GSimpleAction *old_gaction = mainActions.take(action);
    g_simple_action_group_remove(mainActionGroup,
                                 g_action_get_name(G_ACTION(old_gaction)));
    g_signal_handlers_disconnect_by_data(G_OBJECT(old_gaction), action);
    g_clear_object(&old_gaction);

    // and insert the new one
    g_simple_action_group_insert(mainActionGroup,
                                 G_ACTION(new_gaction));
    mainActions.insert(action, new_gaction);


    /**** BEGIN HUD ****/
    if (hudActions.contains(action)) {

        // HUD contains the old one
        HudActionData old_data = hudActions.take(action);

        /* at the moment there is no way of actually removing actions from HUD.
         * for now we just set them as disabled and move them to removedHudActions.
         */
        g_simple_action_set_enabled(old_data.gaction, false);
        hudRemovedActions.append(old_data);
        g_signal_handlers_disconnect_by_data(G_OBJECT(old_data.gaction), action);
    }

    if (action->parameterType() == Action::None) {

        // add a new one. HUD can't handle actions with parameters
        HudActionData new_data = createHudAction(action);
        hudActions.insert(action, new_data);

        // go through all the HUD contexts and add the new descriptor
        foreach(HudContextData hudctx, hudContexts) {
            if (hudctx.actions.contains(action)) {
                hud_action_publisher_add_description(hudctx.publisher,
                                                     new_data.desc);
                g_simple_action_group_insert(hudctx.group,
                                             G_ACTION(new_data.gaction));
            }
        }
    } else {
        // update the context list of actions
        foreach(HudContextData hudctx, hudContexts) {
            if (hudctx.actions.contains(action)) {
                hudctx.actions.remove(action);
            }
        }
    }
    /**** END HUD ****/
}

void
ActionManager::Private::actionNameChanged()
{
    Action *action= qobject_cast<Action *>(sender());
    Q_ASSERT(action != 0);
    updateActionsWhenNameOrTypeHaveChanged(action);
}

void
ActionManager::Private::actionParameterTypeChanged()
{
    Action *action= qobject_cast<Action *>(sender());
    Q_ASSERT(action != 0);
    updateActionsWhenNameOrTypeHaveChanged(action);
}

void
ActionManager::Private::actionEnabledChanged()
{
    Action *action= qobject_cast<Action *>(sender());
    Q_ASSERT(action != 0);
    Q_ASSERT(mainActions.contains(action));
    GSimpleAction *gaction = mainActions[action];
    g_simple_action_set_enabled(gaction, action->enabled());


    /*** BEGIN HUD ***/

    if (hudActions.contains(action)) {
        HudActionData data = hudActions[action];
        g_simple_action_set_enabled(data.gaction, action->enabled());
    }

    /*** END HUD ***/
}

void
ActionManager::Private::actionPropertiesChanged()
{
    Action *action= qobject_cast<Action *>(sender());
    Q_ASSERT(action != 0);
    // we only care about these properties if the Action is currently export to the HUD
    if (hudActions.contains(action))
        updateActionDescription(action, hudActions[action].desc);
}



ActionManager::ActionManager(QObject *parent)
    : QObject(parent),
      d(new Private)
{
    d->activeLocalContext = 0;
    /*! \todo fixme */
    d->hudManager = hud_manager_new("fixme");

    connect(d->globalContext, SIGNAL(actionsChanged()), d.data(), SLOT(contextActionsChanged()));

    d->mainActionGroup = g_simple_action_group_new();
    d->mainExportId = g_dbus_connection_export_action_group(g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, NULL),
                                                            "/com/canonical/unity/actions",
                                                            G_ACTION_GROUP(d->mainActionGroup),
                                                            NULL);
    Q_ASSERT(d->mainExportId != 0);
}

ActionManager::~ActionManager()
{
    g_dbus_connection_unexport_action_group(g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, NULL),
                                            d->mainExportId);
}

void
ActionManager::addAction(Action *action)
{
    if (d->actions.contains(action))
        return;
    d->actions.insert(action);

    QString actionid = action->name();
    GSimpleAction *gaction;

    gaction = d->createNewSimpleAction(action);
    d->mainActions.insert(action, gaction);
    g_simple_action_group_insert(d->mainActionGroup, G_ACTION(gaction));


    connect(action, SIGNAL(nameChanged(QString)), d.data(), SLOT(actionNameChanged()));
    connect(action, SIGNAL(parameterTypeChanged(unity::action::Action::Type)), d.data(), SLOT(actionParameterTypeChanged()));
    connect(action, SIGNAL(enabledChanged(bool)), d.data(), SLOT(actionEnabledChanged()));
    connect(action, SIGNAL(textChanged(QString)), d.data(), SLOT(actionPropertiesChanged()));
    // don't care about iconName
    connect(action, SIGNAL(descriptionChanged(QString)), d.data(), SLOT(actionPropertiesChanged()));
    connect(action, SIGNAL(keywordsChanged(QString)), d.data(), SLOT(actionPropertiesChanged()));
    connect(action, SIGNAL(enabledChanged(bool)), d.data(), SLOT(actionPropertiesChanged()));

    /*** BEGIN HUD ***/
    if (action->parameterType() == Action::None) {
        // HUD can only handle actions without parameter type

        HudActionData hudactdata = d->createHudAction(action);
        d->hudActions.insert(action, hudactdata);

        /*! \todo handle PreviewAction
         *   d->simpleAction = g_simple_action_new(qPrintable(d->actionid), G_VARIANT_TYPE_STRING);
         *    hud_action_description_set_parameterized(d->desc, G_MENU_MODEL(d->paramMenu));
         */
    }
    /*** END HUD ***/

    emit actionsChanged();
}

void
ActionManager::removeAction(Action *action)
{
    if (!d->actions.contains(action))
        return;
    d->actions.remove(action);

    /*! \todo tdb */

    emit actionsChanged();
}

ActionContext *
ActionManager::globalContext()
{
    return d->globalContext;
}

void
ActionManager::addLocalContext(ActionContext *context)
{
    if (d->localContexts.contains(context))
        return;
    d->localContexts.insert(context);
    connect(context, SIGNAL(activeChanged(bool)), d.data(), SLOT(contextActiveChanged(bool)));
    connect(context, SIGNAL(actionsChanged()), d.data(), SLOT(contextActionsChanged()));

    d->updateContext(context);

    if (d->localContexts.count() == 1) {
        // make sure we have at least one active context
        // so, here we set the first one that gets added
        // as active.
        context->setActive(true);
    }
    emit localContextsChanged();
}

void
ActionManager::removeLocalContext(ActionContext *context)
{
    if (!d->localContexts.contains(context))
        return;
    d->localContexts.remove(context);
    disconnect(context, SIGNAL(activeChanged(bool)), d.data(), SLOT(contextActiveChanged(bool)));

    Q_ASSERT(d->hudContexts.contains(context));
    const HudContextData &hudctx = d->hudContexts.value(context);
    g_dbus_connection_unexport_action_group(g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, NULL),
                                            hudctx.exportid);
    d->hudContexts.remove(context);
    emit localContextsChanged();
}

QSet<ActionContext *>
ActionManager::localContexts() const
{
    return d->localContexts;
}

QSet<Action *>
ActionManager::actions() const
{
    return d->actions;
}

#include "unity-action-manager.moc"
