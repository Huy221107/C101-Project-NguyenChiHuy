#include <iostream>
using namespace std;
class NhanVat
{
protected:
    string ten;
    int mau;

public:
    NhanVat(string ten, int mau)
    {
        this->ten = ten;
        this->mau = mau;
    }
    void tanCong()
    {
        cout << ten << " tan cong thuong" << endl;
    }
};
class KiemSu : public NhanVat
{
public:
    KiemSu(string ten, int mau) : NhanVat(ten, mau)
    {
    }
    void tanCong()
    {
        cout << ten << " hp " << mau << " chem kiem -50 sat thuong" << endl;
    }
};
class PhapSu : public NhanVat
{
public:
    PhapSu(string ten, int mau) : NhanVat(ten, mau)
    {
    }
    void tanCong()
    {
        cout << ten << " hp " << mau << " niem chu -80 sat thuong" << endl;
    }
};
int main()
{
    NhanVat nv("Dan lang", 100);
    nv.tanCong();
    KiemSu ks("Kiem Su", 500);
    ks.tanCong();
    PhapSu ps("Phap Su", 200);
    ps.tanCong();
}
