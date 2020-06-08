//
// Created by binybrion on 3/17/20.
//

#ifndef GLFW_LOGGERPIMPL_H
#define GLFW_LOGGERPIMPL_H

#include <string>

namespace Logging
{
    class Logger;

    /**
     *  Pointer to Implementation of the Logger class. Due to heavy includes in the Logger class, this class should
     *  help reduce compile times. See the Logger class for specific functionality.
     */

    class LoggerPIMPL
    {
        public:

            /**
             * Creates an instance of the Logger class using the provides information. Arguments forwarded to the Logger class.
             *
             * @param fileWritingLocation location of file to be written to. If file already exists, contents will be overwritten
             * @param numberBuffers that should be used for the asynchronous logging
             */
            LoggerPIMPL(const std::string &writeLocation, int numberBuffers = 2);

            /**
             * Writes any messages in the buffers that have not yet been written. This is a BLOCKING operation.
              */
            void shutDown();

            /**
             * Forwards write message to the Logger class.
             *
             * @param message to be written to the file specified in the constructor
             */
            void writeMessage(const std::string &message);

            /**
             * Deallocates the Logger class from main memory. This is a BLOCKING operation as any unwritten messages
             * will be written to the file specified in the constructor. Function shutDown() is called.
             */
            ~LoggerPIMPL();

        private:
            Logger *logger = nullptr;
    };
}


#endif //GLFW_LOGGERPIMPL_H
