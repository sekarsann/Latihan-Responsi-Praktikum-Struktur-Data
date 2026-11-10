#include <iostream>

using namespace std;

struct nodeQueue {
    int nota;
    string nama;
    nodeQueue* next;
};

struct nodeStack {
    int nota;
    string nama;
    nodeStack* next;
};

struct nodeTree {
    int nota;
    string nama;
    nodeTree* kiri;
    nodeTree* kanan;
};

nodeQueue* front = NULL;
nodeQueue* last = NULL;
nodeStack* top = NULL;
nodeTree* root = NULL;

// menambahkan node baru ke tree berdasarkan nomor nota.
// nota lebih kecil masuk ke kiri, nota lebih besar masuk ke kanan.
// mengembalikan akar tree setelah penambahan.
nodeTree* tambah_tree(nodeTree* akar, int nota, string nama) {
    if (akar = nullptr){
        return new nodeTree {nota, nama, nullptr, nullptr};
    }
    if (nota < akar->nota){
        akar->kiri = tambah_tree(akar->kiri, nota, nama);
    } else {
        akar->kanan = tambah_tree(akar->kanan, nota, nama); 
    }
    return akar;
}

// mencari node di tree berdasarkan nomor nota.
// mengembalikan node yang ditemukan, atau null jika tidak ada.
nodeTree* cari_tree(nodeTree* akar, int nota) {
    if (akar == nullptr || nota == akar->nota) {
        return akar;}
    if (nota < akar->nota){
        return cari_tree(akar->kiri, nota);
    }
    return cari_tree(akar->kanan, nota);
}

// menghapus node di tree berdasarkan nomor nota.
// harus menangani 3 kasus: tanpa anak, satu anak, dan dua anak.
// mengembalikan akar tree setelah penghapusan.
nodeTree* hapus_tree(nodeTree* akar, int nota) {
    if (akar == nullptr) {
        return nullptr; 
    }
    if (nota < akar->nota) { 
        akar->kiri = hapus_tree(akar->kiri, nota);
    } else if (nota > akar->nota) { 
        akar->kanan = hapus_tree(akar->kanan, nota);
    } else if (akar->kiri == nullptr) {
            nodeTree* sisa = akar->kanan; 
            delete akar;
            return sisa;
    } else if (akar->kanan == nullptr) {
            nodeTree* sisa = akar->kiri; 
            delete akar;
            return sisa;
    } else {
        nodeTree* pengganti = akar->kanan;
        while (pengganti->kiri != nullptr) {
            pengganti = pengganti->kiri;
        }
        akar->nota = pengganti->nota;
        akar->nama = pengganti->nama;
        akar->kanan = hapus_tree(akar->kanan, pengganti->nota);
    }
}

// menampilkan isi tree dengan urutan: node, kiri, kanan.
// parameter no dipakai sebagai nomor urut tampilan.
void preorder(nodeTree* akar, int& no) {
    if (akar == nullptr)
        return;
    cout << no << ". [" << akar->nota << "] " << akar->nama << endl; 
    no++;
    preorder(akar->kiri, no); 
    preorder(akar->kanan, no);
}

// menampilkan isi tree dengan urutan: kiri, node, kanan.
// hasilnya akan terurut berdasarkan nomor nota.
void inorder(nodeTree* akar, int& no) {
    if (akar == nullptr)
        return;
    inorder(akar->kiri, no); 
    cout << no << ". [" << akar->nota << "] " << akar->nama << endl; 
    no++;
    inorder(akar->kanan, no);
}

// menampilkan isi tree dengan urutan: kiri, kanan, node.
void postorder(nodeTree* akar, int& no) {
    if (akar == nullptr)
        return;
    postorder(akar->kiri, no); 
    postorder(akar->kanan, no);
    cout << no << ". [" << akar->nota << "] " << akar->nama << endl; 
    no++;
}

