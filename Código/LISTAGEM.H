//---------------------------------------------------------------------------

#ifndef ListagemH
#define ListagemH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormListagem : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo;
        TButton *BotaoOk;
        TPanel *Panel1;
        TButton *BotaoSalvarListagem;
        void __fastcall BotaoSalvarListagemClick(TObject *Sender);
        void __fastcall BotaoOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormListagem(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormListagem *FormListagem;
//---------------------------------------------------------------------------
#endif
