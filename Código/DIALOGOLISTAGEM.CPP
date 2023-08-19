//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DialogoListagem.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDialogoTipoListagem *DialogoTipoListagem;
//---------------------------------------------------------------------------
__fastcall TDialogoTipoListagem::TDialogoTipoListagem(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDialogoTipoListagem::FormActivate(TObject *Sender)
{
 if(FormPrincipal->iniciado&&RadioConteudo->Items->Count==1)
  {
    RadioConteudo->Items->Add("Prévia");
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogoTipoListagem::BotaoCancelarClick(TObject *Sender)
{
  Close();        
}
//---------------------------------------------------------------------------
void __fastcall TDialogoTipoListagem::BotaoOKClick(TObject *Sender)
{
  Close();
  FormListagem->Memo->Clear();
  if(RadioConteudo->ItemIndex==0) // arquivos selecionados
   {
     for(int i=0; i<FormPrincipal->GridArquivos->Items->Count; i++)
      {
        if(RadioEstilo->ItemIndex==0)
         {
           FormListagem->Memo->Lines->Append(FormPrincipal->GridArquivos->Items->operator [](i)->SubItems->operator [](3)) ;
         }
        else
         {
          if(RadioEstilo->ItemIndex==1)
            {
             FormListagem->Memo->Lines->Append(FormPrincipal->GridArquivos->Items->operator [](i)->Caption+"."+FormPrincipal->GridArquivos->Items->operator [](i)->SubItems->operator [](0));
            }
          else
           {
             FormListagem->Memo->Lines->Append(FormPrincipal->GridArquivos->Items->operator [](i)->Caption);
           }
         }
      }
   }
  else
   {
       AnsiString aux;
     for(int i=0; i<FormPrincipal->GridArquivos->Items->Count; i++)
      {
        if(RadioEstilo->ItemIndex==0)   // nome completo
         {
           aux = ExtractFilePath(FormPrincipal->GridPrevia->Cells[0][i+1])+FormPrincipal->GridPrevia->Cells[1][i+1]+"."+FormPrincipal->GridPrevia->Cells[2][i+1];
           FormListagem->Memo->Lines->Append(aux) ;
         }
        else
         {
          if(RadioEstilo->ItemIndex==1)
            {
             FormListagem->Memo->Lines->Append(FormPrincipal->GridPrevia->Cells[1][i+1]+"."+FormPrincipal->GridPrevia->Cells[2][i+1]);
            }
          else
           {
             FormListagem->Memo->Lines->Append(FormPrincipal->GridPrevia->Cells[1][i+1]);
           }
         }
      }
   }
     FormListagem->ShowModal();
}
//---------------------------------------------------------------------------

