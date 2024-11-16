cmake_minimum_required(VERSION 3.10)
project(st25vf010_app)

# Detectar la arquitectura
if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(ARCH "aarch64")
    set(APP bin/st25vf010_app_aarch64)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "armv7l")
    set(ARCH "armv7l")
    set(APP bin/st25vf010_app_armv7l)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    set(ARCH "x86_64")
    set(APP bin/st25vf010_app_x86_64)
else()
    message(FATAL_ERROR "Arquitectura desconocida")
endif()

# Especificar los archivos fuente
set(SOURCES
    src/main.cpp
    src/*/*.cpp
)

# Configuración de compilador
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

# Añadir bibliotecas y directorios
include_directories(${PROJECT_SOURCE_DIR}/include)

# Crear el ejecutable
add_executable(${PROJECT_NAME} ${SOURCES})

# Enlazar bibliotecas si es necesario
target_link_libraries(${PROJECT_NAME} pthread bcm2835)

# Definir reglas de limpieza
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/obj
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/log
)

