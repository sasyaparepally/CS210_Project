#include<bits/stdc++.h>
using namespace std;


struct  instruction{
		string alpha,beta,gama,delta;
};
multimap<string,string> datamem;
unordered_map<string,string> instructuctionMap;
int check(const string &s) {
    int index = 0;
    int l =s.length();
    while (index < l) {
        if (index == 0 && s[index] == '-') {
            ++index;
            continue;
        }
      else if (!isdigit(s[index])) {
            return 0;
        }
        ++index;
    }
    return 1;
}


string intoHex(int num) {
    stringstream stream;
    stream << hex << setw(6) << setfill('0') << num;
    return stream.str();
}
int convertStringToInt(const string& s) {
    return stoi(s);
}

string twoscomplement(const string &s) {
    int z = convertStringToInt(s);
string sixteen;
    // Convert the number to binary representation
    string binary = bitset<32>(z).to_string();

    if(z>=0){
            sixteen =intoHex(z);
        return sixteen;
    }
    else {
        // Convert 2^24 to binary representation
        string power24 = bitset<32>(1 << 24).to_string();

        // Add the binary representation of 2^24 to the binary representation of the number
        string result = "";
        int carry = 0;
        for (int i = 31; i >= 0; --i) {
            int sum = (binary[i] - '0') + (power24[i] - '0') + carry;
            result = to_string(sum % 2) + result;
            carry = sum / 2;
        }

        // Convert the result back to decimal
        z= stoi(result, nullptr, 2);
         sixteen =intoHex(z);
        return sixteen;
    }
}
 void changingLabelsIntoOffset(vector<instruction>& instructionList) {
    int workingAddress = 0;
    unordered_map<string, int> labelAddresses;

    // Find addresses of all labels
    for (int i = 0; i < instructionList.size(); ++i) {
        if (!instructionList[i].alpha.empty()) {
            labelAddresses[instructionList[i].alpha] = i;
        }
    }

    // Replace labels with values
    workingAddress = 0;

    for (int i = 0; i < instructionList.size(); ++i) {
        if (!instructionList[i].gama.empty() && instructuctionMap.find(instructionList[i].gama) != instructuctionMap.end()) {
            instructionList[i].gama = instructuctionMap[instructionList[i].gama];
        }

        if (instructionList[i].beta == "brz" || instructionList[i].beta== "brlz" || instructionList[i].beta == "br" || instructionList[i].beta == "call") {
            if (!check(instructionList[i].gama)) {
                int reaching_address = i;
                int label_address = labelAddresses[instructionList[i].gama];
                instructionList[i].gama = to_string(label_address-reaching_address);
            }
        }

        ++workingAddress;
    }
}

void handleErrors(const vector<string> &errorMessages, int numErrors) {
    if (numErrors > 0) {
        ofstream errorFile("ERROR.txt");
        if (errorFile.is_open()) {
            for (const string &error : errorMessages) {
                errorFile << error << endl;
            }
            errorFile.close();
            cout << "Errors have been written to ERROR.txt" << endl;
        } else {
            cout << "Failed to open ERROR.txt" << endl;
        }
        cout << "Total number of errors: " << numErrors << endl;
    } else {
        cout << "Compilation successful" << endl;
    }
}
void error(vector<instruction> &instructions, int finish) {
    int numErrors = 0;
    vector<string> errorMessages;
    string e;
    multimap<string, string>::iterator iter;
    for (iter = datamem.begin(); iter != datamem.end(); ++iter) {
        if (iter != datamem.end() && next(iter) != datamem.end() && iter->first == next(iter)->first) {
            string e = iter->first + " label repeated";
            errorMessages.push_back(e);
            numErrors++;
        }
    }
    for (int i = 0; i < instructions.size(); i++) {
        if (instructions[i].beta == "add" || instructions[i].beta == "a2sp" || instructions[i].beta == "sub" || instructions[i].beta == "shl" || instructions[i].beta == "shr" || instructions[i].beta == "sp2a" || instructions[i].beta == "return" || instructions[i].beta == "HALT") {
            if (instructions[i].gama != "") {
                errorMessages.push_back(intoHex(i) + " incorrect syntax");
                numErrors++;
            }
        } else if (instructions[i].beta == "ldc" || instructions[i].beta == "adc" || instructions[i].beta == "ldl" || instructions[i].beta == "stl" || instructions[i].beta == "ldnl" || instructions[i].beta == "stnl" || instructions[i].beta == "adj" || instructions[i].beta == "call" || instructions[i].beta == "brz" || instructions[i].beta == "brlz" || instructions[i].beta == "br" || instructions[i].beta=="data") {
            if (!check(instructions[i].gama)) {
                errorMessages.push_back(intoHex(i) + " not a number");
                numErrors++;
            }
        } else {
            numErrors++;
            errorMessages.push_back(intoHex(i) + " warning incorrect instructionM");
        }
    }
    handleErrors(errorMessages, numErrors);
}


