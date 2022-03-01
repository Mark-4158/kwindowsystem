/*
    SPDX-FileCopyrightText: 2014 Martin Gräßlin <mgraesslin@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only
*/

#include "kdbus_p.h"
#include "kwindowinfo_p_wayland.h"
#ifndef KDE_NO_WARNING_OUTPUT
#include <QDebug>
#endif

static const QString &DBUS_INTERFACE = QStringLiteral("org.kde.KWindowInfo");

class Q_DECL_HIDDEN KWindowInfoPrivate::Private
{
public:
    Private(WId window, NET::Properties properties, NET::Properties2 properties2);
    quint64 window;
#ifndef KDE_NO_WARNING_OUTPUT
    NET::Properties properties;
    NET::Properties2 properties2;
#endif
};

#ifndef KDE_NO_WARNING_OUTPUT
template<typename T >
static void dbusWarning(const QDBusReply<T> &reply)
{
    if (!reply.isValid())
        qWarning() << "org.kde.api.kwindowinfo:" << reply.error();
}
#endif

KWindowInfoPrivate::Private::Private(WId window, NET::Properties properties, NET::Properties2 properties2)
    : window(window)
#ifndef KDE_NO_WARNING_OUTPUT
    , properties(properties)
    , properties2(properties2)
#endif
{
#ifdef KDE_NO_WARNING_OUTPUT
    Q_UNUSED(properties);
    Q_UNUSED(properties2);
#endif
}

KWindowInfoPrivateWayland::KWindowInfoPrivateWayland(WId window, NET::Properties properties, NET::Properties2 properties2)
    : KWindowInfoPrivate(window, properties, properties2)
{
    installPidExtension(this);
    installGtkApplicationIdExtension(this);
    installGeometryExtension(this);
}

KWindowInfoPrivateWayland::~KWindowInfoPrivateWayland()
{
    if (d->properties & NET::CloseWindow) {
        QDBusMessage &&message = dbusMethodCall("close");

        message << d->window;

#ifndef KDE_NO_WARNING_OUTPUT
        const QDBusReply<void> &reply =
#endif
        dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
        dbusWarning(reply);
#endif
    }
}

bool KWindowInfoPrivateWayland::valid(bool withdrawn_is_valid) const
{
    auto &&message = dbusMethodCall("valid");

    message << d->window
            << withdrawn_is_valid;

    const QDBusReply<bool> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

NET::States KWindowInfoPrivateWayland::state() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties & NET::WMState))
        qWarning() << "Pass NET::WMState to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("state");

    message << d->window;

    const QDBusReply<NET::States::Int> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return static_cast<NET::States>(reply.value());
}

bool KWindowInfoPrivateWayland::isMinimized() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (~d->properties & (NET::WMState | NET::XAWMState))
        qWarning() << "Pass NET::WMState and NET::XAWMState to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("isMinimized");

    message << d->window;

    const QDBusReply<bool> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

NET::MappingState KWindowInfoPrivateWayland::mappingState() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties & NET::XAWMState))
        qWarning() << "Pass NET::XAWMState to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("mappingState");

    message << d->window;

    const QDBusReply<qint32> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return static_cast<NET::MappingState>(reply.value());
}

NETExtendedStrut KWindowInfoPrivateWayland::extendedStrut() const
{
    NETExtendedStrut ret;

#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties & NET::WM2ExtendedStrut))
        qWarning() << "Pass NET::WM2ExtendedStrut to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("extendedStrut");

    message << d->window;

    const QDBusReply<QVector<qint32> > &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    auto &&strut = reply.value();
    if (strut.size() == 12) {
      ret.left_width = strut[0];
      ret.left_start = strut[1];
      ret.left_end = strut[2];

      ret.right_width = strut[3];
      ret.right_start = strut[4];
      ret.right_end = strut[5];

      ret.top_width = strut[6];
      ret.top_start = strut[7];
      ret.top_end = strut[8];

      ret.bottom_width = strut[9];
      ret.bottom_start = strut[10];
      ret.bottom_end = strut[11];
    }

    return ret;
}

NET::WindowType KWindowInfoPrivateWayland::windowType(NET::WindowTypes supported_types) const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties & NET::WMWindowType))
        qWarning() << "Pass NET::WMWindowType to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("windowType");

    message << d->window
            << static_cast<quint32>(supported_types);

    const QDBusReply<qint32> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return static_cast<NET::WindowType>(reply.value());
}

QString KWindowInfoPrivateWayland::visibleName() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties & NET::WMVisibleName))
        qWarning() << "Pass NET::WMVisibleName to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("visibleName");

    message << d->window;

    const QDBusReply<QString> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

QString KWindowInfoPrivateWayland::visibleNameWithState() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (~d->properties & (NET::WMVisibleName | NET::WMState | NET::XAWMState))
        qWarning() << "Pass NET::WMVisibleName, NET::WMState and NET::XAWMState to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("visibleNameWithState");

    message << d->window;

    const QDBusReply<QString> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

QString KWindowInfoPrivateWayland::name() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties & NET::WMName))
        qWarning() << "Pass NET::WMName to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("name");

    message << d->window;

    const QDBusReply<QString> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

QString KWindowInfoPrivateWayland::visibleIconName() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties & NET::WMVisibleIconName))
        qWarning() << "Pass NET::WMVisibleIconName to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("visibleIconName");

    message << d->window;

    const QDBusReply<QString> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

QString KWindowInfoPrivateWayland::visibleIconNameWithState() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (~d->properties & (NET::WMVisibleIconName | NET::WMState | NET::XAWMState))
        qWarning() << "Pass NET::WMVisibleIconName, NET::WMState and NET::XAWMState to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("visibleIconNameWithState");

    message << d->window;

    const QDBusReply<QString> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

