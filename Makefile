default:
	g++ -o narsil -I ./leveldb/include/ narsil.cpp ./leveldb/libleveldb.a

clean:
	cd leveldb; make clean
