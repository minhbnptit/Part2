#include <iostream>
#include <vector>
#include <string>
using namespace std;

// ============================================================
//  LỚP ÐA THỨC TRÊN GF(2)
//  Luu hệ số dưới dạng vector<int>: coef[i] = hệ số x^i
// ============================================================
class GF2Polynomial {
private:
    vector<int> coef;

    void normalize() {
        while (coef.size() > 1 && coef.back() == 0)
            coef.pop_back();
    }

public:
    // --- Khởi tạo ---
    GF2Polynomial() : coef(1, 0) {}

    GF2Polynomial(const vector<int>& c) : coef(c) { normalize(); }

    // --- Truy cập ---
    int degree() const { return (int)coef.size() - 1; }

    int operator[](int i) const {
        return (i < (int)coef.size()) ? coef[i] : 0;
    }

    const vector<int>& getCoef() const { return coef; }

    // Trọng số Hamming
    int weight() const {
        int w = 0;
        for (int x : coef) w += x;
        return w;
    }

    bool isZero() const {
        for (int x : coef) if (x) return false;
        return true;
    }

    // --- Phép toán GF(2) ---

    // Cộng (XOR)
    GF2Polynomial operator+(const GF2Polynomial& other) const {
        int n = max(coef.size(), other.coef.size());
        vector<int> res(n, 0);
        for (int i = 0; i < n; i++)
            res[i] = (*this)[i] ^ other[i];
        return GF2Polynomial(res);
    }

    // Nhân
    GF2Polynomial operator*(const GF2Polynomial& other) const {
        vector<int> res(coef.size() + other.coef.size() - 1, 0);
        for (int i = 0; i < (int)coef.size(); i++)
            for (int j = 0; j < (int)other.coef.size(); j++)
                res[i + j] ^= (coef[i] & other.coef[j]);
        return GF2Polynomial(res);
    }

    // Chia lấy dư: this mod divisor
    GF2Polynomial mod(const GF2Polynomial& divisor) const {
        vector<int> rem = coef;
        int degDiv = divisor.degree();

        while ((int)rem.size() - 1 >= degDiv) {
            if (rem.back() == 0) { rem.pop_back(); continue; }
            int shift = (int)rem.size() - 1 - degDiv;
            for (int i = 0; i <= degDiv; i++)
                rem[i + shift] ^= divisor[i];
            while (rem.size() > 1 && rem.back() == 0)
                rem.pop_back();
        }
        return GF2Polynomial(rem);
    }

    // --- In ---
    // Dạng nhị phân "a0 a1 ... an"
    string toBitString(int len = -1) const {
        if (len == -1) len = (int)coef.size();
        string s;
        for (int i = 0; i < len; i++)
            s += (char)('0' + (*this)[i]);
        return s;
    }

    // Dạng đa thức "1 + x + x^3"
    string toPolyString() const {
        string s;
        bool first = true;
        for (int i = 0; i < (int)coef.size(); i++) {
            if (!coef[i]) continue;
            if (!first) s += " + ";
            if      (i == 0) s += "1";
            else if (i == 1) s += "x";
            else             s += "x^" + to_string(i);
            first = false;
        }
        return first ? "0" : s;
    }
};

// ============================================================
//  LỚP VECTOR NHỊ PHÂN  –  từ mã / véc-tơ thu
// ============================================================
class BinaryVector {
private:
    vector<int> bits;

public:
    BinaryVector() {}

    BinaryVector(const vector<int>& b) : bits(b) {}

    // Chuyển qua lại với GF2Polynomial
    BinaryVector(const GF2Polynomial& p, int len) {
        bits.resize(len, 0);
        for (int i = 0; i < len; i++)
            bits[i] = p[i];
    }

    GF2Polynomial toPoly() const { return GF2Polynomial(bits); }

    // --- Truy cập ---
    int  size()            const { return (int)bits.size(); }
    int  operator[](int i) const { return bits[i]; }
    int& operator[](int i)       { return bits[i]; }

    // Trọng số Hamming
    int weight() const {
        int w = 0;
        for (int x : bits) w += x;
        return w;
    }

    // Cộng GF(2) theo từng bit (XOR)
    BinaryVector operator^(const BinaryVector& other) const {
        int n = max(bits.size(), other.bits.size());
        vector<int> res(n, 0);
        for (int i = 0; i < n; i++)
            res[i] = (i < (int)bits.size() ? bits[i] : 0)
                   ^ (i < (int)other.bits.size() ? other.bits[i] : 0);
        return BinaryVector(res);
    }

