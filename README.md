# Phần 2: Giải mã Mã Vòng Tuyến Tính bằng thuật toán Meggitt (Chia dịch vòng)

**Thực hiện bởi:** Bùi Nhật Minh (B24DCAT184) & Nhóm bài tập môn học.

## 1. Giới thiệu chung
Chương trình này thực hiện mô phỏng hệ thống giải mã cho Mã vòng tuyến tính $C(l, k)$ có khoảng cách thiết kế $d_0$ trên trường Galois $GF(2)$. 

Thuật toán sử dụng là **Meggitt Decoder** (Giải mã chia dịch vòng), dựa trên việc tính toán hội chứng (Syndrome) $S(x)$ của véc-tơ thu $r(x)$. Thuật toán có khả năng phát hiện và sửa tối đa $t = \lfloor(d_0 - 1)/2\rfloor$ lỗi.

### Các tính năng nổi bật:
- Xây dựng hoàn chỉnh từ đầu các lớp đối tượng toán học:
  - `GF2Polynomial`: Xử lý đa thức nhị phân (cộng XOR, nhân, chia lấy dư Modulo-2).
  - `BinaryVector`: Xử lý véc-tơ nhị phân (trọng số Hamming, dịch vòng trái/phải).
- Tích hợp **Verbose Mode** (Chế độ chi tiết): In ra từng bước dịch vòng, tính hội chứng và sửa lỗi để phục vụ việc trình bày lời giải tự luận.

---

## 2. Cấu trúc mã nguồn
Mã nguồn được thiết kế theo tư duy Hướng đối tượng (OOP) trong C++, bao gồm:
* **`class GF2Polynomial`**: Biểu diễn đa thức. Hệ số được lưu từ bậc thấp đến bậc cao (ví dụ: $1 + x + x^3$ lưu là `[1, 1, 0, 1]`).
* **`class BinaryVector`**: Biểu diễn từ mã / véc-tơ thu được. Cung cấp hàm `rotateLeft()` và `rotateRight()` đóng vai trò cốt lõi cho thuật toán Meggitt.
* **`class CyclicCode`**: Lớp thực thi thuật toán giải mã chính.
* **`main()`**: Hàm điều khiển chuẩn ICPC, đọc dữ liệu I/O từ Standard Input/Output.

---

## 3. Hướng dẫn cài đặt và biên dịch

Bạn cần một trình biên dịch C++ (chuẩn C++11 trở lên).
Sử dụng lệnh sau trong terminal để biên dịch:

```bash
g++ -O2 main.cpp -o meggitt_decoder
