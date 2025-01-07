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
const fs::path current_dir = fs::current_path();

// User-defined data-types
struct CharacterStats {
	std::string* character_line;
	std::string* stats_line;
	std::string* equipment_line;
	std::string* combat_line;
	std::string* spells_line;
	std::string* rolls_line;
};

// @brief Checks for character file path, creates it if not present
void check_path(const std::string file_name){
	if (!fs::exists(current_dir / file_name)){
		std::ofstream file(current_dir / file_name);
		if (file.is_open()){
			file.close();
		}
	}
}

// @brief Creates an in-terminal menu interactive selection area
std::string make_selection_menu(const std::vector<std::string>& characters){
	using namespace ftxui;
	auto screen = ScreenInteractive::TerminalOutput();
	int selected = 0;

	MenuOption option;
	option.on_enter = screen.ExitLoopClosure();
	auto menu = Menu(&characters, &selected, option);
	menu |= CatchEvent([&](Event event) {
		if (event == Event::Character('q') || event == Event::Escape) {
			screen.ExitLoopClosure()(); // Second set of parenthesis to invoke closure function returned by ExitLoopClosure
			return true;
		}
		return false;
	});
	menu |= bold;

	std::cout << "\nSelect Character (enter to choose selected):\n";
	screen.Loop(menu);

	const std::string name = characters[selected];
	return name.substr(0, name.find(':'));
}

// @brief Undo the last delete action (could be improved with keeping all deletes
// until a new character is made, then wiping file, like a stack)
void undo(){
	const fs::path character_data_path = current_dir / "CHARACTERS.txt";
	const fs::path undo_data_path = current_dir / "UNDO.txt";

	std::ifstream undo_file(undo_data_path);
	std::string deleted_character_data;
	std::getline(undo_file, deleted_character_data); undo_file.close();
	const int removed_location = std::stoi(deleted_character_data.substr(0, deleted_character_data.find(';')));
	deleted_character_data = deleted_character_data.substr(deleted_character_data.find(';')+1);

	std::string new_file;
	std::string line;
	int count = -1;
	std::ifstream character_file(character_data_path);
	while (std::getline(character_file, line)){
		++count;
		if (count == removed_location){
			new_file += deleted_character_data + '\n';
		}
		new_file += line + '\n';
	}
	if (count < removed_location){
		new_file += deleted_character_data + '\n';
	} character_file.close();
	
	std::ofstream outfile_character(character_data_path, std::ios::out);
	outfile_character << new_file;
	outfile_character.close();

	std::ofstream outfile_undo(undo_data_path);
	outfile_undo.close();

	exit(EXIT_SUCCESS);
}

// @brief Save new character data to file CHARACTERS.txt
void save_new_character(){
	using namespace ftxui;

    auto screen = ScreenInteractive::Fullscreen();

	// ------------------------------------------------------------------------
	// Character
	// ------------------------------------------------------------------------
        std::string name = "John";
        std::string class_level = "Fighter 3";
        std::string archetype = "Rune Knight";
        std::string race = "Human";
        std::string background = "Hobo";
        std::string alignment = "Chaotic Neutral";
        auto character = Renderer([&]{
            return vbox({
                hbox({
                    window(text("Info"), vbox({
                        flexbox({
                            vbox({
                                hbox({text("Name: ") | bold, text(name)}),
                                hbox({text("Race: ") | bold, text(race)})
                            }),
                            filler(),
                            vbox({
                                hbox({text("Class: ") | bold, text(class_level)}),
                                hbox({text("Alignment: ") | bold, text(alignment)})
                            }),
                            filler(),
                            vbox({
                                hbox({text("Archetype: ") | bold, text(archetype)}),
                                hbox({text("Background: ") | bold, text(background)})
                            })
                        })
                    })) | flex
                }),
                hbox({
                    window(text("Personal Traits"), vbox({
                        hbox({text("Personality: ") | bold, paragraph("Very happy man")}),
                        separatorCharacter(" - "),
                        hbox({text("Ideals: ") | bold, paragraph("Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money Money")}),
                        separatorCharacter(" - "),
                        hbox({text("Bonds: ") | bold, paragraph("Every man")}),
                        separatorCharacter(" - "),
                        hbox({text("Flaws: ") | bold, paragraph("Too many huz")})
                    })) | flex
                })
            });
        });

	// ------------------------------------------------------------------------
	// Stats
	// ------------------------------------------------------------------------
        auto stats = Renderer([&]{
            return vbox({
                hbox({
                    window(text("Stats"), text("Stats data") | bold)
                })
            });
        });

	// ------------------------------------------------------------------------
	// Equipment
	// ------------------------------------------------------------------------
        auto equipment = Renderer([&]{
            return vbox({
                hbox({
                    window(text("Equipment"), text("Equipment data") | bold)
                })
            });
        });

	// ------------------------------------------------------------------------
	// Combat
	// ------------------------------------------------------------------------
        auto combat = Renderer([&]{
            return vbox({
                hbox({
                    window(text("Combat"), text("Combat data") | bold)
                })
            });
        });

	// ------------------------------------------------------------------------
	// Spells
	// ------------------------------------------------------------------------
        auto spells = Renderer([&]{
            return vbox({
                hbox({
                    window(text("Spells"), text("Spells data") | bold)
                })
            });
        });

	// ------------------------------------------------------------------------
	// Rolls
	// ------------------------------------------------------------------------
        auto rolls = Renderer([&]{
            return vbox({
                hbox({
                    window(text("Rolls"), text("Rolls data") | bold)
                })
            });
        });
    
    // ------------------------------------------------------------------------
	// Tabs
	// ------------------------------------------------------------------------

    int tab_index = 0;
	std::vector<std::string> tab_entries = {
		"Character", "Stats", "Equipment", "Combat", "Spells", "Rolls"
	};
	auto tab_selection = Menu(&tab_entries, &tab_index, MenuOption::HorizontalAnimated());
    auto tab_content = Container::Tab({
        character,
        stats,
        equipment, 
        combat, 
        spells, 
        rolls
    },
    &tab_index);

    auto exit_button = Button("Exit", [&]{screen.Exit();}, ButtonOption::Animated());

    auto main_container = Container::Vertical({
        Container::Horizontal({
            tab_selection,
            exit_button
        }),
        tab_content
    });
    auto main_renderer = Renderer(main_container, [&]{
        return vbox({
            text("DnD 5e Character Sheet TUI") | bold | hcenter,
            hbox({
                tab_selection->Render() | flex,
                exit_button->Render()
            }),
            tab_content->Render() | flex
        });
    });

    screen.Loop(main_renderer);

	exit(EXIT_SUCCESS);
}

