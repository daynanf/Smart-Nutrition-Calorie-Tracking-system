#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// ========== 1. FIXED isSimilar FUNCTION ==========
// Now checks if the search word is INSIDE the food name (better for search)
bool isSimilar(string searchWord, string foodName) {
    // Convert both to lowercase for case-insensitive comparison
    for (int i = 0; i < searchWord.length(); i++)
        searchWord[i] = tolower(searchWord[i]);
    for (int i = 0; i < foodName.length(); i++)
        foodName[i] = tolower(foodName[i]);
    
    // Check if search word appears anywhere in food name
    return foodName.find(searchWord) != string::npos;
}

// ========== 2. FIXED saveCustomFood FUNCTION ==========
// Now uses correct filename format and gets all inputs inside function
void saveCustomFood(string username) {
    string name;
    float size, cal, prot, carb, fat;
    
    // Use correct filename from your spec
    string filename = "user_" + username + "_custom_foods.txt";
    ofstream custom(filename, ios::app);
    
    if (!custom) {
        cout << "Error opening file!" << endl;
        return;
    }
    
    cout << "\n=== Add Custom Food ===" << endl;
    cout << "Enter food name: ";
    cin.ignore(); // Clear input buffer
    getline(cin, name);
    
    cout << "Enter amount (per gram): ";
    cin >> size;
    cout << "Calories: ";
    cin >> cal;
    cout << "Protein (g): ";
    cin >> prot;
    cout << "Carbs (g): ";
    cin >> carb;
    cout << "Fat (g): ";
    cin >> fat;
    
    // Use | format like your foodfile.txt
    custom << name << " | " << cal << " | " << prot << " | " 
           << carb << " | " << fat << " | " << size << endl;
    
    cout << "\nFood saved to " << filename << "!" << endl;
    custom.close();
}

// ========== 3. FIXED addFoodToDailyMeal FUNCTION ==========
// Simpler version that just appends a food line to the daily meal file
void addFoodToDailyMeal(string dailyMealFile, string foodLine) {
    ofstream file(dailyMealFile, ios::app);
    if (!file) {
        cout << "Error opening daily meal file!" << endl;
        return;
    }
    
    file << foodLine << endl;
    file.close();
    cout << "Added to today's log!" << endl;
}

// ========== 4. FIXED MAIN MEAL FUNCTION (NO goto) ==========
// This function handles the food search and logging
void mealFunction(string username) {
    string dailyMealFile = username + "_daily_meals.txt"; // Example filename
    string searchFood;
    int choice;
    
    while (true) { // Loop instead of goto
        cout << "\n=== Search Food ===" << endl;
        cout << "Enter food name (or 'back' to return): ";
        cin.ignore(); // Clear buffer before getline
        getline(cin, searchFood);
        
        if (searchFood == "back") {
            break; // Exit loop to return to main menu
        }
        
        // Open food database
        ifstream foodDB("foodfile.txt");
        if (!foodDB) {
            cout << "Error opening food database!" << endl;
            break;
        }
        
        // Search for matches
        vector<string> matchedFoods;
        vector<string> matchedLines;
        string line;
        
        while (getline(foodDB, line)) {
            // Extract food name from line (first part before '|')
            size_t pos = line.find('|');
            string foodName = line.substr(0, pos);
            
            // Remove extra spaces
            while (!foodName.empty() && foodName[0] == ' ') 
                foodName.erase(0, 1);
            while (!foodName.empty() && foodName.back() == ' ') 
                foodName.pop_back();
            
            // Check if search matches
            if (isSimilar(searchFood, foodName)) {
                matchedFoods.push_back(foodName);
                matchedLines.push_back(line);
            }
        }
        foodDB.close();
        
        // Display results
        if (matchedFoods.empty()) {
            cout << "\nNo matches found!" << endl;
            cout << "1. Add as custom food" << endl;
            cout << "2. Search again" << endl;
            cout << "3. Back to menu" << endl;
            cout << "Choice: ";
            cin >> choice;
            
            if (choice == 1) {
                saveCustomFood(username);
            } else if (choice == 3) {
                break;
            }
            // If choice == 2, loop continues
        } 
        else {
            // Show matches
            cout << "\nFound " << matchedFoods.size() << " match(es):" << endl;
            for (int i = 0; i < matchedFoods.size(); i++) {
                cout << i+1 << ". " << matchedLines[i] << endl;
            }
            cout << "0. None of the above" << endl;
            
            // Get user selection
            int foodChoice;
            cout << "\nSelect food (0-" << matchedFoods.size() << "): ";
            while (!(cin >> foodChoice) || foodChoice < 0 || foodChoice > matchedFoods.size()) {
                cout << "Invalid! Enter 0-" << matchedFoods.size() << ": ";
                cin.clear();
                cin.ignore(100, '\n');
            }
            
            if (foodChoice == 0) {
                cout << "Returning to search..." << endl;
                continue;
            }
            
            // Get selected food
            string selectedLine = matchedLines[foodChoice - 1];
            cout << "\nSelected: " << selectedLine << endl;
            
            // Ask for action
            cout << "\n1. Add to today's meal" << endl;
            cout << "2. Save as custom food first" << endl;
            cout << "3. Cancel" << endl;
            cout << "Choice: ";
            cin >> choice;
            
            if (choice == 1) {
                addFoodToDailyMeal(dailyMealFile, selectedLine);
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                break; // Return to menu after adding
            } 
            else if (choice == 2) {
                saveCustomFood(username);
                // After saving, ask if they want to add it now
                cout << "\nAdd this to today's meal now? (1=Yes, 2=No): ";
                int addNow;
                cin >> addNow;
                if (addNow == 1) {
                    addFoodToDailyMeal(dailyMealFile, selectedLine);
                }
                break;
            }
            // If choice == 3, just break to return to menu
            break;
        }
    }
}

