#include <bits/stdc++.h>
using namespace std;

class Bit {
private:
    int value;

public:
    Bit(int v = 0) {
        value = v & 1;
    }

    int get() const {
        return value;
    }

    Bit operator+(const Bit& other) const {
        return Bit(value ^ other.value);
    }

    Bit operator*(const Bit& other) const {
        return Bit(value & other.value);
    }
};

class BitString {
private:
    vector<Bit> bits;

public:
    BitString() {}

    BitString(string s) {
        for (char c : s) {
            bits.push_back(Bit(c - '0'));
        }
    }

    int size() const {
        return bits.size();
    }

    int get(int i) const {
        return bits[i].get();
    }

    string toString() const {
        string res = "";
        for (Bit b : bits) {
            res += char(b.get() + '0');
        }
        return res;
    }

    vector<int> toPolynomialVector() const {
        vector<int> a;

        for (int i = bits.size() - 1; i >= 0; i--) {
            a.push_back(bits[i].get());
        }

        return a;
    }
};

int main() {
    int n, k;
    string gx, r;

    cin >> n >> k;
    cin >> gx;
    cin >> r;

    BitString g(gx);
    BitString received(r);

    cout << "n = " << n << endl;
    cout << "k = " << k << endl;
    cout << "g(x) = " << g.toString() << endl;
    cout << "r = " << received.toString() << endl;

    return 0;
}

/*
PHAN 1:
- Xay dung lop Bit de bieu dien 1 bit 0 hoac 1.
- Tren GF(2), phep cong bit la XOR.
- Phep nhan bit la AND.

PHAN 2:
- Xay dung lop BitString de bieu dien day bit.
- Chuoi bit duoc dung de bieu dien da thuc nhi phan.
- Vi du: 1011 tuong ung voi da thuc x^3 + x + 1.
*/
