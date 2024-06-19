#include<bits/stdc++.h>
using namespace std;

struct instruction {
    string alpha, beta, gama, delta;
    int epslon;
};

unordered_map<string, string> offsetmap;
stack<int> A, B, SP;
map<int, int> memory;
unordered_map<string, string> instructionMap;

bool check(const string &s) {
    for (char c : s) {
        if (!isdigit(c))
            return false;
    }
    return true;
}

int con(const string& str) {
    int result;
    stringstream ss(str);
    ss >> result;
    return result;
}

void changingLabelsIntoOffset(vector<instruction>& instructionList) {
    unordered_map<string, int> labelAddresses;

    // Find addresses of all labels
    for (int i = 0; i < instructionList.size(); ++i) {
        if (!instructionList[i].alpha.empty()) {
            labelAddresses[instructionList[i].alpha] = i;
        }
    }

    // Replace labels with values
    for (int i = 0; i < instructionList.size(); i++) {
        if (!instructionList[i].gama.empty() && instructionMap.find(instructionList[i].gama) != instructionMap.end()) {
            instructionList[i].gama = instructionMap[instructionList[i].gama];
            instructionList[i].epslon = con(instructionList[i].gama);
        } else if (instructionList[i].beta == "brz" || instructionList[i].beta == "brlz" || instructionList[i].beta == "br" || instructionList[i].beta == "call") {
            if (!instructionList[i].gama.empty() && !check(instructionList[i].gama)) {
                int reaching_address = i;
                int label_address = labelAddresses[instructionList[i].gama];
                offsetmap[instructionList[i].gama] = to_string(label_address - reaching_address);
                instructionList[i].gama = to_string(label_address - reaching_address);
                instructionList[i].epslon = label_address - reaching_address;
            }
        } else {
            instructionList[i].epslon = con(instructionList[i].gama);
        }
    }
}

void emulator(vector<instruction> &instructionList, int stop) {
    int i = 0;
    while (i != stop) {
        if (instructionList[i].beta == "ldc") {
            B.push(A.top());
            A.push(instructionList[i].epslon);
            i++;
        } else if (instructionList[i].beta == "adc") {
            A.push(A.top() + instructionList[i].epslon);
            i++;
        } else if (instructionList[i].beta == "ldl") {
            B.push(A.top());
            A.push(memory[SP.top() + instructionList[i].epslon]);
            i++;
        } else if (instructionList[i].beta == "stl") {
            memory[SP.top() + instructionList[i].epslon] = A.top();
            A.push(B.top());
            i++;
        } else if (instructionList[i].beta == "ldnl") {
            A.push(memory[A.top() + instructionList[i].epslon]);
            i++;
        } else if (instructionList[i].beta == "stnl") {
            memory[A.top() + instructionList[i].epslon] = B.top();
            i++;
        } else if (instructionList[i].beta == "add") {
            instructionList[i].epslon = A.top() + B.top();
            A.push(A.top() + B.top());
            i++;
        } else if (instructionList[i].beta == "sub") {
            instructionList[i].epslon = B.top() - A.top();
            A.push(B.top() - A.top());
            cout << endl << " B " << B.top() << " A " << A.top() << " pass " << instructionList[i].epslon << endl;
            i++;
        } else if (instructionList[i].beta == "shl") {
            int avalue = A.top();
            int bvalue = B.top();
            instructionList[i].epslon = (bvalue << avalue);
            A.push(bvalue << avalue);
            i++;
        } else if (instructionList[i].beta == "shr") {
            int avalue = A.top();
            int bvalue = B.top();
            instructionList[i].epslon = (bvalue >> avalue);
            A.push(bvalue >> avalue);
            i++;
        } else if (instructionList[i].beta == "adj") {
            SP.push(SP.top() + instructionList[i].epslon);
            i++;
        } else if (instructionList[i].beta == "a2sp") {
            instructionList[i].epslon = B.top();
            SP.push(A.top());
            A.push(B.top());
            i++;
        } else if (instructionList[i].beta == "sp2a") {
            instructionList[i].epslon = SP.top();
            A.push(SP.top());
            B.push(A.top());
            i++;
        } else if (instructionList[i].beta == "call") {
            B.push(A.top());
            A.push(i);
            i = i + instructionList[i].epslon;
        } else if (instructionList[i].beta == "return") {
            i = A.top();
            A.push(B.top());
            instructionList[i].epslon = B.top();
        } else if (instructionList[i].beta == "brz") {
            if (A.top() == 0) {
                i = i + instructionList[i].epslon;
            } else {
                i++;
            }
        } else if (instructionList[i].beta == "brlz") {
            if (A.top() < 0) {
                i = i + instructionList[i].epslon;
            } else {
                i++;
            }
        } else if (instructionList[i].beta == "br") {
            i = i + instructionList[i].epslon;
        } else {
            i++;
        }
        cout << endl << " mnemonic " << instructionList[i].beta << " value " << instructionList[i].gama << " fake " << instructionList[i].epslon << endl;
    }
}

void printMemoryContents(const map<int, int>& memory) {
    cout << "Memory contents:" << endl;
    cout << memory.size();
    for (const auto& pair : memory) {
        cout << "Memory address: " << pair.first << ", Value: " << pair.second << endl;
    }
}

int main() {
    int finish;
    vector<instruction> instructionList;
    ifstream infile("bubblesort.txt");
    // Input file containing assembly code
    string line;
    int workingAddress = 0;
    while (getline(infile, line)) {
        stringstream ss(line);
        string flag;
        bool label_found = false;

        string instructionL = "";
        string instructionM = "";
        string operand = "";

        while (ss >> flag) {
            if (flag[0] == ';') // Ignore comments
                break;

            if (!label_found && flag[flag.size() - 1] == ':') {
                // This flag is a label
                instructionL = flag.substr(0, flag.size() - 1);
                label_found = true;
            } else if (instructionM.empty()) {
                // This flag is the mnemonic
                instructionM = flag;
            } else {
                // This flag is the value
                operand = flag;
            }
        }
        if (!instructionL.empty() && !operand.empty() && instructionM == "data") {
            instructionMap[instructionL] = operand;
        }
        if (instructionM == "HALT") {
            instructionList.push_back({instructionL, instructionM, operand});
            finish = workingAddress; // Exit the loop when HALT is encountered
        }

        // Output the formatted instruction
        if (!instructionM.empty()) {
            instructionList.push_back({instructionL, instructionM, operand});
        }

        // Increment address only if the line is not a comment
        if (flag[0] != ';') {
            ++workingAddress;
        }
    }

    A.push(0);
    B.push(0);
    SP.push(0);
    cout << "hi";
    changingLabelsIntoOffset(instructionList);
    emulator(instructionList, finish);
    printMemoryContents(memory);

    return 0;
}
