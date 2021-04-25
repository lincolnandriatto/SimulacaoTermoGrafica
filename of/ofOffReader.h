#ifndef OFOFFREADER_H
#define OFOFFREADER_H
#include <fstream>
#include <string.h>
#include "ofMacros.h"
#include "ofMesh.h"
#include "ofReader.h"
using namespace std;
namespace of
{
template <class _Traits> class ofOffReader : public ofReader<_Traits>
{
public:
	typedef typename _Traits::space space;
	typedef typename _Traits::ids ids;
	typedef typename _Traits::sVertex sVertex;
	typedef typename _Traits::sCell sCell;
	
	typedef ofMesh<_Traits> sMesh;
	
	ofOffReader();
	~ofOffReader();
	
	virtual bool read(sMesh* malha, char* filename);
    virtual bool readPly(sMesh* malha, char* filename);
private:
	bool reader(ifstream &pf, sMesh* malha);
    bool readerPly(ifstream &pf, sMesh* malha);
};
template <class _Traits> ofOffReader<_Traits>::ofOffReader()
{
}
template <class _Traits> ofOffReader<_Traits>::~ofOffReader()
{
}
template <class _Traits> bool ofOffReader<_Traits>::read(sMesh* malha, char *filename)
{
	OF_ASSERT(filename);
	OF_ASSERT(malha);	
	
	ifstream pf(filename);
	
	if( pf.is_open())
	{
			if(! reader(pf, malha))
				return false;
		
			pf.close();
	}
	else
		return false;
	
	return true;
}

template <class _Traits> bool ofOffReader<_Traits>::readPly(sMesh* malha, char *filename)
{
    OF_ASSERT(filename);
    OF_ASSERT(malha);

    ifstream pf(filename);

    if( pf.is_open())
    {
            if(! readerPly(pf, malha))
                return false;

            pf.close();
    }
    else
        return false;

    return true;
}

template <class _Traits> bool ofOffReader<_Traits>::readerPly(ifstream &pf, sMesh* malha)
{
    char auxstr[255];
    space coords[3],aux;
    space aux_coords[3];
    int color[4];
    int i, nv, j, nc, k, idnv,f;
    ids auxvtx[sCell::getDimension() + 1];
    OF_ASSERT(auxstr);

    while( (! pf.eof())&&(strcmp(auxstr,"vertex") != 0) )
        pf >> auxstr;
    pf >> nv;
    while( (! pf.eof())&&(strcmp(auxstr,"face") != 0) )
        pf >> auxstr;
    pf >> nc;
    while( (! pf.eof())&&(strcmp(auxstr,"end_header") != 0) )
        pf >> auxstr;
    if(! pf.eof())
    {

        OF_ASSERT(nv > 0);

        for (i=0; i<nv; i++)
        {
            for(j = 0; j < 3; j++)
                pf >> coords[j];

            malha->addVertex(coords);
            for(j = 0; j < 3; j++)
                pf >> aux_coords[j];
            double norm = sqrt(aux_coords[0]*aux_coords[0]+aux_coords[1]*aux_coords[1]+aux_coords[2]*aux_coords[2]);
            aux_coords[0]=aux_coords[0]/norm;
            aux_coords[1]=aux_coords[1]/norm;
            aux_coords[2]=aux_coords[2]/norm;
            malha->getVertex(malha->getNumberOfVertices()-1)->setNormal(aux_coords);
            pf >> f;
            for(j = 0; j < 4; j++)
                pf >> color[j];
            malha->getVertex(malha->getNumberOfVertices()-1)->setColor(color);

        }
        OF_ASSERT(nc >= 0);

        for (i=0; i<nc; i++)
        {
            pf >> idnv;

            OF_ASSERT(idnv == sCell::getDimension() + 1);

            for (j=0; j<sCell::getDimension() + 1 ; j++)
            {
                pf >> k;
                OF_ASSERT((k >= 0)&&(k < nv));
                auxvtx[j] = k;
            }

            malha->addCell(auxvtx);
            pf >> idnv;
            for(j = 0; j < 4; j++)
                pf >> color[j];
            malha->getCell(malha->getCellMaxId())->setColor(color);
        }
    }
    else
        return false;

    return true;


}

template <class _Traits> bool ofOffReader<_Traits>::reader(ifstream &pf, sMesh* malha)
{
	char auxstr[255];
	space coords[3],aux;
	int i, nv, j, nc, k, idnv;
	ids auxvtx[sCell::getDimension() + 1];
	OF_ASSERT(auxstr);
	
	while( (! pf.eof())&&(strcmp(auxstr,"OFF") != 0) )
		pf >> auxstr;
	
	if(! pf.eof())
	{
		pf >> nv;
		pf >> nc;
		pf >> k;
		
		OF_ASSERT(nv > 0);
	
		for (i=0; i<nv; i++)
		{
			for(j = 0; j < 3; j++)
				pf >> coords[j];
                        //coords[0]*=-1;
                        /*aux=coords[1];
                        coords[1]=-aux;
                        coords[1]=aux;
                        
                        aux=coords[2];
                        coords[2]=coords[1];
                        coords[1]=aux;*/
			
			malha->addVertex(coords);
		}
		OF_ASSERT(nc >= 0);
		
		for (i=0; i<nc; i++)
		{
			pf >> idnv;
		
			OF_ASSERT(idnv == sCell::getDimension() + 1);
		
			for (j=0; j<sCell::getDimension() + 1 ; j++)
			{
				pf >> k;
				OF_ASSERT((k >= 0)&&(k < nv));
				auxvtx[j] = k;
			}
		
			malha->addCell(auxvtx);
		}
	}
	else
		return false;
		
	return true;
}
}
#endif
