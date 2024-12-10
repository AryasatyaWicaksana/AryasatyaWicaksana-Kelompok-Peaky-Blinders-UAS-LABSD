#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <limits.h>

using namespace std;

// Struktur data produk
struct Produk {
    int id;
    string nama;
    string kategori;
    int harga;
};

// Struktur data keranjang belanja
struct KeranjangItem {
    Produk produk;
    int jumlah;
};

// Struktur data transaksi
struct Transaksi {
    int id;
    vector<KeranjangItem> keranjang;
    int totalBiaya;
    int biayaPengiriman;
    int totalPembayaran;
};

// Daftar produk
vector<Produk> daftarProduk = {
    {1, "Smartphone Samsung Galaxy S23", "Elektronik", 12000000},
    {2, "Laptop ASUS ROG Zephyrus G14", "Elektronik", 25000000},
    {3, "TV LED LG 43 Inch", "Elektronik", 6500000},
    {4, "Earbuds Apple AirPods Pro", "Elektronik", 3500000},
    {5, "Kamera DSLR Canon EOS 90D", "Elektronik", 17000000},
    {6, "Sepeda MTB Polygon", "Fitness", 5000000},
    {7, "Dumbbell 5kg", "Fitness", 200000},
    {8, "Matras Yoga", "Fitness", 250000},
    {9, "Mesin Elliptical", "Fitness", 7500000},
    {10, "Resistance Band", "Fitness", 100000},
    {11, "Serum Wajah Vitamin C", "Kecantikan", 150000},
    {12, "Masker Wajah Aloe Vera", "Kecantikan", 75000},
    {13, "Lipstik Matte L'Oréal", "Kecantikan", 120000},
    {14, "Parfum Chanel", "Kecantikan", 2500000},
    {15, "Sabun Cuci Muka Himalaya", "Kecantikan", 35000},
    {16, "Roti Tawar Serba Roti", "Konsumsi", 15000},
    {17, "Kopi Arabica 100g", "Konsumsi", 50000},
    {18, "Mie Instan", "Konsumsi", 5000},
    {19, "Susu UHT Indomilk 1 Liter", "Konsumsi", 18000},
    {20, "Teh Kotak Sosro 500ml", "Konsumsi", 7500}
};

// Keranjang belanja
vector<KeranjangItem> keranjang;

// Riwayat transaksi
vector<Transaksi> riwayatTransaksi;

// Representasi graf untuk fitur 6
vector<vector<pair<int, int>>> graf(10);
void inisialisasiGraf() {
    graf[0] = {{2, 7}, {3, 4}, {6, 4}, {8, 5}};
    graf[1] = {};
    graf[2] = {{1, 5}};
    graf[3] = {};
    graf[4] = {{7, 7}};
    graf[5] = {{4, 9}, {7, 8}, {9, 3}};
    graf[6] = {{1, 4}, {4, 2}, {5, 11}};
    graf[7] = {};
    graf[8] = {{5, 10}};
    graf[9] = {{7, 4}};
}

// Variabel untuk menyimpan status urutan terakhir
enum Urutan { ID, HARGA };
Urutan urutanTerakhir = ID;

// Fungsi untuk mencetak garis horizontal
void cetakGaris(int panjang) {
    for (int i = 0; i < panjang; i++) {
        cout << "=";
    }
    cout << "\n";
}

// Fungsi untuk melihat semua produk yang ada
void lihatProduk() {
    cout << "\nDaftar Produk yang Tersedia:\n";
    cetakGaris(80);
    cout << left << setw(5) << "ID"
         << setw(40) << "Nama Produk"
         << setw(15) << "Kategori"
         << setw(15) << "Harga" << endl;
    cetakGaris(80);

    for (const auto &produk : daftarProduk) {
        cout << left << setw(5) << produk.id
             << setw(40) << produk.nama
             << setw(15) << produk.kategori
             << "Rp. " << setw(10) << produk.harga << endl;
    }
    cetakGaris(80);
}