void pass2(vector<instruction> &instructionList, int finish) {
    vector<string> print2;
    vector<string> obj;
    string s;

    for (int i = 0; i < instructionList.size(); i++) {
        string out;

        if (instructionList[i].beta == "ldc") {
            out = intoHex(i) + " ldc " + twoscomplement(instructionList[i].gama) + "00";
            s=twoscomplement(instructionList[i].gama) + "00";
        } else if (instructionList[i].beta == "adc") {
            out = intoHex(i) + " adc " + twoscomplement(instructionList[i].gama) + "01";
            s=twoscomplement(instructionList[i].gama) + "01";
        } else if (instructionList[i].beta == "ldl") {
            out = intoHex(i) + " ldl " + twoscomplement(instructionList[i].gama) + "02";
            s=twoscomplement(instructionList[i].gama) + "02";
        } else if (instructionList[i].beta == "stl") {
            out = intoHex(i) + " stl " + twoscomplement(instructionList[i].gama) + "03";
            s=twoscomplement(instructionList[i].gama) + "03";
        } else if (instructionList[i].beta == "ldnl") {
            out = intoHex(i) + " ldnl " + twoscomplement(instructionList[i].gama) + "04";
            s=twoscomplement(instructionList[i].gama) + "04";
        } else if (instructionList[i].beta == "stnl") {
            out = intoHex(i) + " stnl " + twoscomplement(instructionList[i].gama) + "05";
            s=twoscomplement(instructionList[i].gama) + "05";
        } else if (instructionList[i].beta == "add") {
            out = intoHex(i) + " add 00000006";
            s="00000006";
        } else if (instructionList[i].beta == "sub") {
            out = intoHex(i) + " sub 00000007";
            s="00000007";
        } else if (instructionList[i].beta == "shl") {
            out = intoHex(i) + " shl 00000008";
            s="00000008";
        } else if (instructionList[i].beta == "shr") {
            out = intoHex(i) + " shr 00000009";
            s="00000009";
        } else if (instructionList[i].beta == "adj") {
            out = intoHex(i) + " adj " + twoscomplement(instructionList[i].gama) + "0a";
            s=twoscomplement(instructionList[i].gama) + "0a";
        } else if (instructionList[i].beta == "a2sp") {
            out = intoHex(i) + " a2sp 0000000b";
            s="0000000b";
        } else if (instructionList[i].beta == "sp2a") {
            out = intoHex(i) + " sp2a 0000000c";
            s="0000000c";
        } else if (instructionList[i].beta == "call") {
            out = intoHex(i) + " call " + twoscomplement(instructionList[i].gama) + "0d";
            s=twoscomplement(instructionList[i].gama) + "0d";
        } else if (instructionList[i].beta == "return") {
            out = intoHex(i) + " return 0000000e";
            s="0000000e";
        } else if (instructionList[i].beta == "brz") {
            out = intoHex(i) + " brz " + twoscomplement(instructionList[i].gama) + "0f";
            s=twoscomplement(instructionList[i].gama) + "0f";
        } else if (instructionList[i].beta == "brlz") {
            out = intoHex(i) + " brlz " + twoscomplement(instructionList[i].gama) + "10";
            s=twoscomplement(instructionList[i].gama) + "10";
        } else if (instructionList[i].beta == "br") {
            out = intoHex(i) + " br " + twoscomplement(instructionList[i].gama) + "11";
            s=twoscomplement(instructionList[i].gama) + "11";
        } else if (instructionList[i].beta == "HALT") {
            out = intoHex(i) + " HALT 00000013";
            s="00000013";
        } else {
            // Handle unsupported instructions or errors
            out = "Unsupported instruction or error at line " + to_string(i);

        }

        print2.push_back(out);
        obj.push_back(s);
    }

    ofstream pass2File("listing.txt");
    if (pass2File.is_open()) {
        for (const string &st : print2) {
            pass2File << st << endl;
        }
        pass2File.close();
        cout << "listing.txt generated successfully." << endl;
    } else {
        cout << "Failed to open listing.txt for writing." << endl;
    }
    ofstream pass2Fil("obj.txt");
    if (pass2Fil.is_open()) {
        for (const string &st : obj) {
            pass2Fil << st << endl;
        }
        pass2Fil.close();
        cout << "obj.txt generated successfully." << endl;
    } else {
        cout << "Failed to open obj.txt for writing." << endl;
    }
}

