# Arch Deformator

Experimental tool that allows installing a full Arch Linux system directly from a running Windows installation, followed by repurposing the original Windows partition.

## Warning

This software is highly experimental and carries significant risk.  
It modifies the disk partition table, installs a new boot entry, and permanently removes the original Windows installation.

Use only if you fully understand the risks and have backups of important data.

## Requirements

- Windows 10 / 11 (64-bit)
- Administrator privileges
- BitLocker disabled on the system drive
- Secure Boot strongly recommended to be disabled
- At least 15 GB of free space on C:

## Build and Usage

1. Compile:
   ```cmd
   g++ deformator.cpp -o deformator.exe
   ```
2. Run as Administrator: 
  ```cmd
  deformator.exe
  ```
3. After completion run as Administrator: 
  ```cmd
  deformator_stage_2.bat
  ```
4. Reboot and select "Arch Linux" in the boot menu.
