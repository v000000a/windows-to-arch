# windows-to-arch

Experimental tool that allows installing a full Arch Linux system directly from a running Windows installation, followed by repurposing the original Windows partition into btrfs.

## Warning

This software is highly experimental and carries significant risk.

It modifies the disk partition table, creates a new boot entry in the Windows Boot Manager, and permanently removes the original Windows installation.

**Use only if you fully understand the risks and have backups of important data.**

## Requirements

- Windows 10 or 11 (64-bit)
- Administrator privileges
- BitLocker must be disabled on the system drive
- Secure Boot is strongly recommended to be disabled in BIOS/UEFI
- At least 15 GB of free space on the C: drive

## Build and Usage

1. Compile the program:
   ```cmd
   g++ stage1.cpp -o stage1.exe
   ```
2. Run as administrator:
   ```cmd
   stage1.exe
   ```
3. After the program finishes, run as administrator:
   ```cmd
   stage2.bat
   ```
4. Reboot, enter boot menu and select "Arch Linux"

Features

   1. Choice of Arch Linux mirror during setup
   2. Automatic EFI partition detection
   3. Full Arch Linux installation via pacstrap
   4. Repurposes the original Windows partition into btrfs

Notes

 1. If the system fails to boot after reboot, disable Secure Boot in BIOS/UEFI.
 2. Recovery of the original Windows installation is not supported after completion.
