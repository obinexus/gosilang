# Run with library path
LD_LIBRARY_PATH=../lib ./test_link

# Or install the library system-wide
sudo cp ../lib/libgosiuml.so /usr/local/lib/
sudo ldconfig
./test_link  # Should work now
