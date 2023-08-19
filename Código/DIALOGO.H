//----------------------------------------------------------------------------
#ifndef dialogoH
#define dialogoH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
//----------------------------------------------------------------------------
class TdialogoOrdem : public TForm
{
__published:
	TBevel *Bevel1;
        TBitBtn *botaoCancelar;
        TBitBtn *botaoOK;
        TLabel *Label1;
        TEdit *caixaNumero;
        TImage *Image1;
        void __fastcall botaoCancelarClick(TObject *Sender);
        void __fastcall caixaNumeroKeyPress(TObject *Sender, char &Key);
        void __fastcall botaoOKClick(TObject *Sender);
private:
public:
	virtual __fastcall TdialogoOrdem(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TdialogoOrdem *dialogoOrdem;
//----------------------------------------------------------------------------
#endif    
