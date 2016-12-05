//
// Created by System Administrator on 10/10/15.
//

#include "read.hh"

namespace read{

    void document::close() {
        fclose(_file);
    }

    string document::get_fname() {
        return _fname;
    }

    bool document::is_Open() {
        return _file!=0;
    }

    bool document::is_EOF() { // if EOF, return 0
        if (!is_Open())
            return 0;
        return feof(_file)!=0;
    }

    int document::readWord(string& word) {
        int status = 0; // 0: '\n'; 1: word; 2: empty line with more
        unsigned int a, ch;
        a = 0;
        while (!is_EOF()) {
            ch = fgetc(_file);
            if (ch == 13) continue; // 13 代表'\n'，跳过初始换行符
            if ((ch == ' ') || (ch == '\t') || (ch == '\n')) {
                if (a > 0) {
                    if (ch == '\n') {
                        ungetc(ch, _file); // 下回看看这一行的作用！！！！！！！
                        status = 0; // 到达句子结尾
                    } else {
                        status = 1; // 到达单词结尾
                    }
                    break;
                }
                if (ch == '\n') { // a == 0 && ch == '\n'
                    word = "</s>"; // "</s>"表示一非空但是全为空白字符
                    status = 2;
                    return status;
                } else continue;
            }
            // printf("%c\n", ch);
            word.push_back(ch);
            ++a;
            // if (a >= MAX_STRING - 1) a--;   // Truncate too long words，太长字符则阉割
        }
        return status;
    }

    void document::readLine(vector<string>& line) {
        string word;
        int status;
        while(!is_EOF()) {
            status = readWord(word);
            if (word.size()) {
                switch (status) {
                    case 0:
                        line.push_back(word);
                        return;
                    case 1:
                        line.push_back(word);
                    case 2:; // skip "empty" empty line
                    default:
                        word.clear(); // !!! 包括 情况1，2
                        break;
                }
            }
        }
        return;
    }

    void document::readLine(string& line) { // 读一整行
        line.clear();
        unsigned int a, ch;
        a = 0;
        while (!is_EOF()) {
            ch = fgetc(_file);
            if (!a && ch == 13) continue; // 13 代表'\n'，跳过初始换行符
            if (0<a && ch == '\n') {
                return;
            }
            line.push_back(ch);
            ++a;
        }
        return;
    }

    void document::readDoc(vector<vector<string> >& doc) {
        vector<string> line;
        while (!is_EOF()) {
            readLine(line);
            if (line.size()) {
                doc.push_back(line);
                line.clear();
            }
        }
        return;
    }

}
