//---------------------------------------------------------------------------

#ifndef TemplateH
#define TemplateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <FileCtrl.hpp>
#include <IniFiles.hpp>
#include "FormPrinc.h"
//---------------------------------------------------------------------------
class TFormTemplates : public TForm
{
__published:	// IDE-managed Components
        TButton *BotaoOK;
        TButton *BotaoSelecionar;
        TListView *Lista;
        TFileListBox *DirList;
        void __fastcall BotaoOKClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall BotaoSelecionarClick(TObject *Sender);
        void __fastcall ListaDblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormTemplates(TComponent* Owner);
        void SelecionaExpressao(AnsiString file);
        void AtualizaOpcoesExpressoes();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTemplates *FormTemplates;
//---------------------------------------------------------------------------
#endif
