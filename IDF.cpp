#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cmath>
using namespace std;

// Clear the screen as per user OS
void clear_screen() {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

// Remove non-alphanumeric characters from a string
string remove_non_alphanumeric(const string& str) {
    string cleanedStr;
    for (char ch : str) {
        if (isalnum(static_cast<unsigned char>(ch)) || ch==' ') {
            cleanedStr += tolower(ch);
        }
    }
    return cleanedStr;
}

// Spliting words from a sentence
vector<string> split_by_whitespace(const string& str) {
    vector<string> words;
    istringstream iss(str);
    string word;
    while (iss>>word) {
        words.push_back(word);
    }    
    return words;
}

// Finding cosine similarity between two sentences
double cosine_similarity(vector<double> query_vec_rep,vector<double> vec_rep) {
    double dot_product = 0;
    double sqrt_sq_sum_query = 0;
    double sqrt_sq_sum_vec = 0;

    for(int i=0;i<query_vec_rep.size();i++) {
        dot_product += query_vec_rep[i]*vec_rep[i];
        sqrt_sq_sum_query += pow(query_vec_rep[i],2);
        sqrt_sq_sum_vec += pow(vec_rep[i],2);
    }

    if (sqrt_sq_sum_query == 0 || sqrt_sq_sum_vec == 0) return 0;

    sqrt_sq_sum_query = sqrt(sqrt_sq_sum_query);
    sqrt_sq_sum_vec = sqrt(sqrt_sq_sum_vec);

    return dot_product/(sqrt_sq_sum_query*sqrt_sq_sum_vec);
}

int main() {
    clear_screen();
    
    int n;
    map<string,int> unique_words;
    cout<<"Enter number of documents: ";
    cin>>n;
    cin.ignore(); //used getline below int

    vector<string> docs(n);
    vector<map<string,int>> doc_words(n);
    
    for(int i=0;i<n;i++) {
        cout<<"D"<<i+1<<": ";
        getline(cin,docs[i]);
        docs[i] = remove_non_alphanumeric(docs[i]);
    }

    for(int i=0;i<n;i++) {
        vector<string> words = split_by_whitespace(docs[i]);
        for (const auto& word : words) {
            unique_words[word]++;
            doc_words[i][word]++;
        }
    }

    vector<vector<double>> vec_rep(n);

    for(int i=0;i<n;i++) {
        map<string,int>::iterator it = unique_words.begin();
        while (it!=unique_words.end()) {
            if(doc_words[i][it->first]) {
                vec_rep[i].push_back(doc_words[i][it->first]*log10(n/it->second));
            }
            else {
                vec_rep[i].push_back(0);
            }
            it++;
        }
    }
    cout<<endl;
    for(int i=0;i<n;i++) {
        cout<<"[";
        for(int j=0;j<unique_words.size();j++) {
            if(j==0) {
                cout<<vec_rep[i][j];
            }
            else {
                cout<<", "<<vec_rep[i][j];
            }
        }
        cout<<"]"<<endl;
    }

    cout<<endl;

    // Take user query and match it with docs
    string query;
    cout<<"Enter your query: ";
    getline(cin,query);
    query = remove_non_alphanumeric(query);
    vector<string> query_words = split_by_whitespace(query);
    map<string,int> query_doc_words;
    for (const auto& word : query_words) {
        query_doc_words[word]++;
    }
    
    vector<double> query_vec_rep;

    map<string,int>::iterator it_query = unique_words.begin();
    while (it_query!=unique_words.end()) {
        if(query_doc_words[it_query->first]) {
            query_vec_rep.push_back(query_doc_words[it_query->first]*log10(n/it_query->second));
        }
        else {
            query_vec_rep.push_back(0);
        }
        it_query++;
    }

    cout<<endl;
    
    for(int i=0;i<n;i++) {
        cout<<"D"<<i+1<<" similarity: "<<cosine_similarity(query_vec_rep,vec_rep[i])<<endl;
    }
    cout<<endl;
    
    return 0;
}