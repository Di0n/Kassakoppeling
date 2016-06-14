# Kassakoppeling
Kassakoppeling-systeem C/C++

<h1>Requirements</h1>

<h2>Libraries</h2>
<li>libcURL 7.47.0 or higher</li>
<li>libjsoncpp-dev 1.7.2 or higher</li>

<h2>OS</h2> 
<li>Linux (32/64)bit</li>
<li>The user needs to be a member of the group "dialout" to get access to the serial ports</li>

<h1>Compiling</h1>
You can either compile this program with GCC-4.9 & G++-4.9 or with MAKE.
<br>Do not compile with windows tools.

<h2>Gcc/G++ Instructions</h2>
<b>Compile with the following flags</b>
<li>-std=c++11, -pthread, -lcurl</li>

<h2>MAKE Instructions</h2>
make -f 'makefile_name'