// Fungsi sorting produk berdasarkan harga atau kategori tertentu dengan pengurutan dua kriteria
void urutkanProduk() {
    int pilihan;
    cout << "Urutkan berdasarkan:\n";
    cout << "1. Harga (Termurah - Termahal)\n";
    cout << "2. Kategori\n";
    cout << "3. ID (Urutan Default)\n";
    cout << "Pilihan: ";
    cin >> pilihan;

    if (pilihan == 1) {
        // Urutkan berdasarkan harga
        sort(daftarProduk.begin(), daftarProduk.end(), [](Produk a, Produk b) {
            return a.harga < b.harga;
        });
        urutanTerakhir = HARGA;
        cout << "Daftar Produk (Diurutkan Berdasarkan Harga):\n";
        lihatProduk();
    } else if (pilihan == 2) {
        // Urutkan berdasarkan kategori
        vector<string> kategoriUnik;
        for (const auto &produk : daftarProduk) {
            if (find(kategoriUnik.begin(), kategoriUnik.end(), produk.kategori) == kategoriUnik.end()) {
                kategoriUnik.push_back(produk.kategori);
            }
        }

        cout << "Pilih Kategori:\n";
        for (int i = 0; i < kategoriUnik.size(); ++i) {
            cout << i + 1 << ". " << kategoriUnik[i] << endl;
        }

        int pilihanKategori;
        cout << "Masukkan pilihan kategori (1-" << kategoriUnik.size() << "): ";
        cin >> pilihanKategori;

        if (pilihanKategori < 1 || pilihanKategori > kategoriUnik.size()) {
            cout << "Pilihan kategori tidak valid.\n";
            return;
        }

        string kategoriDipilih = kategoriUnik[pilihanKategori - 1];
        cout << "Produk dalam kategori \"" << kategoriDipilih << "\":\n";

        // Filter produk berdasarkan kategori yang dipilih
        vector<Produk> produkFiltered;
        for (const auto &produk : daftarProduk) {
            if (produk.kategori == kategoriDipilih) {
                produkFiltered.push_back(produk);
            }
        }

        // Menampilkan produk yang sudah diurutkan sesuai dengan urutan terakhir
        if (urutanTerakhir == HARGA) {
            sort(produkFiltered.begin(), produkFiltered.end(), [](Produk a, Produk b) {
                return a.harga < b.harga;
            });
        } else if (urutanTerakhir == ID) {
            sort(produkFiltered.begin(), produkFiltered.end(), [](Produk a, Produk b) {
                return a.id < b.id;
            });
        }

        // Menampilkan produk yang sudah difilter dan diurutkan
        cout << "\nDaftar Produk yang Tersedia:\n";
        cetakGaris(80);
        cout << left << setw(5) << "ID"
             << setw(40) << "Nama Produk"
             << setw(15) << "Kategori"
             << setw(15) << "Harga" << endl;
        cetakGaris(80);
    
        for (const auto &produk : produkFiltered) {
            cout << left << setw(5) << produk.id
                 << setw(40) << produk.nama
                 << setw(15) << produk.kategori
                 << "Rp. " << setw(10) << produk.harga << endl;
        }
        cetakGaris(80);
    } else if (pilihan == 3) {
        // Urutkan berdasarkan ID (kembali ke urutan default)
        sort(daftarProduk.begin(), daftarProduk.end(), [](Produk a, Produk b) {
            return a.id < b.id;
        });
        urutanTerakhir = ID;
        cout << "Daftar Produk (Diurutkan Berdasarkan ID):\n";
        lihatProduk();
    } else {
        cout << "Pilihan tidak valid.\n";
    }
}

