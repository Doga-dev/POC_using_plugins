#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "PluginManager.h"

int main(int argc, char *argv[])
{
    // Activer la journalisation détaillée
    qSetMessagePattern("[%{time h:mm:ss.zzz}] %{if-debug}DEBUG%{endif}%{if-info}INFO%{endif}%{if-warning}WARNING%{endif}%{if-critical}CRITICAL%{endif}%{if-fatal}FATAL%{endif}: %{message}");

    // Setup application
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    // Définir les identifiants d'application pour QSettings
    QCoreApplication::setOrganizationName("Doga");
    QCoreApplication::setOrganizationDomain("doga.fr");
    QCoreApplication::setApplicationName("Plugin Manager POC");
    
    // Create plugin manager
    PluginManager pluginManager;
    
    // Setup QML engine
    QQmlApplicationEngine engine;
    
    // Expose plugin manager to QML
    engine.rootContext()->setContextProperty("pluginManager", &pluginManager);
    
    // Load main QML file
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    // Scan the plugins directory (relative to application directory)
    QString pluginsDir = QCoreApplication::applicationDirPath() + "/plugins";
    QDir dir(pluginsDir);
    if (!dir.exists()) {
        qWarning() << "Le répertoire des plugins n'existe pas:" << pluginsDir;
        // Créer le répertoire s'il n'existe pas
        if (!dir.mkpath(".")) {
            qWarning() << "Impossible de créer le répertoire des plugins";
        }
    } else {
        qDebug() << "Scan du répertoire des plugins:" << pluginsDir;
        qDebug() << "Fichiers dans le répertoire:";
        QStringList entries = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        foreach (const QString & entry, entries) {
            qDebug() << "  " << entry;
        }
    }

    // Tenter de charger les plugins
    bool pluginsLoaded = pluginManager.scanDirectory(pluginsDir);
    qDebug() << "Résultat du chargement des plugins:" << (pluginsLoaded ? "OK" : "Échec");

    return app.exec();
}
