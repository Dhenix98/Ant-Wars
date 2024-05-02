#pragma once
#include <exception>
#include <stdexcept>
#include <string>

/// @brief throw exception when application couldn't be initialized
struct AppInitException : public std::exception
{
    AppInitException(std::string msg = "Application couldn't be initialized!") : m_Msg(msg) {};
    const char *what() const throw ()
    {
        return m_Msg.c_str();
    }
    std::string m_Msg;
};

/// @brief throw exception when the searched file doesn't exist
struct NonExistingFileException : public std::exception {
    NonExistingFileException(const char *msg = "File does not exist!") : m_Msg(msg) {};
    const char *what() const throw ()
    {
        return m_Msg.c_str();
    }
    std::string m_Msg;
};

/// @brief throw exception when this branch of the methods hasn't been implemented
struct NotImplementedException : public std::logic_error {
    NotImplementedException(const char *msg = "Function/Method not yet implemented") : std::logic_error(msg), m_Msg(msg) {};
    const char *what() const throw ()
    {
        return m_Msg.c_str();
    }
    std::string m_Msg;
};

/// @brief throw exception when this file is corrupted
struct CorruptedFileException : public std::runtime_error {
    CorruptedFileException(const char *msg = "This file is corrupted") : std::runtime_error(msg), m_Msg(msg) {};
    const char *what() const throw ()
    {
        return m_Msg.c_str();
    }
    std::string m_Msg;
};
