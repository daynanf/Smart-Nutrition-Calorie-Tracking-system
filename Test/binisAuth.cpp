#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <limits>
using namespace std;

struct UserAccount {
    string username;
    string password;
    string fullName;
    int age;
    string gender;
    int height;
    int weight;
    string activityLevel;
};

void firstWellcomePage()
{
	cout << "\n"
     << "╔═══════════════════════════════════════════════════════════════════════╗\n"
     << "║ 🍏💪  WELCOME TO THE DAILY NUTRITION & CALORIE TRACKER  💪🍏 ║\n"
     << "╚═══════════════════════════════════════════════════════════════════════╝\n\n"
     << " 🆕  1. Register New User\n"
     << " 🔑  2. Login\n"
     << " ❌  3. Exit\n\n"
     << " ✨  Please Enter Your Choice: ";
}

void userDashboard()
{
	cout << "\n"
     << "╔═════════════════════════════════════════════════════════════════════════════╗\n"
     << "║ 🥗💪  WELCOME TO THE DAILY NUTRITION & CALORIE TRACKER  💪🥗 ║\n"
     << "╚═════════════════════════════════════════════════════════════════════════════╝\n\n";

cout << "╔═════════════════════════════════════════════════════════════════════════════╗\n"
     << "║                              🏠 USER DASHBOARD 🏠                             ║\n"
     << "╠═════════════════════════════════════════════════════════════════════════════╣\n"
     << "║ 1️⃣  View My Profile                         👤                                ║\n"
     << "║ 2️⃣  View Daily Nutrition Targets           📊                                 ║\n"
     << "║ 3️⃣  Add Food to Today's Log                📝                                 ║\n"
     << "║ 4️⃣  View Today's Consumption               🍽️                                 ║\n"
     << "║ 5️⃣  Add Custom Food                        🥘                                 ║\n"
     << "║ 6️⃣  Get Meal Recommendations               🍱                                 ║\n"
     << "║ 7️⃣  End Day & View Summary                 📅                                 ║\n"
     << "║ 8️⃣  Logout                                 🚪                                 ║\n"
     << "╚═════════════════════════════════════════════════════════════════════════════╝\n\n";

cout << "✨ Enter your choice: ";

}

// getValidUsername: for choice == '1' (register) ensure username NOT present.
//                    for choice == '2' (login) ensure username IS present.
void getValidUsername(string &username, char choice) 
{
    while (true) 
    {
        cout << "\nEnter Username: ";
        getline(cin, username);

        if (username.empty()) 
        {
            cout << "\nUsername cannot be empty! Please try again.\n";
            continue;
        }

        bool allDigits = true;
        for (char c : username) 
        {
            if (!isdigit(static_cast<unsigned char>(c))) 
            {
                allDigits = false;
                break;
            }
        }

        if (allDigits) 
        {
            cout << "\nInvalid username! Username cannot be only numbers. Please try again.\n";
            continue;
        } 

        ifstream f("users.txt");
        if (!f)
        {
            // If file does not exist:
            if (choice == '1') {
                // registering — it's fine, file will be created when appending
                break;
            } else {
                // login — no accounts yet
                cout << "No users found (users.txt missing). Please register first.\n";
                continue;
            }
        }
        else
        {
            string line;
            bool found = false;
            while (getline(f, line))
            {
                size_t pos = line.find("|");
                string fileusername = line.substr(0, pos);
                if (username == fileusername)
                {
                    found = true;
                    if (choice == '1') {
                        cout << "The username \"" << username << "\" already exists, go for another one!\n";
                    } else if (choice == '2') {
                        cout << "Hey, 👤" << username << "!\n";
                    }
                    break;
                }
            }
            f.close();

            if (choice == '1' && found) continue;   // registration: can't use existing username
            if (choice == '2' && !found) {          // login: username must exist
                cout << "Username not found. Please try again or register.\n";
                continue;
            }
        }

        // all checks passed
        break;
    }
}


