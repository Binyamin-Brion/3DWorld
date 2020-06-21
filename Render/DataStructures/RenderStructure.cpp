//
// Created by binybrion on 6/20/20.
//

#include "RenderStructure.h"
#include "../../ProgramInformation/WorldSettings.h"

namespace Render::DataStructures
{
    RenderStructure::RenderStructure()
    {
        unsigned int gridSectionsPerWorldLength = ProgramInformation::WorldSettings::getWorldLength() / ProgramInformation::WorldSettings::getGridSectionLength();

        numberGridSections = gridSectionsPerWorldLength * gridSectionsPerWorldLength;
    }

    void RenderStructure::addModel(const ModelRenderingInformation &modelRenderingInformation, const std::vector<GridSectionInstanceRange> &gridSectionsInstanceRange)
    {
        addSortedModel(modelRenderingInformation);

        for(const auto &i : gridSectionsInstanceRange)
        {
            sortedModels.back().gridSectionRenderData[i.gridSectionID].instanceCount += i.instanceCount;
        }

        // Right now all instance offsets for all of the grid section for the most recently added sorted model is the same as
        // the first grid section. This isn't correct as grid sections for a particular grid section may contain instances,
        // which mean subsequent grid sections will have a different offset.
        updateInstanceOffset();
    }

    const std::vector<SortedModel> &RenderStructure::getSortedModels() const
    {
        return sortedModels;
    }

    // Beginning of private functions

    void RenderStructure::addSortedModel(const ModelRenderingInformation &modelRenderingInformation)
    {
        SortedModel sortedMesh;

        sortedMesh.modelRenderingInformation = modelRenderingInformation;

        // The sorted mesh stores information about all grid sections, even if there are no instances to be held in them.
        // Since their instance count will remain at zero, they do not affect the rendering.
        sortedMesh.gridSectionRenderData.insert(sortedMesh.gridSectionRenderData.end(), numberGridSections, GridSectionRenderData{numberInstancesAlreadyLoaded, 0});

        sortedModels.push_back(sortedMesh);
    }

    void RenderStructure::updateInstanceOffset()
    {
        // The number of instances of all the grid sections iterated through so far.
        unsigned int instancesCurrentSortedModel = 0;

        for(auto &i : sortedModels.back().gridSectionRenderData)
        {
            i.instanceOffset += instancesCurrentSortedModel;

            instancesCurrentSortedModel += i.instanceCount;
        }

        // Update this variable so that the next added model does not require looping through all of the grid sections
        // for all of the added models to calculate the initial offset.
        numberInstancesAlreadyLoaded += instancesCurrentSortedModel;
    }
}