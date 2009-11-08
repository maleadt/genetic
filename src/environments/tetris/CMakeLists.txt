#
# Build libraries
#

# Input handler
ADD_LIBRARY(tetris_input input.h input.cpp)

# Output handler
ADD_LIBRARY(tetris_output output.h output.cpp)
FIND_PACKAGE(SDL REQUIRED)
TARGET_LINK_LIBRARIES(tetris_output ${SDL_LIBRARY})
TARGET_LINK_LIBRARIES(tetris_output "SDL_gfx")

# Pieces
ADD_LIBRARY(tetris_pieces pieces.h pieces.cpp)

# Board
ADD_LIBRARY(tetris_board board.h board.cpp)
TARGET_LINK_LIBRARIES(tetris_board tetris_pieces)

# Game
ADD_LIBRARY(tetris_game game.h game.cpp)
TARGET_LINK_LIBRARIES(tetris_game tetris_output)
TARGET_LINK_LIBRARIES(tetris_game tetris_pieces)
TARGET_LINK_LIBRARIES(tetris_game tetris_board)


#
# Build sample executable
#

# Executable
ADD_EXECUTABLE(tetris_sample main.cpp)
TARGET_LINK_LIBRARIES(tetris_sample tetris_input)
TARGET_LINK_LIBRARIES(tetris_sample tetris_output)
TARGET_LINK_LIBRARIES(tetris_sample tetris_pieces)
TARGET_LINK_LIBRARIES(tetris_sample tetris_board)
TARGET_LINK_LIBRARIES(tetris_sample tetris_game)

