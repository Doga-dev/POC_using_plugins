#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QUuid>
#include <QImage>

// Interface for all plugins
class IPlugin {
public:
    virtual ~IPlugin() {}
    
    // Return the object ID that was provided during construction
    virtual QUuid objectId() const = 0;
    
    // Return the plugin ID (constant and unique for each plugin type)
    virtual QUuid pluginId() const = 0;
    
    // Return the plugin icon
    virtual QImage icon() const = 0;
};

// Plugin creation function signature
typedef IPlugin* (*CreatePluginFunc)(const QUuid& objectId);

// Plugin name function signature
typedef const char* (*GetPluginNameFunc)();

#endif // IPLUGIN_H
