//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Listagem.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormListagem *FormListagem;
//---------------------------------------------------------------------------
__fastcall TFormListagem::TFormListagem(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormListagem::BotaoSalvarListagemClick(TObject *Sender)
{
  TSaveDialog *dialogo = new TSaveDialog(Application);
  dialogo->DefaultExt = "txt";
  dialogo->Title = "Entre com o nome do arquivo";
  dialogo->Execute();
  if(dialogo->FileName.Trim()!="")
   {
     Memo->Lines->SaveToFile(dialogo->FileName);
   }
  delete dialogo;
}
//---------------------------------------------------------------------------

void __fastcall TFormListagem::BotaoOkClick(TObject *Sender)
{
  Close();        
}
//---------------------------------------------------------------------------

