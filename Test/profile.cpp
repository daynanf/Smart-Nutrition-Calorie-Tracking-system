// #include "profile.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>    // for round()
#include <ctime>    // for current date in WEIGHT_UPDATE
// #ifndef PROFILE_H
// #define PROFILE_H
#include <string>

// Plain data structure for user profile (no member functions, no OOP)
struct UserProfile {
    std::string name;               // Full name entered by user
    int age;                        // Age in years
    std::string gender;             // "Male" or "Female" (case-sensitive)
    double height_cm;               // Height in centimeters
    double weight_kg;               // Current weight in kilograms
    std::string activityLevel;      // One of: "Sedentary", "Lightly Active", "Moderately Active", "Very Active"
    double dailyCaloriesTarget;     // Rounded TDEE (Mifflin-St Jeor)
    double targetProtein_g;         // Rounded protein target (2 g/kg)
    double targetCarbs_g;           // Rounded carbs target (55% of calories / 4 kcal/g)
    double targetFat_g;             // Rounded fat target (25% of calories / 9 kcal/g)
};

// Function declarations (all free functions, procedural style)

// Collects all profile data from user input during registration
UserProfile collectProfileFromUser();

// Calculates Basal Metabolic Rate using Mifflin-St Jeor formula
double calcBMR(const UserProfile& p);

// Returns activity multiplier based on user's activity level string
double activityMultiplier(const std::string& level);

// Calculates TDEE and all macro targets, updates the profile struct
void calcMacroTargets(UserProfile& p);

// Displays the calculated daily nutrition targets (exact format required)
void displayNutritionTargets(const UserProfile& p);

// Creates the user data file and writes the initial USER_PROFILE block
void createUserDataFile(const std::string& filename, const UserProfile& p);

// Loads the USER_PROFILE block from an existing user data file
void loadProfileFromFile(const std::string& filename, UserProfile& p);

// Updates weight, recalculates targets, rewrites USER_PROFILE safely, appends weight history
void updateWeightAndTargets(const std::string& filename, UserProfile& p);

// #endif // PROFILE_H

// Collects complete user profile during registration
// Called by Member 2 (auth.cpp) during new user signup

UserProfile collectProfileFromUser() {
    UserProfile p;

    std::cout << "Enter your full name: ";
    std::getline(std::cin, p.name);

    std::cout << "Enter your age: ";
    std::cin >> p.age;
    std::cin.ignore();  // consume newline after integer

    std::cout << "Enter your gender (Male/Female): ";
    std::getline(std::cin, p.gender);

    std::cout << "Enter your height in cm: ";
    std::cin >> p.height_cm;
    std::cin.ignore();

    std::cout << "Enter your weight in kg: ";
    std::cin >> p.weight_kg;
    std::cin.ignore();

    std::cout << "Enter your activity level:\n";
    std::cout << "  Sedentary / Lightly Active / Moderately Active / Very Active\n> ";
    std::getline(std::cin, p.activityLevel);

    // Immediately calculate targets based on entered data
    calcMacroTargets(p);

    return p;
}


// Mifflin-St Jeor formula for Basal Metabolic Rate (exact match to sample)

double calcBMR(const UserProfile& p) {
    if (p.gender == "Male") {
        return 10.0 * p.weight_kg + 6.25 * p.height_cm - 5.0 * p.age + 5.0;
    } else {
        return 10.0 * p.weight_kg + 6.25 * p.height_cm - 5.0 * p.age - 161.0;
    }
}


// Returns TDEE multiplier for the given activity level
// Returns 1.2 as safe default if input is invalid/unknown

double activityMultiplier(const std::string& level) {
    if (level == "Sedentary")          return 1.2;
    if (level == "Lightly Active")     return 1.375;
    if (level == "Moderately Active")  return 1.55;
    if (level == "Very Active")        return 1.725;
    
    // Default to Sedentary (safest/lowest calorie estimate)
    return 1.2;
}


// Computes TDEE and macro targets using project-specified ratios
// Updates the UserProfile struct directly

void calcMacroTargets(UserProfile& p) {
    double bmr  = calcBMR(p);
    double tdee = bmr * activityMultiplier(p.activityLevel);

    p.dailyCaloriesTarget = round(tdee);                    // round to nearest whole number
    p.targetProtein_g     = round(p.weight_kg * 2.0);       // 2 g protein per kg bodyweight
    p.targetCarbs_g       = round((p.dailyCaloriesTarget * 0.55) / 4.0);  // 55% calories from carbs
    p.targetFat_g         = round((p.dailyCaloriesTarget * 0.25) / 9.0);  // 25% calories from fat
}


