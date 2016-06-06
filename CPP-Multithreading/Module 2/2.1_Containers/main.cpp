#include <iostream>
#include <vector>
#include <map>


using namespace std;

struct Element {
    int factor;
    unsigned int degree;

    Element(int factor, unsigned int degree){
        this->factor = factor;
        this->degree = degree;
    }

    Element(std::string input) {
        int factor = 1;
        unsigned int degree = 1;

        int positionOfMultiplier = input.find('*');
        bool isMinus = input[0] == '-';
        bool isVariable = input.find('x') != -1;
        int positionOfDegree = input.find('^');

        if (isVariable) {
            if (positionOfDegree != -1) {
                std::string degreeString = input.substr(positionOfDegree + 1, input.length() - positionOfDegree - 1);
                degree = std::stoi(degreeString);
            }

            if (positionOfMultiplier != -1) {
                std::string factorString = input.substr(0, positionOfMultiplier);
                factor = std::stoi(factorString);
            } else if (isMinus) {
                factor = -1;
            }

        } else {
            factor = std::stoi(input);
            degree = 0;
        }

        this->factor = factor;
        this->degree = degree;
    }

    std::string toString(){
        std::string result = "";

        if (this->factor != 0) {
            result.append(std::to_string(this->factor));
            if (this->degree != 0) {
                if (this->degree == 1) {
                    result.append("*x");
                } else {
                    result.append("*x^" + std::to_string(this->degree));
                }
            }
        }

        return result;
    }

    void derivate() {
        int outputFactor = 0;
        unsigned int outputDegree = 0;

        if (this->degree != 0) {
            outputFactor = this->factor * this->degree;
            outputDegree = this->degree - 1;
        }

        this->factor = outputFactor;
        this->degree = outputDegree;
    }
};

std::vector<Element> parseInputString(std::string input) {

    std::vector<Element> result;

    if (input.length() > 0) {

        std::string workString = std::string(input);

        if (workString[0] != '-') {
            workString.insert(0,1,'+');
        }

        while (workString.length() != 0) {
            int rate = 0;
            if (workString[0] == '-') {
                rate = -1;
            } else {
                rate = 1;
            }

            workString.erase(0,1);

            int minusPosition = workString.find('-');
            int plusPosition = workString.find('+');

            if (minusPosition == -1) {
                minusPosition = workString.length();
            }

            if (plusPosition == -1) {
                plusPosition = workString.length();
            }

            int nextPartPosition = std::min(minusPosition, plusPosition);

            std::string currentElement = workString.substr(0,nextPartPosition);
            if (rate == -1) {
                currentElement.insert(0,1,'-');
            }

            workString.erase(0,nextPartPosition);

            result.push_back(Element(currentElement));
        }
    }

    return result;
}

std::string derivative(std::string polynomial) {
    std::vector<Element> polynom = parseInputString(polynomial);

    std::map<unsigned int, int> calculateAllrates;

    for (auto i = polynom.begin(); i != polynom.end();) {
        (*i).derivate();
        if ((*i).factor == 0) {
            polynom.erase(i);
        } else {

            int currentFactor = calculateAllrates[(*i).degree];
            currentFactor += (*i).factor;
            calculateAllrates[(*i).degree] = currentFactor;
            i++;
        }
    }

    polynom.clear();

    for (auto i = calculateAllrates.rbegin(); i != calculateAllrates.rend(); i++) {
        polynom.push_back(Element((*i).second,(*i).first));
    }

    std::string result;

    for(auto i = polynom.begin(); i != polynom.end(); i++) {
        std::string elementString = (*i).toString();
        if (result.length() > 0 && elementString[0] != '-') {
            elementString.insert(0,1,'+');
        }

        result.append(elementString);
    }

    if (result.length() == 0) {
        return "0";
    }

    return result;
}

bool testOfResults(std::string resultOutput, std::string checkedOutput) {
    return resultOutput == checkedOutput;
}

int main() {
    cout << "#- " << testOfResults(derivative("x^2+x"), "2*x+1") << endl;
    cout << "#- " << testOfResults(derivative("2*x^100+100*x^2"), "200*x^99+200*x") << endl;
    cout << "#- " << testOfResults(derivative("x^10000+x+1"), "10000*x^9999+1") << endl;
    cout << "#- " << testOfResults(derivative("-x^2-x^3"), "-3*x^2-2*x") << endl;
    cout << "#- " << testOfResults(derivative("x+x+x+x+x+x+x+x+x+x"), "10") << endl;
    cout << "#- " << testOfResults(derivative("x-x+x-x+x-x+x-x+x-x"), "0") << endl;


    return 0;
}