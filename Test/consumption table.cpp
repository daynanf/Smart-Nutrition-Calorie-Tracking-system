#include <iomanip>
#include<iostream>  // For setw, left, right, setprecision
#include <sstream>  // For stringstream
#include<fstream>
using namespace std;


// // Later, change filename to include date:
// string filename = username + "_" + getTodayDate() + ".txt";




// Add to your code (after structs, before functions)
struct DailyTotal {
    float totalCalories;
    float totalProtein;
    float totalCarbs;
    float totalFat;
    int foodCount;
};

DailyTotal calculateDailyTotal(string filename) {
    DailyTotal totals = {0, 0, 0, 0, 0};
    ifstream file(filename);
    
    if (!file) {
        cout << "No meals logged today yet!" << endl;
        return totals;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        // Parse the line: "Food | calories | protein | carbs | fat | grams"
        stringstream ss(line);
        string foodName, temp;
        
        getline(ss, foodName, '|');  // Food name (we don't use now)
        getline(ss, temp, '|'); totals.totalCalories += stof(temp);
        getline(ss, temp, '|'); totals.totalProtein += stof(temp);
        getline(ss, temp, '|'); totals.totalCarbs += stof(temp);
        getline(ss, temp, '|'); totals.totalFat += stof(temp);
        
        totals.foodCount++;
    }
    
    file.close();
    return totals;
}



//CREATE THE ASCII TABLE
void printTodayConsumption(string username) {
    string filename = username + "_daily_meals.txt";
    DailyTotal totals = calculateDailyTotal(filename);
    
    if (totals.foodCount == 0) {
        cout << "\n=== Today's Consumption ===" << endl;
        cout << "No foods logged today." << endl;
        return;
    }
    
    // ====== ASCII TABLE HEADER ======
    cout << "\n";
    cout << "===============================================================================" << endl;
    cout << "                           TODAY'S CONSUMPTION                                 " << endl;
    cout << "===============================================================================" << endl;
    cout << left << setw(25) << "FOOD ITEM" 
         << right << setw(10) << "CALORIES" 
         << setw(10) << "PROTEIN" 
         << setw(10) << "CARBS" 
         << setw(10) << "FAT" 
         << setw(10) << "GRAMS" << endl;
    cout << "------------------------------------------------------------------------------" << endl;
    
    // ====== READ AND DISPLAY EACH FOOD ======
    ifstream file(filename);
    string line;
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string foodName, calories, protein, carbs, fat, grams;
        
        getline(ss, foodName, '|');
        getline(ss, calories, '|');
        getline(ss, protein, '|');
        getline(ss, carbs, '|');
        getline(ss, fat, '|');
        getline(ss, grams, '|');
        
        // Remove extra spaces
        foodName.erase(0, foodName.find_first_not_of(" "));
        foodName.erase(foodName.find_last_not_of(" ") + 1);
        
        // Print row with aligned columns
        cout << left << setw(25) << (foodName.length() > 24 ? foodName.substr(0, 21) + "..." : foodName)
             << right << setw(10) << fixed << setprecision(1) << stof(calories)
             << setw(10) << fixed << setprecision(1) << stof(protein)
             << setw(10) << fixed << setprecision(1) << stof(carbs)
             << setw(10) << fixed << setprecision(1) << stof(fat)
             << setw(10) << fixed << setprecision(0) << stof(grams) << endl;
    }
    file.close();
    
    // ====== TOTALS ROW ======
    cout << "------------------------------------------------------------------------------" << endl;
    cout << left << setw(25) << "DAILY TOTALS:"
         << right << setw(10) << fixed << setprecision(1) << totals.totalCalories
         << setw(10) << fixed << setprecision(1) << totals.totalProtein
         << setw(10) << fixed << setprecision(1) << totals.totalCarbs
         << setw(10) << fixed << setprecision(1) << totals.totalFat
         << setw(10) << "-" << endl;
    cout << "===============================================================================" << endl;
    
    // ====== SUMMARY ======
    cout << "\n📊 SUMMARY:" << endl;
    cout << "• Foods consumed today: " << totals.foodCount << endl;
    cout << "• Total Calories: " << totals.totalCalories << endl;
    cout << "• Macros: P:" << totals.totalProtein << "g C:" 
         << totals.totalCarbs << "g F:" << totals.totalFat << "g" << endl;
}
