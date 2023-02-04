#include <iostream>
#include <string>
#include <algorithm>
#include <execution> 
#include <chrono> 
#include <map> 
#include <vector> 
#include <deque> 

/*
    C++17 therefore introduced the Boyer-Moore and Boyer-Moore-Horspool search algorithms and
    various interfaces to use them. They are especially for searching substrings in large texts, but can
    also improve finding subsequences in containers or ranges.

    The Boyer-Moore and the Boyer-Moore-Horspool searchers are well known algorithms that precompute
    tables (of hash values) before the search starts to improve the speed of the search if the
    search covers a text and/or substring of significant size. Using them, the algorithms require randomaccess
    iterators (instead of forward iterators, which is enough for a naive search()).
*/


// Using Searchers with search()

template <typename T>
double diff(const T& t0, const T& t1)
{
  return std::chrono::duration<double,std::milli>(t1 - t0).count();
}

void using_search()
{
    std::string text1{"red fox jump over edge with high power under the sunlight"};
    std::string sub1{"text"};

    // 1. String member find():
    std::size_t idx = text1.find(sub1);
    // 2. Algorithm search():
    auto pos = std::search(text1.begin(), text1.end(), sub1.begin(), sub1.end());
    // 3. Parallel algorithm search():
    auto pos1 = std::search(std::execution::par, text1.begin(), text1.end(), sub1.begin(), sub1.end());
    //4. Using a default_searcher:
    auto pos2 = std::search(text1.begin(), text1.end(), std::default_searcher{sub1.begin(), sub1.end()});
    // 5. Using a boyer_moore_searcher:
    auto pos3 = std::search(text1.begin(), text1.end(), std::boyer_moore_searcher{sub1.begin(), sub1.end()});
    // 6. Using a boyer_moore_horspool_searcher:
    auto pos4 = std::search(text1.begin(), text1.end(), std::boyer_moore_horspool_searcher{sub1.begin(), sub1.end()});


    int max = 1000;

    // create a very big string: a b c d e ... z aa bb cc dd ee ... 
    std::string text;
    text.reserve(max*max*30);
    for (int i{1}; i<=max; ++i) {
        for (char c{'a'}; c<='z'; ++c) {
            for (int j{1}; j<=i; ++j) {
                text.push_back(c); 
            }
        }
    }

    // init the substring we search for (max times 'k'):
    std::string substr(max, 'k');
    std::cout << "search substring of " << max
                << " chars in string with " << text.size() << " chars\n"; 

    // init searchers for reuse:
    std::boyer_moore_searcher bm{substr.begin(), substr.end()};
    std::boyer_moore_horspool_searcher bmh{substr.begin(), substr.end()};

    // map to store measurements under a specific name:
    std::map<std::string, std::vector<double>> durs;

    // multiple measurements to make numbers mature:
    for (int i=0; i<5; ++i) {
        std::chrono::steady_clock::time_point t0;
        std::chrono::steady_clock::time_point t1;

        std::string::iterator pos;

        // string member find():
        t0 = std::chrono::steady_clock::now();
        auto idx = text.find(substr);
        t1 = std::chrono::steady_clock::now();
        durs["find()"].push_back(diff(t0, t1));
        std::cout << "idx: " << idx << '\n'; 

        // search() algorithm:
        t0 = std::chrono::steady_clock::now();
        pos = std::search(text.begin(), text.end(), substr.begin(), substr.end());
        t1 = std::chrono::steady_clock::now();
        durs["search()"].push_back(diff(t0, t1));
        std::cout << "idx: " << pos - text.begin() << '\n'; 

        // parallel search() algorithm:
        t0 = std::chrono::steady_clock::now();
        pos = std::search(std::execution::par, text.begin(), text.end(), substr.begin(), substr.end());
        t1 = std::chrono::steady_clock::now();
        durs["par search()"].push_back(diff(t0, t1));
        std::cout << "idx: " << pos - text.begin() << '\n'; 

        // default_searcher:
        t0 = std::chrono::steady_clock::now();
        pos = std::search(text.begin(), text.end(), std::default_searcher(substr.begin(), substr.end()));
        t1 = std::chrono::steady_clock::now();
        durs["search(def)"].push_back(diff(t0, t1));
        std::cout << "idx: " << pos - text.begin() << '\n'; 


        // boyer_moore_searcher:
        t0 = std::chrono::steady_clock::now();
        pos = std::search(text.begin(), text.end(), std::boyer_moore_searcher(substr.begin(), substr.end()));
        t1 = std::chrono::steady_clock::now();
        durs["search(bm)"].push_back(diff(t0, t1));
        std::cout << "idx: " << pos - text.begin() << '\n'; 


        // boyer_moore_horspool_searcher:
        t0 = std::chrono::steady_clock::now();
        pos = std::search(text.begin(), text.end(), std::boyer_moore_horspool_searcher(substr.begin(), substr.end()));
        t1 = std::chrono::steady_clock::now();
        durs["search(bmh)"].push_back(diff(t0, t1));
        std::cout << "idx: " << pos - text.begin() << '\n';

         // reused boyer_moore_searcher:
        t0 = std::chrono::steady_clock::now();
        pos = bm(text.begin(), text.end()).first;
        t1 = std::chrono::steady_clock::now();
        durs["bm()"].push_back(diff(t0, t1));
        std::cout << "idx: " << pos - text.begin() << '\n'; 

        // reused boyer_moore_horspool_searcher:
        t0 = std::chrono::steady_clock::now();
        pos = bmh(text.begin(), text.end()).first;
        t1 = std::chrono::steady_clock::now();
        durs["bmh()"].push_back(diff(t0, t1));
        std::cout << "idx: " << pos - text.begin() << '\n'; 

    }


    // print measurements:
    for (const auto& [name, dvec] : durs) {
        std::cout << "\n" << name << ": ";
        double avg = 0;
        for (const auto& val : dvec) {
            std::cout << val << " ";
            avg += val;
        }
        std::cout << "ms\n";
        std::cout << "  avg: " << avg / static_cast<double>(dvec.size()) << "ms\n";
  }

}


