# Kassakoppeling
Kassakoppeling-system C/C++

<h2>Prerequisites</h2>

<h3>Libraries</h3>
<li><a href="https://curl.haxx.se/download.html">libcURL</a> 7.47.0 or higher</li>
<li><a href="https://packages.debian.org/search?keywords=libjsoncpp-dev">libjsoncpp-dev 1.7.2</a> or higher</li>

<h3>OS</h3> 
<li>Linux (32/64)bit</li>
<li>The user needs to be a member of the group "dialout" to get access to the serial ports</li>

<h2>Compiling</h2>
You can either compile this program with GCC-4.9 & G++-4.9 or with MAKE.
<br>Do not compile with windows tools.

<h3>Gcc/G++ Instructions</h3>
<b>Compile with the following flags</b>
<br>-std=c++11, -pthread, -lcurl

<h3>MAKE Instructions</h3>
make -f 'makefile_name'
