#include "glad.h"
#include "Render/CommandCentre.h"
#include "Window/RenderWindow.h"
#include "World/WorldLogic/WorldGenerator.h"

int main()
{

    Window::RenderWindow renderWindow{"Minecraft", 4, 3, 1280, 720};

    Render::CommandCentre commandCentre;

    World::WorldLogic::WorldGenerator worldGenerator;

    worldGenerator.createWorld();

    while (!renderWindow.windowShouldClose())
    {
        renderWindow.processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        commandCentre.render(renderWindow.getCamera());

        renderWindow.swapBuffers();
        renderWindow.pollEvents();
    }

    renderWindow.terminate();
    return 0;
}