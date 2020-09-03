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
#include <unity/action/PreviewRangeParameter>

#include <QSet>
#include <QDebug>
#include <QCoreApplication>

#include <libintl.h>

// needed for gio includes.
#undef signals
#ifdef HAVE_HUD
#include <libhud-2/hud.h>
#else
#include <gio/gio.h>
#endif

using namespace unity::action;

namespace unity {
namespace action {
/*!
 * \class ActionManager
 *
 * ActionManager exports the application actions to the external components.
 * See \ref page_platform-integration and \ref page_contexts for more details.
 */

// properties

/*!
 * \property ActionContext *ActionManager::globalContext
 *
 * The globalContext of the Application.
 *
 * \note Setting the ActionContext::active on the global context has no effect;
 *
 * \accessors globalContext()
 */

// signals

/*!
 * \fn void ActionManager::localContextsChanged()
 *
 * A local context was either added or removed.
 */

/*!
 * \fn void ActionManager::actionsChanged()
 *
 * An action was either added or removed from the global context
 * or any of the local contexts the manager is currently tracking.
 */
}
}


#define UNITY_ACTION_EXPORT_PATH "/com/canonical/unity/actions"

//! \private
struct Q_DECL_HIDDEN ContextData
{
#ifdef HAVE_HUD
    HudActionPublisher *publisher;
#endif
    QSet<Action *>      actions;

    ContextData() {
#ifdef HAVE_HUD
        publisher = 0;
#endif
    }
    ContextData(const ContextData &other) {
#ifdef HAVE_HUD
        publisher = (HudActionPublisher*)  g_object_ref(other.publisher);
#endif
        actions   = other.actions;
    }
    ContextData &operator= (const ContextData &other) {
        if (this != &other){
#ifdef HAVE_HUD
            g_clear_object(&publisher);
            publisher = (HudActionPublisher*)  g_object_ref(other.publisher);
#endif
            actions   = other.actions;
        }
        return *this;
    }
    ~ContextData() {
#ifdef HAVE_HUD
        g_clear_object(&publisher);
#endif
    }
};

//! \private
struct Q_DECL_HIDDEN ParameterData
{
    GMenuItem     *gmenuitem;
    GSimpleAction *gaction;   // for now we support only one
                              // gaction per parameter

    // for valuesChanged() implementaion;
    PreviewParameter *parameter;

    ParameterData() {
        gmenuitem = 0;
        gaction   = 0;
        parameter = 0;
    }
    ParameterData(const ParameterData &other) {
        gmenuitem = (GMenuItem *)     g_object_ref(other.gmenuitem);
        gaction   = (GSimpleAction *) g_object_ref(other.gaction);
        parameter = other.parameter;
    }
    ParameterData &operator= (const ParameterData &other) {
        if (this != &other){
            g_clear_object(&gmenuitem);
            g_clear_object(&gaction);

            gmenuitem = (GMenuItem *)     g_object_ref(other.gmenuitem);
            gaction   = (GSimpleAction *) g_object_ref(other.gaction);
            parameter = other.parameter;
        }
        return *this;
    }
    ~ParameterData() {
        g_clear_object(&gmenuitem);
        g_clear_object(&gaction);
    }
};


//! \private
struct Q_DECL_HIDDEN ActionData
{
#ifdef HAVE_HUD
    HudActionDescription  *desc;
#endif
    GSimpleAction         *gaction;

    bool isPreviewAction;

    /* preview action data */
    QHash<PreviewParameter *, ParameterData> params;

    /* menu containing the parameter information */
    GMenu *paramMenu;

    ActionData() {
#ifdef HAVE_HUD
        desc      = 0;
#endif
        gaction   = 0;
        paramMenu = 0;

        isPreviewAction = false;
    }
    ActionData &operator= (const ActionData &other) {
        if (this != &other){
#ifdef HAVE_HUD
            g_clear_object(&desc);
#endif
            g_clear_object(&gaction);
            g_clear_object(&paramMenu);

#ifdef HAVE_HUD
            desc      = (HudActionDescription*) g_object_ref(other.desc);
#endif
            gaction   = (GSimpleAction *)       g_object_ref(other.gaction);
            params    = other.params;
            paramMenu = other.paramMenu;
            if (paramMenu != 0)
                g_object_ref(paramMenu);

            isPreviewAction = other.isPreviewAction;
        }
        return *this;
    }
    ActionData(const ActionData &other) {
#ifdef HAVE_HUD
        desc      = (HudActionDescription*) g_object_ref(other.desc);
#endif
        gaction   = (GSimpleAction *)       g_object_ref(other.gaction);
        params    = other.params;
        paramMenu = other.paramMenu;
        if (paramMenu != 0)
            g_object_ref(paramMenu);

        isPreviewAction = other.isPreviewAction;
    }
    ~ActionData() {
#ifdef HAVE_HUD
        g_clear_object(&desc);
#endif
        g_clear_object(&gaction);
        g_clear_object(&paramMenu);
    }
};

