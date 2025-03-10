cmake_minimum_required(VERSION 3.14)

# Activer les fonctionnalités Qt Meta-Object
set(CMAKE_AUTOMOC ON)

# Trouver les packages Qt nécessaires
find_package(Qt5 COMPONENTS Core Gui REQUIRED)

# Inclure le répertoire principal pour accéder à IPlugin.h
include_directories(${CMAKE_SOURCE_DIR}/app/src)

# Inclure les sous-dossiers pour chaque plugin
add_subdirectory(sampleplugin)
add_subdirectory(anotherplugin)
