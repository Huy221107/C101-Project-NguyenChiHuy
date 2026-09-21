#include <iostream>
using namespace std;
class TaiLieu
{
protected:
    string tieuDe;
    int songaymuon;

public:
    TaiLieu(string tieuDe, int songaymuon)
    {
        this->tieuDe = tieuDe;
        this->songaymuon = songaymuon;
    }
    float tinhPhi()
    {
        return 0;
    }
};
class Sach : public TaiLieu
{
    public:
    Sach(string tieuDe, int songaymuon) : TaiLieu(tieuDe, songaymuon)
    {
    }
    float tinhPhi()
    {
        return songaymuon * 2000;
    }
};
class TapChi : public TaiLieu
{
    public:
    TapChi(string tieuDe, int songaymuon) : TaiLieu(tieuDe, songaymuon)
    {
    }
    float tinhPhi()
    {
        if(songaymuon <=7)
            return songaymuon * 3000;
        else
        return songaymuon * 5000;
    }
};
int main()
{
TapChi tc("Tap Chi A", 8);
Sach s("Sach A", 3);
cout << "Phi muon Tap Chi: " << tc.tinhPhi() << endl;
cout << "Phi muon Sach: " << s.tinhPhi() << endl;
    return 0;
}