// needed for QHash.key()
//! \private
bool operator== (const ActionData &a, const ActionData &b) {
    // comparing these pointers is enough
#ifndef HAVE_HUD
    return a.gaction == b.gaction;
#else
    return a.gaction == b.gaction &&
           a.desc    == b.desc;
#endif
}

namespace {
class QuitAction: public Action {
    Q_OBJECT
};

class GlobalActionContext: public ActionContext {
public:
    GlobalActionContext(QObject *parent = 0) :
            ActionContext(parent) {
    }

    void addBuiltInAction(Action *action) {
        built_in_actions << action;
    }

    QSet<Action *> allActions() {
        return actions() + built_in_actions;
    }
private:
    QSet<Action *> built_in_actions;
};

static inline char * _(const char *__msgid) {
        return gettext(__msgid);
}
}

//! \private
class Q_DECL_HIDDEN unity::action::ActionManager::Private : public QObject
{
    Q_OBJECT

public:
    ActionManager *q;

    QSet<Action *> actions;

    GlobalActionContext *globalContext;
    QSet<ActionContext *> localContexts;

    QScopedPointer<Action> quitAction;

    ActionContext *activeLocalContext;

    QHash<ActionContext *, ContextData> contextData;
    QHash<Action *, ActionData>         actionData;
#ifdef HAVE_HUD
    HudManager *hudManager;
#endif

    GSimpleActionGroup *actionGroup;
    guint exportId;

    GDBusConnection *sessionBus;

    Private(ActionManager *mgr)
        : q(mgr)
    {
        globalContext = new GlobalActionContext();
#ifdef HAVE_HUD
        hudManager = 0;
#endif
    }
    ~Private() {
        delete globalContext;
#ifdef HAVE_HUD
        g_clear_object(&hudManager);
#endif
    }

    /* ActionContext */
    void updateContext(ActionContext *context);
    void createContext(ActionContext *context);
    void destroyContext(ActionContext *context);
    void updateHudContext(ActionContext *context,
                          QSet<Action *> oldActions);
    // updates the exported action group.
    void updateActionGroup();
    void setActiveContext(ActionContext *context);

    /* Action */
    void createAction(Action *action);
    void destroyAction(Action *action);
    void createActionData(Action *action, ActionData &adata);
#ifdef HAVE_HUD
    void updateActionDescription(Action *action, HudActionDescription *desc);
#endif
    void updateActionsWhenNameOrTypeHaveChanged(Action *action);
    static void action_activated(GSimpleAction *action,
                                 GVariant      *parameter,
                                 gpointer       user_data);

    /* PreviewAction */
    ActionData createHudPreviewAction(PreviewAction *action);
    void updatePreviewActionParameters(PreviewAction *action, ActionData &adata);
    void updateParameterMenu(PreviewAction *action, const ActionData &adata);

    /* PreviewRangeParameter */
    void updateRange(PreviewRangeParameter * param, const ActionData &adata);
    void updatePreviewParameterRange(PreviewParameter *parameter);
    static void range_action_activated(GSimpleAction *simpleaction,
                                       GVariant      *parameter,
                                       gpointer       user_data);



public slots:

    /* ActionContext signals */
    void contextActiveChanged(bool value);
    void contextActionsChanged();

    /* Action signals */
    void actionNameChanged();
    void actionParameterTypeChanged();
    void actionEnabledChanged();
    void actionPropertiesChanged(); // for all the rest

    /* PreviewAction signals */
    void previewActionParametersChanged();
    void previewActionCommitLabelChanged();

    /* PreviewRangeParameter signals */
    void previewRangeParameterValueChanged();
    void previewRangeParameterPropertiesChanged(); // for all the rest

    /* QObject destroy() handlers */
    void contextDestroyed(QObject *obj);
};


/************************************************************************/
/*                         PUBLIC API                                   */
/************************************************************************/

/*!
 * \param parent parent QObject or 0
 *
 * Creates a new ActionManager.
 *
 * Also creates a global context which can be accessed with ActionManager::globalContext
 */
ActionManager::ActionManager(QObject *parent)
    : QObject(parent),
      d(new Private(this))
{
    d->activeLocalContext = 0;

    GError *error = NULL;
    d->sessionBus = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &error);
    if (error != NULL) {
        qWarning("%s:\n"
                 "\tCould not get session bus. Actions will not be available through D-Bus.\n"
                 "\tReason: %s",
                 __PRETTY_FUNCTION__,
                 error->message);
        g_error_free(error);
        error = NULL;
    }

#ifdef HAVE_HUD
    const char *appid = getenv("APP_ID");
    if (appid == 0) {
        qWarning("%s:\n"
                 "\tCould not determine application identifier. HUD will not work properly.\n"
                 "\tProvide your application identifier in $APP_ID environment variable.",
                 __PRETTY_FUNCTION__);
        appid = "unknown";
    }
    d->hudManager = hud_manager_new(appid);
