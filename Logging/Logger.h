//
// Created by binybrion on 3/17/20.
//

#ifndef GLFW_LOGGER_H
#define GLFW_LOGGER_H

#include <vector>
#include <string>
#include <future>
#include <fstream>

namespace Logging
{
    /**
     *  Writes messages to a file in an asynchronous manner. Used to keep track of any important piece of information
     *  that is created over the course of the program's life.
     */

    class Logger
    {
        public:

            /**
             * Creates a file to be written to, and as well as the buffers and other details used for asynchronous writing.
             *
             * @param fileWritingLocation location of file to be written to. If file already exists, contents will be overwritten
             * @param numberBuffers that should be used for the asynchronous logging
             */
            Logger(const std::string &fileWritingLocation, int numberBuffers);

            /**
             *  Writes any messages in the buffers that have not yet been written. This is a BLOCKING operation.
             */
            void shutDown();

            /**
             * Writes the message to the opened file. Note that due to the asynchronous manner, this may not happen
             * immediately.
             *
             * @param message to be written to the file
             */
            void writeMessage(const std::string &message);

        private:
            const int numberBuffers;
            int currentBufferIndex;
            std::vector<std::vector<std::string>> contentBuffers;
            std::ofstream fileWriter;
            std::future<void> writeFuture;
            std::string fileWritingLocation;

    };
}

#endif //GLFW_LOGGER_H
