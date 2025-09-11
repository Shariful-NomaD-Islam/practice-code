#include <iostream>
#include <string>
#include "ThreadSafeCache.h"

int main()
{
    std::cout << "Starting ThreadSafeCache test..." << std::endl;
    
    ThreadSafeCache<int, std::string> cache([](int key)
                                            { return "Value_" + std::to_string(key); });

    std::cout << "Cache created successfully" << std::endl;

    auto val1 = cache.get(42); // Loads via lambda
    auto val2 = cache.get(42); // Hits cache

    if (val1)
        std::cout << "Loaded: " << *val1 << "\n";
    if (val2)
        std::cout << "Cached: " << *val2 << "\n";
        
    std::cout << "Test completed successfully!" << std::endl;
    return 0;
}
