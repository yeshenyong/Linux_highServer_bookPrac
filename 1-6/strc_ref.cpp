#include <iostream>
#include <string>
#include <memory>
struct free_throws
{
    std::string name;
    int made;
    int attempts;
    float percent;
};

void display(const free_throws & ft);
void set_pc(free_throws & ft);
free_throws & accumulate(free_throws & target, const free_throws & source);
const free_throws & clone1(free_throws & ft);
int main()
{

    free_throws one = {"Ifelsa Branch", 13, 14};
    free_throws two = {"Andor Knott", 10, 16};
    free_throws three = {"Ifelsa Branch", 7, 9};
    free_throws four = {"Ifelsa Branch", 5, 9};
    free_throws five = {"Ifelsa Branch", 6, 14};
    free_throws team = {"Ifelsa Branch", 0, 0};
    
    free_throws dup;
    display(one);
    accumulate(team, one);
    display(team);

    free_throws & join1 = clone1(one);
    std::cout << "join1 \n";
    display(join1);

    return 0;
}

//trun back cannot change left value
const free_throws & clone1(free_throws & ft)
{
    std::unique_ptr<free_throws> myunique;
    *myunique = ft;
    return *myunique;
    // free_throws *pt;
    // *pt = ft;
    // return *pt;
}

void display(const free_throws & ft)
{
    using namespace std;
    cout << "Name : " << ft.name << '\n';
    cout << "Made : " << ft.made << '\n';
    cout << "Attempts : " << ft.attempts << '\n';
    cout << "Percent : " << ft.percent << '\n';
}

void set_pc(free_throws & ft)
{
    if( ft.attempts != 0 )
        ft.percent = 100.0f * float(ft.made)/float(ft.attempts);
    else 
        ft.percent = 0;
}

free_throws & accumulate(free_throws & target, const free_throws & source)
{
    target.attempts += source.attempts;
    target.made += source.made;
    set_pc(target);
    return target;
}

