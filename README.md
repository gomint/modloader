# modloader
A proof-of-concept ModLoader for the Minecraft Bedrock Dedicated Server (BDS).

This modloader works on Microsoft Windows as opposed to existing modloaders which primarily focus on Linux systems. However, Linux support could be added anytime by providing replacements for all code oprtions currently found within the "Windows" subdirectories in the respective targets' sources.

## ❇ How it Works

Existing ModLoaders use the LD_PRELOAD option available on UNIX-like systems to inject a DLL into the memory space of the Bedrock Dedicated Server executable before it begins to run any code of its own. Since this option is unavailable on Windows, we instead provide an executable which launches the Bedrock Dedicated Server executable as a new process but suspends it on creation, i.e. before any actual code is executed. It then creates a thread in that newly created process which loads the ModLoader DLL while the rest of the process is still frozen. Only once the DLL loading thread has finished will the process actually resume. This ensures that the ModLoader DLL is injected into the executable's memory space before it runs any code of its own.

The ModLoader itself contains a mapping of reverse-engineered function pointers to several interesting functions from the Bedrock Dedicated Server executable. These function pointers are retrieved by extracting address offsets from .PDB files shipping with the Bedrock Dedicated Server fully automatically (using a target named SymExtract). Using these functions and a runtime function hooking library allows the ModLoader to integrate with the Bedrock Dedicated Server executable and expose an interface to mods.

There are several limitations to this approach, the most stringent one being that any specific ModLoader DLL version is only compatible with exactly one specific Bedrock Dedicated Server executable. Whenever that executable changes the ModLoader DLL needs to be rebuilt since the address offsets to the required functions might have changed. Furthermore, since ModLoader is accessing internal functions in the Bedrock Dedicated Server their signatures could change at any time, requiring modifications in how ModLoader invokes them.

## 📦 How to Compile

On Windows, a recent version of the Microsoft Visual C++ compiler suite is required. Both a recent version of the compiler as well as a recent version of the Windows Kit (8.1 or 10) is required. The reason for this is that the SymExtract module which extract the required address offsets depends on the dbghelp library that ships with said Windows Kit versions. Lastly, the .pdb file that ships with the desired Bedrock Dedicated Server executable is required. When invoking CMake to build the ModLoader DLL, make sure to specify a `MODLOADER_BEDROCK_PDB` CMake variable that specifies the full path to the .pdb file.

Example:
```
~ > cmake -DMODLOADER_BEDROCK_PDB=<Path to PDB> --build ~/build --target ModLoader
```

## ▶️ How to run

In order to run ModLoader on a Bedrock Dedicated Server use the Preloader executable like this
```
~ > preloader bedrock_server.exe ModLoader.dll
```

## 👥 Contributions

If you wish to contribute to this project, feel free to submit a pull request. If you have any questions regarding this project, please contact [Ciel](https://github.com/cieldeville)
