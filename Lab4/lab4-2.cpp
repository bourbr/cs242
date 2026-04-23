#include <iostream>
#include <string>
#include <stack>
using namespace std;

bool matches(char open, char close) {
    return (open=='(' && close==')') ||
           (open=='{' && close=='}') ||
           (open=='[' && close==']');
}

void check(const string& s) {
    stack<char> st;
    cout << "\nInput: \"" << s << "\"\n";
    cout << "  Step  Char  Action              Stack (top→)\n";
    cout << "  " << string(50,'-') << "\n";

    int step = 0;
    for (char c : s) {
        step++;
        string action;
        if (c=='(' || c=='{' || c=='[') {
            st.push(c);
            action = "push '" + string(1,c) + "'";
        } else if (c==')' || c=='}' || c==']') {
            if (st.empty()) {
                action = "ERROR: closer with empty stack";
                cout << "  " << step << "     '" << c << "'  " << action << "\n";
                cout << "  RESULT: INVALID\n";
                return;
            }
            char top = st.top(); st.pop();
            if (matches(top, c))
                action = "pop '" + string(1,top) + "' matched '" + string(1,c) + "'";
            else {
                action = "ERROR: '" + string(1,top) + "' does not match '" + string(1,c) + "'";
                cout << "  " << step << "     '" << c << "'  " << action << "\n";
                cout << "  RESULT: INVALID\n";
                return;
            }
        } else {
            action = "skip";
        }

        // Print stack contents
        cout << "  " << step << "     '" << c << "'  "
             << action;
        // Print stack (we have to copy it to read without destroying)
        string stack_str = "";
        stack<char> tmp = st;
        while (!tmp.empty()) { stack_str += tmp.top(); stack_str += ' '; tmp.pop(); }
        cout << string(max(0,(int)(22-action.size())), ' ')
             << "[" << stack_str << "]\n";
    }

    if (st.empty())
        cout << "  RESULT: VALID\n";
    else
        cout << "  RESULT: INVALID (unclosed: " << st.size() << " opener(s) remain)\n";
}

int main() {
    check("({[]})");
    check("({[}])");
    check("((())");
    check("hello(world[!])");
    check("");
}
