#ifndef ANOTHER_PLUGIN_H
#define ANOTHER_PLUGIN_H

#include "IPlugin.h"

class AnotherPlugin : public IPlugin {
public:
    AnotherPlugin(const QUuid& objectId);
    virtual ~AnotherPlugin();
    
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

#endif // ANOTHER_PLUGIN_H

