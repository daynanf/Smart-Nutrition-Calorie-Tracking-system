#ifndef AUTH_H
#define AUTH_H

#include "models.h"
#include <string>
using namespace std;

// struct UserAccount {
//     string username;
//     string password;
//     string fullName;
//     int age;
//     string gender;
//     int height;
//     int weight;
//     string activityLevel;
// };

//First Welcome Page
void firstWellcomePage();

// Input-validation helpers
bool getValidUsername(string &username, char choice);
bool getValidPassword(string &password, const string &username, char choice);
bool getValidFullname(string &fullname);
bool getValidAge(int &age);
bool getValidGender(string &gender);
bool getValidHeight(int &height);
bool getValidWeight(int &weight); // changed to int in header for consistency with cpp although cpp had double? wait checking cpp
bool getValidActivityLevel(string &activityLevel);

// User flow functions
void registerUser(char choice);
bool loginUser(UserProfile& profile, char choice);
bool authentication(UserProfile &profile);

#endif
