#include <iostream>  
#include <string>  
#include <vector>    
#include <sstream>  
#include <unordered_map>
#include <algorithm> 
#include <cctype>   
std::unordered_map<std::string, int> sentiment_lexicon = {
    {"good", 1}, {"great", 2}, {"excellent", 2}, {"awesome", 2}, {"fantastic", 2}, {"amazing", 2},
    {"happy", 1}, {"joyful", 1}, {"love", 1}, {"like", 1}, {"positive", 1},

    {"bad", -1}, {"terrible", -2}, {"horrible", -2}, {"awful", -2}, {"disappointing", -1},
    {"sad", -1}, {"unhappy", -1}, {"hate", -1}, {"dislike", -1}, {"negative", -1},

    {"not", -1}, {"no", -1}, 
    {"very", 0.5}, {"extremely", 0.5}, {"really", 0.5}, 
    {"little", -0.5}, {"barely", -0.5}
};
std::string preprocess_text(const std::string& text) {
    std::string processed_text = text;
    std::transform(processed_text.begin(), processed_text.end(), processed_text.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    processed_text.erase(std::remove_if(processed_text.begin(), processed_text.end(),
                                       [](unsigned char c){ return std::ispunct(c) && c != '\'' && c != '-'; }), 
                         processed_text.end());
    return processed_text;
}
int analyze_sentiment(const std::string& sentence) {
    std::string preprocessed_sentence = preprocess_text(sentence);
    std::stringstream ss(preprocessed_sentence);
    std::string word;
    double sentiment_score = 0.0;
    bool negated = false;
    double booster_multiplier = 1.0;

    while (ss >> word) {
     
        if (word == "not" || word == "no") {
            negated = true;
            booster_multiplier = 1.0; 
            continue; 
        }

        if (sentiment_lexicon.count(word)) {
            double lexicon_value = sentiment_lexicon[word];
            if (lexicon_value == 0.5) { 
                booster_multiplier = 1.5; 
                continue;
            } else if (lexicon_value == -0.5) { 
                booster_multiplier = 0.5; 
                continue;
            }
        }
        if (sentiment_lexicon.count(word)) {
            double word_sentiment = sentiment_lexicon[word];

            if (negated) {
                word_sentiment *= -1;
                negated = false;
            }

            word_sentiment *= booster_multiplier;
            booster_multiplier = 1.0; 

            sentiment_score += word_sentiment;
        } else {
        
            negated = false;
            booster_multiplier = 1.0;
        }
    }

    if (sentiment_score > 0) {
        return 1; // Positive
    } else if (sentiment_score < 0) {
        return -1; // Negative
    } else {
        return 0; // Neutral
    }
}

int main() {
    std::cout << "Sentiment Analysis in C++ (Lexicon-Based)" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Enter sentences to analyze sentiment (type 'exit' to quit)." << std::endl;

    std::string input_sentence;
    while (true) {
        std::cout << "\nEnter sentence: ";
        std::getline(std::cin, input_sentence);

        if (input_sentence == "exit") {
            break;
        }

        int sentiment_result = analyze_sentiment(input_sentence);

        std::cout << "Sentiment: ";
        if (sentiment_result > 0) {
            std::cout << "Positive" << std::endl;
        } else if (sentiment_result < 0) {
            std::cout << "Negative" << std::endl;
        } else {
            std::cout << "Neutral" << std::endl;
        }
    }

    std::cout << "Exiting sentiment analyzer. Goodbye!" << std::endl;
    return 0;
}
