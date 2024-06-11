//
//  main.cpp
//  Remote_backup1
//
//  Created by Macbook pro on 6/11/24.
//

#include<iostream>
#include<string>
#include<fstream>
#include<filesystem>

namespace fs = std::filesystem;

bool readConfig(const std::string& configFilePath, std::string& sourceDir, std::string& destDir) {
    std::ifstream configFile(configFilePath);
    if(!configFile.is_open()) {
        std::cerr << "Error: Could not open config file" << std::endl;
        return false;
    }
    std::string line;
    while(std::getline(configFile, line)) {
        if(line.find("source_dir=") == 0) {
            sourceDir = line.substr(strlen("source_dir="));
        } else if(line.find("dest_dir=") == 0) {
            destDir = line.substr(strlen("dest_dir="));
        }
    }
    
    configFile.close();
//    std::cout << sourceDir << " " << destDir << std::endl;
    return !sourceDir.empty() && !destDir.empty();
}

void transferFiles(const std::string& sourceDir, const std::string& destDir) {
    if(fs::exists(destDir)) {
        fs::create_directories(destDir);
    }
    for(const auto& file : fs::directory_iterator(sourceDir)) {
        if(fs::is_regular_file(file.status())) {
            fs::path sourceFilePath = file.path();
            fs::path destFilePath = destDir / sourceFilePath.filename();
            fs::copy_file(sourceFilePath, destFilePath, fs::copy_options::skip_existing);
            std::cout << "Transferred: " << sourceFilePath << "to" << destFilePath << std::endl;
        }
    }
}

int main() {

    std::string sourceDir;
    std::string destDir;
    std::string configFilePath = "/Users/macbookpro/Documents/dev/Remote_backup1/Remote_backup1/config.txt";
    if(!readConfig(configFilePath, sourceDir, destDir)) {
        std::cerr << "Error: Something wrong with the configuration" << std::endl;
    }
    transferFiles(sourceDir, destDir);
    return 0;
}

