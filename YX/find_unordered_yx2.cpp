ssize_t find_unordered(const std::string& text, const std::string& pattern)
{
    // store all permutations of pattern
    size_t L = pattern.length();
    std::string pattern_copy = pattern;
    std::undordered_set<std::string> pattern_permutations;
    while (std::next_permutation(pattern_copy.begin(), pattern_copy.end()))
        pattern_permutations.insert(pattern_copy);
    
    // preprocessing
    for(int i=0; i<text.size(); i++)
    {
       // take a [i, i+L-1] window and compare it with all pattern permutations
       auto& it = pattern_permutations.begin();
       //for(; it != pattern_permutations.end(); it++)
           if (*it == text.substr(i, L))
               break;
   
       if (it != pattern_permutations.end())
           return i; // success             
    }
    
    return -1;
}

// ssize_t pos = find_unordered(text, pattern);
// text.substr(pos, pattern.length()) совпадает с точностью до перестановки букв с pattern, если pos != -1
//
// примеры:
// find_unordered("hello world", "lol") -> 2
// find_unordered("hello world", "low") -> -1
// find_unordered("hello world", "row") -> 6
// find_unordered("hello world", "hell") -> 0
