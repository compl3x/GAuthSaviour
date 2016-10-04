# GAuthSaviour

GAuthSaviour is a simple, cross-platform tool for retrieving the secret keys of your Google Authenticator installation. It can retrieve the GAuth database via adb, as well as extract secrets from existing databases and supports exporting to a WinAuth-compatible file.

## How to use
If you have root over adb then simply execute the program with the `-d` argument (**this is what you want to do if you can boot only into recovery**). 

##### Windows:

1. Download release zip
2. *OPTIONAL: If you want to use the -d flag, you must move adb and associated DLL files into the same folder.*
2. Unzip and `cd` into the folder in cmd
3. Enter your required parameters with the prefix `gauthsaviour`,e.g:

    ```gauthsaviour -w winauth-file.xml```

If you want to do something more advanced with the software, see the parameters below.

## Parameters

```-d``` - Pull database via adb Currently disabled

```-w <file>``` - Convert database to WinAuth file

```-f <file>``` - Load database from filename

Default behaviour is to print all auths from the file `database` in the current dictionary.

## Building

On windows, building is best done with the Code::Blocks project file included in the repository. 

If using Cygwin or another Unix-based environment, adapt the gcc example found [here](https://www.sqlite.org/howtocompile.html#compiling_the_command_line_interface), eg:

```gcc main.c sqlite3.c -lpthread -ldl -o gauthsaviour```

## Contributions
If there's an issue with any of the code, feel free to make a pull request.
