#include <iostream>
using namespace std;
class ConNguoi
{
protected:
    string ten;
    int tuoi;

public:
    ConNguoi(string tenBanDau, int tuoiBanDau)
    {
        ten = tenBanDau;
        tuoi = tuoiBanDau;
    }
    void gioiThieu()
    {
        cout << "Ten la " << ten << ", " << tuoi << " tuoi" << endl;
    }
};
class NhanVien : public ConNguoi
{
public:
    double luong;
    NhanVien(string tenBanDau, int tuoiBanDau, double luongBanDau) : ConNguoi(tenBanDau, tuoiBanDau)
    {
        luong = luongBanDau;
    }
    void gioiThieu()
    {
        ConNguoi::gioiThieu();
        cout << "Toi la nhan vien, luong thang: " << luong << endl;
    }
};
class HocSinh : public ConNguoi
{
public:
    string truong;
    HocSinh(string tenBanDau, int tuoiBanDau, string truongBanDau) : ConNguoi(tenBanDau, tuoiBanDau)
    {
        truong = truongBanDau;
    }
    void gioiThieu()
    {
        ConNguoi::gioiThieu();
        cout << "Toi la hoc sinh, dang hoc tap tai truong: " << truong << endl;
    }
};
int main()
{
    NhanVien nv("Nguyen Chi Huy", 19, 100000);
    HocSinh hs("Tran Thi Lan", 17, "Truong THPT Nguyen Hue");
    cout << "----NhanVien.gioiThieu()----" << endl;
    nv.gioiThieu();
    cout << "----HocSinh.gioiThieu()----" << endl;
    hs.gioiThieu();
    return 0;
}