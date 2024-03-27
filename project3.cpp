#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

inline bool caseInsensitiveStringCompare(const string &str1, const string &str2)
{
    return str1.size() == str2.size() && equal(str1.begin(), str1.end(), str2.begin(), str2.end(), [](char a, char b)
    { return tolower(a) == tolower(b); });
}
namespace Mynamespace
{
    class User
    {}
        string username;
        string password;

    public:
        static vector<User> userDatabase;
        User(const string &u, const string &p) : username(u), password(p) {}
        static bool loginUser(const string &username, const string &password);
    };

    bool User::loginUser(const string &username, const string &password)
    {
        for (const User &user : User::userDatabase)
        {
            if (caseInsensitiveStringCompare(user.username, username) && user.password == password)
            {
                return true;
            }
        }
        return false;
    }
}

using namespace Mynamespace;

vector<User> Mynamespace::User::userDatabase={};

class MediaContent
{
    string videoFile;
public:
    string title;
    string description;
    string genre;
    string language;
    string mediaType;
    MediaContent(const string &t, const string &d, const string &v, const string &g, const string &l)
        : title(t), description(d), videoFile(v), genre(g), language(l) {}
    virtual void setMediaType()
    {
        this->mediaType = "";
    }
    friend void displayContent(const string &genreFilter, const string &languageFilter, const vector<MediaContent *> &contentDatabase);

};
class Movie : public MediaContent
{
public:
    Movie(const string &t, const string &d, const string &v, const string &g, const string &l)
        : MediaContent(t, d, v, g, l) {}
    void setMediaType()
    {
        this->mediaType = "movie";
    }
};

class TVShow : public MediaContent
{
public:
    TVShow(const string &t, const string &d, const string &v, const string &g, const string &l)
        : MediaContent(t, d, v, g, l) {}
    void setMediaType()
    {
        this->mediaType = "tvshow";
    }
};


void displayContent(const string &genreFilter, const string &languageFilter, const vector<MediaContent *> &contentDatabase)
{
    cout << "Available Content:\n";
    for (size_t i = 0; i < contentDatabase.size(); ++i)
    {
        const MediaContent *content = contentDatabase[i];
        if ((genreFilter.empty() || caseInsensitiveStringCompare(content->genre, genreFilter)) && (languageFilter.empty() || caseInsensitiveStringCompare(content->language, languageFilter)))
        {
            cout << i + 1 << ". " << content->title << " - " << content->description << " (" << content->genre << ", " << content->language << ")\n";
        }
    }
}

int main()
{
    vector<MediaContent *> contentDatabase;

    ifstream userFile("users.txt");
    if (!userFile)
    {
        cerr << "Error: Unable to open users file." << endl;
        return 1;
    }

    string username, password;
    while (userFile >> username >> password)
    {
        User::userDatabase.push_back(User(username, password));
    }

    userFile.close();

    ifstream contentFile("content.txt");
    if (!contentFile)
    {
        cerr << "Error: Unable to open content file." << endl;
        return 1;
    }

    string line;
    while (getline(contentFile, line))
    {
        istringstream iss(line);
        string title, description, videoFile, genre, language, mediaType;
        getline(iss, title, ',');
        getline(iss, description, ',');
        getline(iss, videoFile, ',');
        getline(iss, genre, ',');
        getline(iss, language, ',');
        getline(iss, mediaType, ',');
        if (mediaType == "movie")
        {
            contentDatabase.push_back(new Movie(title, description, videoFile, genre, language));
        }
        else if (mediaType == "tvshow")
        {
            contentDatabase.push_back(new TVShow(title, description, videoFile, genre, language));
        }
    }

    contentFile.close();

    auto print = []() -> void
    { cout << "Welcome to the Case-Insensitive Command-Line OTT Platform!\n"; };

    bool loggedIn = false;

    while (true)
    {
        if (!loggedIn)
        {
            cout << "\n1. Login\n2. Exit\n";
            cout << "Enter your choice: ";
            string choiceStr;
            cin >> choiceStr;
            transform(choiceStr.begin(), choiceStr.end(), choiceStr.begin(), ::tolower);

            if (choiceStr == "1")
            {
                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                if (User::loginUser(username, password))
                {
                    loggedIn = true;
                    cout << "Login successful! Enjoy the content.\n";
                }
                else
                {
                    cout << "Login failed. Please try again.\n";
                }
            }
            else if (choiceStr == "2")
            {
                cout << "Thank you for using the OTT platform. Goodbye!\n";
                break;
            }
        }
        else
        {
            string genreFilter, languageFilter;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nEnter genre (case-insensitive) (or press Enter for all): ";
            getline(cin, genreFilter);
            cout << "Enter language (case-insensitive) (or press Enter for all): ";
            getline(cin, languageFilter);
            displayContent(genreFilter, languageFilter, contentDatabase);

            int movieChoice;
            cout << "\nEnter the number of the movie you want to watch (Enter 0 to exit): ";
            cin >> movieChoice;
            if (movieChoice > 0 && movieChoice <= static_cast<int>(contentDatabase.size()))
            {
                cout << "Playing: " << contentDatabase[movieChoice - 1]->title << "...\n";
            }
            else if (movieChoice == 0)
            {
                cout << "Thank you for using the OTT platform. Goodbye!\n";
                break;
            }
            else
            {
                cout << "Invalid movie choice. Please try again.\n";
            }
        }
    }

    // Deallocate memory
    for (MediaContent *content : contentDatabase)
    {
        delete content;
    }
    contentDatabase.clear();

    return 0;
}