    // --- Dịch vòng ---
    // Trái: (a0, a1, ..., a_{n-1}) -> (a1, ..., a_{n-1}, a0)
    void rotateLeft() {
        int first = bits.front();
        bits.erase(bits.begin());
        bits.push_back(first);
    }

    // Phải: (a0, a1, ..., a_{n-1}) -> (a_{n-1}, a0, ..., a_{n-2})
    void rotateRight() {
        int last = bits.back();
        bits.pop_back();
        bits.insert(bits.begin(), last);
    }

    // --- In ---
    string toString() const {
        string s;
        for (int x : bits) s += (char)('0' + x);
        return s;
    }
};

// ============================================================
//  LỚP MÃ VÒNG TUYẾN TÍNH  C(l, k, d0)
// ============================================================
class CyclicCode {
private:
    int           l, k, d0, t;
    GF2Polynomial g;  // da thức sinh g(x)

public:
    // t = floor((d0 - 1) / 2)
    CyclicCode(int l, int k, int d0, const GF2Polynomial& g)
        : l(l), k(k), d0(d0), t((d0 - 1) / 2), g(g) {}

    // Tính hội chứng S(x) = r(x) mod g(x)
    GF2Polynomial syndrome(const BinaryVector& r) const {
        return r.toPoly().mod(g);
    }

    // -------------------------------------------------------
    //  Thuật toán dịch vòng Meggitt
    //  Input : véc-tơ thu r
    //  Output: từ mã phục hồi
    // -------------------------------------------------------
    BinaryVector decode(const BinaryVector& r, bool verbose = false) const {

        if (verbose) {
            cout << "===========================================\n";
            cout << "Ma vong C(" << l << ", " << k << ", " << d0 << ")\n";
            cout << "g(x) = " << g.toPolyString() << "\n";
            cout << "t    = " << t << "\n";
            cout << "r(x) = " << r.toString() << "\n";
            cout << "===========================================\n\n";
        }

        // Bu?c 0: kiểm tra hội chứng ban đầu
        GF2Polynomial s0 = syndrome(r);
        if (s0.isZero()) {
            if (verbose) cout << "S(x) = 0  =>  Khong co loi.\n\n";
            return r;
        }

        BinaryVector working = r;

        for (int i = 0; i < l; i++) {
            GF2Polynomial s = syndrome(working);
            int           w = s.weight();

            if (verbose) {
                cout << "Buoc " << i << ":  r = " << working.toString()
                     << "   S(x) = " << s.toPolyString()
                     << "   w = " << w << "\n";
            }

            if (w <= t) {
                // Vecto l?i e: d?t h?i ch?ng vào các v? trí th?p
                BinaryVector e(s, l);

                // Sửa lỗi
                BinaryVector fixed = working ^ e;

                // Dịch ngược lại i lần để về vị trí gốc
                for (int j = 0; j < i; j++)
                    fixed.rotateLeft();

                if (verbose) {
                    cout << "\n=> Phat hien loi tai buoc " << i << "\n";
                    cout << "   e(x) = " << e.toString() << "\n";
                    cout << "   c(x) = " << fixed.toString() << "\n\n";
                }

                return fixed;
            }

            working.rotateRight();
        }

        if (verbose)
            cout << "Khong the sua loi (vuot qua kha nang t=" << t << ").\n\n";

        return r; // Trả nguyên gốc nếu không sửa được
    }
};

// ============================================================
//  MAIN
// ============================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // --- Ðọc tham số C(l, k, d0) ---
    int l, k, d0;
    cin >> l >> k >> d0;

    // --- Đọc đa thức sinh g(x): (degG+1) h? s? t? a0 -> a_{degG} ---
    int degG = l - k;
    vector<int> gCoef(degG + 1);
    for (int i = 0; i <= degG; i++) cin >> gCoef[i];
    GF2Polynomial g(gCoef);

    // --- Ðọc véc-tơ thu r ---
    vector<int> rBits(l);
    for (int i = 0; i < l; i++) cin >> rBits[i];
    BinaryVector r(rBits);

    // --- Chế độ: 0 = chỉ in kết quả, 1 = chi tiết từng bước ---
    int mode = 0;
    cin >> mode;

    // --- Giải mã ---
    CyclicCode   code(l, k, d0, g);
    BinaryVector result = code.decode(r, mode == 1);

    cout << result.toString() << "\n";

    return 0;
}

