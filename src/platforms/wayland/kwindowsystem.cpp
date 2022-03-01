/*
    SPDX-FileCopyrightText: 2015 Martin Gräßlin <mgraesslin@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "kdbus_p.h"
#include "kwindowsystem_p_wayland.h"
#ifndef KDE_NO_WARNING_OUTPUT
#include <QDebug>
#endif
#include <QList>
#include <QMetaMethod>
#include <QPixmap>
#include <QBuffer>

#define DBUS_INTERFACE DBUS_SERVICE

#ifndef KDE_NO_WARNING_OUTPUT
template<typename T >
static void dbusWarning(const QDBusReply<T> &reply)
{
    if (!reply.isValid())
        qWarning() << "org.kde.api.kwindowsystem:" << reply.error();
}
#endif

QList<WId> KWindowSystemPrivateWayland::windows()
{
    const QDBusReply<QList<WId> > &reply = dbusInvoke("windows");

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

QList<WId> KWindowSystemPrivateWayland::stackingOrder()
{
    const QDBusReply<QList<WId> > &reply = dbusInvoke("stackingOrder");

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

WId KWindowSystemPrivateWayland::activeWindow()
{
    const QDBusReply<WId> &reply = dbusInvoke("activeWindow");

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

void KWindowSystemPrivateWayland::activateWindow(WId win, long time)
{
    QDBusMessage &&message = dbusMethodCall("activateWindow");

    message << static_cast<quint64>(win)
            << static_cast<qint32>(time);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

void KWindowSystemPrivateWayland::forceActiveWindow(WId win, long time)
{
    QDBusMessage &&message = dbusMethodCall("forceActiveWindow");

    message << static_cast<quint64>(win)
            << static_cast<qint32>(time);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

void KWindowSystemPrivateWayland::demandAttention(WId win, bool set)
{
    QDBusMessage &&message = dbusMethodCall("demandAttention");

    message << static_cast<quint64>(win)
            << static_cast<bool>(set);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

bool KWindowSystemPrivateWayland::compositingActive()
{
    return true;
}

int KWindowSystemPrivateWayland::currentDesktop()
{
    const QDBusReply<qint32> &reply = dbusInvoke("currentDesktop");

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

int KWindowSystemPrivateWayland::numberOfDesktops()
{
    const QDBusReply<qint32> &reply = dbusInvoke("numberOfDesktops");

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

void KWindowSystemPrivateWayland::setCurrentDesktop(int desktop)
{
    QDBusMessage &&message = dbusMethodCall("setCurrentDesktop");

    message << static_cast<qint32>(desktop);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

void KWindowSystemPrivateWayland::setOnAllDesktops(WId win, bool b)
{
    QDBusMessage &&message = dbusMethodCall("setOnAllDesktops");

    message << static_cast<quint64>(win)
            << static_cast<bool>(b);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

void KWindowSystemPrivateWayland::setOnDesktop(WId win, int desktop)
{
    QDBusMessage &&message = dbusMethodCall("setOnDesktop");

    message << static_cast<quint64>(win)
            << static_cast<qint32>(desktop);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

void KWindowSystemPrivateWayland::setOnActivities(WId win, const QStringList &activities)
{
    Q_UNUSED(win)
    Q_UNUSED(activities)
}

#if KWINDOWSYSTEM_BUILD_DEPRECATED_SINCE(5, 0)
WId KWindowSystemPrivateWayland::transientFor(WId window)
{
    Q_UNUSED(window)
    return 0;
}

WId KWindowSystemPrivateWayland::groupLeader(WId window)
{
    Q_UNUSED(window)
    return 0;
}
#endif

QPixmap KWindowSystemPrivateWayland::icon(WId win, int width, int height, bool scale, int flags)
{
    Q_UNUSED(scale)
    Q_UNUSED(flags)
    QPixmap icon;
    QDBusMessage &&message = dbusMethodCall("icon");

    message << static_cast<quint64>(win)
            << static_cast<qint32>(width)
            << static_cast<qint32>(height);

    const QDBusReply<QByteArray> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return std::move(icon.loadFromData(reply.value()) && scale ? icon.scaled(width, height)
                                                               : icon);
}

void KWindowSystemPrivateWayland::setIcons(WId win, const QPixmap &icon, const QPixmap &miniIcon)
{
    QDBusMessage &&message = dbusMethodCall("setIcons");
    {
        QByteArray iconBytes, miniIconBytes;
        {
            QBuffer iconBuffer(&iconBytes), miniIconBuffer(&miniIconBytes);

            iconBuffer.open(QIODevice::WriteOnly);
            miniIconBuffer.open(QIODevice::WriteOnly);

            icon.save(&iconBuffer, "PNG");
            miniIcon.save(&miniIconBuffer, "PNG");
        }

        message << static_cast<quint64>(win)
                << iconBytes
                << miniIconBytes;
    }

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

void KWindowSystemPrivateWayland::setType(WId win, NET::WindowType windowType)
{
    QDBusMessage &&message = dbusMethodCall("setType");

    message << static_cast<quint64>(win)
            << static_cast<qint32>(windowType);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

void KWindowSystemPrivateWayland::setState(WId win, NET::States state)
{
    QDBusMessage &&message = dbusMethodCall("setState");

    message << static_cast<quint64>(win)
            << static_cast<quint32>(state);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

void KWindowSystemPrivateWayland::clearState(WId win, NET::States state)
{
    QDBusMessage &&message = dbusMethodCall("clearState");

    message << static_cast<quint64>(win)
            << static_cast<quint32>(state);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

void KWindowSystemPrivateWayland::minimizeWindow(WId win)
{
    QDBusMessage &&message = dbusMethodCall("minimizeWindow");

    message << static_cast<quint64>(win);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

void KWindowSystemPrivateWayland::unminimizeWindow(WId win)
{
    QDBusMessage &&message = dbusMethodCall("unminimizeWindow");

    message << static_cast<quint64>(win);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

void KWindowSystemPrivateWayland::raiseWindow(WId win)
{
    QDBusMessage &&message = dbusMethodCall("raiseWindow");

    message << static_cast<quint64>(win);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

void KWindowSystemPrivateWayland::lowerWindow(WId win)
{
    QDBusMessage &&message = dbusMethodCall("lowerWindow");

    message << static_cast<quint64>(win);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

bool KWindowSystemPrivateWayland::icccmCompliantMappingState()
{
    return false;
}

QRect KWindowSystemPrivateWayland::workArea(int desktop)
{
    Q_UNUSED(desktop)
    return QRect();
}

QRect KWindowSystemPrivateWayland::workArea(const QList<WId> &excludes, int desktop)
{
    Q_UNUSED(excludes)
    Q_UNUSED(desktop)
    return QRect();
}

QString KWindowSystemPrivateWayland::desktopName(int desktop)
{
    QDBusMessage &&message = dbusMethodCall("desktopName");

    message << static_cast<qint32>(desktop);

    const QDBusReply<QString> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

void KWindowSystemPrivateWayland::setDesktopName(int desktop, const QString &name)
{
    QDBusMessage &&message = dbusMethodCall("setDesktopName");

    message << static_cast<qint32>(desktop)
            << name;

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

bool KWindowSystemPrivateWayland::showingDesktop()
{
    const QDBusReply<bool> &reply = dbusInvoke("showingDesktop");

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

void KWindowSystemPrivateWayland::setShowingDesktop(bool showing)
{
    QDBusMessage &&message = dbusMethodCall("setShowingDesktop");

    message << showing;

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

void KWindowSystemPrivateWayland::setUserTime(WId win, long time)
{
    QDBusMessage &&message = dbusMethodCall("setUserTime");

    message << static_cast<quint64>(win)
            << static_cast<qint32>(time);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

void KWindowSystemPrivateWayland::setExtendedStrut(WId win,
                                                   int left_width,
                                                   int left_start,
                                                   int left_end,
                                                   int right_width,
                                                   int right_start,
                                                   int right_end,
                                                   int top_width,
                                                   int top_start,
                                                   int top_end,
                                                   int bottom_width,
                                                   int bottom_start,
                                                   int bottom_end)
{
    QDBusMessage &&message = dbusMethodCall("setExtendedStrut");

    message << static_cast<quint64>(win)
            << static_cast<qint32>(left_width)
            << static_cast<qint32>(left_start)
            << static_cast<qint32>(left_end)
            << static_cast<qint32>(right_width)
            << static_cast<qint32>(right_start)
            << static_cast<qint32>(right_end)
            << static_cast<qint32>(top_width)
            << static_cast<qint32>(top_start)
            << static_cast<qint32>(top_end)
            << static_cast<qint32>(bottom_width)
            << static_cast<qint32>(bottom_start)
            << static_cast<qint32>(bottom_end);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

void KWindowSystemPrivateWayland::setStrut(WId win, int left, int right, int top, int bottom)
{
    QDBusMessage &&message = dbusMethodCall("setStrut");

    message << static_cast<quint64>(win)
            << static_cast<qint32>(left)
            << static_cast<qint32>(right)
            << static_cast<qint32>(top)
            << static_cast<qint32>(bottom);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

bool KWindowSystemPrivateWayland::allowedActionsSupported()
{
    return false;
}

QString KWindowSystemPrivateWayland::readNameProperty(WId window, unsigned long atom)
{
    QDBusMessage &&message = dbusMethodCall("readNameProperty");

    message << static_cast<quint64>(window)
            << static_cast<quint32>(atom);

    const QDBusReply<QString> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

void KWindowSystemPrivateWayland::allowExternalProcessWindowActivation(int pid)
{
    Q_UNUSED(pid)
}

void KWindowSystemPrivateWayland::setBlockingCompositing(WId window, bool active)
{
    Q_UNUSED(window)
    Q_UNUSED(active)
}

bool KWindowSystemPrivateWayland::mapViewport()
{
    return false;
}

int KWindowSystemPrivateWayland::viewportToDesktop(const QPoint &pos)
{
    Q_UNUSED(pos)
    return 0;
}

int KWindowSystemPrivateWayland::viewportWindowToDesktop(const QRect &r)
{
    Q_UNUSED(r)
    return 0;
}

QPoint KWindowSystemPrivateWayland::desktopToViewport(int desktop, bool absolute)
{
    Q_UNUSED(desktop)
    Q_UNUSED(absolute)
    return QPoint();
}

QPoint KWindowSystemPrivateWayland::constrainViewportRelativePosition(const QPoint &pos)
{
    Q_UNUSED(pos)
    return QPoint();
}

void KWindowSystemPrivateWayland::connectNotify(const QMetaMethod &signal)
{
    Q_UNUSED(signal)
}