// meminta input nomor nota dan nama cucian, lalu memasukkannya ke queue (posisi paling belakang).
// error handling: nota bukan angka atau kurang dari 1, nota sudah dipakai
// (cek di queue dan stack), dan nama kosong.
void tambah_cucian() {
    int nota;
    string nama;
    cout << "Masukkan Nomor Nota: ";
    cin >> nota;
    if (cin.fail() || nota <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nomor Nota Harus Angka > 0" << endl;
        return;
    }
    nodeQueue* q = front;
    while (q != nullptr){
        if(q->nota == nota){
            cout << "Nomor Nota Sudah Ada yang Punya!" << endl;
            return;
        }
        q = q->next;
    }
    nodeStack* s = top;
    while (s != nullptr){
        if(s->nota == nota){
            cout << "Nomor Nota Sudah Ada yang Punya!" << endl;
            return;
        }
        s = s->next;
    }
    cout << "Masukkan Nama Cucian: ";
    cin.ignore();
    getline(cin, nama);
    if (nama.empty()) {
        cout << "Nama Cucian Tidak Boleh Kosong" << endl;
        return;
    }
    nodeQueue* bantu = new nodeQueue {nota, nama, nullptr};
    if (front == nullptr) {
        front = last = bantu;
    } else {
        last->next = bantu;
        last = bantu;
    }
    cout << "Berhasil Memasukkan Cucian" << endl;
}

// menampilkan seluruh cucian di queue dari depan sampai belakang.
// error handling jika queue kosong.
void lihat_antrean() {
    if (front == nullptr){
        cout << "Antrean Masih Kosong" << endl;
        return;
    }
    nodeQueue *bantu = front;
    int no = 1;
    cout << "Cucian dalam Antrean: " << endl;
    while (bantu != nullptr){
        cout << no << ". [" << bantu->nota << "] " << bantu->nama << endl;
        bantu = bantu->next;
    }
    cout << endl;
}

// mengambil cucian paling depan dari queue, lalu memindahkannya ke stack (posisi paling atas)
// dan memasukkannya ke tree (rak). node queue dihapus setelah dipindahkan.
// error handling jika queue kosong.
void proses_cucian() {
    if (front == nullptr) {
        cout << "Antrean Masih Kosong" << endl;
        return;
    }

    nodeQueue* hapus = front;
    nodeStack* bantu = new nodeStack {hapus->nota, hapus->nama, top};
    top = bantu;
    root = tambah_tree(root, hapus->nota, hapus->nama);
    cout << "Cucian Berhasil Ditambahkan" << hapus->nama << "!" << endl << endl;
    front = hapus->next;
    if (front == nullptr) {
        last = nullptr;
    }
    delete hapus;
}

// menampilkan seluruh cucian di stack dari atas sampai bawah.
// error handling jika stack kosong.
void lihat_riwayat() {
    if (top == nullptr) {
        cout << "Belum Ada Cucian yang Selesai!" << endl;
        return;
    }
    nodeStack* bantu = top;
    int no = 1;
    cout << "Cucian yang Sudah Selesai: " << endl;
    while (bantu != NULL) {
        cout << no << ". [" << bantu->nota << "] " << bantu->nama << endl;
        bantu = bantu->next;
        no++;
    }
    cout << endl;
}

// meminta input nomor nota, lalu mencarinya di tree (rak) dan menampilkan datanya.
// error handling jika rak kosong, input nota tidak valid, atau nota tidak ditemukan.
void cari_cucian() {
     if (root == nullptr) {
        cout << "Rak Masih Kosong" << endl;
        return;
    }
    int nota;
    cout << "Masukkan Nomor Nota: ";
    cin >> nota;
    if (cin.fail() || nota <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nomor Nota Harus Berupa Angka > 0"<< endl;
        return;
    }
    nodeTree* hasil = cari_tree(root, nota);
    if (hasil == nullptr) {
        cout << "Cucian dengan Nota " << nota << " Tidak Ada di Rak"<< endl;
        return;
    }
    cout << "Ditemukan: [" << hasil->nota << "] " << hasil->nama << endl << endl;
}

