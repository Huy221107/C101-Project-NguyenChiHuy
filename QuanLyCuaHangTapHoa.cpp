#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <locale>
#include <cctype>
#include <codecvt>
#include <cwctype>
#include <ctime>

using namespace std;

// ======================================================
// CAC HAM HO TRO CHUOI UTF-8
// ======================================================

sf::String toSF(const string& s)
{
    return sf::String::fromUtf8(s.begin(), s.end());
}

string toUTF8(const sf::String& s)
{
    const auto utf8 = s.toUtf8();
    return string(utf8.begin(), utf8.end());
}

string chuyenThuong(string s);

// ======================================================
// CLASS SANPHAM
// ======================================================

class SanPham
{
private:
    string ma;
    string ten;
    double donGia;
    int soLuong;

public:
    SanPham()
        : ma(""), ten(""), donGia(0), soLuong(0) {}

    SanPham(string ma, string ten, double donGia, int soLuong)
        : ma(ma), ten(ten), donGia(donGia), soLuong(soLuong) {}

    string getMa() const { return ma; }
    string getTen() const { return ten; }
    double getDonGia() const { return donGia; }
    int getSoLuong() const { return soLuong; }

    void setTen(const string& t) { ten = t; }
    void setDonGia(double gia) { donGia = gia; }
    void setSoLuong(int sl) { soLuong = sl; }
};

struct MatHangHoaDon
{
    string ma;
    string ten;
    int soLuong;
    double donGia;

    double thanhTien() const
    {
        return donGia * soLuong;
    }
};

// ======================================================
// CLASS QUAN LY CUA HANG
// ======================================================

class QuanLySanPham
{
private:
    vector<SanPham> ds;
    string tenFile = "sanpham.txt";

    static bool maNhoHon(const string& trai, const string& phai)
    {
        size_t i = 0;
        size_t j = 0;

        while (i < trai.size() && j < phai.size())
        {
            const unsigned char kyTuTrai =
                static_cast<unsigned char>(trai[i]);
            const unsigned char kyTuPhai =
                static_cast<unsigned char>(phai[j]);

            if (isdigit(kyTuTrai) && isdigit(kyTuPhai))
            {
                const size_t dauSoTrai = i;
                const size_t dauSoPhai = j;

                while (i < trai.size() &&
                       isdigit(static_cast<unsigned char>(trai[i])))
                    ++i;

                while (j < phai.size() &&
                       isdigit(static_cast<unsigned char>(phai[j])))
                    ++j;

                size_t soKhongTrai = dauSoTrai;
                size_t soKhongPhai = dauSoPhai;

                while (soKhongTrai < i && trai[soKhongTrai] == '0')
                    ++soKhongTrai;

                while (soKhongPhai < j && phai[soKhongPhai] == '0')
                    ++soKhongPhai;

                const size_t doDaiTrai = i - soKhongTrai;
                const size_t doDaiPhai = j - soKhongPhai;

                if (doDaiTrai != doDaiPhai)
                    return doDaiTrai < doDaiPhai;

                const int ketQuaSo = trai.compare(
                    soKhongTrai, doDaiTrai,
                    phai, soKhongPhai, doDaiPhai);

                if (ketQuaSo != 0)
                    return ketQuaSo < 0;

                const size_t soKhongThemTrai = soKhongTrai - dauSoTrai;
                const size_t soKhongThemPhai = soKhongPhai - dauSoPhai;

                if (soKhongThemTrai != soKhongThemPhai)
                    return soKhongThemTrai < soKhongThemPhai;
            }
            else
            {
                const int chuThuongTrai = tolower(kyTuTrai);
                const int chuThuongPhai = tolower(kyTuPhai);

                if (chuThuongTrai != chuThuongPhai)
                    return chuThuongTrai < chuThuongPhai;

                ++i;
                ++j;
            }
        }

        return trai.size() - i < phai.size() - j;
    }

public:
    QuanLySanPham()
    {
        docFile();
    }

    const vector<SanPham>& getDanhSach() const
    {
        return ds;
    }

    // Doc file sanpham.txt
    void docFile()
    {
        ds.clear();

        ifstream file(tenFile);
        if (!file.is_open())
            return;

        string dong;

        while (getline(file, dong))
        {
            if (dong.empty())
                continue;

            stringstream ss(dong);
            string ma, ten, giaStr, slStr;

            getline(ss, ma, '|');
            getline(ss, ten, '|');
            getline(ss, giaStr, '|');
            getline(ss, slStr, '|');

            try
            {
                double gia = stod(giaStr);
                int sl = stoi(slStr);

                if (!ma.empty() && gia >= 0 && sl >= 0)
                    ds.emplace_back(ma, ten, gia, sl);
            }
            catch (...)
            {
                // Bo qua dong du lieu khong hop le
            }
        }

        file.close();
    }

    // Luu toan bo danh sach vao file
    bool luuFile() const
    {
        ofstream file(tenFile);

        if (!file.is_open())
            return false;

        for (const auto& sp : ds)
        {
            file << sp.getMa() << "|"
                 << sp.getTen() << "|"
                 << fixed << setprecision(2)
                 << sp.getDonGia() << "|"
                 << sp.getSoLuong() << "\n";
        }

        file.close();
        return true;
    }

    // Kiem tra ma da ton tai
    bool tonTaiMa(const string& ma) const
    {
        for (const auto& sp : ds)
        {
            if (sp.getMa() == ma)
                return true;
        }

        return false;
    }

