#include <rt/groups/simplegroup.h>

namespace rt {

BBox SimpleGroup::getBounds() const {
    return bbox;
}

Intersection SimpleGroup::intersect( const Ray& ray, float previousBestDistance) const {

    Intersection Nearest_Intersection;
    bool found = false;
    for(std::vector<Primitive *>::size_type i = 0; i != this->Primitives.size(); i++) {
        Intersection Intsec = Primitives[i]->intersect(ray, previousBestDistance);
        if (Intsec) {
            previousBestDistance = Intsec.distance;
            Nearest_Intersection = Intsec;
            found = true;
        }
    }
    if (found)
        return Nearest_Intersection;
    else
        return Intersection::failure();

}

void SimpleGroup::rebuildIndex() {
    //do nothing
}

void SimpleGroup::add(Primitive* p) {
    this->Primitives.push_back(p);
    bbox.extend(p->getBounds());
}

void SimpleGroup::setMaterial(Material* m) {
    for(std::vector<Primitive *>::size_type i = 0; i != this->Primitives.size(); i++)
        Primitives[i]->setMaterial(m);
}

void SimpleGroup::setCoordMapper(CoordMapper* cm) {
    for(std::vector<Primitive *>::size_type i = 0; i != this->Primitives.size(); i++)
        Primitives[i]->setCoordMapper(cm);
}

	float SimpleGroup::getArea() const {
		return 0;
	}

}
