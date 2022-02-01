#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Book {
    string title;
    string author;
    int page_c;

public:
    Book(string title, string author, int page_c)
        : title{ title }
        , author{ author }
        , page_c{ page_c }
    {}

    string get_title() { return title; }

    friend ostream& operator<<(ostream& otp, const Book& b) {
        return otp << "Book[" << b.title << " by " << b.author << " with " << b.page_c << " pages]";
    }

};

class LibraryRecord : public Book {
    int catalog_id;
    bool in_library;

public:
    LibraryRecord(Book&& book, bool in_library)
        : Book{ book }
        , in_library{ in_library }
    {
        static int last_catalog_id = 0;
        catalog_id = last_catalog_id++;
    }

    bool IsInLibrary() { return in_library; }

    bool TakeToRead() {
        auto res = in_library;
        in_library = false;
        return res;
    }

    bool ReturnToLibrary() {
        auto res = !in_library;
        in_library = true;
        return res;
    }

    friend ostream& operator<<(ostream& otp, const LibraryRecord& r) {
        return otp << "LibraryRecord#" << r.catalog_id << "[" << static_cast<const Book&>(r) << "; in_library: " << boolalpha << r.in_library << "]";
    }

};

int main()
{
    vector<LibraryRecord> library{};

    library.push_back({ {"book1","author1",200}, true });
    library.push_back({ {"book2","author1",300}, false });
    library.push_back({ {"book3","author2",240}, true });

    {
        cout << "Enter title for search: ";
        string title;
        cin >> title;

        LibraryRecord* found_r = nullptr;
        for (auto& r : library)
            if (r.get_title() == title) {
                found_r = &r;
                break;
            }
        cout << (found_r ? "Book found" : "Book not found") << endl;

        if (found_r) {
            cout << (found_r->IsInLibrary() ? "And it is in library" : "And it was taken") << endl;
        }
    }
    cout << endl;
    {
        for (auto& r : library)
            cout << '\t' << r << endl;

        auto& book_to_take = library[2];
        if (book_to_take.TakeToRead())
            cout << "Book " << book_to_take.get_title() << " successfully taken" << endl;
        else
            cout << "Error taking book " << book_to_take.get_title() << endl;

        for (auto& r : library)
            cout << '\t' << r << endl;

    }

}