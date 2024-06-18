# lled
A Lightweight Lua editor written with ImGUI that can modify itself at runtime.


LLED is a text editor and a Lua interpreter.

* LLED can open files, or create new files with the native file dialogue of the operating system.
* LLED can execute lines or the entire file.
* All processes share the same Lua runtime meaning different files can pass each other variables or functions.
* LLED also provides a terminal shell to the runtime, through the terminal.

```sh
sudo apt install libglfw3 libglfw3-dev
```

![Window Manager Example](/screenshots/wm.png)
