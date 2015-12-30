#ifndef KVPAIR_H
#define KVPAIR_H

// Container for a key-value pair
template <typename Key, typename E>
class KVpair {
private:
	Key k;
	E e;

public:
	KVpair() { }
	KVpair(Key kval, E eval) { k = kval; e = eval; }
	KVpair(const KVpair& o) { k = o.k; e = o.e; }
	
	void operator = (const KVpair& o) { k = o.k; e = o.e; }
	
	Key key() { return k; }
	void setKey(Key ink) { k = ink; }
	E value() { return e; }
	
};

#endif // KVPAIR_H