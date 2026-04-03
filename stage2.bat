@echo off
echo === arch-to-windows v6.3 - Final Step ===
echo.

echo Searching for EFI System Partition...

set EFIDRIVE=
for %%v in (C D E F G H I J K L M N O P Q R S T U V W X Y Z) do (
  mountvol %%v: /S >nul 2>&1
  if exist %%v:\EFI\Boot\bootx64.efi (
    set EFIDRIVE=%%v:
    goto found
  )
  mountvol %%v: /D >nul 2>&1
)

:found
if "%EFIDRIVE%"=="" (
  echo ERROR: Could not locate EFI System Partition automatically.
  pause
  exit /b 1
)

echo Found EFI partition: %EFIDRIVE%

mkdir "%EFIDRIVE%\EFI\Arch" 2>nul

copy "Z:\vmlinuz-linux.efi" "%EFIDRIVE%\EFI\Arch\" >nul
copy "Z:\initramfs-linux.img" "%EFIDRIVE%\EFI\Arch\" >nul
copy "Z:\full_arch_setup.sh" "%EFIDRIVE%\EFI\Arch\" >nul

echo Creating boot entry...

for /f "tokens=2 delims={}" %%g in ('bcdedit /copy {current} /d "Arch Linux"') do set GUID=%%g

bcdedit /set {%GUID%} device partition=Z:
bcdedit /set {%GUID%} path \EFI\Arch\vmlinuz-linux.efi
bcdedit /set {%GUID%} kernelparams "root=LABEL=ARCH_ROOT rw initrd=\EFI\Arch\initramfs-linux.img init=/full_arch_setup.sh"

echo.
echo Operation completed successfully.
echo.
echo Reboot the system.
echo In the boot menu you will see "Arch Linux".
echo.
echo Note: If the system fails to boot, disable Secure Boot in BIOS/UEFI.
pause
