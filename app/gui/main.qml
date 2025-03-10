import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

Window {
    id: window
    visible: true
    width: 800
    height: 600
    title: qsTr("Plugin Manager Demo")

    // Identifiant pour l'application
    // (Résout l'erreur QML Settings: The following application identifiers have not been set)
    Component.onCompleted: {
        if (Qt.application.organization === "" && Qt.application.name === "") {
            Qt.application.organization = "DOGA"
            Qt.application.name = "PluginManager Demo"
        }
    }

    property string selectedInstanceId: ""
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20
        
        // Plugins section
        GroupBox {
            title: "Available Plugins"
            Layout.fillWidth: true
            
            RowLayout {
                anchors.fill: parent
                
                ComboBox {
                    id: pluginComboBox
                    Layout.fillWidth: true
                    model: pluginManager.availablePlugins
                    textRole: "name"
                    
                    // Empty model placeholder text
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        visible: pluginComboBox.count === 0
                        text: "No plugins available"
                        color: "gray"
                    }
                }
                
                Button {
                    text: "Create Instance"
                    enabled: pluginComboBox.count > 0
                    onClicked: {
                        if (pluginComboBox.currentIndex >= 0) {
                            var pluginName = pluginComboBox.model[pluginComboBox.currentIndex].name;
                            pluginManager.createPluginInstance(pluginName);
                        }
                    }
                }
                
                Button {
                    text: "Scan Directory"
                    onClicked: {
                        folderDialog.open();
                    }
                }
            }
        }
        
        // Plugin instances section
        GroupBox {
            title: "Plugin Instances"
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            ColumnLayout {
                anchors.fill: parent
                spacing: 10
                
                // Instances list
                ListView {
                    id: instancesList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: pluginManager.pluginInstances
                    clip: true
                    
                    // Empty model placeholder text
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        visible: instancesList.count === 0
                        text: "No plugin instances"
                        color: "gray"
                    }
                    
                    delegate: Rectangle {
                        width: instancesList.width
                        height: 80
                        color: selectedInstanceId === model.objectId ? "#e0e0e0" : "transparent"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                selectedInstanceId = modelData.objectId;
                            }
                        }
                        
                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 15

                            // Plugin icon avec gestion des erreurs
                            Image {
                                source: modelData.icon || ""
                                Layout.preferredWidth: 64
                                Layout.preferredHeight: 64
                                fillMode: Image.PreserveAspectFit

                                // Rectangle placeholder affiché en cas d'erreur d'image
                                Rectangle {
                                    color: "#cccccc"
                                    visible: parent.status !== Image.Ready
                                    anchors.fill: parent

                                    Text {
                                        anchors.centerIn: parent
                                        text: "No\nIcon"
                                        horizontalAlignment: Text.AlignHCenter
                                        color: "#666666"
                                    }
                                }
                            }
                            
                            // Plugin details
                            ColumnLayout {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                spacing: 5
                                
                                Text {
                                    text: "Name: " + (modelData.name || "undefined")
                                    font.bold: true
                                }
                                
                                Text {
                                    text: "Object ID: " + (modelData.objectId || "undefined")
                                    font.family: "Courier"
                                    elide: Text.ElideMiddle
                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }
                }
                
                // Instance actions
                Button {
                    text: "Destroy Selected Instance"
                    enabled: selectedInstanceId !== ""
                    onClicked: {
                        pluginManager.destroyPluginInstance(selectedInstanceId);
                        selectedInstanceId = "";
                    }
                }
            }
        }
    }
    
    // Folder selection dialog
    FileDialog {
        id: folderDialog
        title: "Select Plugins Directory"
        folder: shortcuts.home
        selectFolder: true
        
        onAccepted: {
            // Convert URL to local path and scan the directory
            var path = folder.toString();
            path = path.replace(/^(file:\/{3})/, "");
            
            // Handle Windows paths
            if (Qt.platform.os === "windows") {
                path = path.replace(/^\//, "");
            }
            
            pluginManager.scanDirectory(path);
        }
    }
}
