# TENSOR HEAVEN - Tenh (pronounced "tench", similar to hyperbolic tangent tanh)


## NOTES
- all library code is in tenh/
  * ideally there will be few/no cpp files
- only the root directory (the directory containing tenh/) should be included, in the same manner as Eigen.
- all test code is in tensorhell/ -- there are currectly ad-hoc tests/prototype code (e.g. eigen_interop_prototype.cpp
  and contraction_prototype.cpp), but the "real" tests are in the standard/ subdirectory.  the CMakeLists.txt file
  in the tensorhell/ subdirectory will build all of these.  the executable name for the "real" tests is "test".


### Future goals
- positive and negative compile tests
- optimize evaluation algorithm (ordering operations to do the minimal amount of computation)
- vectorizing as many operations as possible (in cooperation with loop-unrolling?)
- template specializations for particular tensor contractions for speed (using mathematical special cases)
    * for example, an antisymmetric 3x3 2-tensor contracted with a 3-vector is just a cross product
    * a diagonal 2-tensor contracted with a vector is just component-wise scaling
- certain linear algebraic expressions simplify greatly when expressed in a particular basis,
  so having a formal mechanism to work with explicit bases may clarify and simplify some calculations


### Random TODOs
- implement all (?) reindexing as a lookup table for speed.
- see if it's possible to do strong range check guarantees for indices (`Index_t`) -- this would probably
  require a "for each" construct which would best be accomplished with lambda expressions
- move "private" utility functions/metafunctions into the "Tenh::NonPublic" namespace so that they're not in the
  "Tenh" namespace, for at least a little bit of non-public interface hiding.
- make a nonpublic directory and put all NonPublic code in there (and actually put appropriate stuff in NonPublic namespace)
- make a wrapper for creating analogous `Eigen::Matrix` types which handles the retarded issue with
  `Eigen::Matrix<Scalar,N,1,RowMajor>` not working because it's a column vector (even though the memory would
  be layed out identically).
- decide on template metaprogramming naming convention.  something along the lines of
    * `Blah_t<...>` for types that actually implement stuff and have significant runtime code (is there
      something better than _t ?)
    * `Blah_c<...>` for the conceptual layer
    * `Blah_i<...>` for the templatized interface layer (this is somewhere between _c and _i)
    * `Blah_m<...>` for meta-programmingy things like type lists, related metafunctions, or are
      "terminal" metaprogram types that aren't metafunctions (i.e. they are metaprogram values)
    * `Blah_f<...>` for meta-functions which "return" a type T or a value V
- maybe rename `Tuple_t::body()` to `Tuple_t::body_tuple()`, since the body type is `BodyTuple`
- maybe change array (and array-like) indexing schemes to use signed integers, where negative
  values index from the end of the array (although this would come at a runtime cost).  this
  should probably not include vector/tensor indexing.  could do this with some type similar to
  ComponentIndex_t, like SignedIndex_t -- then provide overloads which accept SignedIndex_t,
  and the original ComponentIndex_t accessors wouldn't be affected at all.
- Add _ to the end of all template parameters
- Make references to Parameter within template classes reference Parameter_ where possible.
- The non-conceptual `IsBlah_f` metafunctions should really be called `InheritsBlah_f`, since these
  are overloaded to reflect the class inheritance structure.  This is to be contrasted with the
  conceptual layer, where `IsBlah_f<T>::V` is true if and only if T actually is exactly a Blah type.
  Probably better yet, IsBlah_f should be named IsExactlyBlah_f, and IsBlah_f would mean "inherits".
- make a bunch of the typedefs in `ImplementationOf_t` classes private, and decide which ones
  to offer publicly, or get rid of.
- fix tensor products of diagonal 2-tensors on factors having non-equal dimension
- do slice addressing by providing an "operator () (ComponentIndex_t<DIM> const &)" for AbstractIndex_c,
  which returns some distinct type that signifies a slice operation.  correspondingly, "operator |" which
  creates abstract multi-indices will have to be extended to account for this.  SliceIndex_t ?
- XXX_f types should have a private constructor to prevent actually instantiating them
  (there are frequent programmer errors involving forgetting to "dereference" a metafunction
  via ::T or ::V).
- add "pullback by linear operator" method to polynomial classes, which essentially plug in
  a given 2-tensor (linear map) into each tensor factor in each summand of a polynomial, so that
  the polynomial has been pulled back onto the domain of the given linear map (Gabe uses operator %
  for this purpose).
- consider disallowing non-square diagonal-2-tensors -- this will ease the optimization of
  direct sum of diagonal 2 tensors (because it would then be a diagonal 2 tensor itself)