// getValidPassword: now accepts username so it checks the stored password for that username
// Signature: void getValidPassword(string &password, char choice, const string &username)
void getValidPassword(string &password, char choice)
{
    // Registration: just ensure non-empty password
    if (choice == '1')
    {
        while (true)
        {
            cout << "\nEnter Password: ";
            getline(cin, password);

            if (password.empty())
            {
                cout << "\nPassword cannot be empty! Please try again.\n";
                continue;
            }
            break; // valid password for registration
        }
        return;
    }

    // Login
    if (choice == '2')
    {
        int chances = 3;

        while (chances > 0)
        {
            cout << "\nEnter Password: ";
            getline(cin, password);

            if (password.empty())
            {
                cout << "\nPassword cannot be empty!\n";
                continue;
            }

            ifstream f("users.txt");
            if (!f)
            {
                cout << "Error: Could not open users file.\n";
                exit(1);
            }

            bool matched = false;
            string line;

            while (getline(f, line))
            {
                // skip username
                size_t pos = line.find("|");
                line = line.substr(pos + 1);

                // extract password (your logic)
                pos = line.find("|");
                string filePassword = line.substr(0, pos);

                if (password == filePassword)
                {
                    matched = true;
                    break;
                }
            }
            f.close();

            if (matched)
            {
                // correct password → return to main
                return;
            }
            else
            {
                chances--;
                if (chances > 0)
                {
                    cout << "Wrong password, try again (you have "
                         << chances << " chances left)\n";
                }
                else
                {
                    cout << "😒 Sorry, run out of chances. Try some other time!\n";
                    exit(0); // stop program on failed login
                }
            }
        }
    }
}


void getValidFullname(string &username) 
{
    while (true) 
	{
        cout << "\nEnter Your Fullname: ";
        getline(cin, username);

        // Check if empty
        if (username.empty()) 
		{
            cout << "\nHey! Everyone has a name 😄. Yours can't be empty—try again!\n";


            continue; // Skip the rest and ask for input again
        }

        // Check if all characters are digits
        bool allDigits = true; // assume all characters are digits
        for (char c : username) 
		{
            if (!isdigit(c)) 
			{   // found a non-digit character
                allDigits = false; 
                break;            // no need to check further
            }
        }

        if (allDigits) 
		{ // username is only numbers → invalid
           cout << "\nOops! Numbers only? Come on, give your name some personality 😎\n";
            continue;
        } 
		
		// Valid username, exit loop
		break;
        
    }
}

void getValidAge(int &age) 
{
    while (true) 
	{
        cout << "\nEnter Age: ";
        cin >> age;

        // Check if input failed (not an integer)
        if (cin.fail()) 
		{
            cout << "Oops! That’s not a valid number. Please enter a whole number.\n";
            cin.clear(); // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
            continue;
        }

        // Clear leftover newline
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Check age range with funny/friendly messages
        if (age < 1) 
		{
            cout << "Whoa! I can't believe you're using our app as a little baby 😄. Try again!\n";
            continue;
        }

        if (age > 200) 
		{
            cout << "Hmm… haven't met anyone above 200 yet 😲. Please enter your real age.\n";
            continue;
        }

        break; // valid age
    }
}

void getValidGender(string &gender) 
{
    while (true) 
	{
        cout << "\nEnter Gender (M/F): ";
        getline(cin, gender);

        // Convert input to lowercase for simple comparison
        for (char &c : gender) c = tolower(c);

        if (gender == "m" || gender == "male") 
		{
            gender = "Male";  // store standardized
            break;
        } else if (gender == "f" || gender == "female") 
		{
            gender = "Female"; // store standardized
            break;
        } else 
		{
            cout << "In Ethiopia, genders are only Male or Female 😅. Please try again.\n";
        }
    }
}

// Function to safely get a valid height (in cm)
void getValidHeight(int &height) {
    while (true) {
        cout << "Enter Height (cm): ";
        cin >> height;

        if (cin.fail()) {
            cout << "Oops! That's not a valid number. Please enter a whole number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (height < 50) {
            cout << "Hmm… you're too short to be using this app 😄. Try a realistic height!\n";
            continue;
        }

        if (height > 300) {
            cout << "Wow! Haven't seen anyone that tall 😲. Enter a realistic height.\n";
            continue;
        }

        break; // valid height
    }
}

