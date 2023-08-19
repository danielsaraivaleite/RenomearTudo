//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Template.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTemplates *FormTemplates;
//---------------------------------------------------------------------------
__fastcall TFormTemplates::TFormTemplates(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormTemplates::BotaoOKClick(TObject *Sender)
{
  Close();        
}
//---------------------------------------------------------------------------
void __fastcall TFormTemplates::FormShow(TObject *Sender)
{
  DirList->Directory =  ".";
  DirList->Directory =  ExtractFilePath(Application->ExeName)+"Templates";
  Lista->Clear();
  TListItem  *ListItem;      // usado para inserir no TListView
  TIniFile *ini;
  for(int i=0; i<DirList->Items->Count; i++) {
    ListItem = Lista->Items->Add(); // pega referência
    ListItem->Caption = DirList->Items->operator [](i);
    // abre descrição template
    AnsiString fullName = ExtractFilePath(Application->ExeName)+"Templates\\"+DirList->Items->operator [](i);
    ini = new TIniFile(fullName);
    ListItem->SubItems->Add(ini->ReadString("Expressao Regular","Descricao", "Expressão do usuário" ));
    delete ini;   
  }

}
//---------------------------------------------------------------------------


void __fastcall TFormTemplates::BotaoSelecionarClick(TObject *Sender)
{
  if(Lista->Selected){
     SelecionaExpressao(ExtractFilePath(Application->ExeName)+"Templates\\"+Lista->Selected->Caption);
  }
}
//---------------------------------------------------------------------------


void TFormTemplates::SelecionaExpressao(AnsiString file)
{

    TIniFile *ini = new TIniFile(file);
    FormPrincipal->CaixaExpressao->Text = ini->ReadString("Expressao Regular","Expressao", "ERRO na expressão!" );
    FormPrincipal->CaixaSubstituirPor->Text = ini->ReadString("Expressao Regular","SubstituirPor", "ERRO na expressão de substituição!" );
    FormPrincipal->CheckApenasPrimeira->Checked = ini->ReadBool("Expressao Regular","LocalizarApenasPrimeira", false );
    FormPrincipal->CheckMenor->Checked = ini->ReadBool("Expressao Regular","LocalizarMenorOcorrencia", false);
    FormPrincipal->CheckManter->Checked = ini->ReadBool("Expressao Regular","ManterRestoInalterado",true );
    FormPrincipal->CheckCase->Checked = ini->ReadBool("Expressao Regular","CaseSensitive",false );
    AtualizaOpcoesExpressoes();
    Close();

}


void __fastcall TFormTemplates::ListaDblClick(TObject *Sender)
{
  if(Lista->Selected){
     SelecionaExpressao(ExtractFilePath(Application->ExeName)+"Templates\\"+Lista->Selected->Caption);
  }        
}
//---------------------------------------------------------------------------


void TFormTemplates::AtualizaOpcoesExpressoes()
{

     FormPrincipal->ER->MergeCopy  = FormPrincipal->CheckManter->Checked;
     FormPrincipal->ER->MergeOnlyFirst  = FormPrincipal->CheckApenasPrimeira->Checked;
     
     if(FormPrincipal->CheckCase->Checked) {
       FormPrincipal->ER->ExpressionOptions >> RegularExpression::CaseInsensitive;
     }
     else FormPrincipal->ER->ExpressionOptions << RegularExpression::CaseInsensitive;

     if(FormPrincipal->CheckMenor) {
       FormPrincipal->ER->MatchOptions << RegularExpression::MatchShortest;
     }
     else FormPrincipal->ER->MatchOptions >> RegularExpression::MatchShortest;


}
