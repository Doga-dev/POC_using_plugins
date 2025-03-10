# Sources du plugin
set(SOURCES
    src/AnotherPlugin.cpp
    src/AnotherPlugin.h
)

# Vérifier que PLUGINS_BIN_PATH est défini
if(NOT DEFINED PLUGINS_BIN_PATH)
    set(PLUGINS_BIN_PATH "${CMAKE_BINARY_DIR}/plugins")
endif()

# Création de la bibliothèque partagée
add_library(anotherplugin SHARED ${SOURCES})

# Définir le dossier de sortie pour le plugin
set_target_properties(anotherplugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PLUGINS_BIN_PATH}
    RUNTIME_OUTPUT_DIRECTORY ${PLUGINS_BIN_PATH}
)

# Inclure les répertoires pour les headers
target_include_directories(anotherplugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/app/src
)

# Lier avec les bibliothèques Qt
target_link_libraries(anotherplugin PRIVATE
    Qt5::Core
    Qt5::Gui
)

# Définir la macro PLUGIN_EXPORT selon la plateforme
if(WIN32)
    target_compile_definitions(anotherplugin PRIVATE PLUGIN_EXPORT=__declspec\(dllexport\))
else()
    target_compile_definitions(anotherplugin PRIVATE PLUGIN_EXPORT=__attribute__\(\(visibility\(\"default\"\)\)\))
endif()

# Copier le plugin dans le dossier "plugins" à côté de l'exécutable
# Note: On ne fait cette copie que si le dossier de sortie des plugins est différent du dossier bin
if(NOT "${PLUGINS_BIN_PATH}" STREQUAL "${CMAKE_BINARY_DIR}/bin/plugins")
    add_custom_command(TARGET sampleplugin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/plugins"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sampleplugin>
        "${CMAKE_BINARY_DIR}/bin/plugins/"
    )
endif()