// Fungsi menambahkan produk ke keranjang
void tambahkanKeKeranjang() {
    int id, jumlah;
    cout << "Masukkan ID produk: ";
    cin >> id;
    cout << "Masukkan jumlah: ";
    cin >> jumlah;

    auto it = find_if(daftarProduk.begin(), daftarProduk.end(), [id](Produk p) {
        return p.id == id;
    });

    if (it != daftarProduk.end()) {
        keranjang.push_back({*it, jumlah});
        cout << "Produk berhasil ditambahkan ke keranjang!\n";
    } else {
        cout << "Produk dengan ID tersebut tidak ditemukan.\n";
    }
}

// Fungsi mencari produk
void cariProduk() {
    int id;
    cout << "Masukkan ID produk: ";
    cin >> id;

    auto it = find_if(daftarProduk.begin(), daftarProduk.end(), [id](Produk p) {
        return p.id == id;
    });

    if (it != daftarProduk.end()) {
        cout << "Produk ditemukan: " << it->nama << ", Harga: Rp. " << it->harga << endl;
    } else {
        cout << "Produk dengan ID tersebut tidak ditemukan.\n";
    }
}

// Fungsi melihat keranjang
void lihatKeranjang() {
    if (keranjang.empty()) {
        cout << "Keranjang kosong.\n";
        return;
    }

    cout << "\nIsi Keranjang Belanja Anda:\n";
    cetakGaris(80);
    cout << left << setw(5) << "No"
         << setw(40) << "Nama Produk"
         << setw(10) << "Jumlah"
         << setw(15) << "Harga Satuan"
         << "Harga Total" << endl;
    cetakGaris(80);

    int total = 0;
    int no = 1;
    for (const auto &item : keranjang) {
        int totalHarga = item.produk.harga * item.jumlah;
        cout << left << setw(5) << no++
             << setw(40) << item.produk.nama
             << setw(10) << item.jumlah
             << "Rp. " << setw(10) << item.produk.harga
             << "Rp. " << setw(10) << totalHarga << endl;
        total += totalHarga;
    }

    cetakGaris(80);
    cout << setw(65) << "Total Harga: " << "Rp. " << setw(10) << total << endl;
    cetakGaris(80);
}

// Fungsi untuk menghapus barang dari keranjang
void hapusDariKeranjang() {
    if (keranjang.empty()) {
        cout << "Keranjang belanja kosong!\n";
        return;
    }

    int idProduk;
    cout << "Masukkan ID produk yang ingin dikurangi/dihapus: ";
    cin >> idProduk;

    // Cari produk dalam keranjang
    bool ditemukan = false;
    for (auto it = keranjang.begin(); it != keranjang.end(); ++it) {
        if (it->produk.id == idProduk) {
            ditemukan = true;

            // Pilihan pengguna
            int pilihan;
            cout << "Pilih opsi:\n";
            cout << "1. Kurangi jumlah barang\n";
            cout << "2. Hapus barang dari keranjang\n";
            cout << "Pilihan: ";
            cin >> pilihan;

            if (pilihan == 1) {
                // Kurangi jumlah barang
                int jumlahKurang;
                cout << "Masukkan jumlah yang ingin dikurangi: ";
                cin >> jumlahKurang;

                if (jumlahKurang >= it->jumlah) {
                    // Jika jumlah setelah pengurangan <= 0, hapus barang
                    cout << "Jumlah barang kurang dari atau sama dengan 0 setelah dikurangi. Barang akan dihapus dari keranjang.\n";
                    keranjang.erase(it);
                } else {
                    // Kurangi jumlah barang
                    it->jumlah -= jumlahKurang;
                    cout << "Jumlah barang berhasil dikurangi. Sisa barang: " << it->jumlah << "\n";
                }
            } else if (pilihan == 2) {
                // Hapus barang sepenuhnya
                cout << "Barang \"" << it->produk.nama << "\" telah dihapus dari keranjang.\n";
                keranjang.erase(it);
            } else {
                cout << "Pilihan tidak valid.\n";
            }
            return;
        }
    }

    if (!ditemukan) {
        cout << "Produk dengan ID " << idProduk << " tidak ditemukan di keranjang.\n";
    }
}

