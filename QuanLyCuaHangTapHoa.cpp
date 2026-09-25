#include <iostream>  // cin, cout
#include <string>    // string
#include <algorithm> // sort, find
#include <iomanip>   // setw, fixed, setprecision
#include <fstream>   // đọc/ghi file
#include <windows.h>
#include <stdexcept>
using namespace std;
class SanPham
{
private:
    string ma;
    string ten;
    double donGia;
    int soLuong;

public:
    SanPham()
    {
    }
    SanPham(string ma, string ten, double donGia, int soLuong)
    {
        this->ma = ma;
        this->ten = ten;
        this->donGia = donGia;
        this->soLuong = soLuong;
    }
    void hienThiMenu()
    {
        cout << "CHƯƠNG TRÌNH QUẢN LÝ CỬA HÀNG TẠP HÓA QUẢ BÁO " << endl;
        cout << "1 . Thêm mới sản phẩm" << endl;
        cout << "2 . Hiển thị danh sách" << endl;
        cout << "3 . Tìm kiếm sản phẩm" << endl;
        cout << "4 . Sắp xếp" << endl;
        cout << "5 . Thống kê " << endl;
        cout << "0 . Thoát " << endl;
        cout << "Chọn chức năng : ";
    }
    void nhapSanPham()
    {
        cout << "Nhập mã: ";
        cin >> ma;

        cout << "Nhâjp tên: ";
        cin.ignore();
        getline(cin, ten);

        cout << "Nhập đơn giá: ";
        cin >> donGia;

        cout << "Nhập số lượng: ";
        cin >> soLuong;
    }

    void themMoiSanPham()
    {
        ofstream themFile("sanpham.txt", ios::app);

        if (!themFile.is_open())
        {
            cout << "Không thể mở file sanpham.txt!" << endl;
            return;
        }
        else
        {
            themFile << ma << "|"
                     << ten << "|"
                     << donGia << "|"
                     << soLuong << endl;
        }
        themFile.close();

        cout << "Đã lưu sản phẩm vào file!" << endl;
    }

