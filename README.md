# RenpyPather
A simple C++ 17 project that reads all .RPY files from a renpy /game folder that output a grapfile than can be rendered with Graphviz

This is my first C++ project


I'm currently running the program in the compiler as the folder location is hardcoded

You have to change the string named path to the folder location something like "C:\\Renpy game\\game"
after running it should have made a file named output.gv
this file can be read by Graphviz (for me dot works well) 
A basic command is (Dot.exe -Tjpg %path%\output.gv -o %imagename%.jpg)

It works best for mostly linear games but your milage may wary. 

It does not read archives or encryptyed files, just plain .RPY files

An earlier version of C++ can be used if you use the experimental version of the filesystem library