#endif
    connect(d->globalContext, SIGNAL(actionsChanged()), d.data(), SLOT(contextActionsChanged()));
    connect(d->globalContext, SIGNAL(activeChanged(bool)), d.data(), SLOT(contextActiveChanged(bool)));
    connect(d->globalContext, SIGNAL(destroyed(QObject*)), d.data(), SLOT(contextDestroyed(QObject *)));

    d->actionGroup = g_simple_action_group_new();

    d->quitAction.reset(new QuitAction());
    d->quitAction->setText(_("Quit"));
    d->quitAction->setDescription(_("Quit the application"));
    d->quitAction->setKeywords(_("Exit;Close"));
    connect(d->quitAction.data(), SIGNAL(triggered(QVariant)), this, SIGNAL(quit()));

    d->createContext(d->globalContext);
    d->globalContext->addBuiltInAction(d->quitAction.data());
    d->updateContext(d->globalContext);
#ifdef HAVE_HUD
    hud_manager_switch_window_context(d->hudManager,
                                      d->contextData[d->globalContext].publisher);
#endif

    d->exportId = 0;
    if (d->sessionBus) {
        d->exportId = g_dbus_connection_export_action_group(d->sessionBus,
                                                            UNITY_ACTION_EXPORT_PATH,
                                                            G_ACTION_GROUP(d->actionGroup),
                                                            &error);
        if (d->exportId == 0) {
            Q_ASSERT(error != NULL);
            qWarning("%s:\n"
                     "\tCould not export the main action group. Actions will not be available through D-Bus.\n"
                     "\tReason: %s",
                     __PRETTY_FUNCTION__,
                     error->message);
            g_error_free(error);
            error = NULL;
        }
    }
}

ActionManager::~ActionManager()
{
    d->globalContext->disconnect(d.data());
    if (d->exportId != 0) {
        Q_ASSERT(d->sessionBus != 0);
        g_dbus_connection_unexport_action_group(d->sessionBus,
                                                d->exportId);
        g_dbus_connection_flush_sync(d->sessionBus, NULL, NULL);
    }
    g_clear_object(&d->sessionBus);
}

/*!
 * \param action action to be added
 *
 * this is a shorthand for
 * \code
 *    manager->globalContext()->addAction(action);
 * \endcode
 *
 * \see ActionContext::addAction()
 */
void
ActionManager::addAction(Action *action)
{
    d->globalContext->addAction(action);
    // if action is deleted before removeAction() is called on it
    // globalContext (ActionContext) handles this case for us.
}

/*!
 * \param action action to be removed
 *
 * this is a shorthand for
 * \code
 *    manager->globalContext()->removeAction(action);
 * \endcode
 *
 * \see ActionContext::removeAction()
 */
void
ActionManager::removeAction(Action *action)
{
    d->globalContext->removeAction(action);
}

ActionContext *
ActionManager::globalContext()
{
    return d->globalContext;
}

/*!
 * \param context context to be added
 *
 * Adds a local context.
 *
 * Calling this function multiple times with the same context
 * does not have any side effects; the context gets added only once.
 *
 * ActionManager monitors if the context is deleted and does the appropriate
 * cleanup when necessary, so it is not mandatory to call removeLocalContext()
 * before the context is destroyed.
 *
 * \note If context::active is true when added,
 *       the context is made the current active one.
 *
 * \note context must not be 0.
 */
void
ActionManager::addLocalContext(ActionContext *context)
{
    Q_ASSERT(context != 0);
    if (context == 0)
        return;
    if (d->localContexts.contains(context) || context == d->globalContext)
        return;
    d->localContexts.insert(context);
    connect(context, SIGNAL(activeChanged(bool)), d.data(), SLOT(contextActiveChanged(bool)));
    connect(context, SIGNAL(actionsChanged()), d.data(), SLOT(contextActionsChanged()));
    connect(context, SIGNAL(destroyed(QObject*)), d.data(), SLOT(contextDestroyed(QObject *)));

    d->createContext(context);
    d->updateContext(context);
    emit localContextsChanged();

    if (context->active()) {
        d->setActiveContext(context);
    }
}

/*!
 * \param context context to be removed
 *
 * Removes a local context.
 *
 * Calling this function multiple times with the same context
 * does not have any side effects; the context gets removed only if
 * it was previously added with addLocalContext().
 *
 * \note  if the removed context is the current active one
 *         after the removal there is no active local context.
 *
 * \note context must not be 0
 */
void
ActionManager::removeLocalContext(ActionContext *context)
{
    Q_ASSERT(context != 0);
    if (context == 0)
        return;
    if (!d->localContexts.contains(context))
        return;
    d->localContexts.remove(context);
    context->disconnect(d.data());

    d->destroyContext(context);
    if (d->activeLocalContext == context) {
        d->activeLocalContext = 0;
        d->updateActionGroup();
    }
    emit localContextsChanged();
}

/*!
 * \returns The set of local contexts the manager is aware of.
 */
QSet<ActionContext *>
ActionManager::localContexts() const
{
    return d->localContexts;
}

/*!
 * \returns The set of actions the manager is currently aware of.
 *
 * If an action is part of multiple contexts it's still only included
 * once in the set.
 */
