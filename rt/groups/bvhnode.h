#ifndef CG1RAYTRACER_GROUPS_BVHNODE_HEADER
#define CG1RAYTRACER_GROUPS_BVHNODE_HEADER

#include <rt/bbox.h>
#include <rt/primitive.h>

namespace rt {

class  BVHNode {
public:
    BVHNode(){}

    BBox bbox;
    BVHNode* leftChild;
    BVHNode* rightChild;
    bool isNodeLeaf;

    void extend(const BBox& newBBox){
        bbox.extend(newBBox);
    }

    typedef std::vector<Primitive*> Primitives;
    Primitives primitives;

};

}

#endif
