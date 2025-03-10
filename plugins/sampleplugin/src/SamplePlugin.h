#ifndef SAMPLE_PLUGIN_H
#define SAMPLE_PLUGIN_H

#include "IPlugin.h"

// Définir la macro PLUGIN_EXPORT si ce n'est pas déjà fait
#if !defined(PLUGIN_EXPORT)
    #if defined(_WIN32) || defined(_WIN64)
        #define PLUGIN_EXPORT __declspec(dllexport)
    #else
        #define PLUGIN_EXPORT __attribute__((visibility("default")))
    #endif
#endif

class SamplePlugin : public IPlugin {
public:
    SamplePlugin(const QUuid& objectId);
    virtual ~SamplePlugin();
    
    // IPlugin interface implementation
    QUuid objectId() const override;
    QUuid pluginId() const override;
    QImage icon() const override;
    
private:
    QUuid m_objectId;
    QUuid m_pluginId;
    QImage m_icon;
};

// Export functions that will be used by the application
extern "C" {
    // Function to create a new plugin instance
    PLUGIN_EXPORT IPlugin* createPlugin(const QUuid& objectId);
    
    // Function to get the plugin name
    PLUGIN_EXPORT const char* getPluginName();
}

#endif // SAMPLE_PLUGIN_H

