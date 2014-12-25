#include <iostream>
#include <cppassert/Assertion.hpp>

using namespace std;
int check_args(int size)
{
    const int ARGS_MIN = 2;
    (void)ARGS_MIN;
    //bool condition = (size!=0);
    CPP_ASSERT_ALWAYS(size!=0)<<"Size shouldnt be 0";
    return 0;
}

int main(int argc, char **)
{
    const int ARGS_MAX = 5;
    std::cerr<<"Hello "<<argc<<std::endl;
    CPP_ASSERT_LT(argc, ARGS_MAX)<<" argc is not lower than "
                                <<ARGS_MAX<<std::endl;
    std::cerr<<"World "<<std::endl;
    check_args(0);
    return 0;
}