- use "enable_if" technique ( http://en.cppreference.com/w/cpp/types/enable_if ) to not generate
  certain functions (e.g. a template type affecting which methods should be available on a
  class, such as all the different types of constructors for ImplementationOf_t).
- design and implement mathematical type string methods and printing for debugging purposes
- design and implement some form of type debugging method (to debug indexed expression
  compile errors)
- make a copy_from(Array_i const &) that does memcpy, memmove, or iterative operator[] access
  depending on what Array_i type it is.
- change metafunction predicates to use _e suffix, to regularize them with the ones produced
  by MAKE_X_ARY_VALUE_EVALUATOR.  maybe just make all predicates be generated using those
  macros from "primitive" metafunctions.
- make a "2-tensor out of LinearEmbedding_c" procedural 2-tensor (is this actually useful?
  yes, for printing/debugging)
- make a "LinearEmbedding_c out of 2-tensor" LinearEmbedding_c, which could be useful for
  more-easily constructing LinearEmbedding_c instances, since they (can) use lookup tables
  anyway.
- make an abstracted summation function/facility so that different methods for summation can
  be implemented -- e.g. fast-naive, or sorted-for-accuracy, etc.
- maybe rename XXXXTyple to XXXXs for brevity.  this should easily be distinguished from the
  XXXXs usage in "typename... XXXXs" because of the dereferencing "XXXXs...", though this
  makes grepping for stuff harder.
- move stuff into "primitive" directory, e.g. preallocatedarray.hpp, memoryarray.hpp, 
  componentgenerator.hpp, componentindex.hpp, dimindex.hpp, list.hpp, etc.
- make abstract multiindices use TensorProduct_c (or TensorProductOfAbstractIndices_c) instead
  of Typle_t.  Similarly, when direct sum indices are implemented, use DirectSum_c.
- make tensor-product-able things implement a "HasDefinedTensorProduct_f" metafunction which
  returns true, so that operator * can be used with an std::enable_if.  Same with direct sum.
- write check to make sure that a LinearEmbedding_c actually is an embedding.
- add "invertible" property to LinearEmbedding_c, and an induced inverted embedding
- composition of LinearEmbedding_c objects.
- change enum string access functions to use switch statements on enum values so that the
  compiler can check for missing cases.
- consider changing ::T and ::V to ::type and ::value to be consistent with the STL.
- make VERBOSE and TERSE strongly typed enums
- change enum string access functions to use switch statements on enum values so that the
  compiler can check for missing cases.


### Implemented features/items
- operator overloads for expression templates
- trace-type expression templates (where a tensor is contracted with itself, a case which is distinct from * style expression templates)
- prohibiting 3+ summed index expressions
- assignment expression templates
- check assignment for aliasing
- symmetric 2-tensors
- antisymmetric 2-tensors
- custom natural pairing (for non-isometric parameterizations)
- scalar multiplication/division in expression templates
- unary negation in expression templates
- printing expression templates
- down-casting tensors (extracting symmetries out of "broken apart" indexed tensor expressions);
  "bundling" multi-indices into a more-specific "total" index type
- up-casting tensors (breaking symmetries indexed tensor expressions);
  "splitting" a tensor "total" index into a less-specific multi-index
- diagonal 2-tensors
- SVD on 2-tensors, using Eigen
- eigen-decomposition of symmetric 2-tensors, using Eigen
- Eigen interop for using Tensor2_t as Eigen::Map (no memory copy necessary)
- functions for converting Tensor2Antisymmetric_t and Tensor2Symmetric_t to Eigen::Matrix types
- eval method for expression templates (to avoid aliasing problem when assigning)
- no_alias method for avoiding the run-time check against aliased expression template assignments
- Euclidean embeddings for vector and tensor types for when an inner-product-compatible representation is needed.
- Basis identifier for each vector space, giving an additional feature to distinguish vector types with,
  for added correctness, which is particularly useful for the Euclidean embedding stuff.
- allow 0-dimensional vector spaces (this was done via -Wno-tautological-compare)
- relaxed the C++-driven type checking in preference to static-assert-driven type checking, made all
  indices used in indexed expressions "weakly typed", except for bundled indices (e.g. the index p in
  t(i|j).bundle(i|j,p) -- it is necessary to know the type of p for this to be a well-defined bundle).  all
  other index types can be inferred from their use in the expression.
- strongly-typed vector spaces, and only natural pairings thereof allowed.
- exterior powers -- i.e. wedges.
- symmetric powers -- e.g. "vee"s.
- diagonal 2-tensor products of based vector spaces
- "mapped" tensors/vectors (using preallocated memory) and "procedural" tensors/vectors (using static
  component-generating functions).
- "basic mode", allowing standard linear algebra notation, for easier adoption by non-experts
- choice of bases for tensors-with-symmetry rendering significant type-system simplification
  and computational speedups.
- a canonical "zero" procedural-array-based ImplementationOf_t for all vector types (including tensor, vee, etc)
- basis vector procedural-array-based ImplementationOf_t for all vector types (including tensor, vee, etc)
- multilinear form evaluation via "operator ()" (e.g. g(x,y) as evaluation of a bilinear form).


### Different multi-indexing schemes
- "split"                 : if t is `Tensor2<Tensor2Sym<V>,Tensor2Sym<V>>`, then `t(P,Q).split(P,i,j),split(Q,k,l)`
                            is converse to bundle, implementation would be similar.
- "multi-index"           : if s is `Tensor2<Tensor5,V>`, then `s(i,j,k,l,m,T)` would be `s(multiindex(i,j,k,l,m),T`)
                            implementation may be difficult if there are nested multi-indices
- "streaming multi-index" : `s/i/j/k/l/m/T` or `s^i^j^k^l^m^T` -- need intermediate "streaming" type
                            left to right, breaking types until there is a match, implementation is probably
                            recursive and relatively easy.  "breaking tensor type boxes apart recursively"
- "barred multi-index"    : `s(i|j|k|l|m|T)`
- "direct sum indices"    : if t is `DirectSum<V,V>`, and s and u are `V` then `t(i+j) = s(i) + u(j)` could work,
                            also if t is `Tensor2<DirectSum<V,V>,DirectSum<V,V>>` and s and u are `Tensor2<V,V>`
                            then `t((a+b)|(c+d)) = s(a|c) + u(b|d)` could fill t as a block diagonal tensor.