QString KWindowInfoPrivateWayland::iconName() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties & NET::WMIconName))
        qWarning() << "Pass NET::WMIconName to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("iconName");

    message << d->window;

    const QDBusReply<QString> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

bool KWindowInfoPrivateWayland::onAllDesktops() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties & NET::WMDesktop))
        qWarning() << "Pass NET::WMDesktop to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("onAllDesktops");

    message << d->window;

    const QDBusReply<bool> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

bool KWindowInfoPrivateWayland::isOnDesktop(int desktop) const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties & NET::WMDesktop))
        qWarning() << "Pass NET::WMDesktop to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("isOnDesktop");

    message << d->window
            << static_cast<qint32>(desktop);

    const QDBusReply<bool> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

int KWindowInfoPrivateWayland::desktop() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties & NET::WMDesktop))
        qWarning() << "Pass NET::WMDesktop to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("desktop");

    message << d->window;

    const QDBusReply<int> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

QStringList KWindowInfoPrivateWayland::activities() const
{
    return std::move(QStringList());
}

QRect KWindowInfoPrivateWayland::geometry() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties2 & NET::WMGeometry))
        qWarning() << "Pass NET::WMGeometry to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("geometry");

    message << d->window;

    const QDBusReply<QVector<qint32> > &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    auto &&rect = reply.value();

    return std::move(rect.size() == 4 ? QRect(rect[0], rect[1], rect[2], rect[3])
                                      : QRect());
}

QRect KWindowInfoPrivateWayland::frameGeometry() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties2 & NET::WMFrameExtents))
        qWarning() << "Pass NET::WMFrameExtents to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("frameGeometry");

    message << d->window;

    const QDBusReply<QVector<qint32> > &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    auto &&rect = reply.value();

    return std::move(rect.size() == 4 ? QRect(rect[0], rect[1], rect[2], rect[3])
                                      : QRect());
}

WId KWindowInfoPrivateWayland::transientFor() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties2 & NET::WM2TransientFor))
        qWarning() << "Pass NET::WM2TransientFor to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("transientFor");

    message << d->window;

    const QDBusReply<WId> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

WId KWindowInfoPrivateWayland::groupLeader() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties2 & NET::WM2GroupLeader))
        qWarning() << "Pass NET::WM2GroupLeader to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("groupLeader");

    message << d->window;

    const QDBusReply<WId> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

WId KWindowInfoPrivateWayland::win() const
{
    auto &&message = dbusMethodCall("win");

    message << d->window;

    const QDBusReply<WId> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

QByteArray KWindowInfoPrivateWayland::windowClassClass() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties2 & NET::WM2WindowClass))
        qWarning() << "Pass NET::WM2WindowClass to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("windowClassClass");

    message << d->window;

    const QDBusReply<QString> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return std::move(reply.value().toLatin1());
}

QByteArray KWindowInfoPrivateWayland::windowClassName() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties2 & NET::WM2WindowClass))
        qWarning() << "Pass NET::WM2WindowClass to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("windowClassName");

    message << d->window;

    const QDBusReply<QString> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return std::move(reply.value().toLatin1());
}

QByteArray KWindowInfoPrivateWayland::windowRole() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties2 & NET::WM2WindowRole))
        qWarning() << "Pass NET::WM2WindowRole to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("windowRole");

    message << d->window;

    const QDBusReply<QString> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return std::move(reply.value().toLatin1());
}

QByteArray KWindowInfoPrivateWayland::clientMachine() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties2 & NET::WM2ClientMachine))
        qWarning() << "Pass NET::WM2ClientMachine to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("clientMachine");

    message << d->window;

    const QDBusReply<QString> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return std::move(reply.value().toLatin1());
}

bool KWindowInfoPrivateWayland::actionSupported(NET::Action action) const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties2 & NET::WM2AllowedActions))
        qWarning() << "Pass NET::WM2AllowedActions to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("actionSupported");

    message << d->window
            << static_cast<quint32>(action);

    const QDBusReply<bool> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}

QByteArray KWindowInfoPrivateWayland::gtkApplicationId() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties2 & NET::WM2GTKApplicationId))
        qWarning() << "Pass NET::WM2GTKApplicationId to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("gtkApplicationId");

    message << d->window;

    const QDBusReply<QString> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return std::move(reply.value().toLatin1());
}

void KWindowInfoPrivateWayland::requestMoveResize(NET::Direction direction) const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties & NET::WMMoveResize))
        qWarning() << "Pass NET::WMMoveResize to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("requestMoveResize");

    message << d->window
            << static_cast<qint32>(direction);

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

void KWindowInfoPrivateWayland::setGeometry(const QRect &rect) const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties2 & NET::WM2MoveResizeWindow))
        qWarning() << "Pass NET::WM2MoveResizeWindow to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("setGeometry");

    message << d->window
            << static_cast<qint32>(rect.x())
            << static_cast<qint32>(rect.y())
            << static_cast<qint32>(rect.width())
            << static_cast<qint32>(rect.height());

#ifndef KDE_NO_WARNING_OUTPUT
    const QDBusReply<void> &reply =
#endif
    dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif
}

int KWindowInfoPrivateWayland::pid() const
{
#ifndef KDE_NO_WARNING_OUTPUT
    if (!(d->properties & NET::WMPid))
        qWarning() << "Pass NET::WMPid to KWindowInfo";
#endif

    auto &&message = dbusMethodCall("pid");

    message << d->window;

    const QDBusReply<int> &reply = dbusCall(message);

#ifndef KDE_NO_WARNING_OUTPUT
    dbusWarning(reply);
#endif

    return reply.value();
}
