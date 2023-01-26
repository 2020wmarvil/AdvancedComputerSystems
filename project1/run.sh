mkdir build
cd build
cmake ..
make

echo ---- L5 Compression ----
echo 1 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 5 1
echo 2 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 5 2
echo 4 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 5 4
echo 8 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 5 8
echo 16 Threads:
time ./ACS_Proj1 ../test_files/biggestfile.txt 5 16

echo ---- L1 Compression ----
echo 1 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 1 1
echo 2 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 1 2
echo 4 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 1 4
echo 8 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 1 8
echo 16 Threads:
time ./ACS_Proj1 ../test_files/biggestfile.txt 1 16

echo ---- L2 Compression ----
echo 1 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 2 1
echo 2 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 2 2
echo 4 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 2 4
echo 8 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 2 8
echo 16 Threads:
time ./ACS_Proj1 ../test_files/biggestfile.txt 2 16

echo ---- L3 Compression ----
echo 1 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 3 1
echo 2 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 3 2
echo 4 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 3 4
echo 8 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 3 8
echo 16 Threads:
time ./ACS_Proj1 ../test_files/biggestfile.txt 3 16

echo ---- L4 Compression ----
echo 1 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 4 1
echo 2 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 4 2
echo 4 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 4 4
echo 8 Thread: 
time ./ACS_Proj1 ../test_files/biggestfile.txt 4 8
echo 16 Threads:
time ./ACS_Proj1 ../test_files/biggestfile.txt 4 16
