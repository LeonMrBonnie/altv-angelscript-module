<p align="center">
    <img width="200" src="https://www.angelcode.com/angelscript/sdk/docs/manual/aslogo.png" alt="TfL Logo">
    <h1 align="center">alt:V AngelScript Module</h1>
    <p align="center">An alt:V module providing support for the <a href="https://www.angelcode.com/angelscript/sdk/docs/manual/index.html">AngelScript scripting language</a></p>
</p>

## How to build

### Prerequisites

- CMake 3.8 or higher
- A working C++ compiler (E.g. MSVC or GCC)
- Git

### Instructions

1. Run `git clone --recurse-submodules https://github.com/LeonMrBonnie/altv-angelscript-module.git` to clone the repository
2. Navigate to the folder with `cd altv-angelscript-module`
3. Run the `build.bat` *(Windows)* or the `build.sh` *(Linux)* to build the project using CMake
4. The built project `.dll` / `.so` can now be found in the `dist` subdirectory

If any errors persist, make sure to follow steps closely.

## How to use

You can find information about the module in the [Wiki](https://github.com/LeonMrBonnie/altv-angelscript-module/wiki).

## How to contribute

If you want to contribute to this in any way, then first of all Thank You!

Just clone the repository, create a new branch to store your changes and create a pull request containing your created branch and changes.

Before commiting a pull request, please make sure to format your code using the projects `clang-format` configuration.
This can be done by either manually using `clang-format` to format the files, using the VSCode task or by running the `tools/format.(bat / sh)` file.

If you are unsure with something and want to know how something works etc. you can contact me on Discord (**LeonMrBonnie#9840**).

> It is suggested to use Visual Studio Code when working on this project,
> as there are already VSCode tasks set-up to simplify the workflow.