### Notes for conceptual layer and refactor
- In order to incorporate the order-of-operations and diagonal 2-tensor contraction optimizations,
  the operator overloads which generate expression templates should do the expression template AST
  optimization (e.g. if the expression is `a(i)*d(i|j)*b(j)` and d is diagonal, then this should turn
  into an expression template that sums `a(k)*d(k|k)*b(k)` with the single index `k`).  In the case where
  a change of order of operations would produce an optimization, the expression template AST tree
  should be modified here -- this affects the return type of each operator overload.
- Should the index-notated expression templates be done at the conceptual level?  The optimization
  and type checking could be done here in isolation from the actual computation -- effectively
  the conceptual level will produce an "optimized AST" expression template.  While the indexed
  expressions should all be tensorial in nature, some of the optimizations depend on the chosen
  basis (e.g. the diagonal based 2-tensor product one), so this should be done at the
  based-vector-space and based-tensor-product level.
- Indexing scheme for wedges and symmetric products consists of sorting the indices of a multi-index.
  For Young tableaux, this would generalize to imposing sorting constraints on various subsets of
  indices (based on the rows (alternating) and columns (symmetric)).  Negative signs are accumulated
  for swaps in rows, and zero is returned for repetitions in the rows.
- Gray coding scheme would be desired for generating non-symmetric multi-indices for
  when iterating through components for e.g. the bundle operation.
- `BasedTensorProductOfVectorSpaces` should be `EmbeddableInTensorProductOf[Based?]VectorSpaces`,
  since basis elements may be non-simple tensors, which would require full 2-tensors to perform
  the embedding.  It's possible that certain BasedTensorProductOfVectorSpaces could embed more
  directly into `TensorProductOfBasedVectorSpaces`, e.g. each basis element is a scalar multiple
  of a source element, or at most the sum of two such elements, and doesn't require the full
  generality of a general linear embedding.
- An advantage of the "embeddable in tensor product" idea is that "smaller" splits can be done --
  e.g. splitting "S^4 V" into "S^2 V \otimes S^2 V" or into "V \otimes S^3 V", and this might be a
  crucial feature, as it really enriches the type system without adding too much library-template-driven
  code (the embeddings would be neatly packaged bits of code that could be user-specified also).
- The "embeddable in" concept(s) probably need to be generalized to be "embeddable into X", e.g.
  `Diagonal2TensorProductOfBasedVectorSpaces_c<A,A>` embeds into `SymmetricPower<2,A>`.
- Possible idea for efficient contraction of tensors having different symmetries -- express their
  contraction as a linear map (a 2-tensor), then contract this with the outer product of the operands.
  This would really depend on the compiler-specific optimizations that are found in assembler code
  investigations.
- TODO: refactor all the AbstractIndex/DimIndex/Factor juggling to use some standard form, like
  (AbstractIndex,Factor) pairs, from which the AbstractIndex, Factor, or DimIndex values can be
  extracted.  This will clean up tons of shitty metaprogram code.
  Or maybe use `TypedIndex_t` after all (it would inherit `DimIndex_t` and contain the factor info)
- Conceptual interface checkers: something like

        template <typename T>
        struct Check_TensorProductOfVectorSpaces_c
        {
            typedef typename T::FactorTypeList FactorTypeList;
            static Uint32 const ORDER = T::ORDER;
            // etc
            static bool const V = true; // used for instantiating/using this check in an static assert
            enum
            {
                STATIC_ASSERT_IN_ENUM(Check_VectorSpace_c<T>::V, MUST_BE_VECTOR_SPACE)
                STATIC_ASSERT_IN_ENUM(EachTypeIsVectorSpace_c<FactorTypeList>, MUST_BE_TYPELIST_OF_VECTOR_SPACES)
            };
        };

  Hopefully this would cause an error if any of the members/typedefs were missing (TODO: Write
  some compile-time positive and negative tests to this effect).  The static asserts would certainly
  work.
- Implement positive compile-time tests which verify that various diagrams commute
  (conceptual hierarchy diagram).  This will require implementing positive/negative compile
  tests in cmake or some such other framework.
- Make a full pass to remove the unnecessary members/member types from concepts
- `ComponentIndex_t` and `DimIndex_t` should possibly be made conceptual types, they interact with
  `AbstractIndex_c`, and right now `DimIndex_t` still overrides `IsAbstractIndex_f` (this is from Ted's
  commit ecebc8c4bd3a54b4f83f2ba74871990828f1114d on the cleanup-conceptual-members branch).
- Add `ElementOf_c<Concept,Id>` (or `ElementOf_c<ConcreteCategoryObject,Id>` ?) which is essentially
  a formal symbol for a particular element of an object in a category.  This will be used e.g.
  in defining an inner product space (which is a vector space and a particular tensor).
- TODO: think about how to use runtime-specified inner products (lazy initialization?), since
  probably the inner product implementation type has to be constructed using the default
  constructor.
- TODO: allow tensor products of non-procedural tensor/vector implementations -- this would
  amount to providing constructors which would take the tensor product operands as parameters,
  but could use the same code.
- TODO: lazy-initialized procedural array -- initialized at runtime once -- something that can't
  easily be done at compile time, e.g. computing the inverse of a given non-diagonal 2-tensor.
  could make LazilyInitializedArray_t or some such, which inherits MemoryArray_i.