    // Them san pham
    bool themSanPham(const string& ma, const string& ten,
                     double gia, int sl)
    {
        if (ma.empty() || ten.empty() || gia < 0 || sl < 0)
            return false;

        if (tonTaiMa(ma))
            return false;

        ds.emplace_back(ma, ten, gia, sl);

        if (!luuFile())
        {
            ds.pop_back();
            return false;
        }

        return true;
    }

    // Tim kiem theo ma hoac ten
    vector<SanPham> timKiem(string tuKhoa) const
    {
        vector<SanPham> ketQua;

        const string khoaLower = chuyenThuong(tuKhoa);

        for (const auto& sp : ds)
        {
            const string ma = chuyenThuong(sp.getMa());
            const string ten = chuyenThuong(sp.getTen());

            if (ma.find(khoaLower) != string::npos ||
                ten.find(khoaLower) != string::npos)
            {
                ketQua.push_back(sp);
            }
        }

        return ketQua;
    }

    // Sap xep theo ma tang dan
    bool sapXepTheoMa()
    {
        vector<SanPham> banSao = ds;

        sort(ds.begin(), ds.end(),
             [](const SanPham& a, const SanPham& b)
             {
                 return maNhoHon(a.getMa(), b.getMa());
             });

        if (!luuFile())
        {
            ds.swap(banSao);
            return false;
        }

        return true;
    }

    int tongLoaiSanPham() const
    {
        return static_cast<int>(ds.size());
    }

    long long tongSoLuong() const
    {
        long long tong = 0;

        for (const auto& sp : ds)
            tong += sp.getSoLuong();

        return tong;
    }

    long double tongGiaTriTonKho() const
    {
        long double tong = 0;

        for (const auto& sp : ds)
            tong += static_cast<long double>(sp.getDonGia())
                    * sp.getSoLuong();

        return tong;
    }

    bool coDuSoLuong(const string& ma, int soLuongCan) const
    {
        for (const auto& sp : ds)
        {
            if (sp.getMa() == ma)
                return sp.getSoLuong() >= soLuongCan;
        }

        return false;
    }

    bool banSanPham(const string& ma, int soLuongMua, double& tongTien)
    {
        for (auto& sp : ds)
        {
            if (sp.getMa() == ma)
            {
                if (sp.getSoLuong() < soLuongMua)
                    return false;

                tongTien = sp.getDonGia() * soLuongMua;
                sp.setSoLuong(sp.getSoLuong() - soLuongMua);
                return luuFile();
            }
        }

        return false;
    }

    bool banNhieuSanPham(const vector<MatHangHoaDon>& danhSach, double& tongTien)
    {
        tongTien = 0;

        for (const auto& item : danhSach)
        {
            if (!coDuSoLuong(item.ma, item.soLuong))
                return false;
            tongTien += item.donGia * item.soLuong;
        }

        vector<SanPham> danhSachCu = ds;
        for (const auto& item : danhSach)
        {
            for (auto& sp : ds)
            {
                if (sp.getMa() == item.ma)
                {
                    sp.setSoLuong(sp.getSoLuong() - item.soLuong);
                    break;
                }
            }
        }

        if (luuFile())
            return true;

        ds.swap(danhSachCu);
        return false;
    }

    bool hoanTacBanNhieuSanPham(const vector<MatHangHoaDon>& danhSach)
    {
        for (const auto& item : danhSach)
        {
            if (!tonTaiMa(item.ma))
                return false;
        }

        vector<SanPham> danhSachCu = ds;
        for (const auto& item : danhSach)
        {
            for (auto& sp : ds)
            {
                if (sp.getMa() == item.ma)
                {
                    sp.setSoLuong(sp.getSoLuong() + item.soLuong);
                    break;
                }
            }
        }

        if (luuFile())
            return true;

        ds.swap(danhSachCu);
        return false;
    }
};

// ======================================================
// CAC HAM VE GIAO DIEN SFML
// ======================================================

void veHinh(sf::RenderWindow& window,
            float x, float y, float w, float h,
            sf::Color mau, float boGoc = 0)
{
    // SFML 2.6 khong co RoundedRectangleShape mac dinh.
    // Dung RectangleShape de ve cac khung giao dien.
    sf::RectangleShape shape(sf::Vector2f(w, h));
    shape.setPosition(x, y);
    shape.setFillColor(mau);
    window.draw(shape);
}

void veChu(sf::RenderWindow& window, sf::Font& font,
           const string& noiDung, float x, float y,
           unsigned int coChu, sf::Color mau)
{
    sf::Text text;
    text.setFont(font);
    text.setString(toSF(noiDung));
    text.setCharacterSize(coChu);
    text.setFillColor(mau);
    text.setPosition(x, y);
    window.draw(text);
}

void veNut(sf::RenderWindow& window, sf::Font& font,
           const string& nhan, sf::FloatRect rect,
           sf::Color mau,
           sf::Color mauChu = sf::Color::White)
{
    sf::RectangleShape nut;
    nut.setPosition(rect.left, rect.top);
    nut.setSize(sf::Vector2f(rect.width, rect.height));
    nut.setFillColor(mau);
    nut.setOutlineThickness(1);
    nut.setOutlineColor(sf::Color(220, 225, 235));
    window.draw(nut);

    sf::Text text;
    text.setFont(font);
    text.setString(toSF(nhan));
    text.setCharacterSize(16);
    text.setFillColor(mauChu);

    sf::FloatRect bounds = text.getLocalBounds();

    text.setPosition(
        rect.left + (rect.width - bounds.width) / 2 - bounds.left,
        rect.top + (rect.height - bounds.height) / 2 - bounds.top + 1
    );

    window.draw(text);
}

