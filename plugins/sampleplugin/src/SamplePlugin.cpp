#include "SamplePlugin.h"

// Définir la macro PLUGIN_EXPORT si ce n'est pas déjà fait
#if !defined(PLUGIN_EXPORT)
    // Define a macro for export declaration depending on platform
    #if defined(_WIN32) || defined(_WIN64)
        #define PLUGIN_EXPORT __declspec(dllexport)
    #else
        #define PLUGIN_EXPORT __attribute__((visibility("default")))
    #endif
#endif

// Static plugin ID - must be unique for each plugin type
static const QUuid PLUGIN_ID = QUuid("{12345678-1234-5678-1234-567812345678}");

SamplePlugin::SamplePlugin(const QUuid& objectId)
    : m_objectId(objectId)
    , m_pluginId(PLUGIN_ID)
{
    // Create a simple icon (colored square)
    m_icon = QImage(64, 64, QImage::Format_ARGB32);
    m_icon.fill(QColor(0, 120, 215)); // Blue color
}

SamplePlugin::~SamplePlugin()
{
    // Cleanup if needed
}

QUuid SamplePlugin::objectId() const
{
    return m_objectId;
}

QUuid SamplePlugin::pluginId() const
{
    return m_pluginId;
}

QImage SamplePlugin::icon() const
{
    return m_icon;
}

// Exported functions implementation
extern "C" {
    PLUGIN_EXPORT IPlugin* createPlugin(const QUuid& objectId)
    {
        return new SamplePlugin(objectId);
    }
    
    PLUGIN_EXPORT const char* getPluginName()
    {
        return "Sample Plugin";
    }
}
