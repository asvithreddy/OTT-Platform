#include <iostream>
#include <fstream>//file handling
#include <sstream>//parsing inputs and convertings strings to numbers 
#include <vector>//Vectors are the same as dynamic arrays with the ability to resize itself automatically when an element is inserted or deleted,
#include <stdexcept>//logic_error, domain_error, invalid_argument, length_error, out_of_range, runtime_error, range_error, overflow_error, and underflow_error.
using namespace std;
// Define a Movie class
class Movie {
private:
    string title;
    string genre;
    int duration; // Duration in minutes
//encapsulation is making members private and making methods/functions to access public
public:
    Movie(const string& title, const string& genre, int duration)
        : title(title), genre(genre), duration(duration) {}

    // Getter methods
    string getTitle() const {
        return title;
    }

    string getGenre() const {
        return genre;
    }

    int getDuration() const {
        return duration;
    }
};
//composition involves including an obj of one class as a member of another class
// Define a MoviesDatabase class using composition
class MoviesDatabase {
private:
    vector<Movie> movies;

public:
    // appendMovie appends a movie to the movies vector.
    void appendMovie(const Movie& movie) {
        movies.push_back(movie);// pre-defined function that is used to insert data or elements at the end of the vector or it pushes the element in the vector from the back 
    }//

    // Function to search for a movie by title
    Movie searchMovieByTitle(const string& title) const {
        for (const auto& movie : movies) {
            if (movie.getTitle() == title) {
                return movie;
            }
        }
        throw runtime_error("Movie not found.");// for this we use the library stdexcept
    }

    // Searches movies by genre in the database and returns a vector of matching movies.
    vector<Movie> searchMoviesByGenre(const string& genre) const {
        vector<Movie> result;
        for (const auto& movie : movies) {
            if (movie.getGenre() == genre) {
                result.push_back(movie);
            }
        }
        return result;
    }

    // aves the movies in the database to a text file. It opens the file, writes each movie's details in a comma-separated format, and prints a success message.
    void saveToFile(const string& filename) const {
        ofstream outputFile(filename);
        for (const auto& movie : movies) {
            outputFile << movie.getTitle() << "," << movie.getGenre() << "," << movie.getDuration() << "\n";
        }
        cout << "Movies saved to file successfully!\n";
    }
};

// Function to display movies in a given genre
void displayMoviesByGenre(const MoviesDatabase& database, const string& genre) {
    vector<Movie> foundMovies = database.searchMoviesByGenre(genre);

    if (foundMovies.empty()) {
        cout << "No movies found in the " << genre << " genre.\n";
    } else {
        cout << "Movies in the " << genre << " genre:\n";
        for (const auto& movie : foundMovies) {
            cout << movie.getTitle() << " (" << movie.getGenre() << ", Duration: " << movie.getDuration() << " minutes)\n";
        }
    }
}
//Exception handling
int main() {
    try {
        MoviesDatabase database;

        // Load movies from a text file if it exists
        ifstream inputFile("movies.txt");
        if (inputFile) {
            string line;
            while (getline(inputFile, line)) {
                // Assuming the format of each line in the file is: Title,Genre,Duration
                istringstream iss(line);// this breaks the line// sstream
                string title, genre;
                int duration;//ws is a whitespace in std
                char comma;

                if (iss >> title >> ws >> comma >> genre >> ws >> comma >> duration) {
                    database.appendMovie(Movie(title, genre, duration));
                }
            }
        }

        int choice;
        do {
            cout << "\nChoose an option:\n";
            cout << "1. Append a movie\n";
            cout << "2. Search for a movie\n";
            cout << "3. Search for movies by genre\n";
            cout << "4. Save movies to file\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    string title, genre;
                    int duration;

                    cout << "\nEnter details for the Movie:\n";
                    cout << "Title: ";
                    cin.ignore(); // Clear the input buffer
                    getline(cin, title);
                    cout << "Genre: ";
                    getline(cin, genre);
                    cout << "Duration (in minutes): ";
                    cin >> duration;

                    database.appendMovie(Movie(title, genre, duration));
                    cout << "Movie appended successfully!\n";
                    break;
                }

                case 2: {
                    string searchTitle;
                    cout << "\nEnter the title of the movie to search: ";
                    cin.ignore(); // Clear the input buffer
                    getline(cin, searchTitle);

                    try {
                        Movie foundMovie = database.searchMovieByTitle(searchTitle);
                        cout << "Found Movie: " << foundMovie.getTitle() << " (" << foundMovie.getGenre()
                                  << ", Duration: " << foundMovie.getDuration() << " minutes)\n";
                    } catch (const exception& e) {
                        cerr << "Error: " << e.what() << endl;
                    }

                    break;
                }

                case 3: {
                    string searchGenre;
                    cout << "\nEnter the genre to search movies: ";
                    cin.ignore(); 
                    getline(cin, searchGenre);

                    displayMoviesByGenre(database, searchGenre);
                    break;
                }

                case 4: {
                    // Save movies to file
                    database.saveToFile("movies.txt");
                    break;
                }

                case 0:
                    cout << "Exiting the program. Goodbye!\n";
                    break;

                default:
                    cout << "Invalid choice. Please enter a valid option.\n";
            }

        } while (choice != 0);

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}

// we choose composition over inheritance
// defining two classes (Movie and MoviesDatabase) and uses composition to include an object of one class (Movie) as a member of 
//another class (MoviesDatabase). Inheritance involves creating a new class by inheriting properties and behaviors from an existing class,
// and it is not utilized in the given code.

//Inheritance is a concept where a new class (subclass or derived class) can inherit attributes and behaviors from an existing class 
//(base class or parent class). If inheritance were used, you would see a relationship like
 //class DerivedClass : public BaseClass, but this is not present in the provided code.

