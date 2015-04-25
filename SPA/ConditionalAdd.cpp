/* When a function is called, a special item is added to the lvalueTable. This
 * item specifies that after running the whole scan, values are added as side
 * effects that are actually a side effect in that function (in opposite to
 * previous presumption "a function modifies everything it can access"). This
 * is done iteratively until the lvalueTable doesn't change anymore. This
 * effectively implements side effect inheritance between functions. */

#include "clang/AST/AST.h"
#include "clang/AST/Stmt.h"
#include <vector>
#include <map>

class FunctionConditionalAdd{
  private:
    clang::NamedDecl constraintFunDecl;
    int argIndex;
    clang::NamedDecl declaredAs;
  public:
    FunctionConditionalAdd(clang::NamedDecl constraintFunDecl, int argIndex, clang::NamedDecl declaredAs):constraintFunDecl(constraintFunDecl),argIndex(argIndex),declaredAs(declaredAs){};
    clang::NamedDecl getConstraintFunDecl(){return constraintFunDecl;}
    int getArgIndex(){return argIndex;}
    clang::NamedDecl getDeclaredAs(){return declaredAs;}
    std::vector<clang::NamedDecl> getFunctionsToAdd;
    void addFunctionToAdd(clang::NamedDecl function);
};

class ConditionalAdd{
  private:
    std::map< clang::NamedDecl* , std::vector<clang::NamedDecl*> > funcAdds; // TODO second is vector of FunctionConditionalAdd
  public:
    void addConditionalConstraint(clang::NamedDecl *funDecl, clang::NamedDecl *constraintFunDecl, int argIndex, clang::NamedDecl varDecl); // (function to add constraints to, function side effects of which will be added)
    void execute(); // iteratively (over i and over functions) add constraints as long as they change
    void dump();
};

////////////////////////////////////////////////////////////

void ConditionalAdd::addConditionalConstraint(clang::NamedDecl *funDecl, clang::NamedDecl *constraintFunDecl, int argIndex, clang::NamedDecl varDecl){
  //funcAdds[funDecl].push_back(constraintFunDecl);
  if(funcAdds.find(funDecl)==funcAdds.end()){
    funcAdds[funDecl]=std::vector<clang::NamedDecl*>();
    funcAdds[funDecl].push_back(constraintFunDecl);
  }else{
    funcAdds[funDecl].push_back(constraintFunDecl);
  }
  std::cout << funDecl->getNameAsString() << " gets side effect to it's " << argIndex << ". argument declared within it as '" << varDecl.getNameAsString() << "' if it has side effect in " << constraintFunDecl->getNameAsString() << " (side effect type will be kept)." << std::endl;
  this->dump();
}

void ConditionalAdd::dump(){
  std::cout << "Cond dump begin >>" << std::endl;
  for(std::map<clang::NamedDecl*, std::vector<clang::NamedDecl*> >::iterator i = funcAdds.begin(); i != funcAdds.end(); ++i) {
    for(std::vector<clang::NamedDecl*>::iterator j = i->second.begin(); j != i->second.end(); ++j){
      std::cout << "Cond: " << i->first->getNameAsString() << " <- " << (*j)->getNameAsString() << std::endl;
    }
  }
  std::cout << " << Cond dump end" << std::endl;
}
