# alt:V AngelScript Module

An alt:V module providing support for the [AngelScript scripting language](https://www.angelcode.com/angelscript/sdk/docs/manual/index.html).

## How to build

First clone the repository (or fork it and clone it) and fetch the submodules.

Next simply run the `build.bat` (Windows) or `build.sh` (Linux) and then the built files should be located in the `dist` directory.

## How to use

Copy the `angelscript-module.dll` to your server `modules` folder and add it to your `server.cfg` modules array.

When you start the server you should see a message similiar to this:
```
Loaded AngelScript module. Version vX.X.X
```

You can now create an AngelScript resource by using the `as` type.

## How to contribute

If you want to contribute to this in any way, then first of all Thank You!

Just clone the repository, create a new branch to store your changes and create a pull request containing your created branch and changes.

If you are unsure with something and want to know how something works etc. you can contact me on [Discord](LeonMrBonnie#9840).