- MSVC doesn't like the following code, when e is a 3-tensor implementation and i is an
  AbstractIndex_c<'i'>:  FORMAT_VALUE(e(i).eval()) (this probably has something to do with
  the derived type passed into ExpressionTemplate_IndexedObject_t when the indexed expression
  is created.  Note that FORMAT_VALUE(e(i*j*k).eval()) works normally.
- TODO: verify that bundle is a left-inverse to split, and then make it so that bundle actually
  left-cancels split.  verifying that bundle actually left-inverts split will require some
  special "no optimize" syntax though, or a verification at the reindexing level.
- IDEA for better/simpler functorially induced types, e.g. inner product spaces and tensor products:
  Have a "ConceptClassOf_f" metafunction which returns things like Concept_BasedVectorSpaces,
  Concept_InnerProductSpace, etc.  Add a metafunction "CommonConceptClassOf_f" which computes the
  "greatest common ancestor" of all the concepts in a typelist.  Then the category of a tensor
  product can be computed -- is it a tensor product of based vector spaces?  is it a tensor
  product of inner product spaces?  is it a formal tensor product?  etc.
  QUESTION: is there just a single concept class for each thing, or is it a set of concept
  classes that each thing is in?  e.g. a tensor product of inner product spaces is all of
  the following: a formal tensor product, a vector space, an inner product space, and a
  tensor product of vector spaces.
- copy constructor for PreallocatedArray_t and subclasses?  maybe one for MemberArray_t and subclasses.


### Features necessary for "version 1.0"
- x implementation of diagonal 2-tensors (of based vector spaces)
- implementation of the optimization for contraction with diagonal 2-tensors (necessary so
    that we can implement inner products and other similar bilinear forms efficiently).
    QUESTION: Is there some reasonable generalization of diagonal 2-tensors to higher order
    tensors?  The answer to this question would likely be informed by results in higher
    order SVDs.
    NOTE: this generalizes to an optimization for contraction with any tensor-with-symmetry;
    contracting "upstairs" via split is (with the appropriate choice of bases) equivalent
    to contracting "downstairs" via a projection (which can often be implemented with an
    operation that is analogously sparse).
- x implement analog of Eigen's "map" (providing a vector/tensor/ext-pow/sym-pow/everything
    implementations which uses some separate, specified block of memory, which could e.g.
    coincide with an Eigen::Matrix).  Maybe do this via abstracting some aspects of Array_t
    into Array_i, making Array_t inherit Array_i, then making PreallocatedArray_t also inherit
    Array_i.  Then the "mapped" versions of each tensor implementation would just flip a
    switch to use Array_t or PreallocatedArray_t (hopefully it can be this simple).
- x refactoring the Eigen interop code to be strongly typed, ideally using the "map" functionality
- do more assembler code inspection for each of the compilers we use (GCC, Clang, MSVC) to
  make sure that it's generating code that's as efficient as we expect it to be (this is
  really important).  record some analysis of each, noting the compiler versions for each
- x refactoring the unit tests to account for the strongly-typed refactor in general
- writing unit tests for the new code (general symmetric and antisymmetric N-tensors)
- x Gabe's "Linear Operator" ("Hom"?) paradigm -- to do non-indexed-expression vector/matrix
    math like Eigen does (so this library could be used by "laymen" without using abstract
    index notation)
- regularize/clean up the template code naming scheme (suffixes _t, _c, _f, _m, _i, etc)
- generating documentation (Doxygen comments for the code, Doxygen-generated high-level
  documentation, and LaTeX writeups of the abstract math), and probably most critically,
  tutorials, examples, and some intro material to get people into tensor math who only
  know about matrices/vectors.
