#ifndef OFMYVERTEX3D_H
#define OFMYVERTEX3D_H
#include "ofMacros.h"
#include "ofVertex3D.h"
namespace of
{
#define SPACE typename ofMyVertex3D<_Traits>::space
#define IDS typename ofMyVertex3D<_Traits>::ids
/** Base class of 3D vertex
 * 
*/
template <class _Traits> class ofMyVertex3D : public ofVertex3D<_Traits>
{
public:
	typedef typename _Traits::space space;
    typedef typename _Traits::ids ids;
	/* Constructor */
	ofMyVertex3D();	
	/* Destructor */
	~ofMyVertex3D();

	ofMyVertex3D(space x, space y, space z);
	//usado no Crust para identificar se pertence a primeira triangulação de delaunay
	bool first;

	unsigned int edgeAtt;
	//diz se o vertice é um ponto de max, min, ou nd
	unsigned int type;
	//armazena os 8 vizinhos do vertice
	int neighbor[8];
	//diz se ja foi usado ou não em alguma aresta como um ponto pode constar em até duas arestas, seu valor maximo deverá se 2
	unsigned int set;
    int color[4];
    space normal[3];
	void setType(int t){type = t;};
	void setSet(int ok){set = ok;};
	void setFirst(bool ok){first = ok;};
	bool getFirst(){return first;};
    void setNormal(space *normal_);
    space* getNormal();
    void setColor(ids *color_);
    ids getColorR();
    ids getColorG();
    ids getColorB();
    ids getColorA();
    space getNormal(ids i);
};
template <class _Traits> ofMyVertex3D<_Traits>::ofMyVertex3D()
{
	type =0;
	edgeAtt=0;
}
template <class _Traits> ofMyVertex3D<_Traits>::ofMyVertex3D(space x, space y, space z)
{
	this->setCoord(0,x);
	this->setCoord(1,y);
	this->setCoord(2,z);
}

template <class _Traits> void ofMyVertex3D<_Traits>::setNormal(space *normal_)
{
    this->normal[0]=normal_[0];
    this->normal[1]=normal_[1];
    this->normal[2]=normal_[2];

}


template <class _Traits> void ofMyVertex3D<_Traits>::setColor(ids *color_)
{
    this->color[0]=color_[0];
    this->color[1]=color_[1];
    this->color[2]=color_[2];
    this->color[3]=color_[3];
}

template <class _Traits> IDS ofMyVertex3D<_Traits>::getColorR()
{
    return color[0];
}

template <class _Traits> IDS ofMyVertex3D<_Traits>::getColorG()
{
    return color[1];
}

template <class _Traits> IDS ofMyVertex3D<_Traits>::getColorB()
{
    return color[2];
}

template <class _Traits> IDS ofMyVertex3D<_Traits>::getColorA()
{
    return color[3];
}

template <class _Traits> SPACE* ofMyVertex3D<_Traits>::getNormal()
{
    return normal;
}
template <class _Traits> SPACE ofMyVertex3D<_Traits>::getNormal(ids i)
{
    return normal[i];
}

template <class _Traits> ofMyVertex3D<_Traits>::~ofMyVertex3D()
{
}

#undef SPACE
#undef IDS
}
#endif
