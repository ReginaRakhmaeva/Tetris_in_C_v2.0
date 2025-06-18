/**
 * @file main.h
 * @brief Заголовочный файл с основными функциями для инициализации и управления
 * игрой.
 */

#ifndef MAIN_H
#define MAIN_H

#if !defined(LIBS_GAMES)
#if defined(_WIN32)
#define LIBS_GAMES "libtetris_liliammo.dll"
#else
#define LIBS_GAMES "libtetris_liliammo.so"
#endif
#endif

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#endif  // MAIN_H
