#include <iostream>
#include <fstream>
#include <filesystem>
#include <fstream>
#include "./procmaps.hpp"


int main () {
    std::cout << "Starting parsing\n";

    std::ifstream file;
    file.open("/proc/self/maps");

    if (file.fail()) {
        std::cout << "Failed to load own map.";
        return 1;
    }

    std::string maps_text(
		(std::istreambuf_iterator<char>(file)),
		(std::istreambuf_iterator<char>())
	);
	std::cout << maps_text << "\n";


    // Parse memory maps
	ProcMap::MemMap maps = ProcMap::MemMap(maps_text);

	// Find the heap map entry
	for (ProcMap::MemEntry& entry : maps.entries) {
		std::cout << "Entry: " << entry.address_start << " [" << entry.address_size << "] '" << entry.pathname << "'\n";
	}

    file.close();
}