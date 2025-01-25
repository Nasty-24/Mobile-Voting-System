
/*
Author:Kibunja Brian Njuguna
Reg No:BSE-05-0188/2024
Date:25/01/2024
Version: Mobile Voting System
*/#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

class VotingSystem {
private:
    std::map<std::string, std::string> voters; // voterID -> encrypted password
    std::map<std::string, int> voterAges; // voterID -> age
    std::map<int, int> candidates; // candidateID -> vote count
    std::map<std::string, bool> hasVoted; // voterID -> voting status
    const std::string adminPassword = "admin123";

    std::string encrypt(const std::string &password) {
        std::string encrypted = password;
        std::transform(encrypted.begin(), encrypted.end(), encrypted.begin(), [](char c) { return c + 3; }); // Simple Caesar cipher
        return encrypted;
    }

    std::string decrypt(const std::string &encrypted) {
        std::string decrypted = encrypted;
        std::transform(decrypted.begin(), decrypted.end(), decrypted.begin(), [](char c) { return c - 3; }); // Reverse Caesar cipher
        return decrypted;
    }

    void loadVoters() {
        std::ifstream file("voters.txt");
        std::string id, encryptedPassword;
        int age;
        while (file >> id >> encryptedPassword >> age) {
            voters[id] = encryptedPassword;
            voterAges[id] = age;
            hasVoted[id] = false;
        }
        file.close();
    }

    void saveVoters() {
        std::ofstream file("voters.txt");
        for (const auto &voter : voters) {
            file << voter.first << " " << voter.second << " " << voterAges[voter.first] << "\n";
        }
        file.close();
    }

    void loadCandidates() {
        std::ifstream file("candidates.txt");
        int id, votes;
        while (file >> id >> votes) {
            candidates[id] = votes;
        }
        file.close();
    }

    void saveCandidates() {
        std::ofstream file("candidates.txt");
        for (const auto &candidate : candidates) {
            file << candidate.first << " " << candidate.second << "\n";
        }
        file.close();
    }

public:
    VotingSystem() {
        loadVoters();
        loadCandidates();
    }

    ~VotingSystem() {
        saveVoters();
        saveCandidates();
    }

    void registerVoter(const std::string &id, const std::string &password, int age) {
        if (voters.find(id) == voters.end()) {
            if (age < 18) {
                std::cout << "You must be at least 18 years old to register.\n";
                return;
            }
            voters[id] = encrypt(password);
            voterAges[id] = age;
            hasVoted[id] = false;
            std::cout << "Voter registered successfully.\n";
        } else {
            std::cout << "Voter ID already exists.\n";
        }
    }

    bool login(const std::string &id, const std::string &password) {
        if (voters.find(id) != voters.end() && decrypt(voters[id]) == password) {
            if (voterAges[id] < 18) {
                std::cout << "Access denied. You must be at least 18 years old to vote.\n";
                return false;
            }
            std::cout << "Login successful.\n";
            return true;
        }
        std::cout << "Invalid ID or password.\n";
        return false;
    }

    void addCandidate(int candidateID) {
        if (candidates.find(candidateID) == candidates.end()) {
            candidates[candidateID] = 0;
            std::cout << "Candidate added successfully.\n";
        } else {
            std::cout << "Candidate ID already exists.\n";
        }
    }

    void vote(const std::string &voterID, int candidateID) {
        if (voterAges[voterID] < 18) {
            std::cout << "Access denied. You must be at least 18 years old to vote.\n";
            return;
        }
        if (hasVoted[voterID]) {
            std::cout << "You have already voted.\n";
            return;
        }
        if (candidates.find(candidateID) == candidates.end()) {
            std::cout << "Invalid candidate ID.\n";
            return;
        }
        candidates[candidateID]++;
        hasVoted[voterID] = true;
        std::cout << "Vote cast successfully.\n";
    }

    void displayResults() {
        std::cout << "Election Results:\n";
        for (const auto &candidate : candidates) {
            std::cout << "Candidate " << candidate.first << ": " << candidate.second << " votes\n";
        }
    }

    void adminPanel() {
        std::string password;
        std::cout << "Enter admin password: ";
        std::cin >> password;
        if (password != adminPassword) {
            std::cout << "Invalid admin password.\n";
            return;
        }

        int choice;
        do {
            std::cout << "\nAdmin Panel:\n";
            std::cout << "1. View All Voters\n";
            std::cout << "2. View All Candidates\n";
            std::cout << "3. Exit Admin Panel\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
                case 1: {
                    std::cout << "Voters List:\n";
                    for (const auto &voter : voters) {
                        std::cout << "Voter ID: " << voter.first << ", Age: " << voterAges[voter.first] << "\n";
                    }
                    break;
                }
                case 2: {
                    std::cout << "Candidates List:\n";
                    for (const auto &candidate : candidates) {
                        std::cout << "Candidate " << candidate.first << "\n";
                    }
                    break;
                }
                case 3:
                    std::cout << "Exiting Admin Panel.\n";
                    break;
                default:
                    std::cout << "Invalid choice.\n";
            }
        } while (choice != 3);
    }
};

int main() {
    VotingSystem vs;
    int choice;

    do {
        std::cout << "\nVoting System Menu:\n";
        std::cout << "1. Register Voter\n";
        std::cout << "2. Login\n";
        std::cout << "3. Add Candidate\n";
        std::cout << "4. Vote\n";
        std::cout << "5. Display Results\n";
        std::cout << "6. Admin Panel\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string id, password;
                int age;
                std::cout << "Enter voter ID: ";
                std::cin >> id;
                std::cout << "Enter password: ";
                std::cin >> password;
                std::cout << "Enter age: ";
                std::cin >> age;
                vs.registerVoter(id, password, age);
                break;
            }
            case 2: {
                std::string id, password;
                std::cout << "Enter voter ID: ";
                std::cin >> id;
                std::cout << "Enter password: ";
                std::cin >> password;
                if (vs.login(id, password)) {
                    std::cout << "Logged in as: " << id << "\n";
                }
                break;
            }
            case 3: {
                int candidateID;
                std::cout << "Enter candidate ID: ";
                std::cin >> candidateID;
                vs.addCandidate(candidateID);
                break;
            }
            case 4: {
                std::string voterID;
                int candidateID;
                std::cout << "Enter your voter ID: ";
                std::cin >> voterID;
                std::cout << "Enter candidate ID: ";
                std::cin >> candidateID;
                vs.vote(voterID, candidateID);
                break;
            }
            case 5:
                vs.displayResults();
                break;
            case 6:
                vs.adminPanel();
                break;
            case 7:
                std::cout << "Exiting system. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 7);

    return 0;
}