QSet<Action *>
ActionManager::actions() const
{
    return d->actions;
}


/************************************************************************/
/*                         ActionContext                                */
/************************************************************************/

void
ActionManager::Private::createContext(ActionContext *context)
{
    Q_ASSERT(!contextData.contains(context));
    static int id = 0;

    ContextData cdata;
#ifdef HAVE_HUD
    /* create a new HUD context */
    cdata.publisher = hud_action_publisher_new(HUD_ACTION_PUBLISHER_ALL_WINDOWS,
                                               qPrintable(QString("action_context_%1").arg(id++)));
    hud_action_publisher_add_action_group(cdata.publisher,
                                          "hud",
                                          UNITY_ACTION_EXPORT_PATH);
    hud_manager_add_actions(hudManager, cdata.publisher);
#endif
    contextData.insert(context, cdata);
}

void
ActionManager::Private::destroyContext(ActionContext *context)
{
    Q_ASSERT(contextData.contains(context));
    ContextData &cdata = contextData[context];

    QSet<Action *> actions = cdata.actions;
    foreach (Action *action, actions) {

        // remove the action from this context
        cdata.actions.remove(action);

        // when actions are removed from this context
        // we can't destroy their ActionData if any of the
        // other contexts contain the action.
        bool inUse = false;
        foreach (const ContextData &cdata, contextData) {
            if (cdata.actions.contains(action)) {
                inUse = true;
                break;
            }
        }
        if (!inUse) {
            destroyAction(action);
        }
    }

    /*! \todo remove publisher from HUD when the API is added */
    contextData.remove(context);
}

void
ActionManager::Private::contextActionsChanged()
{
    ActionContext *context = qobject_cast<ActionContext *>(sender());
    Q_ASSERT(context != 0);
    updateContext(context);
}

void
ActionManager::Private::setActiveContext(ActionContext *context)
{
    if (context == globalContext) {
        // global context is always active.
        return;
    }
    if (!context->active()) {
        // activate the context
        context->setActive(true);
    }
    if (activeLocalContext == 0) {
        activeLocalContext = context;
        updateActionGroup();
#ifdef HAVE_HUD
        hud_manager_switch_window_context(hudManager,
                                          contextData[context].publisher);
#endif
    } else if (activeLocalContext == context) {
        // already active one.
        return;
    } else {
        // deactivate the old active one.
        ActionContext *old = activeLocalContext;
        activeLocalContext = context;
        old->setActive(false);
        updateActionGroup();
#ifdef HAVE_HUD
        hud_manager_switch_window_context(hudManager,
                                          contextData[context].publisher);
#endif
    }
}

void
ActionManager::Private::contextActiveChanged(bool value)
{
    ActionContext *context = qobject_cast<ActionContext *>(sender());
    Q_ASSERT(context != 0);
    Q_ASSERT(contextData.contains(context));

    if (context == globalContext) {
        // global context is always active.
        // ignore the possible change
        return;
    }

    if (value == true) {
        setActiveContext(context);
    } else {
        if (activeLocalContext == context) {
            // the active context was deactivated
            // this means that only the global context is active
            activeLocalContext = 0;
            updateActionGroup();
#ifdef HAVE_HUD
            hud_manager_switch_window_context(hudManager,
                                              contextData[globalContext].publisher);
#endif
        }
    }
}

void
ActionManager::Private::updateActionGroup()
{
    // update the exported action group according to
    // current actions in global context and
    // in the active local context

    QSet<Action *> globalActions = globalContext->allActions();
    QSet<Action *> localActions;
    if (activeLocalContext)
        localActions = activeLocalContext->actions();
    QSet<Action *> currentActions = globalActions + localActions;

    /*
     * First remove the actions that are not part of the current active ones
     */
    QSet<gchar *> removedActionNames;
    gchar **oldActionNames = g_action_group_list_actions(G_ACTION_GROUP(actionGroup));
    for (gchar **actionName = oldActionNames; *actionName != 0; actionName++) {
        bool found = false;
        foreach (Action * action, currentActions) {
            Q_ASSERT(actionData.contains(action));
            const ActionData &adata = actionData[action];
            if (g_strcmp0(g_action_get_name(G_ACTION(adata.gaction)), *actionName) == 0) {
                found = true;
                break;
            }
            // also check for parameter gactions
            foreach (const ParameterData &pdata, adata.params) {
                if (g_strcmp0(g_action_get_name(G_ACTION(pdata.gaction)), *actionName) == 0) {
                    found = true;
                    break;
                }
            }
            if (found)
                break;
        }
        if (!found)
            removedActionNames.insert(*actionName);
    }
    foreach (gchar *name, removedActionNames) {
        g_simple_action_group_remove(actionGroup, name);
    }
    removedActionNames.clear();
    g_strfreev(oldActionNames);


    // NOTE: it is important that we reinsert all the actions so that
    //       the GActions update after parameter types have changed

    // then update all the global actions
    foreach (Action * action, globalActions) {
        Q_ASSERT(actionData.contains(action));
        const ActionData &adata = actionData[action];
        g_simple_action_group_insert(actionGroup, G_ACTION(adata.gaction));
        foreach (const ParameterData &pdata, adata.params) {
            g_simple_action_group_insert(actionGroup, G_ACTION(pdata.gaction));
        }
    }

    // and finally update the local actions.
    // if a local action has the same name than
    // a global one it will replace the global one
    foreach (Action * action, localActions) {
        Q_ASSERT(actionData.contains(action));
        const ActionData &adata = actionData[action];
        g_simple_action_group_insert(actionGroup, G_ACTION(adata.gaction));
        foreach (const ParameterData &pdata, adata.params) {
            g_simple_action_group_insert(actionGroup, G_ACTION(pdata.gaction));
        }
    }
}

