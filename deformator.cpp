#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <windows.h>

using namespace std;

struct Mirror {
    string name;
    string url;
};

string selectMirror() {
    vector<Mirror> mirrors = {
        {"Geo Mirror (automatic)",      "https://geo.mirror.pkgbuild.com/"},
        {"Yandex Mirror",               "https://mirror.yandex.ru/archlinux/"},
        {"Rackspace (USA)",             "https://mirror.rackspace.com/archlinux/"},
        {"Tsinghua University (China)", "https://mirrors.tuna.tsinghua.edu.cn/archlinux/"},
        {"ETH Zurich (Switzerland)",    "https://mirror.ethz.ch/mirror/archlinux/"},
        {"Kernel.org",                  "https://mirrors.kernel.org/archlinux/"},
        {"University of Waterloo (Canada)", "https://mirrors.ocf.berkeley.edu/archlinux/"},
        {"Custom mirror",               ""}
    };

    cout << "\nSelect Arch Linux mirror:\n\n";
    for (size_t i = 0; i < mirrors.size(); ++i) {
        cout << (i + 1) << ". " << mirrors[i].name << endl;
    }

    cout << "\nEnter number (1-" << mirrors.size() << "): ";
    int choice;
    cin >> choice;

    if (choice < 1 || choice > static_cast<int>(mirrors.size())) {
        cout << "Invalid choice. Using default (Geo Mirror).\n";
        return "https://geo.mirror.pkgbuild.com/";
    }

    if (choice == static_cast<int>(mirrors.size())) {
        cout << "Enter full mirror URL (e.g. https://example.com/archlinux/): ";
        string custom;
        cin.ignore();
        getline(cin, custom);
        if (!custom.empty() && custom.back() != '/') custom += '/';
        return custom;
    }

    return mirrors[choice - 1].url;
}

int main() {
    cout << "=== Arch Deformator v6.3 ===\n";
    cout << "Experimental Windows to Arch Linux installer\n\n";

    if (system("manage-bde -status C: | findstr \"Fully Encrypted\" > nul 2>&1") == 0) {
        cout << "ERROR: BitLocker is enabled on the system drive.\n";
        cout << "Disable BitLocker before continuing.\n";
        return 1;
    }

    string username, password;
    cout << "New username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    string baseMirror = selectMirror();
    cout << "\n[+] Selected mirror: " << baseMirror << endl;

    cout << "\n[*] Creating 12 GB partition for Arch Linux...\n";
    ofstream dp("shrink.txt");
    dp << "select volume C\n"
       << "shrink desired=12288\n"
       << "create partition primary\n"
       << "format fs=ntfs quick label=ARCH_ROOT\n"
       << "assign letter=Z\nexit\n";
    dp.close();

    system("diskpart /s shrink.txt");

    cout << "[*] Downloading kernel and initramfs...\n";
    string mirrorPath = baseMirror + "iso/latest/arch/boot/x86_64/";

    vector<pair<string, string>> files = {
        {mirrorPath + "vmlinuz-linux", "Z:\\vmlinuz-linux.efi"},
        {mirrorPath + "initramfs-linux.img", "Z:\\initramfs-linux.img"}
    };

    for (auto& f : files) {
        string cmd = "powershell -Command \"Start-BitsTransfer -Source '" + f.first + "' -Destination '" + f.second + "' -Priority Foreground\"";
        system(cmd.c_str());
        cout << "[+] Downloaded: " << f.second << endl;
    }

    ofstream setup("Z:\\full_arch_setup.sh");
    setup << "#!/bin/bash\n"
          << "set -e\n\n"
          << "echo \"=== Arch Linux installation started ===\"\n\n"
          << "# Format the new partition as btrfs\n"
          << "mkfs.btrfs -f -L arch_root /dev/disk/by-label/ARCH_ROOT\n"
          << "mount /dev/disk/by-label/arch_root /mnt\n\n"
          << "# Install base system\n"
          << "pacstrap /mnt base base-devel linux linux-firmware btrfs-progs networkmanager sudo vim\n"
          << "genfstab -U /mnt >> /mnt/etc/fstab\n\n"
          << "# Chroot configuration\n"
          << "arch-chroot /mnt /bin/bash << 'CHROOT'\n"
          << "  echo \"" << username << ":" << password << "\" | chpasswd\n"
          << "  echo \"root:" << password << "\" | chpasswd\n"
          << "  useradd -m -G wheel " << username << "\n"
          << "  echo \"" << username << " ALL=(ALL) NOPASSWD: ALL\" >> /etc/sudoers\n"
          << "  echo 'arch-linux' > /etc/hostname\n"
          << "  ln -sf /usr/share/zoneinfo/Europe/Helsinki /etc/localtime\n"
          << "  hwclock --systohc\n"
          << "  echo 'en_US.UTF-8 UTF-8' >> /etc/locale.gen\n"
          << "  locale-gen\n"
          << "  echo 'LANG=en_US.UTF-8' > /etc/locale.conf\n"
          << "  systemctl enable NetworkManager\n"
          << "CHROOT\n\n"
          << "# Warning: Windows partition will NOT be automatically destroyed\n"
          << "echo \"=== Installation completed successfully ===\"\n"
          << "echo \"You are now in a minimal Arch Linux environment.\"\n"
          << "echo \"\"\n"
          << "echo \"To repurpose the old Windows partition manually, you can run:\"\n"
          << "echo \"  mkfs.btrfs -f -L windows_data /dev/sdX   (replace /dev/sdX with the old Windows partition)\"\n"
          << "echo \"  btrfs device add /dev/sdX /\"\n"
          << "echo \"\"\n"
          << "echo \"Reboot with: reboot\"\n";
    setup.close();

    cout << "\n[+] Preparation completed successfully.\n";
    cout << "[*] Run deformator_final.bat as Administrator to finish EFI configuration.\n";

    return 0;
}
