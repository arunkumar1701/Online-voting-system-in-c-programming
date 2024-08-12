#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_CANDIDATES 100

struct User {
    char username[50];
    char password[50];
    bool hasVoted;
    bool isAdmin;
};

struct Candidate {
    char id[50];
    char name[50];
    char position[50];
    int votes;
};


// Function prototypes
int readUserDatabase(struct User *userDatabase, const char *filename);
void writeUserDatabase(const struct User *userDatabase, int count, const char *filename);
int readCandidateDatabase(struct Candidate *candidateDatabase, const char *filename);
void writeCandidateDatabase(const struct Candidate *candidateDatabase, int count, const char *filename);
bool authenticateUser(struct User *userDatabase, int userCount, char *username, char *password);
bool hasUserVoted(struct User *userDatabase, int userCount, char *username);
bool isAdmin(struct User *userDatabase, int userCount, char *adminUsername, char *adminPassword);
void addCandidates(struct Candidate *candidateDatabase, int *candidateCount, struct User *userDatabase, int userCount, char *adminUsername, char *adminPassword);
void displayVotingInterface(struct User *userDatabase, int userCount, struct Candidate *candidateDatabase, int candidateCount, char *username);
void displayResults(struct Candidate *candidateDatabase, int candidateCount, struct User *userDatabase, int userCount, char *adminUsername, char *adminPassword);
void getUserCredentials(char *username, char *password);

// Function to read user database from file
int readUserDatabase(struct User *userDatabase, const char *filename) {
    // ... (previous code)
       FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening user database file");
        return -1;
    }

    int count = 0;
    while (fscanf(file, "%s %s %d %d\n", userDatabase[count].username, userDatabase[count].password,
                  &userDatabase[count].hasVoted, &userDatabase[count].isAdmin) == 4) {
        count++;
    }

    fclose(file);
    return count;
}

// Function to write user database to file
void writeUserDatabase(const struct User *userDatabase, int count, const char *filename) {
    // ... (previous code)
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening user database file");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s %d %d\n", userDatabase[i].username, userDatabase[i].password,
                userDatabase[i].hasVoted, userDatabase[i].isAdmin);
    }

    fclose(file);
}

// Function to read candidate database from file
int readCandidateDatabase(struct Candidate *candidateDatabase, const char *filename) {
    // ... (previous code)
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening candidate database file");
        return -1;
    }

    int count = 0;
    while (fscanf(file, "%s %s %s %d\n", candidateDatabase[count].id, candidateDatabase[count].name,
                  candidateDatabase[count].position, &candidateDatabase[count].votes) == 4) {
        count++;
    }

    fclose(file);
    return count;
}
// Function to write candidate database to file
void writeCandidateDatabase(const struct Candidate *candidateDatabase, int count, const char *filename) {
    // ... (previous code)
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening candidate database file");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s %s %d\n", candidateDatabase[i].id, candidateDatabase[i].name,
                candidateDatabase[i].position, candidateDatabase[i].votes);
    }

    fclose(file);
}

// ... (rest of the functions)
// Function to authenticate user
bool authenticateUser(struct User *userDatabase, int userCount, char *username, char *password) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(userDatabase[i].username, username) == 0 && strcmp(userDatabase[i].password, password) == 0) {
            return true;
        }
    }
    return false;
}

// Function to check if the user has already voted
bool hasUserVoted(struct User *userDatabase, int userCount, char *username) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(userDatabase[i].username, username) == 0 && userDatabase[i].hasVoted) {
            return true;
        }
    }
    return false;
}

// Function to authenticate admin
bool isAdmin(struct User *userDatabase, int userCount, char *adminUsername, char *adminPassword) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(userDatabase[i].username, adminUsername) == 0 && strcmp(userDatabase[i].password, adminPassword) == 0 && userDatabase[i].isAdmin) {
            return true;
        }
    }
    return false;
}

// Function to add candidates (Password protected)
void addCandidates(struct Candidate *candidateDatabase, int *candidateCount, struct User *userDatabase, int userCount, char *adminUsername, char *adminPassword) {
    if (isAdmin(userDatabase, userCount, adminUsername, adminPassword)) {
        int numCandidates;
        printf("Enter the number of candidates: ");
        scanf("%d", &numCandidates);

        for (int i = 0; i < numCandidates; i++) {
            printf("Enter Candidate ID for candidate %d: ", i + 1);
            scanf("%s", candidateDatabase[*candidateCount].id);

            printf("Enter Candidate Name for candidate %d: ", i + 1);
            scanf("%s", candidateDatabase[*candidateCount].name);

            printf("Enter Candidate Position for candidate %d: ", i + 1);
            scanf("%s", candidateDatabase[*candidateCount].position);

            candidateDatabase[*candidateCount].votes = 0;
            (*candidateCount)++;
        }

        // Write updated candidate database to file
        writeCandidateDatabase(candidateDatabase, *candidateCount, "candidates.txt");
                       // path should same as in your pc
        printf("Candidates added successfully.\n");
    } else {
        printf("Invalid credentials. Only admin can add candidates.\n");
    }
}

