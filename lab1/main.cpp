#include <vector>
#include <iostream>
#include <algorithm>

int main(){
    std::vector<int> v1;
    std::vector<char> v2;

    v2.push_back('z');
    v2.push_back('k');
    v2.insert(v2.begin()+1, 'c');
    for(int i = 0; i < (int)v2.size(); i++){
        std::cout << i;
        std::cout << ": ";
        std::cout << v2.at(i) << std::endl;
    }

    std::vector<char>::iterator it;

    std::cout << std::endl;

    for(auto x : v2){
        std::cout << x << std::endl;
    }

    std::cout << std::endl;
    
    it = std::find(v2.begin(), v2.end(), 'k');
    std::cout << &it << std::endl;
    std::cout << *it << std::endl;

    
    return 0;
}