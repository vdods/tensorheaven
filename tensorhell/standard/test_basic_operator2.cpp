// ///////////////////////////////////////////////////////////////////////////
// test_basic_operator2.cpp by Victor Dods, created 2013/10/04
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_basic_operator.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace Basic {
namespace Operator {

void AddTests2 (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory("Operator");
    add_particular_tests_for_scalar<Sint32>(dir);
    add_particular_tests_for_scalar<Uint32>(dir);

    // TODO: refactor all this test code so that Op<...> can be tested in a way analogous to Vec.
}

} // end of namespace Operator
} // end of namespace Basic
} // end of namespace Test
