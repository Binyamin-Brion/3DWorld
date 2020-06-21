//
// Created by binybrion on 6/20/20.
//

#ifndef GAMEPROTOTYPE_RENDERSTRUCTURE_H
#define GAMEPROTOTYPE_RENDERSTRUCTURE_H

#include "SortedModel.h"
#include "ModelRenderInformation.h"
#include "GridSectionInstanceRange.h"
#include <vec3.hpp>

namespace Render::DataStructures
{
    /**
     *  Keeps track of the number of instances for each grid section for a particular model.
     *
     *  Example usage:
     *
     *  RenderStructure renderStructure;
     *
     *  GridSectionInstanceRange gridSectionInstanceRange{0, 1}; // Grid Section 0, Instance Count 1
     *
     *  renderStructure.addModel({someLoadedModel}, {gridSectionInstanceRange});
     *
     *  An example of using this structure for rendering is in TerrainVAO.cpp .
     */

    class RenderStructure
    {
        public:

            /**
             * Initializes this structure by calculating the number of grid sections are in the world.
             */
            RenderStructure();

            /**
             * Adds information about a Model's instances within each grid section to this structure.
             *
             * @param modelRenderingInformation the loaded model
             * @param gridSectionsInstanceRange information about the number of model instances for each grid section
             */
            void addModel(const ModelRenderingInformation &modelRenderingInformation, const std::vector<GridSectionInstanceRange> &gridSectionsInstanceRange);

            /**
             * Get the stored models held within this structure.
             *
             * @return vector of added models and their instance information
             */
            [[nodiscard]] const std::vector<SortedModel>& getSortedModels() const;

        private:

            /**
             * Adds a new sorted model to this structure- called when the user calls addModel().
             *
             * @param modelRenderingInformation the loaded model to associate with the new sorted model
             */
            void addSortedModel(const ModelRenderingInformation &modelRenderingInformation);

            /**
             * Updates the instance offset for the instance information in the most recently added sorted model.
             */
            void updateInstanceOffset();

            unsigned int numberGridSections;
            unsigned int numberInstancesAlreadyLoaded = 0;
            std::vector<SortedModel> sortedModels;
    };
}

#endif //GAMEPROTOTYPE_RENDERSTRUCTURE_H
