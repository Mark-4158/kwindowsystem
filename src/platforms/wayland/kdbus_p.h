/*
    SPDX-FileCopyrightText: 2015 Martin Gräßlin <mgraesslin@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
#ifndef KDBUS_P_H
#define KDBUS_P_H

#include <QDBusConnection>
#include <QDBusReply>

static const QString &DBUS_SERVICE = QStringLiteral("org.kde.KWindowSystem"),
                     &DBUS_PATH = QStringLiteral("/org/kde/KWindowSystem");

#define dbusMethodCall(method) \
    QDBusMessage::createMethodCall(DBUS_SERVICE,\
                                   DBUS_PATH,\
                                   DBUS_INTERFACE,\
                                   QStringLiteral(method))
#define dbusCall(message) \
    QDBusConnection::sessionBus().call(message)

#define dbusInvoke(method) dbusCall(dbusMethodCall(method))

#endif