// meminta user memilih pre-order, in-order, atau post-order,
// lalu menampilkan isi tree sesuai pilihan.
// error handling jika rak kosong atau pilihan tidak valid.
void lihat_rak() {
     if (root == nullptr) {
        cout << "Rak Masih Kosong" << endl << endl;
        return;
    }
    char pilihan;
    int no = 1;
    cout << "=== Isi Rak Cucian ===" << endl << endl;
    cout << "1. Pre-order" << endl;
    cout << "2. In-order" << endl;
    cout << "3. Post-order" << endl << endl;
    cout << "Masukkan pilihan anda: ";
    cin >> pilihan;
    cout << endl;
    switch (pilihan) {
        case '1': {
            preorder(root, no);
            break;
        }
        case '2': {
            inorder(root, no);
            break;
        }
        case '3': {
            postorder(root, no);
            break;
        }
        default: {
            cout << "Pilihan yang Anda Masukkan Salah!" << endl;
        }
    }
    cout << endl;
}

// meminta input nomor nota cucian yang diambil pelanggan, lalu menghapusnya dari tree (rak).
// data di stack tidak ikut terhapus.
// error handling jika rak kosong, input nota tidak valid, atau nota tidak ditemukan.
void ambil_cucian() {
     if (root == nullptr) {
        cout << "Rak Masih Kosong" << endl;
        return;
    }
    int nota;
    cout << "Masukkan Nomor Nota: ";
    cin >> nota;
    if (cin.fail() || nota <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nomor Nota Harus Berupa Angka > 0!" << endl;
        return;
    }
    nodeTree* hasil = cari_tree(root, nota);
    if (hasil == nullptr) {
        cout << "Cucian dengan Nota " << nota << " Tidak Ada di Rak!"  << endl;
        return;
    }
    cout << "Cucian " << hasil->nama << " Telah Diambil" << endl;
    root = hapus_tree(root, nota);
}

// menampilkan menu utama dan memanggil fungsi sesuai pilihan user sampai user memilih keluar.
// menu: tambah cucian (bisa diulang), lihat antrean, proses cucian, lihat riwayat,
// cari cucian, lihat isi rak, ambil cucian, dan keluar.
// error handling jika pilihan di luar menu.
int main() {
    char pilihan = '0';
    while (pilihan != '8') {
        cout << "=== Manajemen Laundry Rani ===" << endl << endl;
        cout << "1. Tambah Cucian" << endl;
        cout << "2. Lihat Cucian Dalam Antrean" << endl;
        cout << "3. Proses Cucian" << endl;
        cout << "4. Lihat Cucian Yang Sudah Selesai" << endl;
        cout << "5. Cari Cucian Di Rak" << endl;
        cout << "6. Lihat Isi Rak" << endl;
        cout << "7. Ambil Cucian" << endl;
        cout << "8. Keluar" << endl << endl;
        cout << "Masukkan pilihan anda: ";
        cin >> pilihan;
        switch (pilihan) {
            case '1': {
                char pil_tambah = 'y';
                while (pil_tambah == 'y' || pil_tambah == 'Y') {
                    tambah_cucian();
                    cout << "Apakah anda ingin menambahkan cucian lagi? (y/n): ";
                    cin >> pil_tambah;
                }
                break;
            }
            case '2': {
                lihat_antrean();
                break;
            }
            case '3': {
                proses_cucian();
                break;
            }
            case '4': {
                lihat_riwayat();
                break;
            }
            case '5': {
                cari_cucian();
                break;
            }
            case '6': {
                lihat_rak();
                break;
            }
            case '7': {
                ambil_cucian();
                break;
            }
            case '8': {
                cout << "Program akan dikeluarkan.";
                exit(0);
            }
            default: {
                cout << "Pilihan yang anda masukkan salah!" << endl;
                cout << "Silahkan coba lagi." << endl << endl;
            }
        }
    }
}