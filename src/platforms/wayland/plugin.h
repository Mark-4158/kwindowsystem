/*
    SPDX-FileCopyrightText: 2015 Martin Gräßlin <mgraesslin@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KWINDOWSYSTEM_WAYLAND_PLUGIN_H
#define KWINDOWSYSTEM_WAYLAND_PLUGIN_H

#include "kwindowsystemplugininterface_p.h"

class WaylandPlugin : public KWindowSystemPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.kwindowsystem.KWindowSystemPluginInterface" FILE "wayland.json")
    Q_INTERFACES(KWindowSystemPluginInterface)

public:
    explicit WaylandPlugin(QObject *parent = nullptr);
    ~WaylandPlugin() override;

    KWindowSystemPrivate *createWindowSystem() override;
    KWindowInfoPrivate *createWindowInfo(WId window, NET::Properties properties, NET::Properties2 properties2) override;

protected Q_SLOTS:
    void activeWindowChanged(quint64 id) const;
    void currentDesktopChanged(qint32 desktop) const;
    void desktopNamesChanged() const;
    void numberOfDesktopsChanged(qint32 num) const;
    void showingDesktopChanged(bool showing) const;
    void stackingOrderChanged() const;
    void windowAdded(quint64 id) const;
    void windowChanged(quint64 id, quint32 properties, quint32 properties2) const;
    void windowRemoved(quint64 id) const;
    void workAreaChanged() const;
};

#endif