- x translating LVD code in tenh/meta/lvd.hpp (Victor's legacy metaprogramming lib) into
    "native Tensor Heaven" code (important for legal purposes)
- Talking to Tom about officially licensing the LVD unit test code (important for legal purposes)


### Notes for Gabe's "Linear Operator" ("Hom"?) paradigm
- The general idea is to do linear algebra in the "Hom" formalism, without implicitly
  identifying Hom(V,W) with the tensor product W \otimes V^*.
- This is accomplished by defining operator* to be composition in the ordinary function
  sense (composition of vector-producing thing into linear operator), and defining
  `operator%` to be tensor product.
- In particular, because tensor product is fully associative -- i.e.

        (U \otimes V) \otimes W     is canonically isomorphic to     U \otimes (V \otimes W),

  non-parenthesized tensor expressions are well-defined; the above is written as

        U \otimes V \otimes W.

  This allows the well-definition of operator%, which will render the fully unparenthesized
  tensor product type.
- Additionally, `operator%` will act on linear operators in the functorial sense.  E.g. if

        if A \in Hom(V,W) and B \in Hom(X,Y),
        then A%B \in Hom(V \otimes W, W \otimes Y),

  as in the well-understood category theory of linear algebra.
- Similarly, symmetric and exterior powers of vectors and linear operators can be taken,
  and can be implemented in a form such as the following

    if v \in V, then SymmetricPower<4>(v) \in SymmetricPower<4>(V).

  Same for ExteriorPower.  For completeness, TensorPower can also be implemented, and then
  `operator%` can be a frontend for TensorPower.  Each of these is a functor on the category
  of [based] vector spaces.
- The canonical isomorphism between Hom(V,W) and W \otimes V^* will be available for when
  fancier tensor operations are desired.  For example, constructing some tensor valued thing,
  such as of type V \otimes V^* \otimes V^* \otimes V^*, and then using it as an object
  of any of the following types (which are all mutually distinct).

        Hom(V \otimes V \otimes V, V),
        Hom(V \otimes V^*, V \otimes V),
        Hom(V \otimes V^* \otimes V^*, V).
- A certainly useful (and possibly necessary) feature is to provide an action of the
  permutation group on tensorial things, so e.g. the transpose (adjoint) of a linear
  operator can be taken.


### Notes for generalized embed/coembed (generalizations of split/bundle)
- The "embed" and "coembed" operations are defined via the following properties:
    * Accessing components of an "embedded" vector/tensor should be a sparse operation
      involving no fractions (e.g. direct component access for vees, or plus-or-minus 1
      times a direct component access for wedges).  This no-fraction constraint is
      so that integral scalar representations can be used.
    * "embed" and "coembed" should be mutually adjoint with respect to the relevant
      natural pairings.  That is,
        A(i).embed(i,UpstairsSpace(),j)*B(j) - A(i)*B(j).coembed(j,DownstairsSpace(),i)
      should give the zero scalar.
- There should be abbreviated calls so that the above expression is
    A.embed(UpstairsSpace(),j)*B(j) - A(i)*B.coembed(DownStairsSpace(),i)
- The point of this theory is that doing a natural pairing "upstairs" doesn't take
  advantage of the symmetries available "downstairs", so it's ostensibly more efficient
  to coembed and do the natural pairing "downstairs".
    * for example, if D is a diagonal 2-tensor of type V^* \otimes V^*, and x and y are
      vectors of type V, then
        D(i).embed(i,V^* \otimes V^*,j).split(j,a|b)*x(a)*y(b)
      is the expression for use of D as a bilinear form, but this doesn't use its
      symmetry.  This would be more optimally expressed as
        D(i)*(x(a)*y(b)).bundle(a|b,V \otimes V,j).coembed(j,Diag2TensorSpace,i),
      where the projection would essentially compute the vector
        (x[0]*y[0], x[1]*y[1], ..., x[n-1]*y[n-1]),
      which would then be naturally paired with the diagonal (as a vector).
    * another example is similar to the previous, except where D is a scalar 2-tensor,
      and the projection of x(a)*y(b) would produce the scalar
        x[0]*y[0] + ... + x[n-1]*y[n-1],
      which is the standard dot product.  the natural pairing with the scalar 2-tensor
      is just a scalar multiplication of this value.  this example would actually
      occur for any standard Euclidean space, and in principle could boil down to
      the lean-and-mean computation done in standard linear algebraic libraries.
- The first property essentially amounts to a particular set of basis choices for
  various spaces.  It is possible that some spaces may not embed nicely (e.g. an
  arbitrary subspace of a given based vector space), and those cases should probably
  just use a full embedding 2-tensor (which is the most general way to do it), so
    B(j)*embedding_2_tensor(j|i)*A(i) - A(i)*coembedding_2_tensor(i|j)*B(j)
  should give the zero scalar.
- If the "embed" and "coembed" expression templates were formulated as full 2-tensors
  (i.e. matrices), then their mutual adjointness manifests as their matrices being
  transposes of one another.
- The reindexing maps should be done in the conceptual layer, since they are independent
  of choice of scalar representation (e.g. float).
- Ideally, if B was a bilinear form on V with symmetry and x and y were V vectors,
  then the expression
    x(i)*B(i|j)*y(j)
  would be replaced (via some metaprogramming optimization) with
    B(p)*(x(i)*y(j)).bundle(i|j,V \otimes V,q).coembed(q,SpaceWithSymmetry,p),
  thereby providing optimization without cluttering up the notation/usage from the
  perspective of the user.
- What is required to implement embed/coembed is the following:
    * embed requires:
        ~ bool component_is_procedural_zero (ComponentIndex_t<UPSTAIRS_DIM> const &)
        ~ Scalar_ scalar_factor_for_component (ComponentIndex_t<UPSTAIRS_DIM> const &)
        ~ ComponentIndex_t<DOWNSTAIRS_DIM> source_component_index (ComponentIndex_t<UPSTAIRS_DIM> const &)
    * embed will be used as follows
        diag.embed(dual(v*v),i) * h(i)       // h is of type v*v
        diag.embed(sym<2>(dual(v)),i) * s(i) // s is of type sym<2>(v)
        a(j*k).embed(k,dual(v*v),i) * h(i)   // h is of type v*v (same as before)
        other_diag(i*j)          // would be equivalent to other_diag.embed(v*w,p).split(p,i*j), where other_diag is of type diag(v*w).
    * accessing an embedded expression template will work as follows:
        x.embed(TargetSpace,i)[j] := component_is_procedural_zero(j) ? Scalar_(0) : scalar_factor_for_component(j)*x[source_component_index(j)];
    * probably this information should be provided in template specializations of
        template <typename Domain_, typename Codomain_> struct Embedding_c;
      this would allow embeddings to be requested, and if such an embedding doesn't exist,
      it results in a compile error, as desired.

    * coembed is somewhat more complicated because it may access multiple components.
      it requires:
        ~ Uint32 term_count_for_component (ComponentIndex_t<DOWNSTAIRS_DIM> const &)
        ~ Scalar_ scalar_factor_for_component (ComponentIndex_t<DOWNSTAIRS_DIM> const &i, Uint32 term)
            // assert(term < term_count_for_component(i))
        ~ ComponentIndex_t<UPSTAIRS_DIM> source_component_index (ComponentIndex_t<DOWNSTAIRS_DIM> const &i, Uint32 term)
            // assert(term < term_count_for_component(i))
    * coembed will be used as follows
        (x(j)*y(k)).coembed(j*k, sym<2>(v), i) * s(i) // s is of type dual(sym<2>(v))
        t.coembed(diag(v*w),i) * diag(i)              // diag is of type dual(diag(v*w))
    * accessing a coembedded expression template will work as follows:
        x.coembed(TargetSpace,i)[j] := sum(k = 0 to term_count_for_component(j)-1 of scalar_factor_for_component(j,k)*x[source_component_index(j,k)]);
    * probably this information should be provided in template specializations of
        template <typename Domain_, typename Codomain_> struct Projection_c;
      this would allow coembeddings to be requested, and if such a coembedding doesn't exist,
      it results in a compile error, as desired.

    * the mutual adjointness property should provide a testable property to ensure that the
      functions required by embed/coembed actually do what they claim.
      TODO: figure out the criteria

    * in principle, the functions required by coembed could be determined uniquely from the
      functions required by embed.


### Notes for refined indexed expressions
- In addition to creating abstract multi-indices via the | operator, e.g.
    x(i)*g(i*j)*y(j)
  make "direct sums" of induces, e.g. if x and y are vectors of type U and V respectively,
  and a is in (U \oplus V)^*, then
    a(i+j)*(x(i) + y(j))
  denotes a(i)*(x \oplus y)(i).
- The expression
    a(i+j)*x(i)*y(j)
  should be thought about more -- it may be meaningless, and if it is, it should be an
  error.  The reason is that as the "i+j" direct sum index is iterated over, only one of
  i or j should be able to produce nonzero values in quantities that they each index, so
  the expression "should" be equal to zero, if it means anything at all.
- Then the operator precedence of * and + could be correctly used to make sense of
  expressions like
    T(i*j + k),
  where T is in the direct sum of a 2-tensor (or embeddable-in-2-tensor) and a vector type.
- Implement this by creating types TensorProductOfAbstractIndices_c<AbstractIndexTypeList>
  and DirectSumOfAbstractIndices_c<AbstractIndexTypeList>, each inheriting from some
  base class for abstract indices -- AbstractIndex_c would also need to inherit from this
  base class.  The class hierarchy could look like

    AbstractIndexBase_c<Derived_>
      AbstractIndex_c<SYMBOL>
      TensorProductOfAbstractIndices_c<IndexTypeList>
      DirectSumOfAbstractIndices_c<IndexTypeList>

- Initializing a matrix (a tensor product of direct sums) by blocks would look something like

    block_diagonal((i_0+i_1)*(j_0+j_1)) = upper_left_block(i_0*j_0)
                                        + upper_right_block(i_0*j_1)
                                        + lower_left_block(i_1*j_0)
                                        + lower_right_block(i_1*j_1);

  However, not all four terms are necessary.  The statement

    block_diagonal((i_0+i_1)*(j_0+j_1)) = upper_left_block(i_0*j_0)
                                        + lower_right_block(i_1*j_1);

  should also work, where the upper-right and lower-left blocks are initialized to zero.
  This particular example shows that the implementation of such an indexing scheme is
  nontrivial and requires some thought.


### Notes for conceptual layer refinement to implement clearer/simpler categorical structure
- Add a hierarchy of "concept classes", in which whole classes of concepts have a fixed
  inheritance hierarchy, with a root class of "concept".  This defines a poset structure
  on the set of conceptual classes, and allows the "meet" of a finite set of concepts to
  be defined.  This meet operation will be used in determining the structure of functorially
  induced constructions such as tensor products.
- The current concept class hierarchy is
                    BasedInnerProductSpace
                   /                      \
            InnerProductSpace       BasedVectorSpace
               /        \           /       \
      InnerProduct       VectorSpace         Basis
                 \            |             /
                   \          |           /
                     \        |         /
                       \      |       /
                            Concept
  The nodes in the upper diamond are categories.  Other useful categories that could be
  useful to consider are HermitianSpace, SymplecticSpace, PseudoInnerProductSpace, etc.
  Though maybe all of these could be subsumed into a single "VectorSpace equipped with a
  bilinear form", from which particular properties (e.g. positive-definite, symmetric,
  etc) could be extracted.  Though this is getting away from the conceptual and concept
  class hierarchy as a general principal, so it may not be a good idea.  Or perhaps this
  "Vector space equipped with a bilinear form" is the meet of InnerProductSpace,
  HermitianSpace, SymplecticSpace, etc.