    void hienThiDanhSach()
    {
        ifstream xuatFile("sanpham.txt");

        if (!xuatFile.is_open())
        {
            cout << "Không thể mở file sanpham.txt!" << endl;
            return;
        }

        string sPham;

        while (getline(xuatFile, sPham))
        {
            cout << sPham << endl;
        }

        xuatFile.close();
    }
    void timKiem()
    {
        ifstream timTuKhoa("sanpham.txt");

        if (!timTuKhoa.is_open())
        {
            cout << "Không thể mở timTuKhoa sanpham.txt!" << endl;
            return;
        }

        string tuKhoa;
        cout << "Nhập mã hoặc tên sản phẩm cần tìm: ";

        cin.ignore();
        getline(cin, tuKhoa);

        string dong;
        bool timThay = false;

        while (getline(timTuKhoa, dong))
        {
            stringstream ss(dong);

            string ma;
            string ten;
            string donGia;
            string soLuong;

            getline(ss, ma, '|');
            getline(ss, ten, '|');
            getline(ss, donGia, '|');
            getline(ss, soLuong, '|');

            if (ma == tuKhoa || ten == tuKhoa)
            {
                cout << "Sản phẩm tìm thấy:" << endl;
                cout << "Mã sản phẩm: " << ma << endl;
                cout << "Tên sản phẩm: " << ten << endl;
                cout << "Đơn giá: " << donGia << " đồng" << endl;
                cout << "Số lượng: " << soLuong << endl;

                timThay = true;
            }
        }

        if (!timThay)
        {
            cout << "Không tìm thấy sản phẩm!\n";
        }

        timTuKhoa.close();
    }
    void sapXepTheoMa()
    {
        ifstream fileDoc("sanpham.txt");

        if (!fileDoc.is_open())
        {
            cout << "Không mở được file sanpham.txt!\n";
            return;
        }

        {
            string danhSach[10000];
            int soLuongSanPham = 0;
            string dong;

            while (getline(fileDoc, dong) && soLuongSanPham < 100)
            {
                if (!dong.empty())
                {
                    danhSach[soLuongSanPham] = dong;
                    soLuongSanPham++;
                }
            }

            fileDoc.close();

            // Sắp xếp mã sản phẩm từ bé đến lớn
            for (int i = 0; i < soLuongSanPham - 1; i++)
            {
                for (int j = i + 1; j < soLuongSanPham; j++)
                {
                    string ma1;
                    string ma2;

                    stringstream ss1(danhSach[i]);
                    stringstream ss2(danhSach[j]);

                    getline(ss1, ma1, '|');
                    getline(ss2, ma2, '|');

                    if (ma1 > ma2)
                    {
                        string tam = danhSach[i];
                        danhSach[i] = danhSach[j];
                        danhSach[j] = tam;
                    }
                }
            }

            ofstream fileGhi("sanpham.txt");

            if (!fileGhi.is_open())
            {
                cout << "Không thể ghi vào file sanpham.txt!\n";
                return;
            }

            for (int i = 0; i < soLuongSanPham; i++)
            {
                fileGhi << danhSach[i] << '\n';
            }

            fileGhi.close();

            cout << "Đã sắp xếp sản phẩm theo mã tăng dần!\n";
        }
    }
    void thongKe()
{
    ifstream file("sanpham.txt");

    if (!file.is_open())
    {
        cout << "Không mở được file sanpham.txt!\n";
        return;
    }

    string dong;
    int tongLoaiSanPham = 0;
    long long tongSoLuong = 0;
    long long tongGiaTri = 0;

    while (getline(file, dong))
    {
        if (dong.empty())
        {
            continue;
        }

        string ma;
        string ten;
        string donGia;
        string soLuong;

        stringstream ss(dong);

        getline(ss, ma, '|');
        getline(ss, ten, '|');
        getline(ss, donGia, '|');
        getline(ss, soLuong, '|');

        long long gia;
        long long soLuongSanPham;

        stringstream chuyenGia(donGia);
        stringstream chuyenSoLuong(soLuong);

        if (!(chuyenGia >> gia) || !(chuyenSoLuong >> soLuongSanPham))
        {
            cout << "Dữ liệu không hợp lệ: " << dong << endl;
            continue;
        }

        tongLoaiSanPham++;
        tongSoLuong += soLuongSanPham;
        tongGiaTri += gia * soLuongSanPham;
    }

    file.close();

    cout << "\n===== THỐNG KÊ SẢN PHẨM =====\n";
    cout << "Tổng số loại sản phẩm: " << tongLoaiSanPham << endl;
    cout << "Tổng số lượng sản phẩm: " << tongSoLuong << endl;
    cout << "Tổng giá trị hàng tồn kho: "
         << tongGiaTri << " đồng\n";
}
    {
        int tongLoaiSanPham = 0;
        long long tongSoLuong = 0;
        long double tongGiaTri = 0;

        while (getline(file, dong))
        {
            if (dong.empty())
            {
                continue;
            }

            string ma, ten, donGia, soLuong;
            stringstream ss(dong);

            getline(ss, ma, '|');
            getline(ss, ten, '|');
            getline(ss, donGia, '|');
            getline(ss, soLuong, '|');

            try
            {
                long double gia = stold(donGia);
                long long soLuongSanPham = stoll(soLuong);

                tongLoaiSanPham++;
                tongSoLuong += soLuongSanPham;
                tongGiaTri += gia * soLuongSanPham;
            }
            catch (const invalid_argument &loi)
            {
                cout << "Dữ liệu không hợp lệ: " << dong << endl;
            }
            catch (const out_of_range &loi)
            {
                cout << "Số quá lớn: " << dong << endl;
            }
        }
    }
};
int main()
{
    SetConsoleOutputCP(CP_UTF8); // ghi tieng viet
    SetConsoleCP(CP_UTF8);       // ghi tieng viet
    int luaChon;
    SanPham sp;
    do
    {
        sp.hienThiMenu();
        cin >> luaChon;
        switch (luaChon)
        {
        case 1:
            cout << "Thêm mới sản phẩm" << endl;
            sp.nhapSanPham();
            sp.themMoiSanPham();
            break;
        case 2:
            cout << "Danh Sách sản phẩm" << endl;
            sp.hienThiDanhSach();
            break;
        case 3:
            cout << "Tìm kiếm" << endl;
            sp.timKiem();
            break;
        case 4:
            cout << "Sắp xếp" << endl;
            sp.sapXepTheoMa();
            sp.hienThiDanhSach();
            break;
        case 5:
            cout << "Thống kế " << endl;
            sp.thongKe();
            break;
        case 0:
            cout << "Tạm biệt ! " << endl;
            break;
        default:
            cout << "Lựa chọn không hợp lệ ! " << endl;
        }
    } while (luaChon != 0);
    return 0;
}