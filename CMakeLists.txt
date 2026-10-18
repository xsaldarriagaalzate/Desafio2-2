cmake_minimum_required(VERSION 3.16)

project(Desafio2-2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Desafio2-2 main.cpp
    UdeABook.h
    UdeABook.cpp
    Lista.h
    Cadena.h
    Cadena.cpp
    Foto.h
    Foto.cpp
    Album.h
    Album.cpp
    Usuario.h
    Usuario.cpp
    Mensaje.h
    Mensaje.cpp
    Fecha.h
    Fecha.cpp

)

include(GNUInstallDirs)
install(TARGETS Desafio2-2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