// Fungsi algoritma Dijkstra untuk biaya pengiriman
int dijkstra(int start, int tujuan) {
    vector<int> dist(graf.size(), INT_MAX);
    dist[start] = 0;

    vector<bool> visited(graf.size(), false);

    for (int i = 0; i < graf.size(); i++) {
        int minNode = -1;
        for (int j = 0; j < graf.size(); j++) {
            if (!visited[j] && (minNode == -1 || dist[j] < dist[minNode])) {
                minNode = j;
            }
        }

        visited[minNode] = true;

        for (auto neighbor : graf[minNode]) {
            int jarak = neighbor.second;
            if (dist[minNode] + jarak < dist[neighbor.first]) {
                dist[neighbor.first] = dist[minNode] + jarak;
            }
        }
    }

    return dist[tujuan];
}

// Fungsi untuk melakukan pembayaran
void pembayaran() {
    if (keranjang.empty()) {
        cout << "Keranjang belanja kosong! Tambahkan barang ke keranjang sebelum melakukan pembayaran.\n";
        return;
    }

    cout << "\nDetail Keranjang Belanja Anda:\n";
    cetakGaris(70);
    cout << left << setw(40) << "Nama Produk"
         << setw(10) << "Jumlah"
         << setw(15) << "Harga Satuan"
         << "Harga Total" << endl;
    cetakGaris(70);

    int totalHargaBarang = 0;
    for (const auto &item : keranjang) {
        cout << left << setw(40) << item.produk.nama
             << setw(10) << item.jumlah
             << "Rp. " << setw(10) << item.produk.harga
             << "Rp. " << setw(15) << item.jumlah * item.produk.harga << endl;
        totalHargaBarang += item.jumlah * item.produk.harga;
    }

    cetakGaris(70);
    cout << "Total Harga Barang: Rp. " << totalHargaBarang << "\n";

    // Meminta pengguna untuk memasukkan lokasi tujuan
    int tujuan;
    cout << "Masukkan lokasi tujuan (0-9): ";
    cin >> tujuan;

    if (tujuan < 0 || tujuan > 9) {
        cout << "Lokasi tujuan tidak valid!\n";
        return;
    }

    // Hitung jarak pengiriman menggunakan algoritma shortest path
    int jarakPengiriman = dijkstra(0, tujuan); // Fungsi shortest path
    int biayaPengiriman = jarakPengiriman * 3000;

    cout << "\nDetail Pengiriman:\n";
    cetakGaris(40);
    cout << left << setw(20) << "Jarak (km)"
         << "Biaya Pengiriman" << endl;
    cetakGaris(40);
    cout << left << setw(20) << jarakPengiriman
         << "Rp. " << biayaPengiriman << endl;
    cetakGaris(40);

    // Total pembayaran
    int totalPembayaran = totalHargaBarang + biayaPengiriman;
    cout << "\nTotal Pembayaran: Rp. " << totalPembayaran << "\n";

    // Konfirmasi pembayaran
    char konfirmasi;
    cout << "Apakah Anda ingin melanjutkan pembayaran? (y/n): ";
    cin >> konfirmasi;

    if (konfirmasi == 'y' || konfirmasi == 'Y') {
        // Simpan transaksi ke riwayat
        Transaksi transaksiBaru;
        transaksiBaru.id = riwayatTransaksi.size() + 1;
        transaksiBaru.keranjang = keranjang;
        transaksiBaru.totalBiaya = totalHargaBarang;
        transaksiBaru.biayaPengiriman = biayaPengiriman;
        transaksiBaru.totalPembayaran = totalPembayaran;
        riwayatTransaksi.push_back(transaksiBaru);

        // Kosongkan keranjang setelah pembayaran
        keranjang.clear();
        cout << "\nPembayaran berhasil! Transaksi telah disimpan.\n";
    } else {
        cout << "\nPembayaran dibatalkan.\n";
    }
}

