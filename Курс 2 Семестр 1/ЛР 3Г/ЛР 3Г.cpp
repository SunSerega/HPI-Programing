
#include <iostream>
using namespace std;

class Shape {

protected:
    virtual void InitFrom(istream& inp) = 0;
public:
    friend istream& operator>>(istream& inp, Shape& s) {
        s.InitFrom(inp);
        return inp;
    }

public:
    virtual double GetArea() = 0;

};

class Triangle final : public Shape {
    double sides[3];

protected:
    virtual void InitFrom(istream& inp) {
        inp >> sides[0] >> sides[1] >> sides[2];
    }

public:
    virtual double GetArea() override {
        auto pp = (sides[0] + sides[1] + sides[2]) / 2;
        return sqrt(pp * (pp - sides[0]) * (pp - sides[1]) * (pp - sides[2]));
    }

};

class Rectangle final : public Shape {
    double w, h;

protected:
    virtual void InitFrom(istream& inp) {
        inp >> w >> h;
    }

public:
    virtual double GetArea() override {
        return w*h;
    }

};

class Circle final : public Shape {
    double r;

protected:
    virtual void InitFrom(istream& inp) {
        inp >> r;
    }

public:
    virtual double GetArea() override {
        return 3.14 * r * r;
    }

};

template <class TShape>
void Test(TShape&& s) {
    cout << "Enter data for " << typeid(TShape).name() << ": ";
    cin >> s;
    cout << "Area is " << s.GetArea() << endl;
}

int main()
{
    Test(Triangle{});
    Test(Rectangle{});
    Test(Circle{});
}