// Function to display voting interface
void displayVotingInterface(struct User *userDatabase, int userCount, struct Candidate *candidateDatabase, int candidateCount, char *username) {
    if (hasUserVoted(userDatabase, userCount, username)) {
        printf("Your vote is already registered. You cannot vote again.\n");
    } else {
        printf("Welcome to the voting interface, %s! You can cast your vote now.\n", username);

        // Display the list of candidates
        printf("List of Candidates:\n");
        for (int i = 0; i < candidateCount; i++) {
            printf("%s. %s - %s\n", candidateDatabase[i].id, candidateDatabase[i].name, candidateDatabase[i].position);
        }

        // Get user's vote
        printf("Enter the ID of the candidate you want to vote for: ");
        char selectedCandidateID[50];
        scanf("%s", selectedCandidateID);

        // Find the selected candidate in the database
        int candidateIndex = -1;
        for (int i = 0; i < candidateCount; i++) {
            if (strcmp(candidateDatabase[i].id, selectedCandidateID) == 0) {
                candidateIndex = i;
                break;
            }
        }

        // Check if the candidate ID was found
        if (candidateIndex != -1) {
            // Update candidate's vote count
            candidateDatabase[candidateIndex].votes++;
    

            // Update user's voting status
            for (int j = 0; j < userCount; j++) {
                if (strcmp(userDatabase[j].username, username) == 0) {
                    userDatabase[j].hasVoted = true;
                    break;
                }
            }

            // Write updated databases to files
            writeUserDatabase(userDatabase, userCount, "users.txt");// path should same as in your pc
            writeCandidateDatabase(candidateDatabase, candidateCount, "candidates.txt");
                                                                                 // path should same as in your pc
            printf("Your vote has been registered. Thank you!\n");
        } else {
            // If the selected candidate ID was not found
            printf("Invalid candidate ID. Vote not registered.\n");
        }
    }
}

// Function to display election results
void displayResults(struct Candidate *candidateDatabase, int candidateCount, struct User *userDatabase, int userCount, char *adminUsername, char *adminPassword) {
    if (isAdmin(userDatabase, userCount, adminUsername, adminPassword)) {
        // Display the election results
        printf("Election Results:\n");
        int maxVotes = 0;  // To keep track of the maximum number of votes
        int winnerIndex = -1;  // To store the index of the candidate with the maximum votes
        
        for (int i = 0; i < candidateCount; i++) {
            printf("%s. %s - %s - Votes: %d\n", candidateDatabase[i].id, candidateDatabase[i].name, candidateDatabase[i].position, candidateDatabase[i].votes);
            
            // Determine the winner
            if (candidateDatabase[i].votes > maxVotes) {
                maxVotes = candidateDatabase[i].votes;
                winnerIndex = i;
            }
        }

        // Display the winner
        if (winnerIndex != -1) {
            printf("\nWinner:\n");
            printf("%s. %s - %s - Votes: %d\n", candidateDatabase[winnerIndex].id, candidateDatabase[winnerIndex].name, candidateDatabase[winnerIndex].position, candidateDatabase[winnerIndex].votes);
        } else {
            printf("\nNo winner determined.\n");
        }

    } else {
        printf("Invalid credentials. Only admin can view results.\n");
    }
}

// Function to get user credentials
void getUserCredentials(char *username, char *password) {
    printf("Enter your username: ");
    scanf("%s", username);

    printf("Enter your password: ");
    scanf("%s", password);
}


void runVotingSystem();

int main() {
    runVotingSystem();

    return 0;
}

void runVotingSystem() {
    // Paths to database files
    const char userDatabasePath[] = "users.txt";// path should same as in your pc
    const char candidateDatabasePath[] = "candidates.txt";// path should same as in your pc

    while (1) { // Start an infinite loop
        // Read user and candidate databases from files
        struct User userDatabase[MAX_USERS];
        int userCount = readUserDatabase(userDatabase, userDatabasePath);

        struct Candidate candidateDatabase[MAX_CANDIDATES];
        int candidateCount = readCandidateDatabase(candidateDatabase, candidateDatabasePath);

        // Get user credentials
        char username[50];
        char password[50];
        getUserCredentials(username, password);

        // Authenticate user
        if (isAdmin(userDatabase, userCount, username, password)) {
            // Admin authentication successful
            printf("Admin authentication successful. You may proceed.\n");

            int choice;
            do {
                // Display admin options
                printf("Choose an option:\n");
                printf("1. Add Candidates\n");
                printf("2. View Results\n");
                printf("3. Quit\n");

                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        // Add candidates
                        addCandidates(candidateDatabase, &candidateCount, userDatabase, userCount, username, password);
                        // Write updated candidate database to file
                        writeCandidateDatabase(candidateDatabase, candidateCount, candidateDatabasePath);
                        // Break out of the switch to prompt admin credentials again
                        break;
                    case 2:
                        // View results
                        displayResults(candidateDatabase, candidateCount, userDatabase, userCount, username, password);
                        break;
                    case 3:
                        // Quit the loop
                        printf("Quitting...\n");
                        exit(0); // Exit the program
                        break;
                    default:
                        printf("Invalid choice. Try again.\n");
                }
            } while (choice != 3); // Loop until the user chooses to quit
        } else {
            // Regular user authentication
            int userIndex = -1;
            for (int i = 0; i < userCount; i++) {
                if (strcmp(userDatabase[i].username, username) == 0 && strcmp(userDatabase[i].password, password) == 0) {
                    userIndex = i;
                    break;
                }
            }

            if (userIndex != -1) {
                // User found in the database
                if (hasUserVoted(userDatabase, userCount, username)) {
                    // User has already voted
                    printf("Your vote is already registered. You cannot vote again.\n");
                } else {
                    // User has not voted yet
                    displayVotingInterface(userDatabase, userCount, candidateDatabase, candidateCount, username);

                    // Update user's voting status
                    userDatabase[userIndex].hasVoted = true;

                    // Write updated user database to file
                    writeUserDatabase(userDatabase, userCount, userDatabasePath);
                }
            } else {
                // User not found, contact admin
                printf("User not found. Please contact the admin for assistance.\n");
            }
        }
    }
}