- Maybe only a hierarchy of categories should be implemented, in which case, the hierarchy
  would be only
                    BasedInnerProductSpace
                   /                      \
            InnerProductSpace       BasedVectorSpace
                        \           /
                         VectorSpace
- There will be a "concept class of" metafunction which returns the concept class of any
  particular concept.
- The conceptual hierarchy as implemented by a concept via ParentTypeList should mirror
  the structure as defined by the concept class hierarchy.
- Tensor products (and tensor/symmetric/exterior powers, diagonal 2-tensors, direct sums,
  etc) will now be functors that produce the concept TensorProduct_c (and analogous), and
  the concept class of a tensor product will be determined via the meet of its factors.
- In particular for vector/tensor spaces, there needs to be a well-defined "factor count"
  metafunction, which returns 1 for a vector space and the number of factors for a tensor
  product.  For an exterior/symmetric product, it will be 1.
- There should be a well-defined "direct summand count" metafunction which returns 1 for
  anything but a direct sum, and the number of summands for a direct sum.
- All the "embeddable in tensor product" stuff will go away in preference for the
  vector-space-into-vector-space embeddings which can be requested.  These embeddings
  will be sparse operations that are defined by reindexing (and scale factor) maps in
  the conceptual layer.
- There could be a single ImplementationOf_t class, providing all the facilities (e.g.
  vector indexing, tensor indexing, embedding, direct summand access), possibly through
  interfaces classes (Vector_i, Tensor_i, EmbeddableAsTensor_i, DirectSum_i), and the
  success of various forms of access (e.g. accessing the Nth direct summand, using a
  multi-indexed expression template) would depend on the type of the concept being
  implemented.
