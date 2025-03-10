#include "AnotherPlugin.h"

// Define a macro for export declaration depending on platform
#if defined(_WIN32) || defined(_WIN64)
    #define PLUGIN_EXPORT __declspec(dllexport)
#else
    #define PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

// Static plugin ID - must be unique for each plugin type
static const QUuid PLUGIN_ID = QUuid("{87654321-4321-8765-4321-876543210987}");

AnotherPlugin::AnotherPlugin(const QUuid& objectId)
    : m_objectId(objectId)
    , m_pluginId(PLUGIN_ID)
{
    // Create a simple icon (colored square)
    m_icon = QImage(64, 64, QImage::Format_ARGB32);
    m_icon.fill(QColor(220, 50, 50)); // Red color
}

AnotherPlugin::~AnotherPlugin()
{
    // Cleanup if needed
}

QUuid AnotherPlugin::objectId() const
{
    return m_objectId;
}

QUuid AnotherPlugin::pluginId() const
{
    return m_pluginId;
}

QImage AnotherPlugin::icon() const
{
    return m_icon;
}

// Exported functions implementation
extern "C" {
    PLUGIN_EXPORT IPlugin* createPlugin(const QUuid& objectId)
    {
        return new AnotherPlugin(objectId);
    }
    
    PLUGIN_EXPORT const char* getPluginName()
    {
        return "Another Plugin";
    }
}
