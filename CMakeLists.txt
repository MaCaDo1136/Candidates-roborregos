# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.13)

# Nombre del proyecto
project(candidates-roborregos C CXX ASM)

# Ruta del SDK
set(PICO_SDK_PATH "/home/mario/pico/pico-sdk")

# Incluir el SDK
include(pico_sdk_import.cmake)

# Inicializar el SDK
pico_sdk_init()

# Añadir tu ejecutable
add_executable(${PROJECT_NAME}
    main.cpp
)

# Incluir librerías estándar de Pico
target_link_libraries(${PROJECT_NAME} pico_stdlib)

# Habilitar UART para 'printf'
pico_enable_stdio_uart(${PROJECT_NAME} 1)

# Habilitar USB si quieres
# pico_enable_stdio_usb(${PROJECT_NAME} 1)

# Crear archivo .uf2
pico_add_extra_outputs(${PROJECT_NAME})

