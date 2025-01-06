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

// @brief Loads/deletes specified character name
CharacterStats load_character(const bool delete_character, const std::string name_specified){
	// Get character data line
	std::ifstream file(character_data_path);
	std::string line;
	while (std::getline(file, line)){
		try{
			int delimiter_spot = line.find_first_of(";");
			std::string character_name = line.substr(0, delimiter_spot);
			if (character_name == name_specified){
				break;
			}
		} catch (...){
			file.close();
			std::cerr << "ERROR. No Character of name '" << name_specified << "' found." << std::endl;
			exit(EXIT_FAILURE);
		}
	} file.close();

	if (line.find_first_of(";") == std::string::npos || line.substr(0, line.find_first_of(";")) != name_specified){
		std::cerr << "ERROR. No Character of name '" << name_specified << "' found." << std::endl;
		exit(EXIT_FAILURE);
	}

	// If deleting character, remake file without deleted line
	if (delete_character){
		std::string new_file;
		std::string line2;
		std::ifstream file(character_data_path);
		while (std::getline(file, line2)){
			if (line != line2){
				new_file += line2 + "\n";
			}
		} file.close();
		std::ofstream outfile(character_data_path, std::ios::out);
		outfile << new_file;
        outfile.close();

		std::cout << "Deleted character " << name_specified; // REMOVE

		exit(EXIT_SUCCESS);
	}	std::cout << "Character found, here is data: " << line;//REMOVE

	// Use current character data line to fill stats
	CharacterStats stats;

	return stats;
}

// @brief Opens load TUI to get character data from a file
CharacterStats load_character(){
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
			std::cout << "\n  -d, --delete NAME\tdelete specified character from save data";
			std::cout << "\n  -l, --load NAME\tskip choosing screen and load specified character";

			exit(EXIT_SUCCESS);
		} else if (option == "-c" || option == "--Create"){
			save_new_character();
		} else if (option == "-l" || option == "-d" || option == "--load" || option == "--delete"){
			bool delete_character = false;
			if (option == "-d" || option == "--delete"){delete_character = true;}
			if (argc > 2){
				std::string name_specified = argv[2];
				for (int i = 3; i < argc; i++){
					std::string rest_of_name = argv[i];
					name_specified += " " + rest_of_name;
				}
				load_character(delete_character, name_specified);
				
				exit(EXIT_SUCCESS);
			} else {
				std::cerr << "No load character NAME given";
				exit(EXIT_FAILURE);
			}
		}
	}
	
	// Get Character Stats
		// Health, AC, Speed
	CharacterStats stats = load_character();

	return EXIT_SUCCESS;
}
