//
// Created by binybrion on 3/17/20.
//

#include "LoggerPIMPL.h"
#include "Logger.h"

namespace Logging
{
    LoggerPIMPL::LoggerPIMPL(const std::string &writeLocation, int numberBuffers )
                    :
                        logger{new Logger{writeLocation, numberBuffers}}
    {

    }

    void LoggerPIMPL::shutDown()
    {
        logger->shutDown();
    }

    void LoggerPIMPL::writeMessage(const std::string &message)
    {
        logger->writeMessage(message);
    }

    LoggerPIMPL::~LoggerPIMPL()
    {
        logger->shutDown();

        delete logger;
    }
}
