# checkers_ncurses

you can compile with this command in src directory

// clang++ -pthread -lncurses *.cpp -o testclang++ -pthread -lncurses *.cpp -o test // compile command

or

// g++ -pthread -lncurses *.cpp -o testclang++ -pthread -lncurses *.cpp -o test // compile command

but if you want compile with cmake you need find your ncurses library path and replace it in CMakeLists.txt.
after that save file and enter command ( cmake CMakeLists.txt ), go to build directory and enter ( make ) command.

the game designed for xterm terminal maybe in other terminal can act differnt so i suggest to install and run it in xterm.

for better experince you can run (./run.sh) it is bash script file for set xterm font size and make it bigger
