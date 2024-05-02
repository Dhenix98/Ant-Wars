#pragma once
#include "Exceptions.h"


enum class EDifficulty {
    EASY = 0,
    MEDIUM = 1,
    HARD = 2
};

/// @return string form of difficulty
inline const char *toCharArray(EDifficulty difficulty) {
    switch (difficulty) {
        case EDifficulty::EASY: return "Easy";
        case EDifficulty::MEDIUM: return "Medium";
        case EDifficulty::HARD: return "Hard";
        default: throw NotImplementedException("This difficulty is not implemented in \"toCharArray(EDifficulty difficulty)\"");
    }
}

/// @return difficulty from string
inline EDifficulty toDifficulty(std::string difficulty) {
    if (difficulty == "Easy") {
        return EDifficulty::EASY;
    }
    if (difficulty == "Medium") {
        return EDifficulty::MEDIUM;
    }
    if (difficulty == "Hard") {
        return EDifficulty::HARD;
    }
    throw NotImplementedException("This string is not implemented in \"toDifficulty(std::string difficulty)\"");
}
