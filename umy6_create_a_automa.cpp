#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// Data structure to store chatbot conversations
struct Conversation {
    string user;
    string bot;
    string intent;
    double sentiment;
};

// Function to tokenize user input
vector<string> tokenizeInput(string input) {
    vector<string> tokens;
    string token;
    for (char c : input) {
        if (c == ' ') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to analyze user intent
string getIntent(vector<string> tokens) {
    // Simple intent recognition using keywords
    if (find(tokens.begin(), tokens.end(), "book") != tokens.end()) {
        return "booking";
    } else if (find(tokens.begin(), tokens.end(), "cancel") != tokens.end()) {
        return "cancellation";
    } else {
        return "unknown";
    }
}

// Function to analyze user sentiment
double getSentiment(vector<string> tokens) {
    // Simple sentiment analysis using keywords
    int positive = count(tokens.begin(), tokens.end(), "good") + count(tokens.begin(), tokens.end(), "great");
    int negative = count(tokens.begin(), tokens.end(), "bad") + count(tokens.begin(), tokens.end(), "terrible");
    return (double)positive / (positive + negative);
}

// Function to generate chatbot response
string getResponse(string intent, double sentiment) {
    if (intent == "booking" && sentiment > 0.5) {
        return "Awesome! I've booked your appointment. You'll receive a confirmation email soon.";
    } else if (intent == "cancellation" && sentiment < 0.5) {
        return "Sorry to hear that you need to cancel. Please let me know the reason and I'll assist you with the process.";
    } else {
        return "I didn't quite understand that. Can you please rephrase or provide more context?";
    }
}

int main() {
    ifstream conversationsFile("conversations.txt");
    if (!conversationsFile) {
        cout << "Error opening file." << endl;
        return 1;
    }

    vector<Conversation> conversations;
    string line;
    while (getline(conversationsFile, line)) {
        size_t pos = line.find(",");
        string user = line.substr(0, pos);
        line.erase(0, pos + 1);
        pos = line.find(",");
        string bot = line.substr(0, pos);
        line.erase(0, pos + 1);
        pos = line.find(",");
        string intent = line.substr(0, pos);
        line.erase(0, pos + 1);
        double sentiment = stod(line);

        Conversation conversation;
        conversation.user = user;
        conversation.bot = bot;
        conversation.intent = intent;
        conversation.sentiment = sentiment;

        conversations.push_back(conversation);
    }

    conversationsFile.close();

    for (Conversation conversation : conversations) {
        vector<string> tokens = tokenizeInput(conversation.user);
        string intent = getIntent(tokens);
        double sentiment = getSentiment(tokens);
        string response = getResponse(intent, sentiment);

        cout << "User: " << conversation.user << endl;
        cout << "Bot: " << response << endl;
        cout << "Intent: " << intent << endl;
        cout << "Sentiment: " << sentiment << endl;
        cout << endl;
    }

    return 0;
}