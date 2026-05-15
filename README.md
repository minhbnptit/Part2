# CYCLIC CODE DECODING USING GF(2)

## 1. Giới thiệu

Chương trình được viết bằng ngôn ngữ C++ nhằm mô phỏng quá trình xử lý mã vòng tuyến tính (Cyclic Code) trên trường GF(2).

Chương trình thực hiện:

- Xây dựng lớp Bit.
- Xây dựng lớp Vector nhị phân (BitString).
- Xây dựng lớp đa thức trên GF(2).
- Thực hiện phép chia đa thức nhị phân.
- Tìm syndrome / từ lỗi từ vector thu được.
- Hỗ trợ kiểm tra và phát hiện lỗi trong mã vòng tuyến tính.

---

# 2. Kiến thức sử dụng

## 2.1. Trường GF(2)

GF(2) là trường chỉ gồm hai phần tử:

```txt
0 và 1
```

Các phép toán:

| Phép toán | Kết quả |
|---|---|
| 0 + 0 | 0 |
| 0 + 1 | 1 |
| 1 + 0 | 1 |
| 1 + 1 | 0 |

Trong GF(2):

```txt
Cộng = XOR
Trừ = XOR
Nhân = AND
```

---

## 2.2. Mã vòng tuyến tính

Mã vòng tuyến tính C(n,k,d0) được sinh bởi đa thức sinh:

```txt
g(x)
```

Mọi từ mã hợp lệ đều chia hết cho:

```txt
g(x)
```

Khi nhận được vector:

```txt
r(x)
```

ta thực hiện:

```txt
r(x) mod g(x)
```

Nếu:

```txt
remainder = 0
```

thì khả năng cao không có lỗi.

Ngược lại:

```txt
remainder != 0
```

thì tồn tại lỗi truyền.

---

# 3. Cấu trúc chương trình

Chương trình gồm các lớp chính:

## 3.1. Class Bit

Dùng biểu diễn một bit:

```cpp
0 hoặc 1
```

Hỗ trợ:

- Phép cộng XOR.
- Phép nhân AND.

---

## 3.2. Class BitString

Dùng quản lý chuỗi bit.

Ví dụ:

```txt
101101
```

Hỗ trợ:

- Lưu vector bit.
- Chuyển sang dạng đa thức.
- In chuỗi nhị phân.

---

## 3.3. Class PolynomialGF2

Biểu diễn đa thức trên GF(2).

Ví dụ:

```txt
1011
```

tương ứng:

```txt
x^3 + x + 1
```

Hỗ trợ:

- Chuẩn hóa đa thức.
- Chia đa thức.
- Tìm phần dư.
- Chuyển đổi dạng chuỗi.

---

# 4. Thuật toán chính

## Bước 1

Nhập:

```txt
n, k
g(x)
r
```

Trong đó:

- n: độ dài từ mã.
- k: số bit thông tin.
- g(x): đa thức sinh.
- r: vector thu được.

---

## Bước 2

Chuyển chuỗi bit thành vector hệ số đa thức.

Ví dụ:

```txt
1011
```

sẽ thành:

```txt
x^3 + x + 1
```

---

## Bước 3

Thực hiện phép chia đa thức:

```txt
r(x) / g(x)
```

trên GF(2).

---

## Bước 4

Lấy phần dư:

```txt
syndrome = r(x) mod g(x)
```

---

## Bước 5

Xuất kết quả syndrome / từ lỗi.

---

# 5. Định dạng Input

```txt
n k
g(x)
r
```

Ví dụ:

```txt
7 4
1011
1001011
```

---

# 6. Định dạng Output

```txt
syndrome
```

Ví dụ:

```txt
010
```

---

# 7. Ví dụ chạy chương trình

## Input

```txt
7 4
1011
1001011
```

## Output

```txt
010
```

---

# 8. Bộ test

## test01.in

```txt
7 4
1011
1001001
```

## test01.out

```txt
000
```

---

## test02.in

```txt
7 4
1011
1001011
```

## test02.out

```txt
010
```

---

## test03.in

```txt
7 4
1011
1111111
```

## test03.out

```txt
011
```

---

# 9. Cách biên dịch và chạy

## Compile

```bash
g++ main.cpp -o main
```

## Run

```bash
./main
```

Hoặc trên Windows:

```bash
main.exe
```

---

# 10. Cấu trúc thư mục

```txt
Part2/
│
├── main.cpp
├── README.md
└── tests/
    ├── test01.in
    ├── test01.out
    ├── test02.in
    ├── test02.out
    ├── test03.in
    └── test03.out
```

---

# 11. Kết luận

Chương trình đã mô phỏng thành công:

- Các phép toán trên GF(2).
- Biểu diễn mã vòng tuyến tính.
- Chia đa thức nhị phân.
- Tìm syndrome để phát hiện lỗi.

Đây là nền tảng quan trọng trong:

- Lý thuyết mã.
- Truyền dữ liệu.
- Hệ thống phát hiện và sửa lỗi.
- Mã CRC và mã cyclic thực tế.