void
ActionManager::Private::updateContext(ActionContext *context)
{
    Q_ASSERT(contextData.contains(context));
    ContextData &cdata = contextData[context];

    QSet<Action *> oldActions = cdata.actions;
    QSet<Action *> currentActions;
    if(context == globalContext) {
        currentActions = globalContext->allActions();
    } else {
        currentActions = context->actions();
    }
    QSet<Action *> removedActions = oldActions - currentActions;

    foreach (Action *action, currentActions) {
        // Make sure the manager knows about all of the actions
        if (!actionData.contains(action)) {
            createAction(action);
            Q_ASSERT(actionData.contains(action));
        }

        /* Add the action to the context.
         * This has to be separate from making sure the action is known to manager
         * because some other context might have introduced the same action before already.
         */
        if (!cdata.actions.contains(action)) {
            /* oldActions still contain the original old ones
             * so it's safe to modify cdata.actions here
             */
            cdata.actions.insert(action);
        }
    }


    if (context == globalContext || context == activeLocalContext) {
        // a context that affects the actionGroup have changed.
        // update the group.
        updateActionGroup();
    }


    // update the HUD contexts (publishers)
    if (context == globalContext) {
        // if global context changes we have to update all the local ones, too
        // as a HUD context is a union of the global and a local context
        foreach(ActionContext *localContext, localContexts) {
            updateHudContext(localContext, oldActions);
        }
    }
    updateHudContext(context, oldActions);


    // finally clean up the removed actions
    foreach (Action *action, removedActions) {

        // remove the action from this context
        cdata.actions.remove(action);

        // when actions are removed from this context
        // we can't destroy their ActionData if any of the
        // other contexts contain the action.
        bool inUse = false;
        foreach (const ContextData &cdata, contextData) {
            if (cdata.actions.contains(action)) {
                inUse = true;
                break;
            }
        }
        if (!inUse) {
            destroyAction(action);
        }
    }
}

void
ActionManager::Private::updateHudContext(ActionContext *context, QSet<Action *> oldActions)
{
    Q_ASSERT(contextData.contains(context));
    const ContextData &cdata = contextData[context];

#ifndef HAVE_HUD
    return;
#else

    QSet<Action *> currentActions = context->actions() + globalContext->allActions();
    QSet<Action *> newActions     = currentActions - oldActions;
    QSet<Action *> removedActions = oldActions - currentActions;

    foreach (Action *action, newActions) {
        Q_ASSERT(actionData.contains(action));
        const ActionData &adata = actionData[action];
        hud_action_publisher_add_description(cdata.publisher, adata.desc);
    }
    foreach (Action *action, removedActions) {
        Q_ASSERT(actionData.contains(action));
        const ActionData &adata = actionData[action];
        /* Removing descriptions is not supported in libhud at the moment.
         * For now, let's just empty the label and that will hide the
         * action from the HUD.
         */
        hud_action_description_set_attribute_value(adata.desc,
                                                   G_MENU_ATTRIBUTE_LABEL,
                                                   g_variant_new_string(""));
    }
#endif
}

void
ActionManager::Private::contextDestroyed(QObject *obj)
{
    /* we can not use qobject_cast() as it will fail for
     * objects about to be destroyed. Instead we can simply cast the
     * pointer directly and use it as long as it's not 0.
     */
    ActionContext *ctx = (ActionContext *)obj;
    if (ctx == 0) {
        return;
    }

    if (ctx == globalContext) {
        /* Somebody called delete on globalContext pointer.
         * This is clearly a bug in the client code, but we have to try to
         * manage this in a way we don't crash.
         *
         * Let's just print out a big fat critical.
         */
        qCritical() << __PRETTY_FUNCTION__ << ":\n"
                    << "\tClient called delete on our globalContext pointer.\n"
                    << "\tCreating a new globalContext, but this is clearly a bug\n"
                    << "\tin the client code. ";
        globalContext->disconnect(this);
        destroyContext(globalContext);
        globalContext = new GlobalActionContext();
        createContext(globalContext);
        globalContext->addBuiltInAction(quitAction.data());
        updateContext(globalContext);
        connect(globalContext, SIGNAL(actionsChanged()), this, SLOT(contextActionsChanged()));
        connect(globalContext, SIGNAL(activeChanged(bool)), this, SLOT(contextActiveChanged(bool)));
        connect(globalContext, SIGNAL(destroyed(QObject*)), this, SLOT(contextDestroyed(QObject *)));

        return;
    }

    q->removeLocalContext(ctx);
}

