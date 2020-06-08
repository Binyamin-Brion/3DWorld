//
// Created by binybrion on 3/17/20.
//

#include "Logger.h"


namespace Logging
{
    Logger::Logger(const std::string &fileWritingLocation, int numberBuffers)
            :
                numberBuffers{numberBuffers},
                currentBufferIndex{0},
                fileWritingLocation{fileWritingLocation}
    {
        // Create the buffers used for storing messages for the async writing.
        for(int i = 0; i < numberBuffers; ++i)
        {
            contentBuffers.emplace_back();
        }

        fileWriter.open(fileWritingLocation, std::ofstream::trunc);

        if(!fileWriter.is_open())
        {
            throw std::runtime_error{"Unable to open " + fileWritingLocation + " for writing at: " + __PRETTY_FUNCTION__};
        }
    }

    void Logger::shutDown()
    {
        // Let the fileWriter finish writing the messages of the buffer it was assigned to first. This ensures no concurrent
        // access to the fileWriter and is logically easier to reason about.
        // Note: The result of the future is not 10 ms as it is a short amount of time- 0 ms is not used as file I/O is not
        // the fastest thing in the world.
        while(writeFuture.valid() && writeFuture.wait_for(std::chrono::milliseconds(10)) != std::future_status::ready);

        // Write the existing messages from the buffers that would have stored the messages next. This involves two loops
        // as the currentBufferIndex may not be equal to 0 when this function is called.

        for(int bufferIndex = currentBufferIndex; bufferIndex < numberBuffers; ++bufferIndex)
        {
            for(const auto &i : contentBuffers[bufferIndex])
            {
                fileWriter << i << '\n';
            }

            fileWriter.flush();
        }

        for(int bufferIndex = 0; bufferIndex < currentBufferIndex; ++bufferIndex)
        {
            for(const auto &i : contentBuffers[bufferIndex])
            {
                fileWriter << i << '\n';
            }

            fileWriter.flush();
        }
    }

    void Logger::writeMessage(const std::string &message)
    {
        // If an async operation has not been started yet, or the previous async operation has finished, then start
        // writing asynchronously, using data from a buffer that is not currently being written to.
        if(!writeFuture.valid() || (writeFuture.valid() && writeFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready))
        {
            const int WRITE_BUFFER_INDEX = currentBufferIndex;

            writeFuture = std::async(std::launch::async, [this, WRITE_BUFFER_INDEX]()
            {
                for(const auto &i : contentBuffers[WRITE_BUFFER_INDEX])
                {
                    fileWriter << i << '\n';
                }

                contentBuffers[WRITE_BUFFER_INDEX].clear();

                fileWriter.flush();
            });

            // Increment index of buffer using for storing new messages, as previous buffer is having its contents written to a file.
            currentBufferIndex = (currentBufferIndex + 1) % numberBuffers;
        }

        contentBuffers[currentBufferIndex].push_back(message);
    }
}