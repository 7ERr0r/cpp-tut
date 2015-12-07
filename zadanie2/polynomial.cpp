#include <iostream>
#include <initializer_list>
#include <vector>



class polynomial{
public:

    polynomial(std::initializer_list<int> clist){
        coefficients.insert(coefficients.begin(), clist.begin(), clist.end());
    }
    template<typename ... T>
    polynomial(T... args){
        fill_recursively(args...);
    }
    polynomial(int n){
        coefficients.resize(n, 1);
    }
    polynomial(){
        coefficients.resize(1, 0);
    }
    polynomial operator+(polynomial& other){
        polynomial result;
        result.coefficients.resize(std::max(
            coefficients.size(),
            other.coefficients.size()
        ));
        // Stuck here

    }


    /* Evaluates the polynomial */
    int operator()(int argument){
        int value = 0;
        int multiplier = 1;

        auto it = coefficients.rbegin();
        while(it != coefficients.rend()){
            value += (*it)*multiplier;
            multiplier *= argument;
            it++;
        }
        return value;
    }
private:
    std::vector<int> coefficients;


    template<typename ... T>
    void fill_recursively(int first, T... rest){
        coefficients.push_back(first);
        fill_recursively(rest...);
    }
    template<typename ... T>
    void fill_recursively(int last){
        coefficients.push_back(last);
    }
    /* Friend with the marshal function */
    friend std::ostream& operator<<(std::ostream&, const polynomial&);

};
/* Actual marshal function */
std::ostream& operator<<(std::ostream &stream, const polynomial &p) {
    for(int coefficient : p.coefficients){
        stream << coefficient << " ";
    }
    return stream;
}

int main(){
    polynomial a(1,2,3);

    std::cout << a(4);


}
