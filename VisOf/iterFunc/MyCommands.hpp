#ifndef MY_COMMANDS_HPP_
#define MY_COMMANDS_HPP_

#include "../../scrInteractor.h"
#include "CommandsSet.hpp"

#include "CmdPrintScreenImage.hpp"
#include "CommandsSetShowOfSurface.hpp"

template<typename _TPrint>
class MyCommands : public CommandsSet{
   
public:
   typedef _TPrint                           TPrint;   
   
public:

   MyCommands(TPrint *print_i, scrInteractor *interactor){
      
      RegistryCommand(' ', new CommandsSetShowOfSurface<TPrint>(print_i));
      RegistryCommand('i', new CmdPrintScreenImage(interactor,"screenVis", 2, 0));
   }
     
};

#endif /*MY_COMMANDS_HPP_*/