void veO(sf::RenderWindow& window, sf::Font& font,
         const string& nhan, const sf::String& giaTri,
         float x, float y, float w, bool dangChon)
{
    veChu(window, font, nhan, x, y, 15,
          sf::Color(60, 70, 90));

    sf::RectangleShape o(sf::Vector2f(w, 36));
    o.setPosition(x, y + 25);
    o.setFillColor(sf::Color::White);
    o.setOutlineThickness(1);
    o.setOutlineColor(dangChon
        ? sf::Color(45, 120, 230)
        : sf::Color(210, 215, 225));

    window.draw(o);

    sf::Text text;
    text.setFont(font);
    text.setString(giaTri);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color(35, 40, 55));

    sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition(
        x + 10 - bounds.left,
        y + 25 + (36 - bounds.height) / 2 - bounds.top + 1
    );

    window.draw(text);
}

string dinhDangTien(double gia)
{
    ostringstream oss;
    oss << fixed << setprecision(0) << gia;
    return oss.str();
}

string chuyenThuong(string s)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    std::u16string u16 = convert.from_bytes(s);

    for (char16_t& c : u16)
    {
        const wchar_t wc = static_cast<wchar_t>(c);
        if (std::iswalpha(wc))
            c = static_cast<char16_t>(std::towlower(wc));
    }

    return convert.to_bytes(u16);
}

const SanPham* timSanPhamTheoMaHoacTen(const vector<SanPham>& danhSach,
                                      const string& tuKhoa)
{
    if (tuKhoa.empty())
        return nullptr;

    const string key = chuyenThuong(tuKhoa);

    for (const auto& sp : danhSach)
    {
        const string maSp = chuyenThuong(sp.getMa());
        const string tenSp = chuyenThuong(sp.getTen());

        if (maSp == key || tenSp == key)
            return &sp;
    }

    for (const auto& sp : danhSach)
    {
        const string maSp = chuyenThuong(sp.getMa());
        const string tenSp = chuyenThuong(sp.getTen());

        if (maSp.find(key) != string::npos ||
            tenSp.find(key) != string::npos)
        {
            return &sp;
        }
    }

    return nullptr;
}

bool ghiHoaDonFile(const vector<MatHangHoaDon>& gioHang, double tongTien,
                   const string& hinhThucThanhToan = "Ti\xC3\xAAn m\xE1t")
{
    ofstream file("hoadon.txt", ios::app);
    if (!file.is_open())
        return false;

    const time_t thoiGianHienTai = time(nullptr);
    const tm* thoiGianLocal = localtime(&thoiGianHienTai);
    if (!thoiGianLocal)
        return false;

    file << "===== HOA DON =====\n";
    file << "Thoi gian: "
         << put_time(thoiGianLocal, "%Y-%m-%d %H:%M:%S") << "\n";
    file << "Hinh thuc thanh toan: " << hinhThucThanhToan << "\n";
    file << "Ma SP | Ten san pham | So luong | Don gia | Thanh tien\n";
    for (const auto& item : gioHang)
    {
        file << item.ma << " | " << item.ten << " | "
             << item.soLuong << " | "
             << fixed << setprecision(0) << item.donGia << " | "
             << item.thanhTien() << "\n";
    }

    file << "Tong tien: " << fixed << setprecision(0) << tongTien << " VN\xC4\x90\n";
    file << "==================\n\n";
    file.flush();
    const bool ghiThanhCong = file.good();
    file.close();
    return ghiThanhCong && !file.fail();
}