// @brief Loads/deletes specified character name
CharacterStats load_character(const bool delete_character, const std::string& name_specified){
	// Get character data line
	const fs::path character_data_path = current_dir / "CHARACTERS.txt";
	const fs::path undo_data_path = current_dir / "UNDO.txt";
	std::ifstream file(character_data_path);
	std::string line;
	int count = -1;
	while (std::getline(file, line)){
		++count;
		try{
			int delimiter_spot = line.find(';');
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

	if (line.find(';') == std::string::npos || line.substr(0, line.find(';')) != name_specified){
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
				new_file += line2 + '\n';
			}
			if (line == line2){
				std::ofstream outfile(undo_data_path, std::ios::out);
				outfile << (std::to_string(count) + ';' + line);
				outfile.close();
			}
		} file.close();
		std::ofstream outfile(character_data_path, std::ios::out);
		outfile << new_file;
        outfile.close();

		CharacterStats stats;
		return stats;
	}

	// Use current character data line to fill stats
	CharacterStats stats;

	return stats;
}

// @brief Opens load TUI to get character data from a file, or shows characters
// that can be choosen
CharacterStats load_character(const bool list_characters){
	// Get characters to choose from
	std::vector<std::string> characters;

	std::ifstream file(current_dir / "CHARACTERS.txt");
	std::string line;
	while (std::getline(file, line)){
		int delimiter_spot = line.find(';');
		std::string name = line.substr(0, delimiter_spot);
		std::string rest_of_line = line.substr(delimiter_spot+1);
		std::string class_name = rest_of_line.substr(0, rest_of_line.find(';'));
		characters.push_back(name + ": " + class_name);
		if (list_characters){
			std::cout << "  - " << name << ": " << class_name << '\n';
		}
	} file.close();

	if (list_characters){exit(EXIT_SUCCESS);}

	const std::string name_specified = make_selection_menu(characters); // ex: John: Wizard -> John

	exit(EXIT_SUCCESS);  //REMOVE - Here to test menu functionality
	return load_character(false, name_specified);
}

// @brief Use stats data to create overall interactive character sheet with the
// specified data
void create_main_tui(CharacterStats& stats){
	using namespace ftxui;
	
}

int main(const int argc, const char* argv[]){
	// make sure character file exists
	check_path("CHARACTERS.txt");
	check_path("UNDO.txt");
	CharacterStats stats;

	// Command Line Arguments
	if (argc > 1){
		std::string option = argv[1];
		if (option == "-h" || option == "--help") {
			std::string file_name = argv[0];
			if (file_name[1] == '/'){file_name = file_name.substr(2);} // Cleaner macOS name

			std::cout << "usage: " << file_name << " [-h] [-c] [-d] [-l NAME]";
			std::cout << "\n\nDescripion section";
			std::cout << "\n\ncommands:";
			std::cout << "\n  list\t\t\tshow all characters available to load";
			std::cout << "\n  undo\t\t\trevert previous character deletion";
			std::cout << "\n\noptions:";
			std::cout << "\n  -h, --help\t\tshow help message";
			std::cout << "\n  -c, --create\t\tcreate a new character";
			std::cout << "\n  -d, --delete NAME\tdelete specified character from save data";
			std::cout << "\n  -l, --load NAME\tskip choosing screen and load specified character";

			exit(EXIT_SUCCESS);
		} else if (option == "list"){
			load_character(true);
		} else if (option == "undo"){
			undo();
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
				stats = load_character(delete_character, name_specified);
				create_main_tui(stats);
				
				exit(EXIT_SUCCESS);
			} else {
				std::cerr << "ERROR. No load character NAME given.";
				exit(EXIT_FAILURE);
			}
		}
	}
	
	// Get Character Stats
		// Health, AC, Speed
	stats = load_character(false);
	create_main_tui(stats);

	return EXIT_SUCCESS;
}
