#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
using namespace std;

// ========== ADDED: FoodItem STRUCT ==========
struct FoodItem {
    string name;
    float calories;
    float protein;
    float carbs;
    float fat;
    float servingSize;  // Usually 100g
};

struct DailyTotal {
    float totalCalories;
    float totalProtein;
    float totalCarbs;
    float totalFat;
    int foodCount;
};

// ========== 1. FIXED isSimilar FUNCTION ==========
bool isSimilar(string searchWord, string foodName) {
    for (int i = 0; i < searchWord.length(); i++)
        searchWord[i] = tolower(searchWord[i]);
    for (int i = 0; i < foodName.length(); i++)
        foodName[i] = tolower(foodName[i]);
    return foodName.find(searchWord) != string::npos;
}

// ========== ADDED: Parse line to FoodItem ==========
FoodItem parseFoodLine(string line) {
    FoodItem item;
    stringstream ss(line);
    string part;
    
    getline(ss, part, '|'); 
    item.name = part;
    // Remove spaces from name
    item.name.erase(0, item.name.find_first_not_of(" "));
    item.name.erase(item.name.find_last_not_of(" ") + 1);
    
    getline(ss, part, '|'); item.calories = stof(part);
    getline(ss, part, '|'); item.protein = stof(part);
    getline(ss, part, '|'); item.carbs = stof(part);
    getline(ss, part, '|'); item.fat = stof(part);
    getline(ss, part, '|'); item.servingSize = stof(part);
    
    return item;
}

// ========== ADDED: FoodItem to string ==========
string foodItemToString(FoodItem food) {
    return food.name + " | " + to_string(food.calories) + " | " +
           to_string(food.protein) + " | " + to_string(food.carbs) + " | " +
           to_string(food.fat) + " | " + to_string(food.servingSize);
}

// ========== 2. MODIFIED saveCustomFood FUNCTION ==========
void saveCustomFood(string username) {
    FoodItem newFood;
    
    string filename = "user_" + username + "_custom_foods.txt";
    ofstream custom(filename, ios::app);
    
    if (!custom) {
        cout << "Error opening file!" << endl;
        return;
    }
    
    cout << "\n=== Add Custom Food ===" << endl;
    cout << "Enter food name: ";
    cin.ignore();
    getline(cin, newFood.name);
    
    cout << "Serving size (grams, usually 100): ";
    cin >> newFood.servingSize;
    cout << "Calories per " << newFood.servingSize << "g: ";
    cin >> newFood.calories;
    cout << "Protein (g): ";
    cin >> newFood.protein;
    cout << "Carbs (g): ";
    cin >> newFood.carbs;
    cout << "Fat (g): ";
    cin >> newFood.fat;
    
    custom << foodItemToString(newFood) << endl;
    cout << "\nFood saved to " << filename << "!" << endl;
    custom.close();
}

// ========== 3. MODIFIED addFoodToDailyMeal FUNCTION ==========
// Now calculates based on grams consumed
void addFoodToDailyMeal(string dailyMealFile, FoodItem food, float gramsConsumed) {
    ofstream file(dailyMealFile, ios::app);
    if (!file) {
        cout << "Error opening daily meal file!" << endl;
        return;
    }
    
    // CALCULATION: Adjust nutrients based on grams consumed
    float ratio = gramsConsumed / food.servingSize;
    float adjustedCal = food.calories * ratio;
    float adjustedProt = food.protein * ratio;
    float adjustedCarbs = food.carbs * ratio;
    float adjustedFat = food.fat * ratio;
    
    // Save the consumed portion
    file << food.name << " | " << adjustedCal << " | " << adjustedProt << " | "
         << adjustedCarbs << " | " << adjustedFat << " | " << gramsConsumed << endl;
    
    file.close();
    
    // Show user what was added
    cout << "\n? Added " << gramsConsumed << "g of " << food.name << endl;
    cout << "  Nutrition: " << adjustedCal << " cal, " 
         << adjustedProt << "g protein, " << adjustedCarbs << "g carbs, " 
         << adjustedFat << "g fat" << endl;
}

