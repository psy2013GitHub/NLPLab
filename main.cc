//
// Created by System Administrator on 10/10/15.
//

#include <cstdio>
#include <vector>
#include <string>
#include "io/read.hh"
#include "utils/emo_util.hh"

using namespace std;

int main() {
    read::document doc("./test.utf8", "r");
    vector<string> line;
    emo_util::EmoUtil emo_inst;
    while (!doc.is_EOF()) {
         doc.readLine(line);
         if (line.size()) {
             // filter
             emo_inst.filter_emotion(line);
             //emo_inst.filter_emoji(line);
             //emo_inst.filter_dec(line);
             line.clear();;
         }
         else
             printf("空行\n");
    }
    doc.close();
    return 0;
}