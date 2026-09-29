# Chương trình quản lý cửa hàng tạp hóa

Ứng dụng desktop C++ dùng SFML 2.6 để quản lý sản phẩm và bán hàng. Thông tin sản phẩm được lưu trong `sanpham.txt`; mỗi hóa đơn đã thanh toán được ghi nối tiếp vào `hoadon.txt`.

## Chức năng

- Thêm sản phẩm, tìm kiếm theo mã hoặc tên, xem danh sách và sắp xếp theo mã.
- Danh sách sản phẩm hiển thị theo từng trang vừa khung; dùng con lăn chuột để xem các dòng tiếp theo.
- Tạo một hóa đơn gồm nhiều mặt hàng, nhập số lượng cho từng mặt hàng và kiểm tra tồn kho khi thêm vào giỏ cũng như trước khi thanh toán.
- Khi bán hàng, tìm sản phẩm bằng mã hoặc tên; các kết quả phù hợp hiện thành gợi ý để chọn khi có nhiều sản phẩm trùng từ khóa.
- Khi thanh toán thành công, cập nhật tồn kho, hiển thị hóa đơn có tổng tiền và lưu hóa đơn vào `hoadon.txt`.
- Cảnh báo sản phẩm sắp hết khi tồn kho dưới 5, trên danh sách và ở màn hình thống kê.
- Thống kê tổng số sản phẩm, tổng tồn kho, giá trị tồn kho và doanh thu cộng dồn trong phiên ứng dụng hiện tại.

## Yêu cầu

- Windows
- SFML 2.6.1
- MinGW-w64 GCC 13.1 tương thích với gói SFML đã cài
- Phông Arial tại `C:\Windows\Fonts\arial.ttf`

Mã nguồn và dữ liệu văn bản sử dụng UTF-8. Tệp `tasks.json` của VS Code đã cấu hình đường dẫn compiler và SFML trong môi trường phát triển hiện tại.

## Biên dịch và chạy

Trong VS Code, mở `QuanLyCuaHangTapHoa.cpp` và chạy tác vụ **C/C++: g++.exe build active file**. Có thể biên dịch từ PowerShell bằng:

```powershell
& "D:\Downloads\winlibs-x86_64-mcf-seh-gcc-13.1.0-msvcrt-r5\mingw64\bin\g++.exe" `
  -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 `
  -IC:\bao\sfml\SFML-2.6.1\include `
  -LC:\bao\sfml\SFML-2.6.1\lib `
  .\QuanLyCuaHangTapHoa.cpp -o .\QuanLyCuaHangTapHoa.exe `
  -lsfml-graphics -lsfml-window -lsfml-system
```

Đảm bảo các DLL của SFML nằm trong `PATH` hoặc cạnh tệp `.exe` khi chạy.

## Tệp dữ liệu

Mỗi sản phẩm trong `sanpham.txt` nằm trên một dòng, theo định dạng:

```text
MaSanPham|TenSanPham|DonGia|SoLuong
```

Ví dụ:

```text
SP001|Gạo|15000|20
SP002|Sữa tươi|12000|30
```

Không dùng ký tự `|` trong mã hoặc tên vì đây là ký tự phân cách. Doanh thu phiên làm việc chỉ được giữ trong bộ nhớ và bắt đầu lại từ 0 khi khởi động lại ứng dụng; lịch sử các hóa đơn vẫn được lưu trong `hoadon.txt`.