// ======================================================
// HAM MAIN
// ======================================================

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(1100, 720),
        toSF("Quản lý cửa hàng tạp hóa - SFML 2.6"),
        sf::Style::Titlebar | sf::Style::Close
    );

    window.setFramerateLimit(60);

    // Font tieng Viet
    sf::Font font;

    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        cerr << "Không mở được phông Arial!\n";
        cerr << "Hãy kiểm tra đường dẫn phông chữ trong mã nguồn.\n";
        return 1;
    }

    QuanLySanPham ql;

    enum TrangThai
    {
        DANH_SACH,
        THEM_SAN_PHAM,
        THONG_KE,
        BAN_HANG
    };

    TrangThai trang = DANH_SACH;

    sf::String oTimKiem;
    sf::String oMa, oTen, oGia, oSoLuong;
    sf::String oMaBan, oSoLuongBan;

    int oDangNhap = 0;
    bool dangTimKiem = false;

    string thongBao = "Chào mừng đến với cửa hàng tạp hóa!";
    sf::Clock dongHoThongBao;

    vector<SanPham> ketQuaTimKiem;
    vector<SanPham> ketQuaBanHang;
    vector<MatHangHoaDon> gioHang;
    double doanhThuPhien = 0;
    bool dangLoc = false;
    size_t dongBatDauDanhSach = 0;
    const size_t soDongToiDa = 10;

    auto hopLeKyTuMaHang = [](sf::Uint32 ch)
    {
        return (ch >= 'A' && ch <= 'Z') ||
               (ch >= 'a' && ch <= 'z') ||
               (ch >= '0' && ch <= '9') ||
               ch == '-' || ch == '_' || ch == '.' ||
               ch == ' ' || ch == '/' || ch == '(' || ch == ')' ||
               ch > 127;
    };

    auto hopLeKyTuSoLuong = [](sf::Uint32 ch)
    {
        return ch >= '0' && ch <= '9';
    };

    const sf::Color nen(245, 247, 251);
    const sf::Color xanh(42, 100, 200);
    const sf::Color chuDam(35, 45, 65);
    const sf::Color xam(110, 120, 140);
    const sf::Color mauTrang(255, 255, 255);

    // Cac vi tri nut
    sf::FloatRect nutThem(20, 150, 205, 43);
    sf::FloatRect nutDanhSach(20, 202, 205, 43);
    sf::FloatRect nutTim(20, 254, 205, 43);
    sf::FloatRect nutBanHang(20, 306, 205, 43);
    sf::FloatRect nutSapXep(20, 358, 205, 43);
    sf::FloatRect nutThongKe(20, 410, 205, 43);
    sf::FloatRect nutThoat(20, 620, 205, 43);

    sf::FloatRect nutTimKiem(850, 100, 100, 38);
    sf::FloatRect nutXoaTim(960, 100, 105, 38);

    sf::FloatRect nutLuu(570, 510, 125, 42);
    sf::FloatRect nutHuy(710, 510, 125, 42);
    sf::FloatRect nutThemGio(360, 580, 180, 42);
    sf::FloatRect nutXuatHoaDon(550, 580, 170, 42);
    sf::FloatRect nutXoaGio(730, 580, 120, 42);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseWheelScrolled &&
                trang == DANH_SACH &&
                sf::FloatRect(270, 195, 810, 390).contains(
                    static_cast<float>(event.mouseWheelScroll.x),
                    static_cast<float>(event.mouseWheelScroll.y)))
            {
                const size_t soSanPham = dangLoc
                    ? ketQuaTimKiem.size()
                    : ql.getDanhSach().size();
                const size_t dongCuoi = soSanPham > soDongToiDa
                    ? soSanPham - soDongToiDa
                    : 0;

                if (event.mouseWheelScroll.delta < 0)
                {
                    dongBatDauDanhSach = min(
                        dongCuoi, dongBatDauDanhSach + size_t(3));
                }
                else if (event.mouseWheelScroll.delta > 0)
                {
                    dongBatDauDanhSach = dongBatDauDanhSach > 3
                        ? dongBatDauDanhSach - 3
                        : 0;
                }
            }

            // Xu ly nut bam va o nhap lieu
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mouse(
                    static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y)
                );

                auto bamNut = [&](sf::FloatRect r)
                {
                    return r.contains(mouse);
                };

                if (bamNut(nutThoat))
                {
                    window.close();
                }
                else if (bamNut(nutDanhSach))
                {
                    trang = DANH_SACH;
                    dangLoc = false;
                    oTimKiem.clear();
                    thongBao = "Đang hiển thị danh sách sản phẩm.";
                    dongHoThongBao.restart();
                }
                else if (bamNut(nutThem))
                {
                    trang = THEM_SAN_PHAM;
                    oMa.clear();
                    oTen.clear();
                    oGia.clear();
                    oSoLuong.clear();
                    oDangNhap = 0;
                    thongBao = "Nhập thông tin sản phẩm mới.";
                    dongHoThongBao.restart();
                }
                else if (bamNut(nutTim))
                {
                    trang = DANH_SACH;
                    dangTimKiem = true;
                    dangLoc = false;
                    oTimKiem.clear();
                    thongBao = "Nhập từ khóa rồi nhấn Tìm kiếm.";
                    dongHoThongBao.restart();
                }
                else if (bamNut(nutBanHang))
                {
                    trang = BAN_HANG;
                    gioHang.clear();
                    oMaBan.clear();
                    oSoLuongBan.clear();
                    oDangNhap = 5;
                    thongBao = "Bán hàng: chọn sản phẩm, nhập số lượng và xuất hóa đơn.";
                    dongHoThongBao.restart();
                }
                else if (trang == BAN_HANG &&
                         !bamNut(nutThemGio) &&
                         !bamNut(nutXuatHoaDon) &&
                         !bamNut(nutXoaGio) &&
                         !bamNut(nutSapXep) &&
                         !bamNut(nutThongKe))
                {
                    if (sf::FloatRect(360, 175, 370, 61).contains(mouse) ||
                        sf::FloatRect(360, 200, 370, 36).contains(mouse))
                    {
                        oDangNhap = 5;
                    }
                    else if (sf::FloatRect(360, 240, 180, 61).contains(mouse) ||
                             sf::FloatRect(360, 265, 180, 36).contains(mouse))
                    {
                        oDangNhap = 6;
                    }
                    else
                    {
                        for (size_t i = 0; i < ketQuaBanHang.size() && i < 5; ++i)
                        {
                            sf::FloatRect ketQuaRect(360, 305 + i * 22, 370, 22);
                            if (ketQuaRect.contains(mouse))
                            {
                                oMaBan = toSF(ketQuaBanHang[i].getMa());
                                oDangNhap = 5;
                                thongBao = "Đã chọn " + ketQuaBanHang[i].getMa() + " - " + ketQuaBanHang[i].getTen();
                                dongHoThongBao.restart();
                                break;
                            }
                        }
                        if (oDangNhap != 5 && oDangNhap != 6)
                            oDangNhap = 0;
                    }
                }
                else if (bamNut(nutSapXep))
                {
                    if (ql.sapXepTheoMa())
                        thongBao = "Đã sắp xếp theo mã tăng dần.";
                    else
                        thongBao = "Lỗi khi ghi tệp sanpham.txt!";

                    dongHoThongBao.restart();
                    trang = DANH_SACH;
                    dangLoc = false;
                }
                else if (bamNut(nutThongKe))
                {
                    trang = THONG_KE;
                }
                else if (trang == BAN_HANG && bamNut(nutThemGio))
                {
                    string maNhap = toUTF8(oMaBan);
                    string slStr = toUTF8(oSoLuongBan);
                    try
                    {
                        int sl = stoi(slStr);
                        if (maNhap.empty() || sl <= 0)
                        {
                            thongBao = "Mã hàng hoặc số lượng không hợp lệ.";
                        }
                        else
                        {
                            const SanPham* spTimThay = timSanPhamTheoMaHoacTen(
                                ql.getDanhSach(), maNhap);

                            if (!spTimThay)
                            {
                                thongBao = "Không tìm thấy mã hoặc tên sản phẩm.";
                            }
                            else if (spTimThay->getSoLuong() < sl)
                            {
                                thongBao = "Không đủ tồn kho. Còn " + to_string(spTimThay->getSoLuong()) + " sản phẩm.";
                            }
                            else
                            {
                                bool daCo = false;
                                for (auto& item : gioHang)
                                {
                                    if (item.ma == spTimThay->getMa())
                                    {
                                        if (item.soLuong + sl > spTimThay->getSoLuong())
                                        {
                                            thongBao = "Tổng trong giỏ vượt quá tồn kho hiện có.";
                                            daCo = true;
                                            break;
                                        }
                                        item.soLuong += sl;
                                        daCo = true;
                                        break;
                                    }
                                }

                                if (!daCo)
                                {
                                    MatHangHoaDon item;
                                    item.ma = spTimThay->getMa();
                                    item.ten = spTimThay->getTen();
                                    item.soLuong = sl;
                                    item.donGia = spTimThay->getDonGia();
                                    gioHang.push_back(item);
                                }

                                thongBao = "Đã thêm vào giỏ hàng.";
                                oMaBan.clear();
                                oSoLuongBan.clear();
                            }
                        }
                    }
                    catch (...)
                    {
                        thongBao = "Số lượng phải là số nguyên dương.";
                    }

                    dongHoThongBao.restart();
                }
                else if (trang == BAN_HANG && bamNut(nutXoaGio))
                {
                    gioHang.clear();
                    oMaBan.clear();
                    oSoLuongBan.clear();
                    thongBao = "Đã xóa giỏ hàng.";
                    dongHoThongBao.restart();
                }
                else if (trang == BAN_HANG && bamNut(nutXuatHoaDon))
                {
                    if (gioHang.empty())
                    {
                        thongBao = "Giỏ hàng đang trống.";
                    }
                    else
                    {
                        double tong = 0;
                        bool duTonKho = true;

                        for (const auto& item : gioHang)
                        {
                            const SanPham* sp = nullptr;
                            for (const auto& p : ql.getDanhSach())
                            {
                                if (p.getMa() == item.ma)
                                {
                                    sp = &p;
                                    break;
                                }
                            }

                            if (!sp || sp->getSoLuong() < item.soLuong)
                            {
                                duTonKho = false;
                                break;
                            }
                            tong += item.donGia * item.soLuong;
                        }

                        if (!duTonKho)
                        {
                            thongBao = "Bán hàng thất bại: tồn kho không đủ cho một hoặc nhiều mặt hàng.";
                        }
                        else
                        {
                            bool ok = ql.banNhieuSanPham(gioHang, tong);
                            if (!ok)
                            {
                                thongBao = "Xuất hóa đơn thất bại, vui lòng thử lại.";
                            }
                            else
                            {
                                if (!ghiHoaDonFile(gioHang, tong, "G\xC3\xB3p 1"))
                                {
                                    if (ql.hoanTacBanNhieuSanPham(gioHang))
                                    {
                                        thongBao = "Không ghi được hoadon.txt. Tồn kho đã được hoàn lại; hãy kiểm tra quyền ghi tệp.";
                                    }
                                    else
                                    {
                                        thongBao = "Lỗi ghi hóa đơn và không thể hoàn tồn kho. Hãy kiểm tra sanpham.txt và hoadon.txt.";
                                    }
                                }
                                else
                                {
                                    doanhThuPhien += tong;
                                    thongBao = "Đã xuất hóa đơn Góp 1. Tổng tiền: " + dinhDangTien(tong) + " VNĐ.";
                                    gioHang.clear();
                                    oMaBan.clear();
                                    oSoLuongBan.clear();
                                }
                            }
                        }
                    }

                    dongHoThongBao.restart();
                }
                else if (trang == DANH_SACH &&
                         dangTimKiem && bamNut(nutTimKiem))
                {
                    string tuKhoa = toUTF8(oTimKiem);
                    ketQuaTimKiem = ql.timKiem(tuKhoa);
                    dangLoc = true;
                    thongBao = "Tìm thấy " +
                        to_string(ketQuaTimKiem.size()) +
                        " sản phẩm.";
                    dongHoThongBao.restart();
                }
                else if (trang == DANH_SACH &&
                         dangTimKiem && bamNut(nutXoaTim))
                {
                    oTimKiem.clear();
                    dangLoc = false;
                    thongBao = "Đã xóa từ khóa tìm kiếm.";
                    dongHoThongBao.restart();
                }
                else if (trang == THEM_SAN_PHAM)
                {
                    // Chon o nhap lieu
                    if (sf::FloatRect(390, 220, 440, 61).contains(mouse))
                        oDangNhap = 1;
                    else if (sf::FloatRect(390, 300, 440, 61).contains(mouse))
                        oDangNhap = 2;
                    else if (sf::FloatRect(390, 380, 440, 61).contains(mouse))
                        oDangNhap = 3;
                    else if (sf::FloatRect(390, 460, 440, 61).contains(mouse))
                        oDangNhap = 4;

                    if (bamNut(nutLuu))
                    {
                        string ma = toUTF8(oMa);
                        string ten = toUTF8(oTen);
                        string giaStr = toUTF8(oGia);
                        string slStr = toUTF8(oSoLuong);

                        try
                        {
                            size_t p1, p2;

                            double gia = stod(giaStr, &p1);
                            int sl = stoi(slStr, &p2);

                            if (p1 != giaStr.size() ||
                                p2 != slStr.size() ||
                                gia < 0 || sl < 0)
                            {
                                thongBao = "Giá và số lượng phải hợp lệ!";
                            }
                            else if (ql.themSanPham(ma, ten, gia, sl))
                            {
                                trang = DANH_SACH;
                                thongBao = "Thêm sản phẩm thành công!";
                            }
                            else
                            {
                                thongBao =
                                    "Không thể thêm! Hãy kiểm tra mã, dữ liệu và tệp.";
                            }
                        }
                        catch (...)
                        {
                            thongBao =
                                "Đơn giá và số lượng phải là số hợp lệ!";
                        }

                        dongHoThongBao.restart();
                    }
                    else if (bamNut(nutHuy))
                    {
                        trang = DANH_SACH;
                    }
                }
            }

            // Xu ly nhap van ban
            if (event.type == sf::Event::TextEntered)
            {
                sf::Uint32 unicode = event.text.unicode;

                if (unicode == 8) // Backspace
                {
                    sf::String* o = nullptr;

                    if (trang == THEM_SAN_PHAM)
                    {
                        if (oDangNhap == 1) o = &oMa;
                        if (oDangNhap == 2) o = &oTen;
                        if (oDangNhap == 3) o = &oGia;
                        if (oDangNhap == 4) o = &oSoLuong;
                    }
                    else if (trang == DANH_SACH && dangTimKiem)
                    {
                        o = &oTimKiem;
                    }
                    else if (trang == BAN_HANG)
                    {
                        if (oDangNhap == 5) o = &oMaBan;
                        if (oDangNhap == 6) o = &oSoLuongBan;
                    }

                    if (o && !o->isEmpty())
                        o->erase(o->getSize() - 1, 1);
                }
                else if (unicode >= 32 && unicode != 127)
                {
                    if (trang == THEM_SAN_PHAM)
                    {
                        if (oDangNhap == 1)
                            oMa += unicode;
                        else if (oDangNhap == 2)
                            oTen += unicode;
                        else if (oDangNhap == 3 &&
                                 (unicode >= '0' && unicode <= '9' ||
                                  unicode == '.'))
                            oGia += unicode;
                        else if (oDangNhap == 4 &&
                                 unicode >= '0' && unicode <= '9')
                            oSoLuong += unicode;
                    }
                    else if (trang == DANH_SACH && dangTimKiem)
                    {
                        oTimKiem += unicode;
                    }
                    else if (trang == BAN_HANG)
                    {
                        if (oDangNhap == 5 && hopLeKyTuMaHang(unicode))
                            oMaBan += unicode;
                        else if (oDangNhap == 6 && hopLeKyTuSoLuong(unicode))
                            oSoLuongBan += unicode;
                    }
                }
            }

            // Enter de luu san pham
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Enter &&
                trang == THEM_SAN_PHAM)
            {
                string ma = toUTF8(oMa);
                string ten = toUTF8(oTen);

                try
                {
                    double gia = stod(toUTF8(oGia));
                    int sl = stoi(toUTF8(oSoLuong));

                    if (ql.themSanPham(ma, ten, gia, sl))
                    {
                        trang = DANH_SACH;
                        thongBao = "Thêm sản phẩm thành công!";
                    }
                    else
                    {
                        thongBao = "Dữ liệu không hợp lệ hoặc mã đã bị trùng!";
                    }
                }
                catch (...)
                {
                    thongBao = "Hãy nhập đơn giá và số lượng hợp lệ!";
                }

                dongHoThongBao.restart();
            }
        }

        // ==================================================
        // VE GIAO DIEN
        // ==================================================

        window.clear(nen);

        // Thanh tieu de
        veHinh(window, 0, 0, 1100, 75, xanh);

        veChu(window, font,
              "QUẢN LÝ CỬA HÀNG TẠP HÓA QUẢ BÁO",
              25, 18, 27, sf::Color::White);

        // Sidebar
        veHinh(window, 0, 75, 245, 645, mauTrang);

          veChu(window, font, "CHỨC NĂNG", 25, 100, 17, chuDam);

          veNut(window, font, "+ Thêm sản phẩm",
              nutThem, sf::Color(225, 233, 246), chuDam);
          veNut(window, font, "Danh sách sản phẩm",
              nutDanhSach, sf::Color(225, 233, 246), chuDam);
          veNut(window, font, "Tìm kiếm",
              nutTim, sf::Color(225, 233, 246), chuDam);
          veNut(window, font, "Bán hàng",
              nutBanHang, sf::Color(225, 233, 246), chuDam);
          veNut(window, font, "Sắp xếp theo mã",
              nutSapXep, sf::Color(225, 233, 246), chuDam);
          veNut(window, font, "Thống kê",
              nutThongKe, sf::Color(225, 233, 246), chuDam);
          veNut(window, font, "Thoát",
              nutThoat, sf::Color(200, 65, 65));

        // Thanh thong bao
        veHinh(window, 265, 665, 810, 35, mauTrang);

        veChu(window, font, thongBao, 280, 673, 14,
              sf::Color(60, 100, 75));

        // ==================================================
        // TRANG DANH SACH
        // ==================================================

        if (trang == DANH_SACH)
        {
            veChu(window, font, "DANH SÁCH SẢN PHẨM",
                  275, 95, 24, chuDam);

            // O tim kiem
            if (dangTimKiem)
            {
                sf::RectangleShape o(sf::Vector2f(250, 38));
                o.setPosition(590, 100);
                o.setFillColor(mauTrang);
                o.setOutlineThickness(1);
                o.setOutlineColor(xanh);
                window.draw(o);

                sf::Text t;
                t.setFont(font);
                t.setString(oTimKiem);
                t.setCharacterSize(15);
                t.setFillColor(chuDam);
                t.setPosition(600, 107);
                window.draw(t);

                    veNut(window, font, "Tìm kiếm",
                      nutTimKiem, xanh);
                    veNut(window, font, "Xóa lọc",
                      nutXoaTim, xam);
            }

            // Khung bang
            float x = 270;
            float y = 155;
            float w = 810;

            veHinh(window, x, y, w, 40,
                   sf::Color(225, 233, 246));

            veChu(window, font, "MÃ SẢN PHẨM", x + 12, y + 10,
                  14, chuDam);
            veChu(window, font, "TÊN SẢN PHẨM", x + 110, y + 10,
                  14, chuDam);
            veChu(window, font, "ĐƠN GIÁ", x + 475, y + 10,
                  14, chuDam);
            veChu(window, font, "SỐ LƯỢNG", x + 660, y + 10,
                  14, chuDam);

            const vector<SanPham>& ds = ql.getDanhSach();
            const vector<SanPham>& hienThi = ds;

            vector<SanPham> danhSachHienThi =
                dangLoc ? ketQuaTimKiem : hienThi;

            const size_t dongCuoi = danhSachHienThi.size() > soDongToiDa
                ? danhSachHienThi.size() - soDongToiDa
                : 0;
            dongBatDauDanhSach = min(dongBatDauDanhSach, dongCuoi);

            float rowY = y + 40;
            int soDong = 0;

            for (size_t i = dongBatDauDanhSach;
                 i < danhSachHienThi.size() &&
                 i < dongBatDauDanhSach + soDongToiDa;
                 ++i)
            {
                const auto& sp = danhSachHienThi[i];

                sf::Color mauNen = (soDong % 2 == 0)
                    ? sf::Color::White
                    : sf::Color(250, 251, 253);

                veHinh(window, x, rowY, w, 39, mauNen);

                veChu(window, font, sp.getMa(),
                      x + 12, rowY + 9, 14, chuDam);

                string ten = sp.getTen();
                if (ten.size() > 34)
                    ten = ten.substr(0, 31) + "...";

                if (sp.getSoLuong() < 5)
                    ten += " (sắp hết)";

                veChu(window, font, ten,
                      x + 110, rowY + 9, 14, chuDam);

                ostringstream gia;
                gia << fixed << setprecision(0)
                    << sp.getDonGia();

                veChu(window, font, gia.str(),
                      x + 475, rowY + 9, 14, chuDam);

                veChu(window, font, to_string(sp.getSoLuong()),
                      x + 660, rowY + 9, 14, chuDam);

                rowY += 39;
                soDong++;
            }

            if (danhSachHienThi.empty())
            {
                    veChu(window, font, "Chưa có sản phẩm để hiển thị.",
                      400, 240, 17, xam);
            }

            veChu(window, font,
                  "Tổng số loại sản phẩm: " +
                  to_string(ql.tongLoaiSanPham()),
                  275, 635, 15, chuDam);

            if (!danhSachHienThi.empty())
            {
                const size_t dongDau = dongBatDauDanhSach + 1;
                const size_t dongCuoiDangXem = min(
                    dongBatDauDanhSach + soDongToiDa,
                    danhSachHienThi.size());
                veChu(window, font,
                      "Đang xem " + to_string(dongDau) + "-" +
                      to_string(dongCuoiDangXem) + "/" +
                      to_string(danhSachHienThi.size()),
                      800, 635, 14, xam);
            }
        }

        // ==================================================
        // TRANG THEM SAN PHAM
        // ==================================================

        else if (trang == THEM_SAN_PHAM)
        {
            veChu(window, font, "THÊM SẢN PHẨM MỚI",
                  300, 105, 25, chuDam);

            veHinh(window, 275, 155, 800, 420, mauTrang);

              veO(window, font, "Mã sản phẩm",
                 oMa, 390, 180, 440, oDangNhap == 1);

              veO(window, font, "Tên sản phẩm",
                 oTen, 390, 260, 440, oDangNhap == 2);

              veO(window, font, "Đơn giá (VNĐ)",
                 oGia, 390, 340, 440, oDangNhap == 3);

              veO(window, font, "Số lượng",
                 oSoLuong, 390, 420, 440, oDangNhap == 4);

              veNut(window, font, "LƯU SẢN PHẨM",
                  nutLuu, xanh);

              veNut(window, font, "HỦY",
                  nutHuy, xam);

            veChu(window, font,
                   "Nhấp vào ô cần nhập, điền dữ liệu rồi nhấn LƯU.",
                  320, 580, 14, xam);
        }

        // ==================================================
        // TRANG THONG KE
        // ==================================================

        else if (trang == THONG_KE)
        {
            veChu(window, font, "THỐNG KÊ KHO HÀNG",
                  300, 105, 25, chuDam);

            // The san pham
            veHinh(window, 280, 170, 240, 130,
                   sf::Color(225, 238, 255));
            veChu(window, font, "TỔNG LOẠI SẢN PHẨM",
                  300, 195, 15, chuDam);
            veChu(window, font,
                  to_string(ql.tongLoaiSanPham()),
                  300, 235, 30, xanh);

            // The so luong
            veHinh(window, 550, 170, 240, 130,
                   sf::Color(225, 247, 235));
            veChu(window, font, "TỔNG SỐ LƯỢNG",
                  570, 195, 15, chuDam);
            veChu(window, font,
                  to_string(ql.tongSoLuong()),
                  570, 235, 30, sf::Color(30, 145, 85));

            // The gia tri ton kho
            veHinh(window, 820, 170, 250, 130,
                   sf::Color(255, 240, 220));
            veChu(window, font, "GIÁ TRỊ TỒN KHO",
                  840, 195, 15, chuDam);

            ostringstream tong;
            tong << fixed << setprecision(0)
                 << ql.tongGiaTriTonKho();

            veChu(window, font, tong.str() + " VNĐ",
                  840, 235, 23, sf::Color(200, 110, 30));

            veChu(window, font,
                "Giá trị tồn kho = đơn giá × số lượng",
                  300, 340, 17, xam);

            veChu(window, font,
                "Dữ liệu thống kê được tính từ tệp sanpham.txt",
                  300, 375, 15, xam);

            veChu(window, font,
                "Doanh thu trong phiên làm việc: " +
                dinhDangTien(doanhThuPhien) + " VNĐ",
                  300, 410, 17, sf::Color(120, 70, 20));

            vector<const SanPham*> sapHetHang;
            for (const auto& sp : ql.getDanhSach())
            {
                if (sp.getSoLuong() < 5)
                    sapHetHang.push_back(&sp);
            }

            veChu(window, font,
                  "CẢNH BÁO HÀNG SẮP HẾT (tồn kho < 5): " +
                  to_string(sapHetHang.size()),
                  300, 455, 16, sf::Color(180, 70, 35));

            if (sapHetHang.empty())
            {
                veChu(window, font, "Không có mặt hàng nào sắp hết.",
                      300, 482, 14, xam);
            }
            else
            {
                const size_t soDongCanhBao = min(sapHetHang.size(), size_t(7));
                for (size_t i = 0; i < soDongCanhBao; ++i)
                {
                    const SanPham& sp = *sapHetHang[i];
                    veChu(window, font,
                          sp.getMa() + " - " + sp.getTen() +
                          " | Còn " + to_string(sp.getSoLuong()),
                          315, 482 + i * 22, 14, chuDam);
                }

                if (sapHetHang.size() > soDongCanhBao)
                {
                    veChu(window, font,
                          "... và " +
                          to_string(sapHetHang.size() - soDongCanhBao) +
                          " mặt hàng khác.",
                          315, 482 + soDongCanhBao * 22, 14, xam);
                }
            }
        }
        else if (trang == BAN_HANG)
        {
            veChu(window, font, "BÁN HÀNG",
                  300, 105, 25, chuDam);

            veHinh(window, 275, 155, 800, 150, sf::Color(255, 255, 255));
            veO(window, font, "Mã hoặc tên sản phẩm", oMaBan, 360, 175, 370, oDangNhap == 5);
            veO(window, font, "Số lượng", oSoLuongBan, 360, 240, 180, oDangNhap == 6);

            string tuKhoaBanHang = toUTF8(oMaBan);
            ketQuaBanHang.clear();
            if (!tuKhoaBanHang.empty())
            {
                string key = chuyenThuong(tuKhoaBanHang);
                for (const auto& sp : ql.getDanhSach())
                {
                    string maSp = chuyenThuong(sp.getMa());
                    string tenSp = chuyenThuong(sp.getTen());
                    if (maSp.find(key) != string::npos ||
                        tenSp.find(key) != string::npos)
                    {
                        ketQuaBanHang.push_back(sp);
                    }
                }
            }

            if (!tuKhoaBanHang.empty() && !ketQuaBanHang.empty())
            {
                sf::RectangleShape khungTim(sf::Vector2f(370, 95));
                khungTim.setPosition(360, 300);
                khungTim.setFillColor(sf::Color(255, 255, 255));
                khungTim.setOutlineThickness(1);
                khungTim.setOutlineColor(sf::Color(200, 210, 220));
                window.draw(khungTim);

                for (size_t i = 0; i < ketQuaBanHang.size() && i < 4; ++i)
                {
                    const auto& sp = ketQuaBanHang[i];
                    sf::FloatRect box(360, 305 + i * 22, 370, 22);
                    sf::Color bg = (i % 2 == 0) ? sf::Color(248, 250, 252) : sf::Color(255, 255, 255);
                    veHinh(window, box.left, box.top, box.width, box.height, bg);
                    veChu(window, font, sp.getMa() + " - " + sp.getTen(),
                          box.left + 6, box.top + 4, 13, sf::Color(35, 45, 65));
                }
            }

            veHinh(window, 275, 400, 800, 170, sf::Color(255, 255, 255));
            veChu(window, font, "GIỎ HÀNG", 290, 415, 18, chuDam);

            if (gioHang.empty())
            {
                veChu(window, font, "Chưa có mặt hàng nào trong giỏ.", 290, 435, 15, xam);
            }
            else
            {
                double tongGio = 0;
                for (const auto& item : gioHang)
                    tongGio += item.thanhTien();

                const size_t soDongHienThi = min(gioHang.size(), size_t(6));
                for (size_t i = 0; i < soDongHienThi; ++i)
                {
                    const auto& item = gioHang[i];
                    string dong = item.ten + " x " + to_string(item.soLuong) + " = " + dinhDangTien(item.thanhTien()) + " VNĐ";
                    veChu(window, font, dong, 290, 435 + i * 18, 15, chuDam);
                }

                veChu(window, font, "Tổng giỏ: " + dinhDangTien(tongGio) + " VNĐ",
                      290, 435 + soDongHienThi * 18 + 4, 16, sf::Color(180, 80, 0));
            }

            veNut(window, font, "THÊM VÀO GIỎ", nutThemGio, xanh);
            veNut(window, font, "XUẤT HÓA ĐƠN", nutXuatHoaDon, sf::Color(28, 116, 49));
            veNut(window, font, "XÓA GIỎ", nutXoaGio, xam);
        }

        window.display();
    }

    return 0;
}