void passOne(vector<instruction> &instructionList){
int finish;
    vector<string> print1;
    unordered_map<string, int> instructuctionL_addresses; // Map to store addresses of instructuctionLs
    int address = 0;

    for (int i = 0; i < instructionList.size(); i++) {
        // Check if the current instruction has a instructuctionL
        if (!instructionList[i].alpha.empty()) {
            // Assign address to the instructuctionL
            instructuctionL_addresses[instructionList[i].alpha] = address;
        }
        // Increment address
         if (instructionList[i].beta == "data") {
            // Increment address only if the instructionM is 'data'
            datamem.insert({instructionList[i].alpha, instructionList[i].gama});
            instructuctionMap.insert({instructionList[i].alpha, instructionList[i].gama});
        }
        // Check if instruction is HALT
        else if (instructionList[i].beta == "HALT") {
            finish=address; // finish processing further instructions

        }
         address++;

    }
// Output instructuctionLs with their addresses
    for (const auto& pair : instructuctionL_addresses) {
        string format1 = pair.first + " " + intoHex(pair.second);
        print1.push_back(format1);
    }

    ofstream pa("output.txt");
    if (pa.is_open()) {
        for (const string &str : print1) {
            pa << str << endl;
        }
        pa.close();
        cout << "Output written to output.txt successfully." << endl;
    } else {
        cout << "Error: Unable to open file for writing." << endl;
    }
error(instructionList,finish);

pass2(instructionList,finish);

//print1(vector)->address instructuctionL
}



int main() {
    instruction currentInstruction;
    vector<instruction> instructionList;
    ifstream inputFile("test.txt"); // Input file containing assembly code
    string flag;

    int workingAddress = 0;
    while (getline(inputFile, flag)) {
        stringstream ss(flag);
        string flag;
        int mark = 0; // Using an integer to track label detection

        string instructionL = "";
        string instructionM = "";
        string operand = "";

        while (ss >> flag) {
            if (flag[0] == ';')
                break;

            if (mark == 0 && flag.back() == ':') { // Check for colon at the end of the flag
                instructionL = flag.substr(0, flag.size() - 1);
                mark = 1; // Set labelFound to 1 when label is found
            } else if (instructionM == "") { // Check if instructionM is empty
                // This flag is the mnemonic
                instructionM = flag;
            } else {
                // This flag is the operand
                operand = flag;
            }
        }
        if (instructionL != "" && operand != "" && instructionM == "data") {
            instructuctionMap[instructionL] = operand;
        }
        if(instructionL != ""){
            datamem.insert({instructionL,operand});
        }
        // Output the formatted instruction
        if (instructionM != "") {
            currentInstruction.alpha = instructionL;
            currentInstruction.beta = instructionM;
            currentInstruction.gama = operand;
            instructionList.push_back(currentInstruction);
        }

        // Increment address only if the line is not a comment
        if (!(flag[0] == ';')) {
            ++workingAddress;
        }
    }

   changingLabelsIntoOffset(instructionList);
      passOne(instructionList);
    return 0;
}