// Prints the calculated daily targets in the exact required format
// Used after registration and after weight updates

void displayNutritionTargets(const UserProfile& p) {
    std::cout << "Your Daily Nutrition Targets:\n";
    std::cout << "Calories: "<< p.dailyCaloriesTarget<< "\n";
    std::cout << "Protein: "<< p.targetProtein_g<< "g\n";
    std::cout << "Carbohydrates: "<< p.targetCarbs_g<< "g\n";
    std::cout << "Fat: "<< p.targetFat_g<< "g\n";
}


// Internal helper: writes the USER_PROFILE block in EXACT format
// Used by both create and update functions to avoid duplication

static void writeProfileBlock(std::ostream& out, const UserProfile& p) {
    out << "USER_PROFILE\n";
    out << "Name: " << p.name << "\n";
    out << "Age: " << p.age << "\n";
    out << "Gender: " << p.gender << "\n";
    out << "Height: " << p.height_cm << "\n";
    out << "Weight: " << p.weight_kg << "\n";
    out << "Activity_Level: " << p.activityLevel << "\n";
    out << "Daily_Calorie_Target: " << p.dailyCaloriesTarget << "\n";
    out << "\n";  // blank line separates profile from future DAILY_LOG blocks
}

// Creates the user data file during registration and writes initial profile
// Called by Member 2 (auth.cpp) after collecting profile data

void createUserDataFile(const std::string& filename, const UserProfile& p) {
    std::ofstream fout(filename);
    if (!fout) {
        std::cerr << "Error: Cannot create file " << filename << "\n";
        return;
    }

    writeProfileBlock(fout, p);
    fout.close();

    std::cout << "User profile saved to " << filename << "\n";
}

// Loads the USER_PROFILE block from the user's data file
// Called after successful login to restore user data

void loadProfileFromFile(const std::string& filename, UserProfile& p) {
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "Error: Cannot open file " << filename << "\n";
        return;
    }

    std::string line;
    bool inProfile = false;

    while (std::getline(fin, line)) {
        if (line == "USER_PROFILE") {
            inProfile = true;
            continue;
        }
        if (inProfile && line.empty()) {
            break;  // end of profile block
        }
        if (!inProfile) continue;

        size_t colonPos = line.find(": ");
        if (colonPos == std::string::npos) continue;

        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 2);

        if (key == "Name")  p.name = value;
        else if (key == "Age")p.age= std::stoi(value);
        else if (key == "Gender")p.gender= value;
        else if (key == "Height")p.height_cm= std::stod(value);
        else if (key == "Weight")p.weight_kg= std::stod(value);
        else if (key == "Activity_Level")p.activityLevel= value;
        else if (key == "Daily_Calorie_Target") p.dailyCaloriesTarget = std::stod(value);
    }

    fin.close();

    // Recalculate macros in case weight or other values changed externally
    calcMacroTargets(p);
}
// Main weight update function (called from main menu)
// Prompts for new weight, recalculates targets, safely updates file
void updateWeightAndTargets(const std::string& filename, UserProfile& p) {
    double newWeight;
    std::cout << "Enter your new weight in kg: ";
    std::cin >> newWeight;
    std::cin.ignore();

    // Update profile and recalculate everything
    p.weight_kg = newWeight;
    calcMacroTargets(p);

    std::cout << "Weight updated to " << newWeight << " kg.\n";
    displayNutritionTargets(p);

    // Open file for both reading and writing
    std::fstream file(filename, std::ios::in | std::ios::out);
    if (!file) {
        std::cerr << "Error: Cannot open " << filename << " for update\n";
        return;
    }
    // Overwrite only the USER_PROFILE block at the beginning
    file.seekp(0);                    // go to start of file
    writeProfileBlock(file, p);       // rewrite updated profile

    // Append weight history entry at the end
    file.seekp(0, std::ios::end);     // go to end of file

    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    file << "WEIGHT_UPDATE\n";
    file << "Date: " << (1900 + localTime->tm_year) << "-"
         << (1 + localTime->tm_mon) << "-" << localTime->tm_mday << "\n";
    file << "Weight: " << newWeight << "\n\n";
    file.close();
    std::cout << "Profile updated and weight change recorded.\n";
}