/*
    SPDX-FileCopyrightText: 2015 Martin Gräßlin <mgraesslin@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "plugin.h"
#include "kdbus_p.h"
#include "kwindowsystem.h"
#include "kwindowinfo_p_wayland.h"
#include "kwindowsystem_p_wayland.h"
#ifndef KDE_NO_WARNING_OUTPUT
#include <QDebug>
#endif

#define dbusConnect(name, receiver, slot) \
    QDBusConnection::sessionBus().connect(DBUS_SERVICE,\
                                          DBUS_PATH,\
                                          DBUS_SERVICE,\
                                          QStringLiteral(name),\
                                          receiver,\
                                          slot)

#define dbusDisconnect(name, receiver, slot) \
    QDBusConnection::sessionBus().disconnect(DBUS_SERVICE,\
                                             DBUS_PATH,\
                                             DBUS_SERVICE,\
                                             QStringLiteral(name),\
                                             receiver,\
                                             slot)

#ifdef KDE_NO_WARNING_OUTPUT
#define dbusWarning(succeded, message) succeded
#else
#define dbusWarning(succeded, message) \
    if (!succeded)\
        qWarning() << message
#endif

// public
WaylandPlugin::WaylandPlugin(QObject *parent)
    : KWindowSystemPluginInterface(parent)
{
    dbusWarning(dbusConnect("activeWindowChanged",
                            this,
                            SLOT(activeWindowChanged(quint64))),
                "Cannot catch activeWindowChanged() over D-Bus");

    dbusWarning(dbusConnect("currentDesktopChanged",
                            this,
                            SLOT(currentDesktopChanged(qint32))),
                "Cannot catch currentDesktopChanged() over D-Bus");
    dbusWarning(dbusConnect("desktopNamesChanged",
                            this,
                            SLOT(desktopNamesChanged())),
                "Cannot catch desktopNamesChanged() over D-Bus");
    dbusWarning(dbusConnect("numberOfDesktopsChanged",
                            this,
                            SLOT(numberOfDesktopsChanged(qint32))),
                "Cannot catch numberOfDesktopsChanged() over D-Bus");
    dbusWarning(dbusConnect("showingDesktopChanged",
                            this,
                            SLOT(showingDesktopChanged(bool))),
                "Cannot catch showingDesktopChanged() over D-Bus");
    dbusWarning(dbusConnect("stackingOrderChanged",
                            this,
                            SLOT(stackingOrderChanged())),
                "Cannot catch stackingOrderChanged() over D-Bus");

    dbusWarning(dbusConnect("windowAdded",
                            this,
                            SLOT(windowAdded(quint64))),
                "Cannot catch windowAdded() over D-Bus");
    dbusWarning(dbusConnect("windowChanged",
                            this,
                            SLOT(windowChanged(quint64, quint32, quint32))),
                "Cannot catch windowChanged() over D-Bus");
    dbusWarning(dbusConnect("windowRemoved",
                            this,
                            SLOT(windowRemoved(quint64))),
                "Cannot catch windowRemoved() over D-Bus");

    dbusWarning(dbusConnect("workAreaChanged",
                            this,
                            SLOT(workAreaChanged())),
                "Cannot catch workAreaChanged() over D-Bus");
}

WaylandPlugin::~WaylandPlugin()
{
    dbusDisconnect("activeWindowChanged",
                   this,
                   SLOT(activeWindowChanged(qint32)));

    dbusDisconnect("currentDesktopChanged",
                   this,
                   SLOT(currentDesktopChanged(qint32)));
    dbusDisconnect("desktopNamesChanged", this, SLOT(desktopNamesChanged()));
    dbusDisconnect("numberOfDesktopsChanged",
                   this,
                   SLOT(numberOfDesktopsChanged(qint32)));
    dbusDisconnect("showingDesktopChanged",
                   this,
                   SLOT(showingDesktopChanged(bool)));
    dbusDisconnect("stackingOrderChanged", this, SLOT(stackingOrderChanged()));

    dbusDisconnect("windowAdded", this, SLOT(windowAdded(quint64)));
    dbusDisconnect("windowChanged",
                   this,
                   SLOT(windowChanged(quint64, quint32, quint32)));
    dbusDisconnect("windowRemoved", this, SLOT(windowRemoved(quint64)));

    dbusDisconnect("workAreaChanged", this, SLOT(workAreaChanged()));
}

KWindowSystemPrivate *WaylandPlugin::createWindowSystem()
{
    return new KWindowSystemPrivateWayland();
}

KWindowInfoPrivate *WaylandPlugin::createWindowInfo(WId window, NET::Properties properties, NET::Properties2 properties2)
{
    return new KWindowInfoPrivateWayland(window, properties, properties2);
}

// protected Q_SLOTS
void WaylandPlugin::activeWindowChanged(quint64 id) const
{
    Q_EMIT KWindowSystem::self()->activeWindowChanged(id);
}

void WaylandPlugin::currentDesktopChanged(qint32 desktop) const
{
    Q_EMIT KWindowSystem::self()->currentDesktopChanged(desktop);
}

void WaylandPlugin::desktopNamesChanged() const
{
    Q_EMIT KWindowSystem::self()->desktopNamesChanged();
}

void WaylandPlugin::numberOfDesktopsChanged(qint32 num) const
{
    Q_EMIT KWindowSystem::self()->numberOfDesktopsChanged(num);
}

void WaylandPlugin::showingDesktopChanged(bool showing) const
{
    Q_EMIT KWindowSystem::self()->showingDesktopChanged(showing);
}

void WaylandPlugin::stackingOrderChanged() const
{
    Q_EMIT KWindowSystem::self()->stackingOrderChanged();
}

void WaylandPlugin::windowAdded(quint64 id) const
{
    Q_EMIT KWindowSystem::self()->windowAdded(id);
}

void WaylandPlugin::windowChanged(quint64 id, quint32 properties, quint32 properties2) const
{
    Q_EMIT KWindowSystem::self()->windowChanged(id,
                                                static_cast<NET::Property>(properties),
                                                static_cast<NET::Property2>(properties2));
}

void WaylandPlugin::windowRemoved(quint64 id) const
{
    Q_EMIT KWindowSystem::self()->windowRemoved(id);
}

void WaylandPlugin::workAreaChanged() const
{
    Q_EMIT KWindowSystem::self()->workAreaChanged();
}
