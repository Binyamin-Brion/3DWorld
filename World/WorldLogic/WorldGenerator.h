//
// Created by binybrion on 6/12/20.
//

#ifndef GAMEPROTOTYPE_WORLDGENERATOR_H
#define GAMEPROTOTYPE_WORLDGENERATOR_H

#include "GridSection.h"

namespace World::WorldLogic
{
    class WorldGenerator
    {
        public:

            void createWorld();
            const std::vector<GridSection>& getTerrainData() const;

        private:

            void fillGridSectionSurfaceCubes(GridSection &gridSection);

            std::vector<GridSection> gridSections;
    };
}

#endif //GAMEPROTOTYPE_WORLDGENERATOR_H
