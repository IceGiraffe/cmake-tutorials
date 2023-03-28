#include <boost/array.hpp>
#include <iostream>

using boost::array;
using std::cin;
using std::cout;
using std::endl;

int main()
{
    array<int, 10> myArr;
    cout << myArr.at(0) << endl;
    return 0;
}