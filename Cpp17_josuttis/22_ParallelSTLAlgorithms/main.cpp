#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <execution>  // for the execution policy
#include <cstdlib>    // for atoi()
#include "timer.h"



void using_parallel_for_each()
{
    /*
    As you can see, using the parallel algorithms is in principle pretty easy:
    • Include header <execution>
    • Call the algorithms the way you would usually call algorithms with an additional first argument,
    which often will be simply std::execution::par.
    In this case we use the standard algorithm for_each() to compute the square root of the members
    value of all elements in the passed vector coll. Due to the additional first parameter std::execution::par
    we request the algorithm to run in parallel mode: 
    */

   int numElems{1'000'000};

    struct Data{
        double value; // initial value
        double sqrt; // parallel computed square root
    };

    // initialize NumElems values without square root:
    std::vector<Data> coll;
    coll.reserve(numElems);
    for(int i=0; i<numElems; ++i)
    {
        coll.push_back(Data{i * 4.37, 0});
    }

    // parallel computation of square roots:
    for_each(std::execution::par, coll.begin(), coll.end(), 
        [](auto& val)
        {
            val.sqrt = std::sqrt(val.value);
        });

    // loop to make measurements mature:
    for(int i{0}; i<50; ++i)
    {
        Timer t;
        //sequential execution
        for_each(std::execution::seq,
                 coll.begin(), coll.end(),
                 [] (auto& val)
                 {
                    val.sqrt = std::sqrt(val.value);
                 });
        t.printDiff("sequential: ");

        // parallel execution:
        for_each(std::execution::par,
                coll.begin(), coll.end(),
                [](auto& val) {
                val.sqrt = std::sqrt(val.value);
                });
        t.printDiff("parallel:   ");
        std::cout << '\n';
    }
    /*
    Again that is not a general proof where and when parallel algorithms are worth it. But it demonstrates
    that even for non-trivial numeric operations it can be worth to use them.
    The key is, it’s worth with
    • long operations
    • many many elements
    For example, using a parallel version of the algorithm count_if() counting the number of even
    elements in a vector of ints was never worth it; even not with 1,000,000,000 elements:
    */

}

void using_parallel_sort()
{

    int numElems{10'000};

    std::vector<std::string> coll;
    for(int i=0; i<numElems / 2; ++i)
    {
        coll.emplace_back("id" + std::to_string(i));
        coll.emplace_back("ID" + std::to_string(i));
    }

    // loop to make measurements mature:
    for(int i{0}; i<50; ++i)
    {
        Timer t;
        //sequential execution
        sort(std::execution::seq, coll.begin(), coll.end());
        t.printDiff("sequential: ");

        // parallel execution:
        sort(std::execution::par, coll.begin(), coll.end());
        t.printDiff("parallel:   ");
        std::cout << '\n';
    }
}

void seq_accumulate(long num)
{
    /*
    For example, reduce() was introduced as a parallel form of accumulate(), which “accumulates”
    all elements (you can define, which operation performs the “accumulation”). For example, consider
    the following usage of accumulate():
    */
   // create coll with num sequences of 1 2 3 4:
    std::vector<long> coll;
    coll.reserve(num * 4);
    for (long i=0; i < num; ++i) {
        coll.insert(coll.end(), {1, 2, 3, 4});
    }

    auto sum = std::accumulate(coll.begin(), coll.end(), 0L);
    std::cout << "accumulate(): " << sum << '\n';
}

void par_reduce(long num)
{
    /*
    For example, reduce() was introduced as a parallel form of accumulate(), which “accumulates”
    all elements (you can define, which operation performs the “accumulation”). For example, consider
    the following usage of accumulate():
    */
   // create coll with num sequences of 1 2 3 4:
    std::vector<long> coll;
    coll.reserve(num * 4);
    for (long i=0; i < num; ++i) {
        coll.insert(coll.end(), {1, 2, 3, 4});
    }

    auto sum = std::reduce(std::execution::par, coll.begin(), coll.end(), 0L);
    std::cout << "reduce(): " << sum << '\n';
}



int main()
{
    // using_parallel_for_each();
    // using_parallel_sort();
    
    seq_accumulate(1);
    seq_accumulate(100);
    seq_accumulate(1000000);
    par_reduce(1);
    par_reduce(100);
    par_reduce(1000000);
    return 0;
}  

