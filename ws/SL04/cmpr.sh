# Alsami Technologies, 2020

# Sami Alameddine

# Configure this script to your needs

echo Alsami Web Server
echo What do you want the binary to be named?
read biname
gcc -o $biname foo.c -o2
./$biname
