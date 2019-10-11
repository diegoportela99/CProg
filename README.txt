******************************************************************
   _____ _    _ ____   _____ _______    _______ _____ ____  _   _
  / ____| |  | |  _ \ / ____|__   __|/\|__   __|_   _/ __ \| \ | |
 | (___ | |  | | |_) | (___    | |  /  \  | |    | || |  | |  \| |
  \___ \| |  | |  _ < \___ \   | | / /\ \ | |    | || |  | | . ` |
  ____) | |__| | |_) |____) |  | |/ ____ \| |   _| || |__| | |\  |
 |_____/ \____/|____/|_____/   |_/_/    \_\_|  |_____\____/|_| \_|
  _____       _______       ____           _____ ______
 |  __ \   /\|__   __|/\   |  _ \   /\    / ____|  ____|
 | |  | | /  \  | |  /  \  | |_) | /  \  | (___ | |__
 | |  | |/ /\ \ | | / /\ \ |  _ < / /\ \  \___ \|  __|
 | |__| / ____ \| |/ ____ \| |_) / ____ \ ____) | |____
 |_____/_/    \_\_/_/    \_\____/_/    \_\_____/|______|

******************************************************************
Developed by: Michael Lardner, Owen Dowley, Ray Robinson,
Jacob Elali, Diego Portela for UTS Fundamentals of C
Programming.
******************************************************************

Welcome to the Substation Database program. This program has been
developed to showcase a number of C programming techniques which
we have learnt throughout the semester.

This program can import suitable .csv files which are in the
appropriate format for the clients need.

******************************************************************
INSTALLATION
******************************************************************

To install the program, simply use the provided make file. This
can be done by typing in "make" in the terminal, while in the
folder containing the provided files.

To uninstall, simply type "make clean" to remove all of the files
created by using "make".

Your terminal may prompt you to install the make package if it is
not currently present on your computer.

******************************************************************
HOW TO USE
******************************************************************

After installing the program, simply type "./SSD.out" to run the
program.

After running the program, you will be brought to the main
menu, from here you can select to import a csv file or load
an encrypted database file (which must be encrypted by this
program). After loading a desired file, select the file submenu
to access more features, including search, export and save
as an ecrypted database file. To exit the program, simply
return to the main menu, from which there is an exit program
option.

There is also a few alternative runtime modes available:

Typing "./SSD.out -c" will bring you to the compression debug
menu. Here you can import a csv and then test the compression
functionality (which is incomplete in this version of the
program).

Typing "./SSD.out -colour" will bring you to the colour menu.
From here you can select a colour to change your text to
when running the program. This is reset to the default
text colour when exiting the program.

******************************************************************
DEBUG
******************************************************************

There is some debug functionality which can be activated by
entering debug.h and uncommenting the "#define DEBUG" line.
This simply enables a few lines of code which print information
when running certain functions.

******************************************************************
README written by Michael Lardner
2019
