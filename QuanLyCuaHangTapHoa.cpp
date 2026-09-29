
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

        // Chuyen chuoi ve chu thuong voi cac ky tu ASCII
        transform(tuKhoa.begin(), tuKhoa.end(),
                  tuKhoa.begin(),
                  [](unsigned char c)
                  {
                      return static_cast<char>(tolower(c));
                  });

        for (const auto& sp : ds)
        {
            string ma = sp.getMa();
            string ten = sp.getTen();

            transform(ma.begin(), ma.end(), ma.begin(),
                      [](unsigned char c)
                      {
                          return static_cast<char>(tolower(c));
                      });

            transform(ten.begin(), ten.end(), ten.begin(),
                      [](unsigned char c)
                      {
                          return static_cast<char>(tolower(c));
                      });

            if (ma.find(tuKhoa) != string::npos ||
                ten.find(tuKhoa) != string::npos)
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
        rect.left + (rect.width - bounds.width) / 2
                   - bounds.left,
        rect.top + (rect.height - bounds.height) / 2
                  - bounds.top - 2
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
    text.setPosition(x + 9, y + 32);

    window.draw(text);
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
        THONG_KE
    };

    TrangThai trang = DANH_SACH;

    sf::String oTimKiem;
    sf::String oMa, oTen, oGia, oSoLuong;

    int oDangNhap = 0;
    bool dangTimKiem = false;

    string thongBao = "Chào mừng đến với cửa hàng tạp hóa!";
    sf::Clock dongHoThongBao;

    vector<SanPham> ketQuaTimKiem;
    bool dangLoc = false;

    const sf::Color nen(245, 247, 251);
    const sf::Color xanh(42, 100, 200);
    const sf::Color chuDam(35, 45, 65);
    const sf::Color xam(110, 120, 140);
    const sf::Color mauTrang(255, 255, 255);

    // Cac vi tri nut
    sf::FloatRect nutThem(20, 150, 205, 43);
    sf::FloatRect nutDanhSach(20, 202, 205, 43);
    sf::FloatRect nutTim(20, 254, 205, 43);
    sf::FloatRect nutSapXep(20, 306, 205, 43);
    sf::FloatRect nutThongKe(20, 358, 205, 43);
    sf::FloatRect nutThoat(20, 620, 205, 43);

    sf::FloatRect nutTimKiem(850, 100, 100, 38);
    sf::FloatRect nutXoaTim(960, 100, 105, 38);

    sf::FloatRect nutLuu(570, 510, 125, 42);
    sf::FloatRect nutHuy(710, 510, 125, 42);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

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

            veChu(window, font, "MÃ SP", x + 12, y + 10,
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

            float rowY = y + 40;
            int soDong = 0;

            for (const auto& sp : danhSachHienThi)
            {
                if (soDong >= 11)
                    break;

                sf::Color mauNen = (soDong % 2 == 0)
                    ? sf::Color::White
                    : sf::Color(250, 251, 253);

                veHinh(window, x, rowY, w, 39, mauNen);

                veChu(window, font, sp.getMa(),
                      x + 12, rowY + 9, 14, chuDam);

                string ten = sp.getTen();
                if (ten.size() > 34)
                    ten = ten.substr(0, 31) + "...";

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
        }

        window.display();
    }

    return 0;
}