
//
// Created by System Administrator on 10/10/15.
//

#ifndef NLPCPP_READ_HH
#define NLPCPP_READ_HH

#define MAX_STRING 100

#include <cstdio>
#include <vector>
#include <string>

using namespace std;

namespace read {

    class document {
    public:
        document(const string& infile, const char* mode): _fname(infile) {_file = fopen(infile.c_str(), mode); };
        document(const char* infile,  const char* mode): _fname(infile)  {_file = fopen(infile, mode); };
        ~document() {};

        string get_fname();

        void close();
        bool is_Open();
        bool is_EOF();

        int  readWord(string& );
        void readLine(vector<string>& );
        void readLine(string& );
        void readDoc(vector<vector<string> >& );

    private:
        string _fname;
        FILE* _file;
    };

}
#endif //NLPCPP_READ_HH
