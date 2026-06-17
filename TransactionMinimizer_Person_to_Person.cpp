#include <bits/stdc++.h>
using namespace std;

class Friend {
public:
    string name;
    int netAmount;
    set<string> paymentModes;
};

int getMinIndex(Friend listOfNetAmounts[], int numFriends) {
    int min = INT_MAX, minIndex = -1;
    for (int i = 0; i < numFriends; i++) {
        if (listOfNetAmounts[i].netAmount == 0) continue;
        if (listOfNetAmounts[i].netAmount < min) {
            minIndex = i;
            min = listOfNetAmounts[i].netAmount;
        }
    }
    return minIndex;
}

int getSimpleMaxIndex(Friend listOfNetAmounts[], int numFriends) {
    int max = INT_MIN, maxIndex = -1;
    for (int i = 0; i < numFriends; i++) {
        if (listOfNetAmounts[i].netAmount == 0) continue;
        if (listOfNetAmounts[i].netAmount > max) {
            maxIndex = i;
            max = listOfNetAmounts[i].netAmount;
        }
    }
    return maxIndex;
}

pair<int, string> getMaxIndex(Friend listOfNetAmounts[], int numFriends, int minIndex, Friend input[], int maxNumPaymentModes) {
    int max = INT_MIN;
    int maxIndex = -1;
    string matchingPaymentMode;
    
    for (int i = 0; i < numFriends; i++) {
        if (listOfNetAmounts[i].netAmount == 0) continue;
        if (listOfNetAmounts[i].netAmount < 0) continue;

        vector<string> v(maxNumPaymentModes);
        vector<string>::iterator ls = set_intersection(listOfNetAmounts[minIndex].paymentModes.begin(), listOfNetAmounts[minIndex].paymentModes.end(), listOfNetAmounts[i].paymentModes.begin(), listOfNetAmounts[i].paymentModes.end(), v.begin());
        
        if ((ls - v.begin()) != 0 && max < listOfNetAmounts[i].netAmount) {
            max = listOfNetAmounts[i].netAmount;
            maxIndex = i;
            matchingPaymentMode = *(v.begin());
        }
    }
    
    return make_pair(maxIndex, matchingPaymentMode);
}

void printAns(vector<vector<pair<int, string>>> ansGraph, int numFriends, Friend input[]) {
    cout << "\nThe transactions for minimum cash flow are as follows: \n\n";
    for (int i = 0; i < numFriends; i++) {
        for (int j = 0; j < numFriends; j++) {
            if (i == j) continue;
            if (ansGraph[i][j].first != 0 && ansGraph[j][i].first != 0) {
                if (ansGraph[i][j].first == ansGraph[j][i].first) {
                    ansGraph[i][j].first = 0;
                    ansGraph[j][i].first = 0;
                } else if (ansGraph[i][j].first > ansGraph[j][i].first) {
                    ansGraph[i][j].first -= ansGraph[j][i].first;
                    ansGraph[j][i].first = 0;
                    cout << input[i].name << " pays Rs " << ansGraph[i][j].first << " to " << input[j].name << " via " << ansGraph[i][j].second << endl;
                } else {
                    ansGraph[j][i].first -= ansGraph[i][j].first;
                    ansGraph[i][j].first = 0;
                    cout << input[j].name << " pays Rs " << ansGraph[j][i].first << " to " << input[i].name << " via " << ansGraph[j][i].second << endl;
                }
            } else if (ansGraph[i][j].first != 0) {
                cout << input[i].name << " pays Rs " << ansGraph[i][j].first << " to " << input[j].name << " via " << ansGraph[i][j].second << endl;
            } else if (ansGraph[j][i].first != 0) {
                cout << input[j].name << " pays Rs " << ansGraph[j][i].first << " to " << input[i].name << " via " << ansGraph[j][i].second << endl;
            }
            ansGraph[i][j].first = 0;
            ansGraph[j][i].first = 0;
        }
    }
    cout << "\n";
}

