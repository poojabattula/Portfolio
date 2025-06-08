#include <iostream>  // Required for input/output operations (e.g., std::cout, std::cin)
#include <string>    // Required for string manipulation (e.g., std::string)
#include <vector>    // Required for dynamic arrays (e.g., std::vector)
#include <sstream>   // Required for string stream operations (e.g., std::stringstream)
#include <unordered_map> // Required for hash map (e.g., std::unordered_map)
#include <algorithm> // Required for algorithms like std::transform, std::remove_if
#include <cctype>    // Required for character manipulation (e.g., ::tolower, ::ispunct)

// Define a global sentiment lexicon.
// This unordered_map stores words as keys and their associated sentiment scores as values.
// Positive scores for positive words, negative for negative, and zero for neutral (though neutral words often aren't explicitly in the lexicon).
std::unordered_map<std::string, int> sentiment_lexicon = {
    {"good", 1}, {"great", 2}, {"excellent", 2}, {"awesome", 2}, {"fantastic", 2}, {"amazing", 2},
    {"happy", 1}, {"joyful", 1}, {"love", 1}, {"like", 1}, {"positive", 1},

    {"bad", -1}, {"terrible", -2}, {"horrible", -2}, {"awful", -2}, {"disappointing", -1},
    {"sad", -1}, {"unhappy", -1}, {"hate", -1}, {"dislike", -1}, {"negative", -1},

    {"not", -1}, {"no", -1}, // These are treated as negation words
    {"very", 0.5}, {"extremely", 0.5}, {"really", 0.5}, // Simple boosters
    {"little", -0.5}, {"barely", -0.5} // Simple diminishers
};

/**
 * @brief Preprocesses the input text by converting it to lowercase and removing punctuation.
 * @param text The input string to preprocess.
 * @return The preprocessed string.
 */
std::string preprocess_text(const std::string& text) {
    std::string processed_text = text;

    // Convert all characters in the string to lowercase.
    std::transform(processed_text.begin(), processed_text.end(), processed_text.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    // Remove punctuation characters.
    // std::remove_if moves elements that satisfy the predicate to the end, and returns an iterator to the new end.
    // erase then removes these elements from the string.
    processed_text.erase(std::remove_if(processed_text.begin(), processed_text.end(),
                                       [](unsigned char c){ return std::ispunct(c) && c != '\'' && c != '-'; }), // Keep apostrophes and hyphens for contractions/compound words
                         processed_text.end());
    return processed_text;
}

/**
 * @brief Analyzes the sentiment of a given sentence using a lexicon-based approach.
 * It tokenizes the text, looks up words in a sentiment lexicon, and applies
 * basic negation and booster/diminisher logic.
 * @param sentence The input sentence to analyze.
 * @return An integer representing the sentiment score.
 * Positive for positive sentiment, negative for negative, and zero for neutral.
 */
int analyze_sentiment(const std::string& sentence) {
    std::string preprocessed_sentence = preprocess_text(sentence);
    std::stringstream ss(preprocessed_sentence);
    std::string word;
    double sentiment_score = 0.0;
    bool negated = false;
    double booster_multiplier = 1.0;

    // Iterate through each word (token) in the preprocessed sentence.
    while (ss >> word) {
        // Check for negation words.
        if (word == "not" || word == "no") {
            negated = true;
            booster_multiplier = 1.0; // Reset booster for negation context
            continue; // Move to the next word without scoring the negation word itself
        }

        // Check for booster/diminisher words.
        if (sentiment_lexicon.count(word)) {
            double lexicon_value = sentiment_lexicon[word];
            if (lexicon_value == 0.5) { // It's a booster
                booster_multiplier = 1.5; // Apply a booster effect
                continue;
            } else if (lexicon_value == -0.5) { // It's a diminisher
                booster_multiplier = 0.5; // Apply a diminisher effect
                continue;
            }
        }

        // If the word is in our sentiment lexicon.
        if (sentiment_lexicon.count(word)) {
            double word_sentiment = sentiment_lexicon[word];

            // Apply negation if applicable.
            if (negated) {
                word_sentiment *= -1; // Invert the sentiment
                negated = false; // Reset negation for the next words
            }

            // Apply booster/diminisher multiplier.
            word_sentiment *= booster_multiplier;
            booster_multiplier = 1.0; // Reset booster/diminisher for the next word

            sentiment_score += word_sentiment;
        } else {
            // If the word is not in the lexicon, and no negation/booster was active, reset them.
            negated = false;
            booster_multiplier = 1.0;
        }
    }

    // Convert the double score to an integer for final classification.
    // This simple thresholding can be refined for more granular sentiment.
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
        std::getline(std::cin, input_sentence); // Read the entire line

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
