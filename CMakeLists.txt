cmake_minimum_required(VERSION 3.16)

project(PluginManagerDemo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)

SET(PLUGINS_BIN_PATH ${CMAKE_BINARY_DIR}/bin/plugins)

# Créer un répertoire pour les plugins compilés
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
file(MAKE_DIRECTORY ${PLUGINS_BIN_PATH})

# Inclure les sous-répertoires
add_subdirectory(app)
add_subdirectory(plugins)
