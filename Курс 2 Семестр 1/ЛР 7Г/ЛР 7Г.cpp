#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include "CultureInfo.h"
using namespace std;

class FileWriter {
    fstream f;
    vector<streamoff> val_poss{};

    template <class T>
    void WriteStruct(const T& val) {
        f.write((char*)&val, sizeof(T));
    }
    template <class T>
    T ReadStruct() {
        T res{};
        f.read((char*)&res, sizeof(T));
        return res;
    }

public:
    FileWriter(string fname) :
        f{ fname, ios::in | ios::out | ios::binary | (std::filesystem::exists(fname) ? 0 : ios::trunc)}
    {
        if (!f) throw 0;
        f.seekg(0, ios::end);
        auto size = f.tellg();
        f.seekp(0, ios::beg);
        while (true) {
            auto pos = f.tellg();
            if (pos == size) break;
            val_poss.push_back(pos);
            ReadRecord();
            if (!f) throw 0;
        }
        
        //copy(val_poss.begin(), val_poss.end(), ostream_iterator<size_t>(cout, " "));
    }

    auto RecordCount() { return val_poss.size(); }

    void AppendRecord(const CultureInfo& val) {
        f.seekp(0, ios::end);
        val_poss.push_back(f.tellp());

        WriteStruct(val.get_name().length());
        f.write(val.get_name().c_str(), val.get_name().length());

        WriteStruct(val.get_type());
        WriteStruct(val.get_area());
        WriteStruct(val.get_effi());

        //cout << f.tellp() << endl;
    }

    CultureInfo ReadRecord() {

        string name;
        auto name_len = ReadStruct<decltype(name.length())>();
        name.resize(name_len);
        f.read(&name[0], name_len);

        auto type = ReadStruct<CultureInfo::CultureType>();
        auto area = ReadStruct<int>();
        auto effi = ReadStruct<float>();

        return { name, type, area, effi };
    }
    CultureInfo ReadRecord(size_t ind) {
        f.seekg(val_poss[ind]);
        return ReadRecord();
    }

    void Close() {
        f.close();
    }

};

int main()
{
    auto db_fname = "db.bin";

    cout << "Array size: ";
    size_t c;
    cin >> c;

    auto a = static_cast<CultureInfo*>( operator new[](c*sizeof(CultureInfo)) );
    for (size_t i = 0; i < c; ++i)
        new(&a[i]) CultureInfo(cin);

    FileWriter wr{ db_fname };

    while (true) {

#pragma warning( disable : 6385 )
        for (size_t i = 0; i < c; ++i)
            cout << a[i] << endl;
#pragma warning( default : 6385 )

        cout << endl;

        cout << "What to do?" << endl;
        cout << "0. Exit" << endl;
        cout << "1. Write record to file" << endl;
        cout << "2. Read record from file" << endl;
        cout << "3. Save all" << endl;
        cout << "4. Load all" << endl;
        cout << "5. Clear file" << endl;

        int action_id;
        cin >> action_id;
        if (!action_id) break;
        switch (action_id)
        {
        case 1: {
            cout << "Enter array index (0.." << c-1 << "): ";
            size_t ind;
            cin >> ind;
            if (ind < c)
                wr.AppendRecord(a[ind]); else
                cout << "Invalid index" << endl;
        } break;
        case 2: {
            auto f_size = wr.RecordCount();
            cout << "Enter array index (0.." << c-1 << ") and file index (0.." << f_size-1 << "): ";
            size_t a_ind, f_ind;
            cin >> a_ind >> f_ind;
            if (a_ind < c && f_ind < f_size)
                a[a_ind] = wr.ReadRecord(f_ind); else
                cout << "Invalid index" << endl;
        } break;
        case 3: {
            for (size_t i = 0; i < c; ++i)
                wr.AppendRecord(a[i]);
        } break;
        case 4: {
            operator delete[](a);
            c = wr.RecordCount();
            a = static_cast<CultureInfo*>(operator new[](c * sizeof(CultureInfo)));
            for (size_t i = 0; i < c; ++i)
                new (&a[i]) auto(wr.ReadRecord(i));
        } break;
        case 5: {
            wr.Close();
            remove( db_fname );
            wr = { db_fname };
        } break;
        default:
            cout << "Wrong action id";
        }

    }

    wr.Close();
    operator delete[](a);
}