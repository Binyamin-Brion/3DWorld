//
// Created by binybrion on 6/12/20.
//

#ifndef GAMEPROTOTYPE_WORLDSETTINGS_H
#define GAMEPROTOTYPE_WORLDSETTINGS_H

namespace ProgramInformation
{
    class WorldSettings
    {
        public:

            static unsigned int getWorldLength();
            static unsigned int getGridSectionLength();
            static unsigned int getIndividualCubeLength();

        private:

            static unsigned int worldLength;
            static unsigned int gridSectionLength;
            static unsigned int individualCubeLength;
    };
}

#endif //GAMEPROTOTYPE_WORLDSETTINGS_H