// Fungsi untuk melihat riwayat transaksi
void lihatRiwayat() {
    if (riwayatTransaksi.empty()) {
        cout << "Belum ada riwayat transaksi.\n";
        return;
    }

    int pilihan;
    cout << "\nPilih opsi:\n";
    cout << "1. Lihat seluruh transaksi\n";
    cout << "2. Lihat detail transaksi\n";
    cout << "Pilihan: ";
    cin >> pilihan;

    if (pilihan == 1) {
        // Tampilkan seluruh transaksi (ID dan total pembayaran)
        cout << "\nSeluruh Transaksi:\n";
        cetakGaris(50);
        cout << left << setw(15) << "ID Transaksi" 
             << setw(25) << "Total Pembayaran" << endl;
        cetakGaris(50);

        for (const auto &transaksi : riwayatTransaksi) {
            cout << left << setw(15) << transaksi.id 
                 << "Rp. " << setw(20) << transaksi.totalPembayaran << endl;
        }

        cetakGaris(50);
    } else if (pilihan == 2) {
        // Lihat detail transaksi berdasarkan ID
        int idTransaksi;
        cout << "Masukkan ID Transaksi: ";
        cin >> idTransaksi;

        bool ditemukan = false;
        for (const auto &transaksi : riwayatTransaksi) {
            if (transaksi.id == idTransaksi) {
                ditemukan = true;

                cetakGaris(90);
                cout << "ID Transaksi: " << transaksi.id << "\n";

                cetakGaris(90);
                cout << left << setw(40) << "Nama Produk"
                     << setw(10) << "Jumlah"
                     << setw(15) << "Harga Satuan"
                     << "Harga Total" << endl;
                cetakGaris(90);

                for (const auto &item : transaksi.keranjang) {
                    cout << left << setw(40) << item.produk.nama
                         << setw(10) << item.jumlah
                         << "Rp. " << setw(10) << item.produk.harga
                         << "Rp. " << setw(15) << item.jumlah * item.produk.harga << endl;
                }

                cetakGaris(90);
                cout << "Total Harga Barang  : Rp. " << transaksi.totalBiaya << "\n";
                cout << "Jarak Pengiriman    : " << transaksi.biayaPengiriman / 3000 << " km\n";
                cout << "Biaya Pengiriman    : Rp. " << transaksi.biayaPengiriman << "\n";
                cout << "Total Pembayaran    : Rp. " << transaksi.totalPembayaran << "\n";
                cetakGaris(90);
                break;
            }
        }

        if (!ditemukan) {
            cout << "Transaksi dengan ID " << idTransaksi << " tidak ditemukan.\n";
        }
    } else {
        cout << "Pilihan tidak valid.\n";
    }
}

// Fungsi utama
int main() {
    inisialisasiGraf();

    int pilihan;
    do {
        cout << "\nMenu:\n";
        cout << "1. Lihat Semua Produk\n";
        cout << "2. Urutkan Produk\n";
        cout << "3. Tambah ke Keranjang\n";
        cout << "4. Cari Produk\n";
        cout << "5. Lihat Keranjang\n";
        cout << "6. Kurangi/Hapus dari Keranjang\n";
        cout << "7. Pembayaran\n";
        cout << "8. Lihat Riwayat Transaksi\n";
        cout << "0. Keluar\n";
        cout << "Pilihan: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: lihatProduk(); break;
            case 2: urutkanProduk(); break;
            case 3: tambahkanKeKeranjang(); break;
            case 4: cariProduk(); break;
            case 5: lihatKeranjang(); break;
            case 6: hapusDariKeranjang(); break;
            case 7: pembayaran(); break;
            case 8: lihatRiwayat(); break;
            case 0: cout << "Keluar dari program.\n"; break;
            default: cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 0);

    return 0;
}