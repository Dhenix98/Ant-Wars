#pragma once

#ifdef __linux__
#include <SDL2/SDL.h>
#elif _WIN32
#include <SDL.h>
#endif

#include "Exceptions.h"


enum class EColor { RED_HIVE, BLUE_HIVE, GREEN_HIVE, GRAY_HIVE, BLACK, WHITE, GRAY, GREEN_DIFFICULTY, ORANGE_DIFFICULTY, RED_DIFFICULTY };

/// @brief convert to string from EColor
inline const char *toCharArray(EColor color) {
    switch (color) {
        case EColor::RED_HIVE: return "Red";
        case EColor::BLUE_HIVE: return "Blue";
        case EColor::GREEN_HIVE: return "Green";
        case EColor::GRAY_HIVE: return "Gray";
        default: throw NotImplementedException("This color is not implemented in \"toCharArray(EColor color)\"");
    }
}

/// @brief convert to EColor from string
inline EColor toColor(std::string color) {
    if (color == "Red") {
        return EColor::RED_HIVE;
    }
    if (color == "Blue") {
        return EColor::BLUE_HIVE;
    }
    if (color == "Green") {
        return EColor::GREEN_HIVE;
    }
    if (color == "Gray") {
        return EColor::GRAY_HIVE;
    }

    throw NotImplementedException("This string is not implemented in \"toColor(std::string color)\"");
}

/// @brief converts EColor to SDL_Color
inline SDL_Color toSDLColor(EColor color) {
    switch (color) {
        case EColor::RED_HIVE: {
            return { 204, 0, 31, 255 };
        }
        case EColor::BLUE_HIVE: {
            return { 63, 72, 204, 255 };
        }
        case EColor::GREEN_HIVE: {
            return { 26, 134, 58, 255 };
        }
        case EColor::GRAY_HIVE: {
            return { 127, 127, 127, 255 };
        }
        case EColor::BLACK: {
            return { 0, 0, 0, 255 };
        }
        case EColor::WHITE: {
            return { 255, 255, 255, 255 };
        }
        case EColor::GRAY: {
            return { 127, 127, 127, 255 };
        }
        case EColor::GREEN_DIFFICULTY: {
            return { 0, 255, 0, 255 };
        }
        case EColor::ORANGE_DIFFICULTY: {
            return { 255, 127, 0, 255 };
        }
        case EColor::RED_DIFFICULTY: {
            return { 255, 0, 0, 255 };
        }
    }
    throw NotImplementedException("This color is not implemented in \"toSDLColor(EColor color)\"");
}
