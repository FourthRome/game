#include<Level.h>

Level::Level(const std::string& path) { // TO DO optimize read file
    std::ifstream room_file(path);
    tiles.clear(); // tiles{};
    char c;
    if (room_file.is_open()) {
        while (room_file.get(c)) {
            //chars.push_back(c);
            if (c != '\n')
                tiles.push_back(Tile(c));
        }
        std::cout << chars.size() << std::endl;
    }
    else { std::cout << "ERROR::COULDN'T OPEN FILE:" << path << std::endl; }

}