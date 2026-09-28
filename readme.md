# Chương trình quản lý cửa hàng tạp hóa

Chương trình console viết bằng C++, dùng lớp `SanPham` để quản lý danh sách sản phẩm của cửa hàng. Dữ liệu được lưu trong file văn bản `sanpham.txt`.

## Chức năng

Chọn chức năng từ menu:

| Lựa chọn | Chức năng |
| --- | --- |
| 1 | Nhập và thêm sản phẩm mới vào file |
| 2 | Hiển thị nội dung danh sách sản phẩm |
| 3 | Tìm sản phẩm theo chính xác mã hoặc tên |
| 4 | Sắp xếp sản phẩm theo mã tăng dần |
| 5 | Thống kê số loại sản phẩm, tổng số lượng và tổng giá trị tồn kho |
| 0 | Thoát chương trình |

## Yêu cầu

- Windows
- Trình biên dịch C++ hỗ trợ C++11, chẳng hạn MinGW g++
- Lưu mã nguồn bằng mã hóa UTF-8 để hiển thị tiếng Việt

Chương trình dùng `windows.h` để đặt bảng mã console UTF-8 nên không được thiết kế để biên dịch nguyên trạng trên các hệ điều hành không phải Windows.

## Biên dịch và chạy

Mở Terminal tại thư mục chứa `QuanLyCuaHangTapHoa.cpp`, sau đó chạy:

```powershell
g++ -std=c++11 QuanLyCuaHangTapHoa.cpp -o QuanLyCuaHangTapHoa.exe
.\QuanLyCuaHangTapHoa.exe
```

Nếu dùng VS Code, có thể mở thư mục dự án và chạy tác vụ build C++ đã cấu hình trong `.vscode/tasks.json`.

## File dữ liệu

Chương trình đọc và ghi `sanpham.txt` trong thư mục làm việc hiện tại. Mỗi sản phẩm nằm trên một dòng, các trường được phân cách bằng dấu `|`:

```text
MaSanPham|TenSanPham|DonGia|SoLuong
```

Ví dụ:

```text
SP001|Gạo|15000|20
SP002|Sữa tươi|12000|30
```

Trong đó, đơn giá và số lượng cần nhập dưới dạng số để chức năng thống kê có thể tính tổng giá trị tồn kho (`đơn giá × số lượng`). Tên sản phẩm có thể chứa dấu cách và tiếng Việt có dấu; không dùng dấu `|` trong các trường dữ liệu vì đây là ký tự phân cách.

Khi thêm sản phẩm, chương trình ghi nối tiếp vào file. Nếu `sanpham.txt` chưa tồn tại, hãy tạo file trong thư mục chạy chương trình; thao tác thêm cũng có thể tạo file khi mở thành công.

## Lưu ý

- Chức năng sắp xếp hiện đọc tối đa 100 dòng sản phẩm.
- Tìm kiếm yêu cầu mã hoặc tên khớp chính xác, có phân biệt chữ hoa/chữ thường.
- Mã được sắp xếp theo thứ tự ký tự; nên dùng mã có độ dài số đồng nhất, ví dụ `SP001`, `SP002`, `SP010`.
- Mã nguồn sử dụng `stringstream` để đọc và tách dữ liệu, vì vậy cần có dòng `#include <sstream>` cùng các thư viện đầu chương trình.