// Returns {relayIndex, modeDebtorToRelay, modeRelayToCreditor}
// Returns {-1, "", ""} if no valid relay found
struct RelayResult { int index; string mode1; string mode2; };

RelayResult findRelay(Friend listOfNetAmounts[], int numFriends, 
                      int minIndex, int simpleMaxIndex, int maxNumPaymentModes) {
    for (int r = 0; r < numFriends; r++) {
        if (r == minIndex || r == simpleMaxIndex) continue;
        if (listOfNetAmounts[r].netAmount == 0) continue;
        
        // Check: relay shares a mode with debtor (minIndex)
        vector<string> v1(maxNumPaymentModes);
        auto it1 = set_intersection(
            listOfNetAmounts[minIndex].paymentModes.begin(), listOfNetAmounts[minIndex].paymentModes.end(),
            listOfNetAmounts[r].paymentModes.begin(), listOfNetAmounts[r].paymentModes.end(),
            v1.begin());
        if ((it1 - v1.begin()) == 0) continue; // no shared mode with debtor
        
        // Check: relay shares a mode with creditor (simpleMaxIndex)
        vector<string> v2(maxNumPaymentModes);
        auto it2 = set_intersection(
            listOfNetAmounts[r].paymentModes.begin(), listOfNetAmounts[r].paymentModes.end(),
            listOfNetAmounts[simpleMaxIndex].paymentModes.begin(), listOfNetAmounts[simpleMaxIndex].paymentModes.end(),
            v2.begin());
        if ((it2 - v2.begin()) == 0) continue; // no shared mode with creditor
        
        return {r, *v1.begin(), *v2.begin()};
    }
    return {0, // fallback to index 0 if no compatible relay found
            listOfNetAmounts[minIndex].paymentModes.empty() ? "" : *listOfNetAmounts[minIndex].paymentModes.begin(),
            listOfNetAmounts[simpleMaxIndex].paymentModes.empty() ? "" : *listOfNetAmounts[simpleMaxIndex].paymentModes.begin()};
}

