#define NDEBUG
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
//#include <cmath>
#include <gsh/InOut.hpp>
#include <gsh/TypeDef.hpp>
#include <gsh/Algorithm.hpp>
#include <gsh/Vec.hpp>
#include <gsh/Arr.hpp>
#include <gsh/Random.hpp>
#include <gsh/Timer.hpp>
#include <gsh/Numeric.hpp>
#include <gsh/Macro.hpp>
#include <gsh/Util.hpp>
#include <gsh/DisjointSet.hpp>
#include <iostream>
#include <cassert>
#include <sstream>

int main() {
    try {
        [[maybe_unused]] gsh::Rand32 engine;
        {
            using namespace std;
            using namespace gsh;
            using namespace itype;
            using namespace ftype;
            /*
            ostringstream oss;
            u64 cor = 0;
            REP(i, 100000000) {
                u32 tmp = engine() / 150000;
                cor += tmp;
                oss << tmp << ' ';
            }
            oss << "              ";
            std::cout << cor << std::endl;
            string s = oss.str();
            StringReader r(s.data(), s.size());
            ClockTimer t;
            u64 res = 0;
            REP(i, 100000000) {
                res += Parser<i16>{}(r);
            }
            t.print();
            cout << res << endl;
            */
            Rand32 engine;
            //for (u32 i = 0; i != 1000000000; ++i) engine();
            engine.discard(1000000000);
            std::cout << engine() << std::endl;
        }
    } catch (gsh::Exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
