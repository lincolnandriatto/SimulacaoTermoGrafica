#ifndef OFVERTEXSTARITERATORSURFACEVERTEX_H
#define OFVERTEXSTARITERATORSURFACEVERTEX_H
#include "ofMacros.h"
#include "ofMesh.h"
#include "ofSing.h"
#include "ofIterator.h"
#include "ofList.h"
namespace of
{
#define SVERTEX typename ofVertexStarIteratorSurfaceVertex<_Traits>::sVertex
#define IDS typename ofVertexStarIteratorSurfaceVertex<_Traits>::ids
#define TCURRENT typename std::pair<int, int>
template <class _Traits> 
class ofVertexStarIteratorSurfaceVertex : public ofIterator<_Traits>
{
public:
	
	typedef typename _Traits::sCell 	   sCell;
	typedef typename _Traits::sVertex 	sVertex;
	typedef typename _Traits::ids 		ids;
	typedef ofSing<_Traits>				   sSing;
	typedef typename _Traits::sMesh		sMesh;
   typedef typename std::pair<ids, ids>      TCurrent; //cell and corner ids
	ofVertexStarIteratorSurfaceVertex(sMesh *_mesh);
	
	~ofVertexStarIteratorSurfaceVertex();
	
	bool initialize(ids init);
	bool finish();
	bool notFinish();
	
	bool operator++();
	sVertex *operator->();
	sVertex *operator*();
    ids operator&();
	
private:
	TCurrent  iatu;
   TCurrent  istart;
   
int tempp;   
   
ids  iinit, iv;
sCell *atu;
sVertex *v;

ofList<ids> lista, flags;

int status, is;
// 0 - iterator sem posicao
// 1 - iterator em posicao normal
// 2 - iterator no limite inferior
// 3 - iterator no limite superior

void addNeighbor();
};
   template <class _Traits> 
   ofVertexStarIteratorSurfaceVertex<_Traits>::
ofVertexStarIteratorSurfaceVertex(sMesh *_mesh)
   : ofIterator<_Traits>(_mesh)
{
   iatu.first = NULL;
   iatu.second=-2;
  
	status = 0;
	v = NULL;
    tempp = 0;
   
}
   template <class _Traits> 
   ofVertexStarIteratorSurfaceVertex<_Traits>::
~ofVertexStarIteratorSurfaceVertex()
{
}
   template <class _Traits> 
   bool 
   ofVertexStarIteratorSurfaceVertex<_Traits>::
initialize(ids init)


       {
           OF_ASSERT(init >= 0);

           iv = init;
           v = this->mesh->getVertex(iv);

           status = 1;

           OF_ASSERT(v->inMesh());

           iatu.first =-1;
           iinit = is = -1;

           atu = NULL;

           lista.clear();
           flags.clear();
           //lista.insert(iv);
           //flags.insert(iv);

           return ++(*this);
       }



   template<class _Traits> 
   bool 
   ofVertexStarIteratorSurfaceVertex<_Traits>::
operator++()
{
   /*ids   iaux;
   
//cout << tempp <<endl;
if(tempp++ == 40){
 status = 3;
 return false;
}
   
   
	if(status == 1)//iterador em posicao normal
	{
		
      if(!flagBoundary){
      
         iaux = this->mesh->getCell(iatu.first)->getMateId(iatu.second);
         if(iaux == -1){ // vertex facing the boundary
          cout<<"boundary"<<endl;
            flagBoundary = true; 
            iatu.second = (iatu.second+1)%3;
            
            atu = this->mesh->getVertex(this->mesh->getCell(iatu.first)->getVertexId(iatu.second) );
            
            return true;
            
         }else{
            
            ids auxCellId;
            auxCellId = iatu.first;
            iatu.first = this->mesh->getCell(iatu.first)->getMateId(iatu.second);
            iatu.second = (this->mesh->getCell(iatu.first)->getMateIndex(auxCellId) + 2)%3;
            
            atu = this->mesh->getVertex(this->mesh->getCell(iatu.first)->getVertexId(iatu.second) );
            if(iatu.first == istart.first && iatu.second == istart.second){
            cout<<"finish"<<endl;
               status = 3;
               return false;
            }
            
            return true; 
         }
         
      }else{
         status = 3;
         return false;
      }
   }*/
    ids auxCellId;
       if(status == 1)
       {
           if(iatu.first < 0)
           {
               if(is < 0)
               {
                   is = 0;
                   iatu.first = v->getSingCell(is);
                   iatu.second = this->mesh->getCell(iatu.first)->getVertexId((this->mesh->getCell(iatu.first)->getVertexIndex(iv)+1)%3) ;
                   if(iatu.first>-1)
                   {
                       OF_ASSERT(iatu.first >= 0);

                       atu = this->mesh->getCell(iatu.first);
                       OF_ASSERT( (atu) && (atu->inMesh()) );

                       flags.insert(iatu.first);

                       addNeighbor();

                       iinit = iatu.first;
                   }
                   else
                       status=3;
               }
           }
           else
           {
               OF_ASSERT( atu->getVertexIndex(iv) >= 0);

               if(lista.empty())
               {
                   is++;
                   iatu.first = v->getSingCell(is);
                   iatu.second = this->mesh->getCell(iatu.first)->getVertexId((this->mesh->getCell(iatu.first)->getVertexIndex(iv)+1)%3) ;

                   if(iatu.first >= 0)
                   {
                       //OF_ASSERT(!flags.inList(iatu));
                       lista.insert(iatu.first);
                       flags.insert(iatu.first);
                   }
                   else
                   {
                       atu = NULL;
                       iatu.first = -1;
                       status = 3;
                       return false;
                   }
               }

               iatu.first  = lista.first();
               iatu.second = this->mesh->getCell(iatu.first)->getVertexId((this->mesh->getCell(iatu.first)->getVertexIndex(iv)+1)%3) ;
               lista.erase(0);

               atu = this->mesh->getCell(iatu.first);
               OF_ASSERT( (atu) && (atu->inMesh()) );

               addNeighbor();
           }

           return true;
       }
       else

  return false;
}

   template<class _Traits> void ofVertexStarIteratorSurfaceVertex<_Traits>::addNeighbor()
   {
       ids temp;

       temp = atu->getMateId((atu->getVertexIndex(iv)+1)%3);
       if((temp >= 0)&&(!flags.inList(temp)))
       {
           flags.insert(temp);
           lista.insert(temp);
       }
       temp = atu->getMateId((atu->getVertexIndex(iv)+2)%3);
       if((temp >= 0)&&(!flags.inList(temp)))
       {
           flags.insert(temp);
           lista.insert(temp);
       }


       OF_ASSERT( atu->getVertexIndex(iv) >= 0);
   }

template<class _Traits> SVERTEX* ofVertexStarIteratorSurfaceVertex<_Traits>::operator->()
{
	return atu;
}
template<class _Traits> SVERTEX* ofVertexStarIteratorSurfaceVertex<_Traits>::operator*()
{
	return atu;
}
template<class _Traits> IDS ofVertexStarIteratorSurfaceVertex<_Traits>::operator&()
{
    return iatu.second;
}
template<class _Traits> bool ofVertexStarIteratorSurfaceVertex<_Traits>::finish()
{
	return ((status == 2)||(status == 3));
}
template<class _Traits> bool ofVertexStarIteratorSurfaceVertex<_Traits>::notFinish()
{
	return ((status == 0)||(status == 1));
}
#undef SCELL
#undef IDS
#undef TCURRENT
}
#endif 
