#pragma once
#include <visualizer/DisplaySfml.hpp>
//#include <PriorityQueueIter.hpp>
#include <n_puzzle.hpp>

constexpr unsigned int pixelBorder = 3;

struct dataVisualizer {
	sf::Vector2f position;
	ValuePuzzle id;
};

typedef Grid<dataVisualizer> GridVisualizer;

GridVisualizer interpolateGridVisualizer(GridVisualizer &a, GridVisualizer &b, float ratio);
GridVisualizer gridContainerToGridVisualizer(GridContainer &grid);
void updateRenderTextureFromgridContainer(sf::RenderTexture &render, GridContainer &a, GridContainer &b, float ratio, sf::Texture const &texture);
