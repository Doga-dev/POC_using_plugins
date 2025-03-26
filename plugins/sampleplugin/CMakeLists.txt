# Sources du plugin
set(SOURCES
    ${PUBLIC_API_PATH}/IPlugin.h

    src/SamplePlugin.cpp
    src/SamplePlugin.h
)

# Création de la bibliothèque partagée
add_library(sampleplugin SHARED ${SOURCES})

# Définir le dossier de sortie pour le plugin
set_target_properties(sampleplugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PLUGINS_BIN_PATH}
    RUNTIME_OUTPUT_DIRECTORY ${PLUGINS_BIN_PATH}
)

# Inclure les répertoires pour les headers
target_include_directories(sampleplugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${PUBLIC_API_PATH}
)

# Lier avec les bibliothèques Qt
target_link_libraries(sampleplugin PRIVATE
    Qt5::Core
    Qt5::Gui
)

# Définir la macro PLUGIN_EXPORT selon la plateforme
if(WIN32)
    target_compile_definitions(sampleplugin PRIVATE PLUGIN_EXPORT=__declspec\(dllexport\))
else()
    target_compile_definitions(sampleplugin PRIVATE PLUGIN_EXPORT=__attribute__\(\(visibility\(\"default\"\)\)\))
endif()

# Copier le plugin dans le dossier "plugins" à côté de l'exécutable
add_custom_command(TARGET sampleplugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sampleplugin>
    ${PLUGINS_BIN_PATH}/
)
