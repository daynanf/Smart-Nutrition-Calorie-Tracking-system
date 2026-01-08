# Smart-Nutrition-Calorie-Tracking-system

## 📘 Project Overview
The **Smart-Nutrition-Calorie-Tracking-system** is a console-based C++ application designed to help users manage their dietary habits. It creates personalized nutritional targets based on user statistics (age, gender, height, weight, activity level) using the Mifflin-St Jeor equation. Users can log meals, view their daily consumption against targets, add custom foods, and receive algorithm-based meal recommendations.

---

## 🏗️ System Architecture & Workflow

The system is built on a modular architecture where `main.cpp` acts as the central controller, delegating specific functionality to specialized modules (`auth`, `profile`, `food`, `recommendations`).

### 1. High-Level Control Flow (`main.cpp`)
The application follows a **Two-State State Machine** pattern implemented in the `main()` function:

1.  **Authentication State (Outer Loop)**
    *   The program starts by calling `setupConsole()` for UTF-8 support.
    *   It enters an infinite loop calling `authentication(currentUser)`.
    *   **Action**: Shows the Welcome Screen.
    *   **Result**: Returns `true` only if a user successfully logs in. If users exit, the program terminates here.

2.  **Dashboard State (Inner Loop)**
    *   Once `authentication` returns `true`, the program enters the **Dashboard Loop**.
    *   **Action**: Calls `userDashboardMenu()` to display the 9-option main menu.
    *   **Input**: Reads integer input (1-9).
    *   **Routing**: Uses a `switch` statement to call the appropriate function (e.g., `runAddFood`, `displayProfile`).
    *   **Logout**: Case 9 sets `loggedIn = false`, breaking the inner loop and returning control to the Authentication State.

### 2. Module Breakdown

#### A. Authentication Module (`auth.cpp` / `auth.h`)
*   **Role**: Handles user registration, login, and robust input validation.
*   **Key Function**: `authentication(UserProfile&)`
    *   Calls `firstWellcomePage()` to show options.
    *   **Register Flow**: Calls `registerUser(choice)`.
        *   Collects inputs using helper functions (`getValidUsername`, `getValidPassword`, etc.).
        *   Validates "Back" requests (`'b'`) at every step.
        *   Saves the new user to `users.txt` and creates a personal data file.
    *   **Login Flow**: Calls `loginUser(UserProfile&, choice)`.
        *   Validates credentials against `users.txt`.
        *   Loads the user's profile into memory via `loadProfileFromFile`.

#### B. Profile Module (`profile.cpp` / `profile.h`)
*   **Role**: Manages user data and nutritional math.
*   **Key Logic**:
    *   **BMR Calculation**: `calcBMR` uses the Mifflin-St Jeor formula:
        *   *Male*: `(10*weight) + (6.25*height) - (5*age) + 5`
        *   *Female*: `(10*weight) + (6.25*height) - (5*age) - 161`
    *   **Macro Targets**: `calcMacroTargets` derives goals from TDEE (Total Daily Energy Expenditure):
        *   Protein: 2g per kg of body weight.
        *   Fat: 25% of total TDEE.
        *   Carbs: Remaining calories (approx 55%).
    *   **File I/O**: `loadProfileFromFile` and `createUserDataFile` handle the `USER_PROFILE` block in user data files.

#### C. Food & Logging Module (`food.cpp` / `food.h`)
*   **Role**: Handles database interactions, searching, and logging daily meals.
*   **Key Functions**:
    *   `runAddFood`: The primary logging interface.
        *   Asks to search in "General Foods" or "Custom Foods".
        *   Calls `searchInFile` to find matches.
        *   Calculates nutritional values based on portion size (e.g., if valid portion is 100g and user eats 200g, all nutrients are x2).
        *   Calls `saveToDailyLog` to append the entry.
    *   `saveToDailyLog`: Complex file operation that:
        1. Reads the user's data file.
        2. Locates or creates the `DAILY_LOG` section for the `currentDate`.
        3. Appends the new food entry.
        4. Updates the daily totals summary line.

#### D. Recommendations Module (`recommendations.cpp` / `recommendations.h`)
*   **Role**: Analyzes current intake and suggests foods to meet remaining targets.
*   **Algorithm (`runMealRecommendations`)**:
    1.  **Status Check**: Calls `getConsumedToday` to find out what has been eaten.
    2.  **Gap Analysis**: Calculates `Remaining = Target - Consumed`.
    3.  **Filtration/Sorting**:
        *   Loads foods from `foods.txt`.
        *   If user selects "High Protein", finds foods with high protein/calorie ratio.
        *   sorts candidates using a Bubble Sort implementation.
    4.  **Presentation**: Shows top 5 suggestions.
    5.  **Action**: Allows immediate addition of a suggestion to the log via `autoAddFood`.

#### E. Utils Module (`utils.cpp` / `utils.h`)
*   **Role**: General purpose helpers.
*   **Key Features**:
    *   `checkForBack`: The global "Go Back" logic handler.
    *   `safeStoi`/`safeStof`: Crash-safe string-to-number conversions.
    *   `printHeader`: Standardized UI headers.

---

## 📂 Data Structure & Persistence

The system uses a flat-file database approach:

### 1. User Registry (`users.txt`)
Stores login credentials for all users.
*   **Format**: `Username|Password|DataFilename|FullName|Age|Gender|Height|Weight|ActivityLevel`

### 2. User Data File (`user_USERNAME_data.txt`)
Stores specific data for a single user. Contains two main sections:
*   **USER_PROFILE**: Static stats and calculated targets.
*   **DAILY_LOG**: Chronological log of meals.
    *   **Format**: `Date|MealType|FoodName|Grams|Cals|Protein|Carbs|Fat`

### 3. Food Database (`foods.txt`)
The master list of standard foods.
*   **Format**: `FoodName|ServingSize|Calories|Protein|Carbs|Fat`

### 4. Custom Food File (`user_USERNAME_custom_foods.txt`)
Stores foods created by the user. Follows the same format as `foods.txt`.

---

## 💻 Compilation & Installation

### Prerequisites
*   A C++ Compiler (GCC/G++ recommended).
*   Windows OS (due to `windows.h` dependency for console styling).

### Build Command
To compile the entire system, run the following command in your terminal:

```bash
g++ main.cpp auth.cpp profile.cpp food.cpp recommendations.cpp utils.cpp -o nutrition_tracker
```

### Running the App
```bash
./nutrition_tracker
```

---

## 🚀 Key Features Summary
1.  **Smart Auth**: Registration, Login, and secure Password check.
2.  **Dynamic Targets**: Auto-updates goals when weight changes.
3.  **Food Logging**: Searchable database + Custom food creation.
4.  **Math-Based Recommendations**: Suggests meals based on what nutrients you are *missing* right now.
5.  **Detailed Reports**: End-of-day summary comparing intake vs. targets.
6.  **User Experience**: "Back" buttons in every menu, emoji-enhanced UI, and input validation for all fields.
