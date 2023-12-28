# On Your Own Machine
## Windows

The Windows Subsystem for Linux (WSL) is a tool allowing Linux to run in Windows natively.

1.  Follow [Microsoft’s instructions](https://docs.microsoft.com/en-us/windows/wsl/install-win10) for installing WSL, choosing Ubuntu as the distribution.
2.  Continue following Microsoft’s instructions for [initializing your Ubuntu distribution](https://docs.microsoft.com/en-us/windows/wsl/initialize-distro)
3.  Download the packages used for C++ by running the following command to install the packages we will be using:

```
sudo apt-get update; sudo apt-get install clang-6.0 libc++abi-dev libc++-dev git gdb valgrind graphviz imagemagick gnuplot make
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-6.0 100
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-6.0 100
sudo update-alternatives --install /usr/bin/llvm-symbolizer llvm-symbolizer /usr/bin/llvm-symbolizer-6.0 100
```

4.  If you’re using Ubuntu 20.04 and/or running into issues with standard library `include` statements:
    
    ```
    sudo apt-get install clang
    ```
    

### Using your Windows Desktop in WSL/Ubuntu

When you start Ubuntu, your shell will begin in the Linux home directory. However, you will likely want to work with files that are inside of your Windows file system. By default, your `C:` drive is mounted at `/mnt/c`.

Some find it useful to create a shortcut to jump directly to your Windows Desktop:

-   Navigate to your Windows desktop within the Linux shell:

```
  cd /mnt/c/Users/
  cd <Your-User-Name-on Windows>  // Use `ls` to find the available users in green
  cd Desktop
```

-   In your Windows Desktop directory you just navigated to, run the following command to create a symbolic link (ex: a shortcut) from your Linux home directory to your Windows desktop to easily navigate there in the future:

```
  ln -s `pwd` ~/desktop
```

-   In the future when you launch WSL/Ubuntu, you can immediately jump to your Windows desktop by running `cd desktop`.

## MacOS

Tools used on macOS will not always give results consistent with those used on EWS. Make sure to test your code on EWS to make sure it will work with our grading environment.

### To install [Homebrew](https://brew.sh/): (You can skip this if you already have Homebrew installed)

In your terminal, (Applications->Utilities->Terminal) run:

```
xcode-select --install
```

to install the Xcode command line tools.

Next, we will install the [Homebrew Package Manager](https://brew.sh/) for macOS using the following command:

```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
```

### To install the necessary packages:

We need to install some tooling packages for future assignments. Run:

```
brew install ghostscript
brew install imagemagick
brew install graphviz
```

As you run each of these commands, it is recommended to follow Homebrew’s instructions for updating your PATH and compiler variables.

### LLDB

LLDB is the default debugger for Xcode, so you should already have it installed by now. It is very similar to the GNU debugger, GDB. Here is a [LLDB cheatsheet](https://gist.github.com/ryanchang/a2f738f0c3cc6fbd71fa)

### Valgrind

Valgrind might not be compatible with your version of MacOS. Please check whether it is compatible on [valgrind’s official website](https://valgrind.org/) before trying to install.

Homebrew can also be used to install Valgrind.

To do so, run:

```
brew edit valgrind
```

This will open your default code editor. In the opened file, change the URL in the `head` section from `https://sourceware.org/git/valgrind.git` to `git://sourceware.org/git/valgrind.git` and run the following:

```
brew update
brew install --HEAD valgrind
```

# DISCLAIMER
If you would like to incorporate any portion of code in this repository into your own work, please cite as follows:

```
... ; This portion of code is written by Bryan Wang (https://github.com/Beryex/UIUC-ECE220)
```

THE AUTHOR OF THIS REPOSITORY SHALL **IN NO CASE** BE HELD LIABLE FOR ANY KIND OF ACADEMIC MISCONDUCT, INCLUDING BUT NOT LIMITED TO PLAGIARISM, AS A RESULT OF IMPROPER OR MISSING CITATION IN THE CORRESPONDING WORK.
