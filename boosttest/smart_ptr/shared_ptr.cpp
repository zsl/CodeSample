#include <memory>
#include <iostream>
//#include <boost/shared_ptr.hpp>
//#include <boost/enable_shared_from_this.hpp>
//#include <boost/make_shared.hpp>

using std::cout;
using std::endl;
//using namespace boost;
using namespace std;

class C {
public:
	C(){
		cout << "C.\n";
	}

	C(int){
		cout << "C_int.\n";
	}

	~C(){
		cout << "~C.\n";
	}
	//shared_ptr<C> data;
};

class Derive : public C{
public:
	void info(){
		cout << "Derive.\n";
	}
};

int main(){
	//shared_ptr<C> pc = make_shared<C>(); // 使用make_shared，可以避免使用new
	//
	//cout << pc.use_count();

	//shared_ptr<C> pc1 = pc->getThis();

	//cout << pc.use_count() << endl;
	//cout<< pc1.use_count() << endl;
	//pc1.reset();
	//cout << pc.use_count() << endl;

	//pc.reset();  // 释放

	//unique_ptr<C[]> up(new C[10]); 
	
	shared_ptr<C> sc(new Derive);
	//sc->data = sc;

	//shared_ptr<Derive> sd = static_pointer_cast<Derive>(sc);
	//sd->info();

	weak_ptr<C> wp(sc);
	if (!wp.expired())
		cout << "live.\n";
	sc.reset();
	if (!wp.expired())
		cout << "live.\n";
	else
		cout << "dead.\n";
}