// Function to safely get a valid weight (in kg)
void getValidWeight(int &weight) {
    while (true) {
        cout << "Enter Weight (kg): ";
        cin >> weight;

        if (cin.fail()) {
            cout << "Oops! That's not a valid number. Please enter a whole number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (weight < 10) {
            cout << "Hmm… that weight is too low 😄. Enter a realistic weight.\n";
            continue;
        }

        if (weight > 500) {
            cout << "Whoa! Haven't seen anyone that heavy 😲. Enter a realistic weight.\n";
            continue;
        }

        break; // valid weight
    }
}

void getValidActivityLevel(string &activityLevel)
{
    const string allActivityLevels[] = {
        "Sedentary",
        "Lightly Active",
        "Moderately Active",
        "Very Active"
    };

    int choice;

    while (true)
    {
        cout << "\nSelect Activity Level:\n"
             << "1. Sedentary\n"
             << "2. Lightly Active\n"
             << "3. Moderately Active\n"
             << "4. Very Active\n"
             << "Enter choice: ";

        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice < 1 || choice > 4)
        {
            cout << "Choice out of range. Enter 1 to 4.\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        activityLevel = allActivityLevels[choice - 1];
        break;
    }
}
int main() {
	SetConsoleOutputCP(CP_UTF8); // enable UTF-8 output for emojis
    firstWellcomePage();

    char choice;
    cin >> choice;
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear leftover newline


    UserAccount ua;
    
    while(true)
    {
    	switch (choice) 
		{
	        case '1': 
			{
	            cout << "\n--- Register New User ---\n";
	
	            
			    getValidUsername(ua.username,choice);
		
				getValidPassword(ua.password,choice);
				
				getValidFullname(ua.fullName);
				
				getValidAge(ua.age);
				
				getValidGender(ua.gender);
				
				getValidHeight(ua.height);
			
				getValidWeight(ua.weight);
				
				getValidActivityLevel(ua.activityLevel);
	
	
	            // Append mode and raw string literal for file path
	            ofstream file("users.txt", ios::app);
	            if (!file.is_open()) {
	                cout << "Error: Could not open file. Make sure the directory exists!" << endl;
	                return 1;
	            }
	
	            file << ua.username << "|"
	                 << ua.password << "|"
	                 << ua.fullName << "|"
	                 << ua.age << "|"
	                 << ua.gender << "|"
	                 << ua.height << "|"
	                 << ua.weight << "|"
	                 <<ua.activityLevel<<"\n";
	            file.close();
	
	            cout << "\n--------------------------------------------\n"
	                 << " 🎉✅ Profile created successfully! 👤 \n"
	                 << "--------------------------------------------\n\n";
	                 
	                 
	            cout << "👉 Press Enter to login to your account...";
	            cin.ignore(numeric_limits<streamsize>::max(), '\n');
    			cin.get();
    			system("cls");
    			firstWellcomePage();
    			cin>>choice;
	            continue;
	        }
	
		    case '2': 
			{
			    cout << "\n--- Login ---\n";
			    string inputUsername, inputPassword;
			
			    getValidUsername(inputUsername, choice);
			    getValidPassword(inputPassword, choice);
			
			    cout << "\n\nLogin successful! \n";
			    cout << "Welcome back, 👤 " << inputUsername << "!\n\n";
			    cin.clear();
			    cout << "👉 Press Enter to continue.....";
	            cin.ignore(numeric_limits<streamsize>::max(), '\n');
    			cin.get();
    			
    			// Clear screen and show welcome
			    system("cls"); 
			
			    // Display user dashboard directly
			    userDashboard();
			
			    // You can later handle dashboard choices in another loop or function
			    return 0;  // exits main after showing dashboard choice prompt
			}

	
	        case '3':
	        {
        		cout << "\nThank you! Goodbye 👋\n";
            	exit(0);	
			}
	           	
	        default:
	        {
        		cout << "\nInvalid choice!\n";
        		
        		//asking again for another value of choice and go back to the while loop
        		cout << "\nEnter Your Choice: ";
			    cin >> choice;
			    cin.ignore(numeric_limits<streamsize>::max(), '\n');
			    continue;
			}
	    }	
	}
    return 0;
}