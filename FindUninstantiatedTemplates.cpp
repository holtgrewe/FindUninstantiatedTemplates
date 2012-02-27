//===- FindUninstantiatedTemplates.cpp ------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Example clang plugin which prints the names of all uninstantiated functions
// and member functions together with their source position.
//
//===----------------------------------------------------------------------===//

#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Decl.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"
using namespace clang;

namespace {

class FindUninstantiatedTemplatesConsumer : public ASTConsumer,
                                            public RecursiveASTVisitor<FindUninstantiatedTemplatesConsumer> {
public:
  ASTContext * ctx;
  
  FindUninstantiatedTemplatesConsumer() : ctx(0) {
  }

  virtual void HandleTranslationUnit(ASTContext & Ctx) {
    ctx = &Ctx;
    TraverseDecl(Ctx.getTranslationUnitDecl());
  }
  
  bool VisitDecl(Decl * decl) {
    if (!isa<FunctionTemplateDecl>(*decl))
      return true;  // We only want template (member) functions.

    // Downcast ftDecl.
    FunctionTemplateDecl * ftDecl = dyn_cast<FunctionTemplateDecl>(decl);
    
    if (ftDecl->spec_begin() != ftDecl->spec_end())
      return true;  // We look for function template declarations *without* specializations.
  
    if (!ftDecl->isThisDeclarationADefinition())
        return true;  // Ignore if this function template declaration is not a definition.

    llvm::errs() << "Uninstantiated template: ";
    ftDecl->getSourceRange().getBegin().print(llvm::errs(), ctx->getSourceManager());
    llvm::errs() << "\n";
    
    return true;
  }
};

class FindUninstantiatedTemplatesAction : public PluginASTAction {
protected:
  ASTConsumer *CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) {
    return new FindUninstantiatedTemplatesConsumer();
  }

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string>& args) {
    for (unsigned i = 0, e = args.size(); i != e; ++i) {
      llvm::errs() << "FindUninstantiatedTemplatesAction arg = " << args[i] << "\n";

      // Example error handling.
      if (args[i] == "-an-error") {
        DiagnosticsEngine &D = CI.getDiagnostics();
        unsigned DiagID = D.getCustomDiagID(
          DiagnosticsEngine::Error, "invalid argument '" + args[i] + "'");
        D.Report(DiagID);
        return false;
      }
    }
    if (args.size() && args[0] == "help")
      PrintHelp(llvm::errs());

    return true;
  }
  void PrintHelp(llvm::raw_ostream& ros) {
    ros << "Help for FindUninstantiatedTemplatesAction plugin goes here\n";
  }

};

}

static FrontendPluginRegistry::Add<FindUninstantiatedTemplatesAction>
X("find-uitpl", "find uninstantiated templates");