/************************************************************************/
/*                              Action                                  */
/************************************************************************/

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
        PreviewAction *previewAction = qobject_cast<PreviewAction *>(action);
        if (previewAction) {
            QString state(g_variant_get_string(parameter, NULL));
            if (state == "start") {
                emit previewAction->started();
                return;
            } else if (state == "end") {
                // just skip for now
                return;
            } else if (state == "commit") {
                emit previewAction->trigger();
                return;
            } else if (state == "reset") {
                emit previewAction->resetted();
                return;
            } else if (state == "cancel") {
                emit previewAction->cancelled();
                return;
            } else {
                qWarning("Unknown PreviewAction state: %s", qPrintable(state));
            }
        } else {
            Q_ASSERT(action->parameterType() == Action::String);
            QString arg(g_variant_get_string(parameter, NULL));
            action->trigger(QVariant(arg));
        }
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
ActionManager::Private::createAction(Action *action)
{
    Q_ASSERT(!actionData.contains(action));

    ActionData adata;
    adata.isPreviewAction = (qobject_cast<PreviewAction *>(action) != 0);
    createActionData(action, adata);
    actionData.insert(action, adata);

    connect(action, SIGNAL(nameChanged(QString)), this, SLOT(actionNameChanged()));
    connect(action, SIGNAL(parameterTypeChanged(unity::action::Action::Type)), this, SLOT(actionParameterTypeChanged()));
    connect(action, SIGNAL(enabledChanged(bool)), this, SLOT(actionEnabledChanged()));
    connect(action, SIGNAL(textChanged(QString)), this, SLOT(actionPropertiesChanged()));
    // don't care about iconName
    connect(action, SIGNAL(descriptionChanged(QString)), this, SLOT(actionPropertiesChanged()));
    connect(action, SIGNAL(keywordsChanged(QString)), this, SLOT(actionPropertiesChanged()));
    connect(action, SIGNAL(enabledChanged(bool)), this, SLOT(actionPropertiesChanged()));

    if (adata.isPreviewAction) {
        PreviewAction *previewAction = qobject_cast<PreviewAction *>(action);
        Q_ASSERT(previewAction != 0);
        connect(previewAction, SIGNAL(commitLabelChanged(QString)), this, SLOT(previewActionCommitLabelChanged()));
        connect(previewAction, SIGNAL(parametersChanged()), this, SLOT(previewActionParametersChanged()));
    }

    actions.insert(action);
    emit q->actionsChanged();
}

void
ActionManager::Private::destroyAction(Action *action)
{
    Q_ASSERT(actionData.contains(action));
    const ActionData &adata = actionData[action];

    action->disconnect(this);

    actionData.remove(action);
    actions.remove(action);
    emit q->actionsChanged();
}

void ActionManager::Private::createActionData(Action *action, ActionData &adata)
{
    const GVariantType *paramType = NULL;
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

    if (adata.isPreviewAction) {
        if (action->parameterType() != Action::None) {
            qWarning("%s:\n"
                     "\tPreviewAction parameter type is not Action::None\n"
                     "\tThis is not supported.\n"
                     "\tSetting the parameter type to None",
                     __PRETTY_FUNCTION__);
            action->setParameterType(Action::None);
        }
        // PreviewActions have to have string paramType as an implementation detail.
        paramType = G_VARIANT_TYPE_STRING;
    }

    QString actionid = action->name();
    adata.gaction = g_simple_action_new(qPrintable(actionid),
                                                   paramType);
    g_simple_action_set_enabled(adata.gaction, action->enabled());
    g_signal_connect(G_OBJECT(adata.gaction),
                     "activate",
                     G_CALLBACK(Private::action_activated),
                     action);

#ifdef HAVE_HUD
    adata.desc = hud_action_description_new(qPrintable(QString("hud.%1").arg(actionid)), NULL);
    updateActionDescription(action, adata.desc);
#endif

    if (adata.isPreviewAction) {
        PreviewAction *previewAction = qobject_cast<PreviewAction *>(action);
        Q_ASSERT(previewAction != 0);
        adata.paramMenu = g_menu_new();
        updatePreviewActionParameters(previewAction, adata);
        updateParameterMenu(previewAction, adata);
#ifdef HAVE_HUD
        hud_action_description_set_parameterized(adata.desc, G_MENU_MODEL(adata.paramMenu));
#endif
    }
}

