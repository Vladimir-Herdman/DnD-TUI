#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <fstream> // for creating character data file
#include <filesystem> // for writing/reading character data between uses
#include <iostream> // for cout with cli section
#include <cstdlib> // for exit

namespace fs = std::filesystem;


// Globals
const fs::path character_data_path = fs::current_path() / "CHARACTERS.txt";

// User-defined section
struct CharacterStats {
	int health;
	int ac;
	int speed;
};

// @brief Checks for character file path, creates it if not present
void check_character_path(){
	if (!fs::exists(character_data_path)){
		std::ofstream file(character_data_path);
		if (file.is_open()){
			file.close();
		}
	}
}

// @brief Save new character data to file CHARACTERS.txt
void save_new_character(){

	exit(EXIT_SUCCESS);
}

// @brief Opens load TUI to get character data from a file
CharacterStats load_character(const bool del){
	// Get characters to choose from
	std::vector<std::string> characters;

	std::ifstream file(character_data_path);
	std::string line;
	while (std::getline(file, line)){
		int delimiter_spot = line.find_first_of(";");
		characters.push_back(line.substr(0, delimiter_spot));
	} file.close();
	
	CharacterStats stats;

	return stats;
}

int main(const int argc, const char* argv[]){
	// make sure character file exists
	check_character_path();

	// Command Line Arguments
	if (argc > 1){
		std::string option = argv[1];
		if (option == "-h" || option == "--help") {
			std::string file_name = argv[0];
			if (file_name[1] == '/'){file_name = file_name.substr(2);} // Cleaner macOS name

			std::cout << "usage: " << file_name << " [-h] [-c] [-d] [-l NAME]";
			std::cout << "\n\nDescripion section";
			std::cout << "\n\noptions:";
			std::cout << "\n  -h, --help\t\tshow help message";
			std::cout << "\n  -c, --create\t\tcreate a new character";
			std::cout << "\n  -d, --delete NAME\tdelete specified character from save data, no NAME for interactive";
			std::cout << "\n  -l, --load NAME\tskip choosing screen and load specified character";

			exit(EXIT_SUCCESS);
		} else if (option == "-c" || option == "--Create"){
			save_new_character();
		} else if (option == "-d" || option == "--delete"){
			load_character(true);
		} else if (option == "-l" || option == "--load"){
			if (argc > 2){
				std::string name_to_delete = argv[2];
				for (int i = 3; i < argc; i++){
					std::string rest_of_name = argv[i];
					name_to_delete += " " + rest_of_name;
				}
				
				exit(EXIT_SUCCESS);
			} else {
				std::cerr << "No load character NAME given";
				exit(EXIT_FAILURE);
			}
		}
	}
	
	// Get Character Stats
		// Health, AC, Speed
	CharacterStats stats = load_character(false);

	return EXIT_SUCCESS;
}
