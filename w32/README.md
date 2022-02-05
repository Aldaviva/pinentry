![pinentry](logo-32.bmp) pinentry-w32
===

This program provides a passphrase entry dialog for Windows. Its user interface is built with the Win32 API, and it's used by default in [Git for Windows](https://git-scm.com/download/win). By contrast, [Gpg4win](https://www.gpg4win.org) uses the Qt UI by default.

## Fork with mouse position fix
This fork contains a [fix](https://github.com/Aldaviva/pinentry/commit/2e347c1d7fac5f46c30f1f6d54e048076adf67a2) for the issue where the window does not have focus when it appears, and you have to click the window before typing your passphrase. 

This is caused by the program trying to focus itself when it appears by sending a synthetic mouse click, but the calculation of the pointer position is wrong. When [normalizing the coordinates](https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-mouseinput#remarks) to be in the range [0, 65,535], it uses integer division instead of floating-point division, so some error is introduced, the amount of which depends on your screen resolution. On a 1920×1080 screen, the mouse will move about 4px too far to the left, which still works fine, but on a 2560×1440 screen, it moves about 30px too far to the left, which puts it about 24px outside the window. When it clicks the mouse outside the window, it leaves the window unfocused, so you have to manually move your mouse and click the window to focus it before typing your passphrase.

## Installation
If you installed Git for Windows or Gpg4win, you already have a broken version of `pinentry-w32.exe` in `usr/bin/pinentry.exe` and `usr/bin/pinentry-w32.exe` with the aforementioned mouse position defect.

### Install this fork of `pinentry-w32.exe`
1. Download the [latest release](https://github.com/Aldaviva/pinentry/releases/latest) ZIP, or the [latest artifact](https://nightly.link/Aldaviva/pinentry/workflows/w32/w32-mouse-position-fix/pinentry-w32.zip) from the [workflow builds](https://github.com/Aldaviva/pinentry/actions/workflows/w32.yml?query=is%3Asuccess).
1. Extract the ZIP file to a directory on your hard drive, such as `C:\Program Files (x86)\GnuPG\`.

### Configure GPG to use this program
1. If it doesn't already exist, create an empty [GPG Agent configuration file](https://www.gnupg.org/documentation/manuals/gnupg/Agent-Options.html) and save it in `%USERPROFILE%\.gnupg\gpg-agent.conf`.
1. In `gpg-agent.conf`, append the following line.
    ```text
    pinentry-program "C:\Program Files (x86)\GnuPG\pinentry-w32.exe"
    ```
1. If `gpg-agent.exe` is already running, you must kill that process so it will load your new configuration.

### Configure Git to use GPG
1. Ensure that GPG signing is enabled for Git commits by running the following command if you haven't already.
    ```cmd
    git config --global commit.gpgsign true
    ```
1. To set up a GPG key and use it to sign Git commits, you can follow the [instructions on GitHub](https://docs.github.com/en/authentication/managing-commit-signature-verification).
1. To avoid a `git-hook` error when committing within Visual Studio, you may want to configure Git to use the absolute path of `gpg.exe`, since [Visual Studio doesn't put it on the `PATH` by default](https://developercommunity.visualstudio.com/t/Commit-Problem-in-Visual-Studio/1524505#T-N1553996-N1576529).
    ```cmd
    git config --global gpg.program "C:/Program Files/Git/usr/bin/gpg.exe"
    ```

## Compiling
If you wish to compile this program yourself, you can use MinGW on Linux.

### Operating systems
Tested with Debian 11 Bullseye and Ubuntu 20.04 Focal Fossa.

### Install dependencies
```sh
apt install make binutils automake gettext texinfo mingw-w64 libgpg-error-dev libassuan-mingw-w64-dev
```

### Build
```sh
cd w32
./autogen.sh
./autogen.sh --build-w32
make
```

### Output files
- `w32/pinentry-w32.exe`
- `/usr/i686-w64-mingw32/bin/libassuan-0.dll`
- `/usr/i686-w64-mingw32/bin/libgpg-error-0.dll`