#ifdef HAVE_HUD
void
ActionManager::Private::updateActionDescription(Action *action,
                                                HudActionDescription *desc)
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

    PreviewAction *previewAction = qobject_cast<PreviewAction *>(action);
    if (previewAction != 0) {
        hud_action_description_set_attribute_value(desc,
                                                   "commitLabel",
                                                   g_variant_new_string(qPrintable(previewAction->commitLabel())));
    }

    QuitAction *quitAction = qobject_cast<QuitAction *>(action);
    if (quitAction != 0) {
        hud_action_description_set_attribute_value(desc,
                                                   "hud-toolbar-item",
                                                   g_variant_new_string("quit"));
    }
}
#endif

void
ActionManager::Private::updateActionsWhenNameOrTypeHaveChanged(Action *action)
{
    Q_ASSERT(actionData.contains(action));
    ActionData &adata = actionData[action];
    ActionData tmpdata;

    tmpdata.isPreviewAction = adata.isPreviewAction;
    createActionData(action, tmpdata);

    // update the gaction
    g_signal_handlers_disconnect_by_data(G_OBJECT(adata.gaction), action);
    g_clear_object(&adata.gaction);
    adata.gaction = (GSimpleAction *)g_object_ref(tmpdata.gaction);
    if (globalContext->allActions().contains(action) ||
        (activeLocalContext != 0 && activeLocalContext->actions().contains(action))) {
        updateActionGroup();
    }

#ifdef HAVE_HUD
    // update the desc
    // go through all the HUD contexts and add the new descriptor
    foreach(ContextData cdata, contextData) {
        if (cdata.actions.contains(action)) {

            // remove the old one
            /* Removing descriptions is not supported in libhud at the moment.
             * For now, let's just empty the label and that will hide the
             * action from the HUD.
             */
            hud_action_description_set_attribute_value(adata.desc,
                                                       G_MENU_ATTRIBUTE_LABEL,
                                                       g_variant_new_string(""));

            hud_action_publisher_add_description(cdata.publisher,
                                                 tmpdata.desc);
        }
    }
    g_clear_object(&adata.desc);
    adata.desc = (HudActionDescription *)g_object_ref(tmpdata.desc);
#endif
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
    Q_ASSERT(actionData.contains(action));
    g_simple_action_set_enabled(actionData[action].gaction, action->enabled());
}

void
ActionManager::Private::actionPropertiesChanged()
{
    Action *action= qobject_cast<Action *>(sender());
    Q_ASSERT(action != 0);
    Q_ASSERT(actionData.contains(action));
#ifdef HAVE_HUD
    updateActionDescription(action, actionData[action].desc);
#endif
}

/************************************************************************/
/*                         PreviewAction                                */
/************************************************************************/

void
ActionManager::Private::previewActionParametersChanged()
{
    PreviewAction *action = qobject_cast<PreviewAction *>(sender());
    Q_ASSERT(action != 0);
    Q_ASSERT(actionData.contains(action));
    ActionData &adata = actionData[action];

    updatePreviewActionParameters(action, adata);
    updateParameterMenu(action, adata);

    if (globalContext->actions().contains(action) ||
        (activeLocalContext != 0 && activeLocalContext->actions().contains(action))) {
        updateActionGroup();
    }
}

void
ActionManager::Private::previewActionCommitLabelChanged()
{
    Action *action= qobject_cast<Action *>(sender());
    Q_ASSERT(action != 0);
    Q_ASSERT(actionData.contains(action));
#ifdef HAVE_HUD
    updateActionDescription(action, actionData[action].desc);
#endif
}

void
ActionManager::Private::updatePreviewActionParameters(PreviewAction *action,
                                                      ActionData &adata)
{
    QList<PreviewParameter *> currentParameters = action->parameters();
    QList<PreviewParameter *> newParameters;
    QSet<PreviewParameter *> removedParameters;

    foreach (PreviewParameter *parameter, currentParameters) {
        if (!adata.params.contains(parameter)) {
            /* Keep this as a list now as the previewActions unit test relies
             * on parameter GActions to be created in the same order as the
             * PreviewParameters are added to the PreviewAction.
             */
            newParameters.append(parameter);
        }
    }

    foreach (PreviewParameter *parameter, adata.params.keys()) {
        if (!currentParameters.contains(parameter)) {
            removedParameters.insert(parameter);
        }
    }

    // remove the old
    foreach (PreviewParameter *parameter, removedParameters) {
        if (qobject_cast<PreviewRangeParameter *>(parameter)) {
            PreviewRangeParameter *range = qobject_cast<PreviewRangeParameter *>(parameter);
            Q_ASSERT(adata.params.contains(range));
            ParameterData pdata = adata.params[range];

            g_signal_handlers_disconnect_by_data(G_OBJECT(pdata.gaction),
                                                 range);

            range->disconnect(this);

            adata.params.remove(range);
        } else {
            qWarning("%s:\n"
                     "\tUnknown PreviewParameter subclass: %s",
                     __PRETTY_FUNCTION__,
                     parameter->metaObject()->className());
        }
    }

    // add the new
    foreach (PreviewParameter *parameter, newParameters) {
        if (qobject_cast<PreviewRangeParameter *>(parameter)) {
            PreviewRangeParameter *range = qobject_cast<PreviewRangeParameter *>(parameter);

            static int id = 0;

            ParameterData pdata;

            QString actionid = QString("unity-action-range-param-%1").arg(id++);

            pdata.gmenuitem = g_menu_item_new("", qPrintable(QString("hud.") + actionid));
            g_menu_item_set_attribute_value(pdata.gmenuitem,
                                            "parameter-type",
                                            g_variant_new_string("slider"));

            pdata.gaction = g_simple_action_new(qPrintable(actionid), G_VARIANT_TYPE_DOUBLE);
            g_signal_connect(G_OBJECT(pdata.gaction),
                             "activate",
                             G_CALLBACK(Private::range_action_activated),
                             range);

            pdata.parameter = range;
            connect(range, SIGNAL(valueChanged(float)), this, SLOT(previewRangeParameterValueChanged()));
            connect(range, SIGNAL(textChanged(QString)), this, SLOT(previewRangeParameterPropertiesChanged()));
            connect(range, SIGNAL(minimumValueChanged(float)), this, SLOT(previewRangeParameterPropertiesChanged()));
            connect(range, SIGNAL(maximumValueChanged(float)), this, SLOT(previewRangeParameterPropertiesChanged()));

            adata.params.insert(range, pdata);
            updateRange(range, adata);
        } else {
            qWarning("%s:\n"
                     "\tUnknown PreviewParameter subclass: %s",
                     __PRETTY_FUNCTION__,
                     parameter->metaObject()->className());
        }
    }
}