// ========== ADDED: Search in both files ==========
void searchInFile(string filename, string searchWord, vector<FoodItem>& results) {
    ifstream file(filename);
    if (!file) return;  // File might not exist yet
    
    string line;
    while (getline(file, line)) {
        FoodItem item = parseFoodLine(line);
        if (isSimilar(searchWord, item.name)) {
            results.push_back(item);
        }
    }
    file.close();
}

// ========== 4. MODIFIED mealFunction ==========
void mealFunction(string username) {
     string dailyMealFile = username + "_daily_meals.txt";
    string customFoodFile = "user_" + username + "_custom_foods.txt";      //assume bini opened them
    string searchFood;
    int choice;
    
    while (true) {
        cout << "\n=== Search Food ===" << endl;
        cout << "Enter food name (or 'back' to return): ";
        cin.ignore();
        getline(cin, searchFood);
        
        if (searchFood == "back") break;
        
        // SEARCH BOTH FILES
        vector<FoodItem> allResults;
        
        // Search main database
        searchInFile("foodfile.txt", searchFood, allResults);
        
        // Search custom foods
        searchInFile(customFoodFile, searchFood, allResults);
        
        // Display results
        if (allResults.empty()) {
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
            }  // if choice is 2 it continues
            continue;
        } 
        else {
            // Show matches
            cout << "\nFound " << allResults.size() << " match(es):" << endl;
            for (int i = 0; i < allResults.size(); i++) {
                cout << i+1 << ". " << allResults[i].name 
                     << " (" << allResults[i].calories << " cal per " 
                     << allResults[i].servingSize << "g)" << endl;
            }
            cout << "0. None of the above" << endl;
            
            // Get selection
            int foodChoice;
            cout << "\nSelect food (0-" << allResults.size() << "): ";
            while (!(cin >> foodChoice) || foodChoice < 0 || foodChoice > allResults.size()) {
                cout << "Invalid! Enter 0-" << allResults.size() << ": ";
                cin.clear();
                cin.ignore(100, '\n');
            }
            
            if (foodChoice == 0) {
                cout << "Returning to search..." << endl;
                continue;
            }
            
            FoodItem selected = allResults[foodChoice - 1];
            
            // ASK FOR GRAMS
            float grams;
            cout << "\nSelected: " << selected.name << endl;
            cout << "How many grams did you consume? ";
            cin >> grams;
            
            // Ask action
            cout << "\n1. Add to today's meal (" << grams << "g)" << endl;
            cout << "2. Save as custom food first" << endl;
            cout << "3. Cancel" << endl;
            cout << "Choice: ";
            cin >> choice;
            
            if (choice == 1) {
                addFoodToDailyMeal(dailyMealFile, selected, grams);
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            } 
            else if (choice == 2) {
                saveCustomFood(username);
                cout << "\nAdd this to today's meal now? (1=Yes, 2=No): ";
                int addNow;
                cin >> addNow;
                if (addNow == 1) {
                    // Use the newly saved food
                    FoodItem newFood;
                    // You would need to get this from saveCustomFood or re-ask
                    // For simplicity, we'll use the selected item
                    addFoodToDailyMeal(dailyMealFile, selected, grams);
                }
                break;
            }
            else {
                break;
            }
        }
    }
}

// ========== EXAMPLE MAIN ==========
int main() {
    string currentUser = "john";  // From registration
    
    // ASSUMPTION: Files are created when user registers
    // user_john_custom_foods.txt and john_daily_meals.txt exist
    
    while (true) {
        cout << "\n=== MAIN MENU ===" << endl;
        cout << "1. Add food to today's log" << endl;
        cout << "2. View today's consumption" << endl;
        cout << "3. Logout" << endl;
        cout << "Choice: ";
        
        int menuChoice;
        cin >> menuChoice;
        
        if (menuChoice == 1) {
            mealFunction(currentUser);
        }
        else if (menuChoice == 2) {
            cout << "Today's consumption table would appear here." << endl;
        }
        else if (menuChoice == 3) {
            cout << "Goodbye!" << endl;
            break;
        }
        else {
            cout << "Invalid choice!" << endl;
        }
    }
    return 0;
}