void using_general_subsequence_searchers()
{
    /*
        Boyer-Moore and Boyer-Moore-Horspool were developed as string searchers. 
        However, C++17 adopts them as generic algorithms so that you can use them to find a subsequence of elements in a container or range.
    */
    int max = 1000;

    // create a very big vector:
    //  0 1 2 3 4 ... 9 00 11 22 33 44 ... 
    std::vector<int> coll;
    coll.reserve(max*max*10);
    for (int i{1}; i<=max; ++i) {
        for (int v{0}; v<=9; ++v) {
            for (int j{1}; j<=i; ++j) {
                coll.push_back(v);
            }
        }
    }

    // init the subsequence we search for (max times '4'):
    std::deque<int> sub(max, 4);
    std::cout << "search sequence of " << max << " ints in vector with " << coll.size() << " ints\n"; 

    // init searchers for reuse:
    std::boyer_moore_searcher bm{sub.begin(), sub.end()};
    std::boyer_moore_horspool_searcher bmh{sub.begin(), sub.end()};

    // map to store measurements under a specific name:
    std::map<std::string, std::vector<double>> durs;

    // multiple measurements to make numbers mature:
    for (int i=0; i<5; ++i) {
        std::chrono::steady_clock::time_point t0, t1;
        std::vector<int>::iterator pos;

        // search() algorithm:
        t0 = std::chrono::steady_clock::now();
        pos = std::search(coll.begin(), coll.end(),
                        sub.begin(), sub.end());
        t1 = std::chrono::steady_clock::now();
        durs["search()"].push_back(diff(t0, t1));
        std::cout << "idx: " << pos - coll.begin() << '\n'; 

        // parallel search() algorithm:
        t0 = std::chrono::steady_clock::now();
        pos = std::search(std::execution::par,
                        coll.begin(), coll.end(),
                        sub.begin(), sub.end());
        t1 = std::chrono::steady_clock::now();
        durs["par search()"].push_back(diff(t0, t1));
        std::cout << "idx: " << pos - coll.begin() << '\n'; 


        // default_searcher:
        t0 = std::chrono::steady_clock::now();
        pos = std::search(coll.begin(), coll.end(),
                            std::default_searcher(sub.begin(), sub.end()));
        t1 = std::chrono::steady_clock::now();
        durs["search(def)"].push_back(diff(t0, t1));
        std::cout << "idx: " << pos - coll.begin() << '\n'; 

        // boyer_moore_searcher:
        t0 = std::chrono::steady_clock::now();
        pos = std::search(coll.begin(), coll.end(),
                            std::boyer_moore_searcher(sub.begin(), sub.end()));
        t1 = std::chrono::steady_clock::now();
        durs["search(bm)"].push_back(diff(t0, t1));
        std::cout << "idx: " << pos - coll.begin() << '\n'; 

        // boyer_moore_horspool_searcher:
        t0 = std::chrono::steady_clock::now();
        pos = std::search(coll.begin(), coll.end(),
                        std::boyer_moore_horspool_searcher(sub.begin(), sub.end()));
        t1 = std::chrono::steady_clock::now();
        durs["search(bmh)"].push_back(diff(t0, t1));
        std::cout << "idx: " << pos - coll.begin() << '\n'; 

        // reused boyer_moore_searcher:
        t0 = std::chrono::steady_clock::now();
        pos = bm(coll.begin(), coll.end()).first;
        t1 = std::chrono::steady_clock::now();
        durs["bm()"].push_back(diff(t0, t1));
        std::cout << "idx: " << pos - coll.begin() << '\n'; 

        // reused boyer_moore_horspool_searcher:
        t0 = std::chrono::steady_clock::now();
        pos = bmh(coll.begin(), coll.end()).first;
        t1 = std::chrono::steady_clock::now();
        durs["bmh()"].push_back(diff(t0, t1));
        std::cout << "idx: " << pos - coll.begin() << '\n'; 
    }

    // print measurements:
    for (const auto& elem : durs) {
        std::cout << "\n" << elem.first << ": ";
        double avg = 0;
        for (const auto& val : elem.second) {
        std::cout << val << " ";
        avg += val;
        }
        std::cout << "ms\n";
        std::cout << "  avg: " << avg / elem.second.size() << "ms\n";
    }
}


/*
void using_search_directly()
{
    
    // Alternatively, you can use the function-call operator of the searcher, which returns a pair of both the begin and the end of a found subsequence.
    // The code would look as follows:
    

    std::boyer_moore_searcher bm{sub.begin(), sub.end()};

    for (auto begend = bm(text.begin(), text.end());
    begend.first != text.end();
    begend = bm(begend.second, text.end())) {
        std::cout << "found '" << sub << "' at index " << begend.first - text.begin() << '-' << begend.second - text.begin() << '\n';
    }   


    // However, because you can use std::tie() to reassign new values to structured bindings of a std::pair<>, you can simplify the code as follows:
    std::boyer_moore_searcher bm{sub.begin(), sub.end()};

    for (auto [beg, end] = bm(text.begin(), text.end());
        beg != text.end();
    std::tie(beg,end) = bm(end, text.end())) {
        std::cout << "found '" << sub << "' at index " << beg - text.begin() << '-'<< end - text.begin() << '\n';
    }

}
*/


int main()
{   
    // using_search();
    using_general_subsequence_searchers();

    return 0;
}  

