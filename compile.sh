LIBNAME=WaveReader

echo Compiling lib$LIBNAME.so...
g++ -fPIC -shared *.cc -o lib$LIBNAME.so

echo Copying headers and lib...
cp lib$LIBNAME.so $HOME/lib/.
cp *.hh $HOME/include/.
