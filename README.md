# GAuthSaviour
![GAuthSaviour Screenshot](http://i.imgur.com/Cttb4gn.png)

GAuthSaviour is a simple, cross-platform tool for retrieving the secret keys of your Google Authenticator installation. It can retrieve the GAuth database via adb, as well as extract secrets from existing databases and supports exporting to a WinAuth-compatible file.

#### Usage

```
Usage:  gasaviour.exe [-d] [-f <file>] [-w <file>]

        -d              Dump database via adb (must have adb in directory of PATH)
        -f <file>       Read database from <file>
        -w <file>       Convert database to Winauth <file>
        
        Examples:
        
        gasaviour.exe -f temp                   Read database file "temp"
        gasaviour.exe -f temp -w auth.xml       Read database file "temp" and convert to winauth "auth.xml"
        gasaviour.exe -d -w auth.xml            Read database via adb and convert to winauth "auth.xml"
        gasaviour.exe -w auth.xml               Read database file "databases" and convert to winauth "auth.xml"
```


<<<<<<< HEAD
#### Building
=======
If you want to do something more advanced with the software, see the parameters below.

## Parameters

```-d``` - Pull database via adb Currently disabled

```-w <file>``` - Convert database to WinAuth file

```-f <file>``` - Load database from filename

Default behaviour is to print all auths from the file `database` in the current dictionary.

## Building
>>>>>>> 2da5fc43b5c7216ceaf18b5470fb6ad4f520edde

On windows, building is best done with the Code::Blocks project file included in the repository. 

If using Cygwin or another Unix-based environment, adapt the gcc example found [here](https://www.sqlite.org/howtocompile.html#compiling_the_command_line_interface), eg:

```gcc main.c sqlite3.c -lpthread -ldl -o gauthsaviour```

#### Legal

Check LICENSE.md