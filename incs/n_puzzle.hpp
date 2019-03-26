#ifndef N_PUZZLE_N_PUZZLE_HPP
#define N_PUZZLE_N_PUZZLE_HPP

#include <Grid.hpp>

typedef uint16_t ValuePuzzle;

typedef std::vector<std::vector<ValuePuzzle >> RawArray;

typedef Grid<ValuePuzzle> GridContainer;


#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RST "\x1B[0m"

#endif //N_PUZZLE_N_PUZZLE_HPP
