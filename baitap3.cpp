#include <iostream>
using namespace std;
class NhanSu
{
protected:
    string hoTen;
    float luongCoBan;

public:
    NhanSu(string hoTen, float luongCoBan)
    {
        this->hoTen = hoTen;
        this->luongCoBan = luongCoBan;
    }
    float tinhLuong()
    {
        return luongCoBan;
    }
    void inLuong()
    {
        cout << "Ho va ten :" << hoTen << " , luong :" << tinhLuong() << endl;
    }
};
class NhanVienGio : public NhanSu
{
private:
    float soGio;

public:
    NhanVienGio(string hoTen, float luongCoBan, float soGio) : NhanSu(hoTen, luongCoBan)
    {
        this->soGio = soGio;
    }
    long long tinhLuong()
    {
        return (long long)luongCoBan + soGio * 50000;
    }
    void inLuong()
    {
        cout << "Ho va ten: " << hoTen << " (theo gio)" << " , luong :" << tinhLuong() << endl;
    }
};
class TruongPhong : public NhanSu
{
private:
    int soNVQuanLy;

public:
    TruongPhong(string hoTen, float luongCoBan, int soNVQuanLy) : NhanSu(hoTen, luongCoBan)
    {
        this->soNVQuanLy = soNVQuanLy;
    }
    long long tinhLuong()
    {
        return (long long)luongCoBan + soNVQuanLy * 300000;
    }
    void inLuong()
    {
        cout << "Ho va ten: " << hoTen << " (truong phong)" << " , luong :" << tinhLuong() << endl;
    }
};
int main()
{
    NhanVienGio nvg("Huy", 5000000, 20);
    nvg.inLuong();
    TruongPhong tp("Binh", 8000000, 5);
    tp.inLuong();
}