- Implement scalar fields as 0-tensors
- Do all the 0-tensor and 0th tensor/symmetric/exterior power stuff in the conceptual
  layer (need to know what scalar field the tensor product is taken over), where a
  0 tensor is the scalar field.  This can happen before the whole conceptual layer
  refinement.
- The DimensionOf_f metafunction should take an "OverScalarField_" parameter, defaulting
  to NullType (which indicates the base field of the argument should be used), but can
  be specified e.g. to find the real dimension of a complex vector space.
- Probably the embed/project stuff will need to happen at the same time as all this,
  or perhaps the embed/project stuff could happen before it.


### Notes for terse type strings
- The goal is to provide human-readable (i.e. non-template-notation) strings for the
  many types in tenh as compactly as possible, especially the conceptual types.
- Things that represent mathematical objects (e.g. vector spaces, tensor products)
  should be written in a notation not dissimilar from standard mathematical notation.
  The functions that generate instances of the various conceptual types would be
  a reasonable basis for the notation, and furthermore, it would be ideal to have
  those two be identical.
- Perhaps there should also be a "latex string" for the mathematical types.
- Perhaps the terse type strings don't need to uniquely identify the types.
- Which types need to have such a string?
    * All _c types
    * All _t types
- By default the terse string will be the same as the verbose (i.e. existing) type string
- In strings that require parentheses, the goal should be to use the minimal amount.


### Notes for [probable] upgrade to C++11 (to happen after "version 1.0" is reached)
- TODO: look into C++11 support for embedded (and other) compilers that we might have to
  deal with in the future.
- "Template typedefs" are possible through "alias declarations" in C++11:

        template <size_t N>
        using Vector = Matrix<N, 1>;

  The type Vector<3> is equivalent to Matrix<3, 1>.
- x All the TypeList_t stuff (which is deeply interwoven into Tensor Heaven) could be refactored
  to use variadic templates, though it would probably be good to do a bunch of experiments and
  gain experience using them before settling on a design/best practice.
- Tons of the metaprogramming uses static const variables -- these can be changed to use constexpr,
  though it looks like lack of MSVC support will hold this one back for a long time.
- Static asserts with error strings will be awesome (no more making enums for the error messages).
- x Use nullptr
- Change private constructors to use the new " = delete" notation added in C++11.
- Move semantics for move constructors for various memory-containing structures (e.g. vectors)
- Once "decltype" becomes available, it would be possible to do things like

    typedef BasedVectorSpace_c<...> B;
    B b;
    // this would be typename DualOf_f<B>::T
    typedef decltype(dual(b)) DualOfB;
    // this would be TensorProductOfBasedVectorSpaces_c<Typle_t<B,typename DualOf_f<B>::T>>
    typedef decltype(b * dual(b)) EndomorphismOfB;
    // this would be SymmetricPowerOfBasedVectorSpace_c<3,DualOfB>
    typedef decltype(sym<3>(dual(b))) Sym3_DualOfB;

  which would make the conceptual type hierarchy much easier to use.  To make "parenthesized"
  types, e.g. (U * V) * (W * X), which would be a 2-tensor of 2-tensors, a formal
  parenthesized AST note should be used, since the parenthesization of a C++ expression is
  not possible to determine via operator overloading.  The code would look like

    typedef decltype(paren(u * v) * paren(w * x)) TProd;

  Contrast this with the current way:

    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<U,V>> U_otimes_V;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<W,X>> W_otimes_X;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<U_otimes_V,W_otimes_X>> TProd;
- Use of ordinary functions and decltype to implement metaprograms.  For example,
    
    decltype(v * dual(u)) t; // makes t an instance of a 2-tensor conceptual type
    decltype(concat(int(),float(),bool())) x; // makes x an instance of Typle_t<int,float,bool>
    decltype(on_each(typle, evaluator)) evaluated_typle; // evaluator is some _e instance

  The advantage of this is that it allows constructions in "standard C++" using instances
  of pure types (i.e. a type that has no members and no code), without the need for the
  "typename" or "template" keyword, or "dereferencing" metafunction values via ::T or ::V,
  and to get back from the instance world to the type world, "decltype" is used.  To get
  constexpr-like functionality, functions could be defined that use the _f type metafunctions;

    template <typename... Types_>
    Uint32 length (Typle_t<Types_...> const &) { return Length_f<Typle_t<Types_...>>::V; }

  Another useful function could be (assuming it works)

    template <typename T_>
    bool types_are_equal (T_ const &, T_ const &) { return true; }
    template <typename T0_, typename T1_>
    bool types_are_equal (T0_ const &, T1_ const &) { return false; }


### Notes/Reflections on the development of a really really template-heavy math library
- The overarching conclusion is that C++ template metaprogramming (TMP) is the shittiest
  functional programming language that is widely used -- it's an emergent property of the original
  idea of making generic code (container classes, etc), and so was never designed as a first class
  language, and that really shows.  Doing things in C++ TMP which are quick and easy in a "real"
  functional language like Haskell require lots of extra red tape (e.g. all the extra uses of
  the "typename" and "template" keywords) and feel kludgey.  The error messages it produces are
  clearly tiny glimmers of the light of Hell leaking into our reality.
