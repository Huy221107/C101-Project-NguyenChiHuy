# Quản lý cửa hàng tạp hóa

Ứng dụng desktop viết bằng C++11 và SFML 2.6.1 để quản lý sản phẩm, bán hàng và theo dõi tồn kho. Giao diện hỗ trợ tiếng Việt.

## Chức năng

- Quản lý sản phẩm: thêm sản phẩm, xem danh sách, tìm theo mã hoặc tên và sắp xếp theo mã.
- Danh sách sản phẩm hiển thị tối đa 10 dòng cùng lúc. Cuộn con lăn chuột trên bảng để xem các sản phẩm tiếp theo; khi tìm kiếm, danh sách được lọc theo từ khóa.
- Bán hàng: tìm sản phẩm theo mã hoặc tên, chọn từ danh sách gợi ý, nhập số lượng và thêm nhiều mặt hàng vào giỏ.
- Kiểm tra tồn kho khi thêm mặt hàng và khi xuất hóa đơn. Khi xuất thành công, tồn kho được cập nhật trong `sanpham.txt`.
- Ghi hóa đơn vào `hoadon.txt`, gồm thời gian xuất, mặt hàng, số lượng, đơn giá và tổng tiền. Nếu không ghi được hóa đơn, ứng dụng báo lỗi và cố gắng hoàn lại tồn kho.
- Cảnh báo sản phẩm sắp hết khi tồn kho dưới 5.
- Thống kê số loại sản phẩm, tổng số lượng, giá trị tồn kho, doanh thu trong phiên hiện tại và danh sách mặt hàng sắp hết.

## Yêu cầu

- Windows
- Trình biên dịch MinGW-w64 g++ có hỗ trợ C++11
- SFML 2.6.1 (module Graphics, Window và System)
- Arial tại `C:\Windows\Fonts\arial.ttf`

Các tệp văn bản sử dụng UTF-8. Ứng dụng đọc và ghi `sanpham.txt`, `hoadon.txt` theo thư mục làm việc hiện tại.

## Biên dịch và chạy

### VS Code

Mở thư mục dự án và chạy tác vụ **C/C++: g++.exe build active file**. Các tệp `.vscode/tasks.json` và `.vscode/launch.json` trong repository là cấu hình mẫu cho môi trường phát triển ban đầu. Cập nhật đường dẫn compiler, debugger, thư mục include/lib/bin của SFML cho phù hợp với máy trước khi dùng.

### PowerShell

Thay đường dẫn compiler và SFML dưới đây bằng vị trí đã cài trên máy:

```powershell
$compiler = "D:\path\to\mingw64\bin\g++.exe"
$sfml = "C:\path\to\SFML-2.6.1"

& $compiler `
  -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 `
  "-I$sfml\include" "-L$sfml\lib" `
  .\QuanLyCuaHangTapHoa.cpp -o .\QuanLyCuaHangTapHoa.exe `
  -lsfml-graphics -lsfml-window -lsfml-system
```

Đặt các DLL SFML cần thiết trong `PATH` hoặc cạnh tệp `.exe`, sau đó chạy:

```powershell
.\QuanLyCuaHangTapHoa.exe
```

## Dữ liệu

Mỗi dòng trong `sanpham.txt` có định dạng:

```text
MaSanPham|TenSanPham|DonGia|SoLuong
```

Ví dụ:

```text
SP001|Gạo|15000.00|20
SP002|Sữa tươi|12000.00|30
```

Không dùng ký tự `|` trong mã hoặc tên sản phẩm vì đây là ký tự phân cách.

Hóa đơn được ghi nối tiếp vào `hoadon.txt`. Doanh thu phiên làm việc chỉ được lưu trong bộ nhớ, bắt đầu lại từ 0 khi khởi động lại ứng dụng; lịch sử hóa đơn vẫn được giữ trong tệp.