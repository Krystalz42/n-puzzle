#pragma once
#include <visualizer/DisplaySfml.hpp>
#include <n_puzzle.hpp>

namespace Visualizer {

	struct PuzzlePiece {
		sf::Vector2f position;
		ValuePuzzle id;
	};

	typedef Grid<PuzzlePiece> GridPuzzle;

	GridPuzzle interpolateGridPuzzle(GridPuzzle &a, GridPuzzle &b, float ratio);
	GridPuzzle gridContainerToGridPuzzle(GridContainer &grid);

}