void
ActionManager::Private::updateParameterMenu(PreviewAction *action, const ActionData &adata)
{
    Q_ASSERT(adata.paramMenu != 0);
    /* as g_menu and g_menu_model don't support indexing
     * we just have to clear the whole menu and build a new one
     */
    // newest glib has g_menu_remove_all(), so we can use it in the future
    while (g_menu_model_get_n_items(G_MENU_MODEL(adata.paramMenu)) > 0) {
        g_menu_remove(adata.paramMenu, 0);
    }

    foreach (PreviewParameter *parameter, action->parameters()) {
        Q_ASSERT(adata.params.contains(parameter));
        Q_ASSERT(adata.params[parameter].gmenuitem);
        g_menu_append_item(adata.paramMenu,
                           adata.params[parameter].gmenuitem);
    }
#ifdef HAVE_HUD
    hud_action_description_set_parameterized(adata.desc, G_MENU_MODEL(adata.paramMenu));
#endif

}


/************************************************************************/
/*                     PreviewRangeParameter                            */
/************************************************************************/


void
ActionManager::Private::range_action_activated(GSimpleAction *simpleaction,
                                               GVariant      *parameter,
                                               gpointer       user_data)
{
    Q_UNUSED(simpleaction);
    PreviewRangeParameter *that;
    that = qobject_cast<PreviewRangeParameter *>((QObject*)user_data);
    Q_ASSERT(that != 0);

    float value = g_variant_get_double(parameter);
    that->setValue(value);
}


void
ActionManager::Private::previewRangeParameterValueChanged()
{
    /*! \bug preview parameters should use GAction::target to pass the values
     *       instead of GAction::activated(). We can implement reset() only
     *       by using target
     *
     *       This also requires changes to the HUD UI on the shell side.
     *
     *       The code below should be updated to actually change the state on each gaction.
     *       if left uncommented we just generate additional range_action_activated() calls
     *       without any benefit.
     */
#if 0
    PreviewRangeParameter *parameter = qobject_cast<PreviewRangeParameter *>(sender());
    Q_ASSERT(parameter != 0);
    foreach (const ActionData &adata, actionData) {
        foreach (const ParameterData &pdata, adata.params) {
            if (pdata.parameter == parameter) {
                g_action_activate(G_ACTION(pdata.gaction), g_variant_new_double(parameter->value()));
            }
        }
    }
#endif
}

void
ActionManager::Private::previewRangeParameterPropertiesChanged()
{
    PreviewRangeParameter *parameter = qobject_cast<PreviewRangeParameter *>(sender());
    Q_ASSERT(parameter != 0);
    foreach (const ActionData &adata, actionData) {
        if (adata.params.contains(parameter)) {
            updateRange(parameter, adata);
            PreviewAction *previewAction = qobject_cast<PreviewAction *>(actionData.key(adata));
            Q_ASSERT(previewAction != 0);
            updateParameterMenu(previewAction, adata);
        }
    }
}

void
ActionManager::Private::updateRange(PreviewRangeParameter *range, const ActionData &adata)
{
    Q_ASSERT(adata.params.contains(range));
    ParameterData pdata = adata.params[range];

    g_menu_item_set_attribute_value(pdata.gmenuitem, "min", g_variant_new_double(range->minimumValue()));
    g_menu_item_set_attribute_value(pdata.gmenuitem, "max", g_variant_new_double(range->maximumValue()));
    g_menu_item_set_attribute_value(pdata.gmenuitem, G_MENU_ATTRIBUTE_LABEL, g_variant_new_string(qPrintable(range->text())));
}


#include "unity-action-manager.moc"