void minimizeCashFlow(int numFriends, Friend input[], unordered_map<string, int>& indexOf, int numTransactions, vector<vector<int>>& graph, int maxNumPaymentModes) {
    Friend listOfNetAmounts[numFriends];
    
    for (int b = 0; b < numFriends; b++) {
        listOfNetAmounts[b].name = input[b].name;
        listOfNetAmounts[b].paymentModes = input[b].paymentModes;
        
        int amount = 0;
        for (int i = 0; i < numFriends; i++) {
            amount += (graph[i][b]);
        }
        for (int j = 0; j < numFriends; j++) {
            amount += ((-1) * graph[b][j]);
        }
        listOfNetAmounts[b].netAmount = amount;
    }
    
    vector<vector<pair<int, string>>> ansGraph(numFriends, vector<pair<int, string>>(numFriends, {0, ""}));
    int numZeroNetAmounts = 0;
    int settlementCount = 0;
    
    for (int i = 0; i < numFriends; i++) {
        if (listOfNetAmounts[i].netAmount == 0) numZeroNetAmounts++;
    }

    while (numZeroNetAmounts != numFriends) {
        int minIndex = getMinIndex(listOfNetAmounts, numFriends);
        pair<int, string> maxAns = getMaxIndex(listOfNetAmounts, numFriends, minIndex, input, maxNumPaymentModes);
        int maxIndex = maxAns.first;
        
        if (maxIndex == -1) {
            int simpleMaxIndex = getSimpleMaxIndex(listOfNetAmounts, numFriends);
            RelayResult relay = findRelay(listOfNetAmounts, numFriends, minIndex, simpleMaxIndex, maxNumPaymentModes);
            int relayIndex = relay.index;
    
            ansGraph[minIndex][relayIndex].first += abs(listOfNetAmounts[minIndex].netAmount);
            ansGraph[minIndex][relayIndex].second = relay.mode1;  // debtor → relay leg
    
            ansGraph[relayIndex][simpleMaxIndex].first += abs(listOfNetAmounts[minIndex].netAmount);
            ansGraph[relayIndex][simpleMaxIndex].second = relay.mode2;  // relay → creditor leg
    
            listOfNetAmounts[simpleMaxIndex].netAmount += listOfNetAmounts[minIndex].netAmount;
            listOfNetAmounts[minIndex].netAmount = 0;
            settlementCount += 2; // relay = 2 legs
    
            if (listOfNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;
            if (listOfNetAmounts[simpleMaxIndex].netAmount == 0) numZeroNetAmounts++;
        } else {
            int transactionAmount = min(abs(listOfNetAmounts[minIndex].netAmount), listOfNetAmounts[maxIndex].netAmount);
            
            ansGraph[minIndex][maxIndex].first += transactionAmount;
            ansGraph[minIndex][maxIndex].second = maxAns.second;
            
            listOfNetAmounts[minIndex].netAmount += transactionAmount;
            listOfNetAmounts[maxIndex].netAmount -= transactionAmount;
            settlementCount++;
            
            if (listOfNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;
            if (listOfNetAmounts[maxIndex].netAmount == 0) numZeroNetAmounts++;
        }
    }
    
    printAns(ansGraph, numFriends, input);
    if (numTransactions > 0) {
        double reduction = 100.0 * (numTransactions - settlementCount) / numTransactions;
        cout << "Original transactions entered: " << numTransactions << "\n";
        cout << "Minimum settlements computed:  " << settlementCount << "\n";
        cout << "Reduction:                     " << (int)reduction << "%\n\n";
    }
}

int main() {
    cout << "\n\t\t\t\t********************* Welcome to TRANSACTION MINIMIZER ***********************\n\n\n";
    cout << "This system minimizes the number of transactions among multiple friends in different parts of the world that use different modes of payment. \n\n";
    cout << "Enter the number of friends participating in the transactions.\n";
    int numFriends;
    cin >> numFriends;
    
    Friend input[numFriends];
    unordered_map<string, int> indexOf;
    
    cout << "Enter the names of the friends.\n";
    for (int i = 0; i < numFriends; i++) {
        cout << "Friend " << i + 1 << ": ";
        cin >> input[i].name;
        indexOf[input[i].name] = i;
    }
    
    cout << "Enter the number of transactions.\n";
    int numTransactions;
    cin >> numTransactions;
    
    vector<vector<int>> graph(numFriends, vector<int>(numFriends, 0));
    
    cout << "Enter the details of each transaction as stated:\n";
    cout << "Debtor Friend, Creditor Friend, and Amount\n";
    for (int i = 0; i < numTransactions; i++) {
        auto ordinal = [](int n) -> string {
            if (n % 100 >= 11 && n % 100 <= 13) return to_string(n) + "th";
            switch (n % 10) {
                case 1: return to_string(n) + "st";
                case 2: return to_string(n) + "nd";
                case 3: return to_string(n) + "rd";
                default: return to_string(n) + "th";
            }
        };
        cout << ordinal(i + 1) << " transaction: ";
        string debtor, creditor;
        int amount;
        cin >> debtor >> creditor >> amount;
        
        graph[indexOf[debtor]][indexOf[creditor]] = amount;
    }
    
    cout << "Enter the payment modes for each friend.\n";
    for (int i = 0; i < numFriends; i++) {
        cout << "Payment modes for " << input[i].name << " (separated by space, end with -1): ";
        string mode;
        while (cin >> mode && mode != "-1") {
            input[i].paymentModes.insert(mode);
        }
    }

    int maxNumPaymentModes = 0;
    for (int i = 0; i < numFriends; i++) {
        maxNumPaymentModes = max(maxNumPaymentModes, (int)input[i].paymentModes.size());
    }
    
    minimizeCashFlow(numFriends, input, indexOf, numTransactions, graph, maxNumPaymentModes);
    return 0;
}
