#include <iostream>
#include <cmath>
#include <utility>



enum class equation_n_of_returns
{
    none=0, one, two, infinite
};



// уравнение вида a*x*x + b*x + c = 0
equation_n_of_returns solve_equation(int a, int b, int c, double& x1, double& x2)
{
    double ad = a;
    double bd = b;
    double cd = c;
    
    double D = bd*bd - 4*ad*cd;
    
    if (ad == 0 && bd != 0)
    {
        x1 = static_cast<double>(-cd/bd);
        return equation_n_of_returns::one;
    }
    else if (ad == 0 && bd == 0 && cd != 0)
    {
        return equation_n_of_returns::none;
    }
    else if (ad == 0 && bd == 0 && cd ==0)
    {
        return equation_n_of_returns::infinite;
    }
    
    if (D < 0)
    {
        return equation_n_of_returns::none;
    }
    
    if (D == 0)
    {
        x1 = static_cast<double>(-bd/(2*ad));
        return equation_n_of_returns::one;
    }
    
    if (D > 0)
    {
        x1 = static_cast<double>((-bd + std::sqrt(D))/(2*ad));
        x2 = static_cast<double>((-bd - std::sqrt(D))/(2*ad));
        
        return equation_n_of_returns::two;
    }
}




int main() 
{
    double x1 = 0;
    double x2 = 0;
    auto x = solve_equation(0, 5, 4, x1, x2);
    std::cout << (int)x << " " << x1 << " " << x2 << std::endl;
    
    return 0;
}

