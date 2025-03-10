#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <QObject>
#include <QLibrary>
#include <QMap>
#include <QDir>
#include <QUuid>
#include <QString>
#include <QVariantMap>
#include <QVariantList>
#include "IPlugin.h"

struct PluginInfo {
    QString name;
    QString filePath;
    QLibrary* library;
    CreatePluginFunc createFunc;
    GetPluginNameFunc nameFunc;
};

struct PluginInstance {
    IPlugin* plugin;
    QString name;
    QUuid objectId;
    QImage icon;
};

class PluginManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList availablePlugins READ availablePlugins NOTIFY availablePluginsChanged)
    Q_PROPERTY(QVariantList pluginInstances READ pluginInstances NOTIFY pluginInstancesChanged)

public:
    explicit PluginManager(QObject *parent = nullptr);
    ~PluginManager();

    // Method to scan a directory for plugins
    Q_INVOKABLE bool scanDirectory(const QString& dirPath);
    
    // Method to create a plugin instance
    Q_INVOKABLE bool createPluginInstance(const QString& pluginName);
    
    // Method to destroy a plugin instance
    Q_INVOKABLE bool destroyPluginInstance(const QString& objectId);
    
    // Methods to get data for UI
    QVariantList availablePlugins() const;
    QVariantList pluginInstances() const;

signals:
    void availablePluginsChanged();
    void pluginInstancesChanged();

private:
    // Map of available plugins (plugin name -> plugin info)
    QMap<QString, PluginInfo> m_plugins;
    
    // Map of plugin instances (object ID -> instance)
    QMap<QString, PluginInstance> m_instances;
    
    // Load a plugin from a file
    bool loadPlugin(const QString& filePath);
    
    // Unload all plugins
    void unloadPlugins();
};

#endif // PLUGIN_MANAGER_H