- Trying to have the compiler enforce the conceptual type system through the C++ type system
  is untenable, as it is too strict and the compile errors it produces are unreadable.
    * A good trick is to write template metafunctions with less constraint and then use explicit static
      asserts to enforce the conceptual type system.  Ted calls this a "honey bucket" for the compiler.
      For example, to enforce that only up/down index pairs can be contracted in a tensor expression,
      write the C++ template code to allow any pairs of indices to be contracted, and then use a
      static assert to enforce the up/down pairing, thereby controlling the situation when there
      is an index mismatch.  The error message produced is then readable and meaningful.
- Trying to encode the full, deep mathematical relationships in a C++ class hierarchy proved to be
  too difficult, because inheritance in the mathematical sense doesn't work exactly the same way.
    * The solution appears to be (though the full solution is yet to be implemented as of 2013.07.21)
      implementing the mathematical relationships in "pure" types (compile-time only, no code) in a
      "conceptual" layer, and put the implementations in its own layer.  This forces the designer
      not to mix up the two domains, and the division enhances clarity.  (Update: 2013.10.12: this
      is now more or less fully implemented).
- Template code takes forever to compile in some cases.  This certainly happened in some of the
  Tensor Hell unit tests, where there the same templatized tests were being run on many types.
    * When possible and when it makes sense (such as in unit tests where there are many test cases),
      break code that uses templates up into many separate source files, so that parallel computation
      can be used to speed things along.
- TODO: once conceptual inheritance stuff is done, write up some notes on this and about the
  conceptual layer in general.  Should probably look into prior art on implementing C++ concepts
  (boost?).
- Haskell turns out to be a great model for the metaprogramming.  Embrace it.
- Using a sentinel type e.g. for "fill with" turns out to be a great way to get around constructor
  choice ambiguity.  This ambiguity was a problem when we wanted to use conversion operators on
  expression templates that weren't conversion to the scalar type.
- The addition of "decltype" in the C++11 standard allows ordinary, templatized C++ functions to
  fill the role of metafunctions which are usually implemented as structs containing a typedef.
  The way this works is that instances of the various "pure types" (informally defined as types
  that have no non-pure members) are what carry the type information, and that the full power of
  C++ function overloading and operator overloading can be used to add this expressiveness and
  directness to metaprogramming.  The decltype keyword is used to "ascend" back into the type
  system, allowing the C++-function-based program to be evaluated at compile time.


### Notes from Tensor Heaven future planning
- Projects which will use Tensor Heaven
    * Multivariate polynomials (c.f. Kevin's polynomial class); will use fully symmetric tensors
    * Higher order SVD (implement in C++ with Tenh, then use some C++-to-Matlab layer interface
      library to provide this facility in Matlab for David's use)
    * General cumulants (also uses fully symmetric tensors)
    * Manifold/differential geometry
        - sheaf theory (maps from manifolds to tensors)
        - Young tableaux (for Riemannian curvature tensor)
        - exterior calculus (uses alternating tensors)
        - covariant calculus (exploiting symmetries in e.g. Christoffel symbols, Riemann tensor, etc)
    * Linear algebra "frontend" for Tensor Heaven -- no indexed expressions; use "Hom" formalism
      so that effectively nothing above a 2-tensor is ever used.  The type system will be traversed
      by explicit notation.

      E.g. if R \in W \otimes V^*  and  w \in V \otimes V, then

              R*w

      should be a 1-slot in-line contraction having resulting type W \otimes V, and

              TensorPower<2>(R)*w

      has type W \otimes W, and TensorPower<2>(R) \in Hom(V \otimes V, W \otimes W).

      The C++ operator* will compute an unambiguous in-line contraction (think box diagrams
      with lines connecting the contracted boxes).  Note that taking the tensor/exterior/symmetric
      power is a functor on the category of [based] vector spaces.
    * Tensor calculus library with pullback formalism.
- Features necessary for project use
    * Inner products
    * Induced inner products
    * Euclidean embedding
    * Induced euclidean embedding
    * Eigen interop
    * More assembler code investigation
- Documentation
    * High level abstract math documents (e.g. linear algebraic category theory)
        - Linear algebraic category theory
        - Linear algebraic constructions
            ~ Choices of bases for various things (e.g. so that dual/sym-power functors commute)
            ~ Induced multilinear forms
    * Journal articles
        - Conceptual layer, conceptual inheritance
        - Tensor type theory (navigating the type system, e.g. parenthesizing tensor products
          to get various naturally isomorphic types)
    * Doxygen documentation
        - High level "how this works" and "here is the structure of the library" docs
        - Class-level documentation of classes, methods, functions, members, etc.
    * Tutorials/examples which can be involved/related to unit tests
    * Code-level documentation of algorithms (non-Doxygen comments)
- Licensing
    * I (we) would like to see it released open-source for general use, and then license
      it for commercial use.  This is something to talk to legal about.
    * Open source would imply that outside programmers could/would submit patches.
      Because of the commercial licensing, we would have to have them give up the rights
      to the source code changes.

### Interesting Facts
- On 2013.10.03,
  `find . -name "*.hpp" -exec grep -vE "^[ ]*$|^[ ]*//.*|^[ ]*\{$|^[ ]*\};?$|^[ ]*:$" {} \; | wc -l`
  produced 6289 (lines of meaningful code)
