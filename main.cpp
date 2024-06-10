#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <algorithm>

enum SideType {Right, Top, Left, Bottom, StartSide, NoSide};
std::string side_string(SideType type) {
    switch (type) {
	case Right:
	    return "Right";
	case Top:
	    return "Top";
	case Left:
	    return "Left";
	case Bottom:
	    return "Bottom";
	case StartSide:
	    return "StartSide";
	case NoSide:
	    return "NoSide";
	default:
	    return "null";
    }
}

struct Side {
    SideType side_type;
    char letters[3];

    Side(SideType side, char l, char c, char r) {
        side_type = side;
	letters[0] = l;
        letters[1] = c;
        letters[2] = r;
    }
};

SideType found_on_sides(char letter, std::vector<Side> sides) {
    for (Side side : sides) {
        for (char side_letter : side.letters)
	    if (side_letter == letter) return side.side_type;
    }
    return NoSide;
}

std::vector<std::string> solve(std::vector<std::string> words, std::vector<Side> sides) {
    // Remove all words that dont have the letters of the sides.
    // Take a list of all letters on the sides
    std::vector<char> available_letters;
    for (Side side : sides)
	for (char letter : side.letters)
	    available_letters.push_back(letter);
    std::sort(available_letters.begin(), available_letters.end());
    available_letters.erase(unique(available_letters.begin(), available_letters.end()), available_letters.end());
    // compare the word's letters to side letters
    // List of preliminary words, that we'll use to do the actual solving.
    std::vector<std::string> prel_words;
    for (std::string word : words) {
	std::vector<char> tmp_word;
	for (char letter : word)
	    tmp_word.push_back(letter);
	std::sort(tmp_word.begin(), tmp_word.end());
	tmp_word.erase(unique(tmp_word.begin(), tmp_word.end()), tmp_word.end());
	if (std::includes(available_letters.begin(), available_letters.end(), tmp_word.begin(), tmp_word.end()))
	    prel_words.push_back(word);	
    }
    // Temporary side combinations, so that it doesnt need to be calculated each time;
    std::vector<Side> right, top, left, bottom;
    right.push_back(sides[1]);  right.push_back(sides[2]);   right.push_back(sides[3]);
    top.push_back(sides[0]);    top.push_back(sides[2]);     top.push_back(sides[3]);
    left.push_back(sides[0]);   left.push_back(sides[1]);    left.push_back(sides[3]);
    bottom.push_back(sides[0]); bottom.push_back(sides[1]);  bottom.push_back(sides[2]);

    std::vector<std::string> final_words;
    for (std::string  word : prel_words) {
	SideType current_side = StartSide;
	for (char letter : word) {
	    if (current_side == NoSide) break;
	    switch (current_side) {
	        case StartSide:
		    current_side = found_on_sides(letter, sides);
		    break;
		case Right:
		    current_side = found_on_sides(letter, right);
		    break;
		case Top:
		    current_side = found_on_sides(letter, top);
		    break;
		case Left:
		    current_side = found_on_sides(letter, left);
		    break;
		case Bottom:
		    current_side = found_on_sides(letter, bottom);
		    break;
		case NoSide:
		    break;
	    }
	}
	if (current_side != NoSide) {
            final_words.push_back(word);
	}
    }
    return final_words;
}

std::vector<Side> get_sides(const char *side_desc) {
    size_t len = strlen(side_desc);
    if (len != 15) exit(-1);
    int char_index = 0;
    char side_chars[12];
    for (int i = 0; i < 15; ++i) {
	char letter = side_desc[i];
        if (letter != '-') {
	    side_chars[char_index] = letter;
	    char_index++;
	}
    }
    std::vector<Side> sides;
    for (int i = 0; i < 4; ++i) {
	SideType type = (SideType)(i);
	Side side(type, side_chars[3*i + 0], side_chars[3*i + 1], side_chars[3*i + 2]);
	sides.push_back(side);
    } 
    return sides;
}

// Read the file
std::vector<std::string> read_file(const char *file_name) {
    std::ifstream file(file_name);
    std::string line;
    std::vector<std::string> words;

    while (std::getline(file, line)) {
	if (line.size() > 2)
	    words.push_back(line);
    }
    return words;
}

bool compare_length(const std::string &a, const std::string &b) {
    return a[0] < b[0];
}

void write_to_file(const char *file_name, std::vector<std::string> words) {
    std::ofstream file_stream;
    file_stream.open(file_name);
    for(std::string word : words)
        file_stream << word << std::endl; 
    file_stream.close();
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        std::cout << "Letter Boxed Solver" << std::endl;
	std::cout << "by viljokass\n" << std::endl;
	std::cout << "Use like this:" << std::endl;
	std::cout << " * First specify input file - these words will be filtered." << std::endl;
	std::cout << " * Then specify the letters on the game, like this: abc-def-ghi-jkl " << std::endl;
	std::cout << " * Lastly, specify the output file to write the results." << std::endl << std::endl;
	return 0;
    }

    std::vector<std::string> words = read_file(argv[1]);
    std::vector<Side> sides = get_sides(argv[2]); 
    std::vector<std::string> final_words = solve(words, sides);
    std::sort(final_words.begin(), final_words.end(), compare_length);
    write_to_file(argv[3], final_words); 
    
    return 0;
}
