#include "PluginManager.h"
#include <QDebug>
#include <QBuffer>

PluginManager::PluginManager(QObject *parent)
    : QObject(parent)
{
}

PluginManager::~PluginManager()
{
    // Clean up all instances
    QMapIterator<QString, PluginInstance> it(m_instances);
    while (it.hasNext()) {
        it.next();
        delete it.value().plugin;
    }
    m_instances.clear();
    
    // Unload all plugins
    unloadPlugins();
}

bool PluginManager::scanDirectory(const QString& dirPath)
{
    // Clear existing plugins
    unloadPlugins();
    
    QDir dir(dirPath);
    if (!dir.exists()) {
        qWarning() << "Directory does not exist:" << dirPath;
        return false;
    }
    
    // Get shared library files
    QStringList filters;
    #if defined(Q_OS_WIN)
        filters << "*.dll";
    #elif defined(Q_OS_MACOS)
        filters << "*.dylib";
    #else
        filters << "*.so";
    #endif
    
    QStringList files = dir.entryList(filters, QDir::Files);
    bool success = false;
    
    foreach (const QString & file, files) {
        QString filePath = dir.absoluteFilePath(file);
        if (loadPlugin(filePath)) {
            success = true;
        }
    }
    
    if (success) {
        emit availablePluginsChanged();
    }
    
    return success;
}

bool PluginManager::loadPlugin(const QString& filePath)
{
    // Create a new library instance
    QLibrary* library = new QLibrary(filePath);

    // Définir le mode de chargement pour plus de détails sur les erreurs
    library->setLoadHints(QLibrary::ResolveAllSymbolsHint | QLibrary::ExportExternalSymbolsHint);

    // Try to load the library
    if (!library->load()) {
        qWarning() << "Failed to load plugin:" << filePath << "-" << library->errorString();
        delete library;
        return false;
    }
    
    // Find the exported functions
    CreatePluginFunc  createFunc = (CreatePluginFunc )library->resolve("createPlugin");
    GetPluginNameFunc nameFunc   = (GetPluginNameFunc)library->resolve("getPluginName");
    
    if (!createFunc || !nameFunc) {
        qWarning() << "Le plugin ne contient pas les exports requis:" << filePath;
        qWarning() << "createPlugin existe:" << (createFunc != nullptr);
        qWarning() << "getPluginName existe:" << (nameFunc != nullptr);

        // Essayer d'autres noms possibles (souvent les compilateurs ajoutent des décorations)
        if (!createFunc) {
            createFunc = (CreatePluginFunc)library->resolve("_createPlugin");
            qWarning() << "_createPlugin existe:" << (createFunc != nullptr);
        }

        if (!nameFunc) {
            nameFunc = (GetPluginNameFunc)library->resolve("_getPluginName");
            qWarning() << "_getPluginName existe:" << (nameFunc != nullptr);
        }

        // Si toujours pas de fonctions, abandonner
        if (!createFunc || !nameFunc) {
            library->unload();
            delete library;
            return false;
        }
    }

    // Obtenir le nom du plugin
    const char* namePtr = nameFunc();
    if (!namePtr) {
        qWarning() << "getPluginName a retourné un pointeur nul:" << filePath;
        library->unload();
        delete library;
        return false;
    }

    QString name = QString::fromUtf8(namePtr);
    if (name.isEmpty()) {
        qWarning() << "Le nom du plugin est vide:" << filePath;
        name = QFileInfo(filePath).baseName(); // Utiliser le nom de fichier comme alternative
    }

    // Store plugin info
    PluginInfo info;
    info.name = name;
    info.filePath = filePath;
    info.library = library;
    info.createFunc = createFunc;
    info.nameFunc = nameFunc;
    
    m_plugins[name] = info;
    
    qDebug() << "Loaded plugin:" << name << "from" << filePath;
    return true;
}

void PluginManager::unloadPlugins()
{
    // Unload all libraries
    QMapIterator<QString, PluginInfo> it(m_plugins);
    while (it.hasNext()) {
        it.next();
        const PluginInfo& info = it.value();
        info.library->unload();
        delete info.library;
    }
    
    m_plugins.clear();
    emit availablePluginsChanged();
}

bool PluginManager::createPluginInstance(const QString& pluginName)
{
    if (!m_plugins.contains(pluginName)) {
        qWarning() << "Plugin not found:" << pluginName;
        return false;
    }
    
    // Generate a new UUID for this instance
    QUuid objectId = QUuid::createUuid();

    try {
        // Créer une nouvelle instance de plugin
        const PluginInfo& info = m_plugins[pluginName];
        IPlugin* plugin = info.createFunc(objectId);

        if (!plugin) {
            qWarning() << "Échec de création de l'instance de plugin:" << pluginName;
            return false;
        }

        // Vérifier l'objectId retourné
        QUuid returnedId = plugin->objectId();
        qDebug() << "ObjectId fourni:" << objectId.toString();
        qDebug() << "ObjectId retourné:" << returnedId.toString();

        // Stocker l'instance
        PluginInstance instance;
        instance.plugin = plugin;
        instance.name = pluginName;
        instance.objectId = objectId;
        instance.icon = plugin->icon();

        if (instance.icon.isNull()) {
            qDebug() << "Le plugin a retourné une icône nulle, une icône par défaut sera utilisée";
        } else {
            qDebug() << "Icône récupérée, taille:" << instance.icon.width() << "x" << instance.icon.height();
        }

        m_instances[objectId.toString()] = instance;

        qDebug() << "Instance de plugin créée:" << pluginName << "avec ID:" << objectId.toString();

        // S'assurer que le signal est émis
        emit pluginInstancesChanged();

        return true;
    } catch (const std::exception& e) {
        qWarning() << "Exception lors de la création de l'instance de plugin:" << e.what();
    } catch (...) {
        qWarning() << "Exception inconnue lors de la création de l'instance de plugin";
    }
    return false;
}

bool PluginManager::destroyPluginInstance(const QString& objectId)
{
    if (!m_instances.contains(objectId)) {
        qWarning() << "Plugin instance not found:" << objectId;
        return false;
    }
    
    // Remove and delete the instance
    PluginInstance instance = m_instances.take(objectId);
    delete instance.plugin;
    
    qDebug() << "Destroyed plugin instance with ID:" << objectId;
    emit pluginInstancesChanged();
    
    return true;
}

QVariantList PluginManager::availablePlugins() const
{
    QVariantList result;
    
    QMapIterator<QString, PluginInfo> it(m_plugins);
    while (it.hasNext()) {
        it.next();
        QVariantMap plugin;
        plugin["name"] = it.key();
        result.append(plugin);
    }
    
    return result;
}

QVariantList PluginManager::pluginInstances() const
{
    QVariantList result;
    
    QMapIterator<QString, PluginInstance> it(m_instances);
    while (it.hasNext()) {
        it.next();
        const PluginInstance& instance = it.value();

        QVariantMap item;
        item["objectId"] = instance.objectId.toString();
        item["name"] = instance.name;

        // Convert QImage to base64 data URL for QML
        if (!instance.icon.isNull()) {
            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            instance.icon.save(&buffer, "PNG");
            buffer.close();
            QString iconUrl = QString("data:image/png;base64,") + QString::fromLatin1(byteArray.toBase64());
            item["icon"] = iconUrl;
        } else {
            // Fournir une icône par défaut si l'icône du plugin est nulle
            item["icon"] = QString("qrc:/placeholder-icon.svg");  // Assurez-vous d'ajouter cette ressource
        }

        result.append(item);
    }
    
    return result;
}
