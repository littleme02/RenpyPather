// Renpy Pather.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <filesystem>
#include <vector>


using namespace std;
int counter = 0;

string filelocation, returned;
list<string> labels;
list<string> files;


class Label
{	// Access specifier
public:
	string label_name, location;
	int line_pos, length;
	list<string> jumps;
	string parent = "";
	};

int main()
{
	// Find all files
		string path = "Edit this"; // Game folder location
	for (auto & p : filesystem::recursive_directory_iterator(path)) {
		std::cout << p.path() << std::endl;
		string foundpath = p.path().string();
		size_t found = foundpath.find("rpy");
		if (found < 5000 && foundpath.back() == 'y') {
			//foundpath.erase(0, path.length() + 1);
			files.push_back(foundpath);
		}
	}

	list<Label> All_labels;


	for (string file : files) {   //This loop finds all Labels
		string line;
		ifstream working_file(file);
		if (working_file.is_open())
		{
			int chars = 0;
			int line_number = 0;
			string last_root;
			while (getline(working_file, line))
			{
				line_number += 1;
				chars += line.length();
				size_t found = line.find("label");
				if (found == 0) {
					line.erase(0, 6); //Remove line so only label is left
					line.erase(line.size() - 1);
					Label lab;
					if (line.front() == '.') { //If the label has a parent, insert parent name infront of label
						line.insert(0,last_root);
						lab.parent = last_root;
					}
					else {
						last_root = line;
					}
					lab.label_name = line;
					lab.location = file;
					lab.line_pos = line_number;
					All_labels.push_back(lab);
					cout << line << "\n";
				}
			}
			//cout << file <<" is: " << chars << " chars \n";
			working_file.close();
		}
		else cout << "Unable to open file";
		int yes = 0;

	}
	list<Label> temp_labels; //Reads all the labels and finds the jump points
	for (Label worklabel : All_labels) {
		string line;
		ifstream working_file(worklabel.location);
		if (working_file.is_open())
		{
			string last_root = "tits";
			int line_number = 0;
			cout << "reading " << worklabel.label_name;
			bool next_label_found = false;
			while (getline(working_file, line)) 
			{
				line_number += 1;				
				if (line_number >= worklabel.line_pos && !next_label_found) {//This can probaly be optimised to it jumps directly to the line NR it is supposed to read instead of going thou the entire file
					size_t found_jump = line.find(" jump ");
					size_t found_talk = line.find("\""); //ignore lines with dialoge, its just someone mentioning jump
					size_t found_devcomment = line.find("#"); //ignore dev comments
					if (found_jump < 8000 && found_talk > 80000 && found_devcomment > 80000) { //if jump found add it to the list of jumps in the label object
						line.erase(0, 6+found_jump);
						if (line.front() == '.' && worklabel.parent == "") { //checks if label has a parent
							line.insert(0, worklabel.label_name);
						}
						if (line.front() == '.' && worklabel.parent != "") {
							line.insert(0, worklabel.parent);
						}
						//line.erase(line.size() - 1);
						worklabel.jumps.push_back(line); //push line to list
											}
					size_t found_label = line.find("label");
					size_t found_semi = line.find(":");
					if (found_label < 8000 && line_number > worklabel.line_pos && (found_talk > 80000 && found_devcomment > 80000)) {
						if (found_semi < 8000) {
							line.erase(found_semi, 1); //remove ending semicolon from label
						}
						if (worklabel.jumps.size() == 0) { //If no jumps where found, the end of the label jumps to next label in file
							line.erase(0, 6);
							if (line.front() == '.' && worklabel.parent == "") {
								line.insert(0, worklabel.label_name);
							}
							if (line.front() == '.' && worklabel.parent != "") {
								line.insert(0, worklabel.parent);
							}
							worklabel.jumps.push_back(line);
						}
						next_label_found = true;
						worklabel.length = (line_number - worklabel.line_pos);
						temp_labels.push_back(worklabel);
						cout << " " << worklabel.length << " lines and " << worklabel.jumps.size() << " jumps" << "\n";
					}
				}
			}
			if (!next_label_found) {
				worklabel.length = (line_number - worklabel.line_pos);
				temp_labels.push_back(worklabel);
				cout << "    " << worklabel.length << " lines and " << worklabel.jumps.size() << " jumps" << "\n";
			}

		}
		else cout << "Unable to open file " << worklabel.location << "\n";
		counter += 1;
	}
	All_labels = temp_labels;
	temp_labels.clear();

	ofstream graphfile;
	graphfile.open("output.gv");
	graphfile << "digraph G{\n" ;
	for (Label worklabel : All_labels) { //outputs graphfile

		for (string jump : worklabel.jumps)
		{
			graphfile << "\t" <<"\""<< worklabel.label_name << "\"";
			graphfile << " -> "<< "\""<<jump << "\"";
			graphfile << "\n";
		}	
		cout << worklabel.label_name << "\n";
	}
	graphfile << "}";
	graphfile.close();

	return 0;
}

