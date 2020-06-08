#include "glad.h"
#include "Render/CommandCentre.h"
#include "Window/RenderWindow.h"

int main()
{

    Window::RenderWindow renderWindow{"Minecraft", 4, 3, 1280, 720};

    Render::CommandCentre commandCentre;

    while (!renderWindow.windowShouldClose())
    {
        renderWindow.processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        commandCentre.render();

        renderWindow.swapBuffers();
        renderWindow.pollEvents();
    }

    renderWindow.terminate();
    return 0;
}