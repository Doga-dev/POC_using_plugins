set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Sources de l'application
set(SOURCES
    src/main.cpp
    src/PluginManager.cpp
    src/PluginManager.h
    src/IPlugin.h
    gui/qml.qrc
)

# Création de l'exécutable
add_executable(${PROJECT_NAME} ${SOURCES})

# Définir le dossier de sortie
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Inclure les répertoires pour les headers
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Lier avec les bibliothèques Qt
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Quick
    Qt5::Gui
)

# Définitions de compilation
target_compile_definitions(${PROJECT_NAME} PRIVATE
    QT_DEPRECATED_WARNINGS
    QT_QML_DEBUG
    QT_DISABLE_DEPRECATED_BEFORE=0x060000
)

# Copier les DLLs Qt nécessaires pour Windows
if(WIN32)
    # Liste des DLLs Qt essentielles à copier
    set(QT_LIBS Core Gui Quick QuickControls2 QmlModels Qml Network)
    
    foreach(QT_LIB ${QT_LIBS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Qt5::${QT_LIB}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endforeach()
endif()

# Créer le répertoire plugins à côté de l'exécutable
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins
)
