#ifndef SHEPOLYWRITER_H

#define SHEPOLYWRITER_H

#include <iostream>
#include <fstream>
#include <string>

class shePolyWriter 

{

public:

	
	

	shePolyWriter();

	~shePolyWriter();

	

	bool Write(sheSurface* malha, string filename, double norma=1.0);



private:



	bool Writer(ofstream &pf, sheSurface *malha, double norma=1.0);

};


shePolyWriter::shePolyWriter()

{



}



shePolyWriter::~shePolyWriter()

{

}


bool shePolyWriter::Write(sheSurface* malha, string filename, double norma)

{

	ofstream pf(filename.c_str());

	

	if( pf.is_open())

	{

			if(! Writer(pf, malha,norma))

				return false;

		

			pf.close();

	}

	else

		return false;

	

	return true;

}

bool shePolyWriter::Writer(ofstream &pf, sheSurface* malha, double norma)

{

	int i = 1,j;
	double xmin, ymin,zmin;
	double xmax, ymax,zmax;
	

	pf << "# part 1 node list" << endl;

	
	

	pf << malha->GetNumVertices()+8 << " 3 0 0 " << endl;

	

	

	sheIterator<sheVertex> itv;

	

	for(itv=malha->BeginVertex(); itv.NotFinish(); ++itv)

	{


	

			pf << i << " " << itv->GetX()*norma << " "  << itv->GetY()*norma << " "  << itv->GetZ()*norma << endl;
			i++;

		

	}
	malha->GetVertices()->CalculateMinMaxCoordinates();
	malha->GetVertices()->GetMinMaxCoordinates(xmin,xmax,ymin,ymax,zmin,zmax);

	pf << i << " "  << xmin*norma-3.0 << " " << ymin*norma-3.0 << " " << zmin*norma-3.0 << endl;
	i++;
	pf << i << " "  << xmin*norma-3.0 << " " << ymax*norma+3.0 << " " << zmin*norma-3.0 << endl;
	i++;
	pf << i << " "  << xmax*norma+3.0 << " " << ymax*norma+3.0 << " " << zmin*norma-3.0 << endl;
	i++;
	pf << i << " "  << xmax*norma+3.0 << " " << ymin*norma-3.0 << " " << zmin*norma-3.0 << endl;
	i++;
	pf << i << " "  << xmin*norma-3.0 << " " << ymin*norma-3.0 << " " << zmax*norma+3.0 << endl;
	i++;
	pf << i << " "  << xmin*norma-3.0 << " " << ymax*norma+3.0 << " " << zmax*norma+3.0 << endl;
	i++;
	pf << i << " "  << xmax*norma+3.0 << " " << ymax*norma+3.0 << " " << zmax*norma+3.0 << endl;
	i++;
	pf << i << " "  << xmax*norma+3.0 << " " << ymin*norma-3.0 << " " << zmax*norma+3.0 << endl;
	
	int nv = malha->GetNumVertices();

	pf << "# part 2 face list" << endl;

	pf << malha->GetNumCells()+10 << " 1" << endl;

	pf <<"1 0 0" << endl;
	pf << "3 " << nv+1 <<" "<< nv+2 <<" "<< nv+3 << endl;
	pf <<"1 0 0" << endl;
	pf << "3 " << nv+1 <<" "<< nv+3 <<" "<< nv+4 << endl;
	pf <<"1 0 0" << endl;
	pf << "3 " << nv+1 <<" "<< nv+6 <<" "<< nv+2 << endl;
	pf <<"1 0 0" << endl;
	pf << "3 " << nv+1 <<" "<< nv+5 <<" "<< nv+6 << endl;
    pf <<"1 0 0" << endl;
	pf << "3 " << nv+1 <<" "<< nv+4 <<" "<< nv+8 << endl;
	pf <<"1 0 0" << endl;
	pf << "3 " << nv+1 <<" "<< nv+5 <<" "<< nv+8 << endl;
    pf <<"1 0 0" << endl;
	pf << "3 " << nv+4 <<" "<< nv+3 <<" "<< nv+8 << endl;
	pf <<"1 0 0" << endl;
	pf << "3 " << nv+4 <<" "<< nv+7 <<" "<< nv+8 << endl;
	pf <<"1 0 0" << endl;
	pf << "3 " << nv+3 <<" "<< nv+8 <<" "<< nv+6 << endl;
	pf <<"1 0 0" << endl;
	pf << "3 " << nv+3 <<" "<< nv+6 <<" "<< nv+7 << endl;

	
	
	



	sheIterator<sheCell> itc;

	sheIterator<sheHalfEdge> ite;	

	for(itc=malha->GetShellArray()[0]->BeginCell(); itc.NotFinish(); ++itc)

	{
		pf <<"1 0 1 "  << endl;
		pf << "3 "; 

	

		for(ite=itc->BeginHalfEdge();ite.NotFinish();++ite)
			
			pf << " " << ite->GetVertex()->GetId()+1;

		pf << endl; 

	}

	

	

	pf << "# part 3 hole list" << endl;
	pf << "0" << endl;

	pf << "# part 4 region list" << endl;
	pf << "1" << endl;
	pf << "0.0 0.0 0.0 1 1" << endl;
	
	

	return true;

}




#endif