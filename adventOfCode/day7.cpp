#include "day7.h"
#include "daySetup.h"
#include <iostream>
#include <string>
#include <map>

struct dir* getNewDir(std::string dirName);
void addSubDir(dir*& parentDir, std::string subDirName);
struct dir*& changeDirs(std::vector<std::string> directoryPath, dir* root);
struct dir* buildSystem(std::vector<std::string> lines);

// Basic directory
struct dir {
	std::string dirName;
	int dirSize = 0;
	std::map<std::string, dir*> subDirs = {};
};

day7::day7(std::string fileName){
	// Getting all of the lines from the file
	daySetup setup(fileName);
	lines = setup.getLineStrings();

	// Running the code for part 1;
	std::cout << "The sum of the sizes of valid directories is " << part1() << " units";
}

int day7::part1() {
	dir* root = buildSystem(lines);
	return 8008135;
}

// Function to build a filesystem based on the text file
dir* buildSystem(std::vector<std::string> lines) {
	// Used for all command handling
	std::string command;
	std::vector<std::string> dirPath;
	dir* root = getNewDir("/");
	dir* currentDir = root;

	// Used to handle cd command
	std::string dirName;  // Name of new dir to switch into

	// Used to handle ls command
	std::vector<std::string> dirContents; // Vector to hold everything given by ls
	int x = 1; // Number of lines advanced
	std::string lsLine; // Variable to hold line given by ls, appended to dirContents
	int fileSize; 
	std::string newDirName;

	// Part 1- Build the filesystem
	// Using index so it can advance an nth number of times inside the same loop for ls command
	for (int i = 1; i < lines.size(); i++) {
		// All commands start with $
		if (lines[i][0] == '$') {
			command = lines[i].substr(2);
			if (command == "ls") {
				// Get all directory contents
				lsLine = lines[i + x];
				while (lsLine[0] != '$') {
					dirContents.push_back(lsLine);
					x++;
					if (i + x < lines.size()) {
						lsLine = lines[i + x];
					}
					else { break; }
				}
				x = 1;

				// Handling directory contents
				for (std::string content : dirContents) {
					if (content.substr(0, 3) == "dir") {
						// Add subdirectory to tree
						newDirName = content.substr(content.find(" ") + 1);
						addSubDir(currentDir, newDirName);
						//std::cout << "Added " << newDirName << " to " << currentDir->dirName << std::endl;
					}
					// If it is not a sub-directory, it is a file with the size
					else {
						fileSize = stoi(content.substr(0, (content.size() - content.find(" "))));
						currentDir->dirSize += fileSize;
						//std::cout << "Added file of size: " << fileSize << " to " << currentDir->dirName << std::endl;

					}
				}
				dirContents.clear();
			}
			// If it's not ls, it's going to be "cd" followed by a directory name
			else {
				dirName = command.substr(3);
				if (dirName == "..") {
					// Move into parent directory
					dirPath.pop_back();
					currentDir = changeDirs(dirPath, root);
				}
				else {
					// Move into subdirectory
					dirPath.push_back(dirName);
					currentDir = changeDirs(dirPath, root);
				}
			}
		}
		
		// If it's not a command continue to the next loop
		else {
			continue;
		}
	}
	return root;
}

// Function to get make a new directory
dir* getNewDir(std::string newDirName) {
	dir* newDir = new dir();
	newDir->dirName = newDirName;
	return newDir;
}

// Function to change directories given a path list and root directory
dir*& changeDirs(std::vector<std::string> directoryPath, dir* root) {
	dir*& currentDir = root;
	for (std::string dir : directoryPath) {
		currentDir = currentDir->subDirs[dir];
	}

	return currentDir;
}

// Function to add a subdirectory to a specific directory
void addSubDir(dir*& parentDir, std::string subDirName) {
	//Create a new subdirectory
	dir* subDir = getNewDir(subDirName);
	parentDir->subDirs.insert({ subDirName, subDir });
}