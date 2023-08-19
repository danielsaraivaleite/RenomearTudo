//---------------------------------------------------------------------------

#ifndef FormPrincH
#define FormPrincH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ImgList.hpp>
#include <math.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <DateUtils.hpp>
#include <Math.hpp>
#include <StrUtils.hpp>
#include <Graphics.hpp>
#include "DialogoPastas.h"
#include "DialogoListagem.h"
#include "Template.h"
#include "Template.h"
#include "sobre.h"
#include "confirma.h"
#include "id3.h"
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <mmsystem.h>
#include <Grids.hpp>
#include "RegularExpression.h"
#include "RegularExpressionCustom.h"

//---------------------------------------------------------------------------
class TFormPrincipal : public TForm
{
__published:	// IDE-managed Components
        TPageControl *Abas;
        TTabSheet *PgArquivos;
        TTabSheet *PgSubstituir;
        TTabSheet *PgInserir;
        TTabSheet *PgIndexado;
        TLabel *Label1;
        TLabel *LabelTotal;
        TListView *GridArquivos;
        TImageList *ativos;
        TRadioGroup *RadioOrdem;
        TLabel *Label2;
        TScrollBox *ScrollBox1;
        TMemo *Memo1;
        TToolBar *BarraPrincipal;
        TToolButton *BotaoAplicar;
        TToolButton *BotaoAdicionar;
        TToolButton *BotaoRelatorio;
        TToolButton *BotaoFechar;
        TToolButton *BotaoAjuda;
        TPanel *Panel1;
        TImageList *inativos;
        TImageList *EmFoco;
        TTabSheet *PgPrevia;
        TToolBar *ToolBar2;
        TToolButton *BotaoUP;
        TImageList *ListaPadraoImagens;
        TToolBar *ToolBar3;
        TToolButton *BotaoDOWN;
        TToolBar *ToolBar4;
        TToolButton *BotaoAPAGA;
        TStatusBar *BarraStatus;
        TToolButton *BotaoIncluirPastas;
        TOpenDialog *DialogoAbriArquivos;
        TToolBar *ToolBar1;
        TToolButton *BotaoLimpaTudo;
        TGroupBox *GrupoLocalizarSubstituir;
        TEdit *Subs_CaixaLocalizar;
        TEdit *Subs_CaixaPor;
        TLabel *Label3;
        TLabel *Label4;
        TCheckBox *Subs_CheckCoincidir;
        TRadioGroup *Subs_RadioOcorrencias;
        TButton *Subs_Aplicar;
        TLabel *Label5;
        TGroupBox *GrupoMudarExtensao;
        TButton *Ext_Aplicar;
        TButton *Maisculas_Aplicar;
        TEdit *Ext_CaixaExt;
        TLabel *Label6;
        TGroupBox *GrupoCaractereEspecifico;
        TGroupBox *GroupBox4;
        TStringGrid *GridPrevia;
        TLabel *Label7;
        TProgressBar *BarraProgresso;
        TLabel *Label8;
        TLabel *Label9;
        TEdit *Caract_Indice;
        TEdit *Caract_Por;
        TButton *Caract_Aplicar;
        TGroupBox *GrupoMaiusculas;
        TRadioGroup *Maiusculas_RadioOpcao;
        TGroupBox *GrupoLocalizarSubstituirCaractere;
        TButton *CaractLocalizarSubstituirAplicar;
        TLabel *Label10;
        TEdit *Caract_LocalizarSubstituirIndice;
        TLabel *Label11;
        TLabel *Label12;
        TRadioGroup *RadioGroup1;
        TButton *Ext_AplicarLocalizarSubstituir;
        TLabel *Label13;
        TEdit *Ext_Localizar;
        TLabel *Label14;
        TEdit *Ext_Por;
        TEdit *Caract_LocalizarSubstituirLocalizar;
        TLabel *Label15;
        TLabel *Label16;
        TEdit *Caract_LocalizarSubstituirPor;
        TRadioGroup *RadioEstilo;
        TGroupBox *GrupoIniciarEm;
        TGroupBox *GrupoPrefixo;
        TButton *BotaoPadraoAplicar;
        TEdit *CaixaPrefixo;
        TGroupBox *GrupoSufixo;
        TEdit *CaixaSufixo;
        TLabel *Label17;
        TLabel *Label18;
        TGroupBox *QuadroInserir;
        TRadioGroup *RadioPosicao;
        TGroupBox *GrupoPosicaoEspecifica;
        TEdit *CaixaPosicaoEspecifica;
        TLabel *Label21;
        TRadioGroup *RadioInserir;
        TGroupBox *GrupoTextoInserir;
        TEdit *CaixaTextoInserir;
        TButton *BotaoAplicarInserir;
        TMainMenu *MainMenu1;
        TMenuItem *Ajuda1;
        TMenuItem *Sobre1;
        TGroupBox *GrupoIncremento;
        TUpDown *BotaoAumentaDiminuiIncremento;
        TEdit *CaixaIncremento;
        TEdit *CaixaIniciarEm;
        TUpDown *BotaoAumentaDiminuiIniciarEm;
        TUpDown *BotaoAumentaDiminuiPosicaoEspecifica;
        TUpDown *UpDown1;
        TUpDown *UpDown2;
        TLabel *Label28;
        TLabel *Label29;
        TButton *BotaoDesfazer;
        TButton *BotaoAplicarAlteracoes;
        TButton *Button1;
        TTabSheet *TabSheet1;
        TGroupBox *GroupBox1;
        TLabel *LabelAjusteManual;
        TButton *BotaoAplicarMp3;
        TLabel *LabelNomeNovo;
        TGroupBox *GrupoPadrao;
        TEdit *CaixaPadrao;
        TRadioGroup *RadioRenomearInserir;
        TLabel *Label23;
        TLabel *Label24;
        TRadioGroup *GrupoCondicaoTags;
        TToolBar *BarraInsereTag;
        TToolButton *BotaoDropDownTag;
        TMainMenu *MenuTags;
        TMenuItem *Titulo1;
        TMenuItem *Teste1;
        TMenuItem *Arista1;
        TMenuItem *Genero1;
        TMenuItem *Ano1;
        TMenuItem *Comentario1;
        TMenuItem *Outro1;
        TMenuItem *Album1;
        TRadioGroup *RadioSeNaoExistir;
        TEdit *CaixaTextoBranco;
        TLabel *LabelNomeVelho;
        TMenuItem *N1;
        TTabSheet *PgEliminar;
        TGroupBox *GroupBox2;
        TLabel *Label27;
        TRadioGroup *RadioPosicaoFinal;
        TEdit *CaixaPosicaoFinal;
        TUpDown *UpDownInicial;
        TUpDown *UpDownFinal;
        TButton *AplicarEliminarSubsequencia;
        TRadioGroup *RadioEliminarEspecial;
        TEdit *CaixaPosicaoInicial;
        TRadioGroup *RadioPosicaoInicial;
        TEdit *CaractereInicial;
        TButton *AplicarEliminarEspecial;
        TEdit *CaractereFinal;
        TGroupBox *GroupBox3;
        TLabel *LabelImportante;
        TMenuItem *Numero1;
        TLabel *Label22;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label25;
        TTabSheet *PgExpressao;
        TGroupBox *GroupBox5;
        TGroupBox *GroupBox6;
        TEdit *CaixaExpressao;
        TGroupBox *GroupBox7;
        TEdit *CaixaSubstituirPor;
        TGroupBox *GroupBox8;
        TGroupBox *GroupBox9;
        TCheckBox *CheckApenasPrimeira;
        TCheckBox *CheckMenor;
        TCheckBox *CheckManter;
        TCheckBox *CheckCase;
        TButton *BotaoAplicarExpressaoRegular;
        TSpeedButton *BotaoAbrirExpressao;
        TSpeedButton *BotaoSalvarExpressao;
        TSpeedButton *BotaoCarregartemplates;
        RegularExpression *ER;
        TSaveDialog *DlgSalvarRegex;
        TOpenDialog *DlgAbrirRegex;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall GridArquivosColumnClick(TObject *Sender,
          TListColumn *Column);
        void __fastcall GridArquivosCompare(TObject *Sender,
          TListItem *Item1, TListItem *Item2, int Data, int &Compare);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall BotaoCancelarClick(TObject *Sender);
        void __fastcall RadioOrdemClick(TObject *Sender);
        void __fastcall BotaoUPClick(TObject *Sender);
        void __fastcall BotaoDOWNClick(TObject *Sender);
        void __fastcall BotaoAPAGAClick(TObject *Sender);
        void __fastcall BotaoIncluirPastasClick(TObject *Sender);
        void __fastcall BotaoAdicionarClick(TObject *Sender);
        void __fastcall BotaoFecharClick(TObject *Sender);
        void __fastcall BotaoLimpaTudoClick(TObject *Sender);
        void __fastcall Subs_AplicarClick(TObject *Sender);
        void __fastcall Ext_AplicarClick(TObject *Sender);
        void __fastcall Ext_AplicarLocalizarSubstituirClick(
          TObject *Sender);
        void __fastcall Sobre1Click(TObject *Sender);
        void __fastcall RadioPosicaoClick(TObject *Sender);
        void __fastcall RadioInserirClick(TObject *Sender);
        void __fastcall CaixaPosicaoEspecificaKeyPress(TObject *Sender,
          char &Key);
        void __fastcall CaixaIniciarEmKeyPress(TObject *Sender, char &Key);
        void __fastcall Caract_AplicarClick(TObject *Sender);
        void __fastcall CaractLocalizarSubstituirAplicarClick(
          TObject *Sender);
        void __fastcall Maisculas_AplicarClick(TObject *Sender);
        void __fastcall BotaoAplicarInserirClick(TObject *Sender);
        void __fastcall BotaoPadraoAplicarClick(TObject *Sender);
        void __fastcall Subs_CaixaLocalizarKeyPress(TObject *Sender,
          char &Key);
        void __fastcall Subs_CaixaPorKeyPress(TObject *Sender, char &Key);
        void __fastcall Ext_CaixaExtKeyPress(TObject *Sender, char &Key);
        void __fastcall Ext_LocalizarKeyPress(TObject *Sender, char &Key);
        void __fastcall Ext_PorKeyPress(TObject *Sender, char &Key);
        void __fastcall CaixaTextoInserirKeyPress(TObject *Sender,
          char &Key);
        void __fastcall CaixaPrefixoKeyPress(TObject *Sender, char &Key);
        void __fastcall CaixaSufixoKeyPress(TObject *Sender, char &Key);
        void __fastcall BotaoDesfazerClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall BotaoRelatorioClick(TObject *Sender);
        void __fastcall BotaoAplicarAlteracoesClick(TObject *Sender);
        void __fastcall GridPreviaKeyPress(TObject *Sender, char &Key);
        void __fastcall GridPreviaSetEditText(TObject *Sender, int ACol,
          int ARow, const AnsiString Value);
        void __fastcall GridPreviaSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall BotaoAplicarMp3Click(TObject *Sender);
        void __fastcall Teste1Click(TObject *Sender);
        void __fastcall Arista1Click(TObject *Sender);
        void __fastcall Genero1Click(TObject *Sender);
        void __fastcall Ano1Click(TObject *Sender);
        void __fastcall Comentario1Click(TObject *Sender);
        void __fastcall CaixaPadraoKeyPress(TObject *Sender, char &Key);
        void __fastcall Album1Click(TObject *Sender);
        void __fastcall GrupoCondicaoTagsClick(TObject *Sender);
        void __fastcall RadioSeNaoExistirClick(TObject *Sender);
        void __fastcall CaixaTextoBrancoKeyPress(TObject *Sender,
          char &Key);
        void __fastcall N1Click(TObject *Sender);
        void __fastcall RadioPosicaoInicialClick(TObject *Sender);
        void __fastcall RadioPosicaoFinalClick(TObject *Sender);
        void __fastcall AplicarEliminarEspecialClick(TObject *Sender);
        void __fastcall AplicarEliminarSubsequenciaClick(TObject *Sender);
        void __fastcall LabelImportanteMouseEnter(TObject *Sender);
        void __fastcall LabelImportanteMouseLeave(TObject *Sender);
        void __fastcall Numero1Click(TObject *Sender);
        void __fastcall BotaoCarregartemplatesClick(TObject *Sender);
        void __fastcall BotaoAplicarExpressaoRegularClick(TObject *Sender);
        void __fastcall CheckApenasPrimeiraClick(TObject *Sender);
        void __fastcall BotaoSalvarExpressaoClick(TObject *Sender);
        void __fastcall BotaoAbrirExpressaoClick(TObject *Sender);
private:
        void virtual __fastcall WMDropFiles(TWMDropFiles &message);

public:
        AnsiString NomeArq;   // arquivo passado como argumento
        int ColunaOrdenar; // coluna a ordenar o listview
        int ColunaAnterior; // guarda a cópia da variável acima, q
                           // é alterada na execução da ordenação
        int aleatorio[10]; // array de numeros aleatorios
        int pos_aleat;     // posição no array
        AnsiString *nomes; // tabela de recuperação(desfazer)
        AnsiString *extensoes;
        __fastcall TFormPrincipal(TComponent* Owner);
        // Busca Binária na TListView
        bool Busca_Bin(TListView* lista, AnsiString texto, int coluna, int tam);
        // Busca Sequencial numa TListView
        bool BuscaSequencial(TListView* lista, AnsiString texto, int coluna, int last);
        // diz se algum processo de renomeamento já foi aplicado à prévia
        bool iniciado;
        bool desfazercriado;
        bool Abrir_Previa();    // prepara a prévia
        void CriaRecuperacao();
        void habilita_botoes();
        TStringList* GetTokens(AnsiString texto); // cria opção 'desfazer'

        // Drag & drop handlers
        BEGIN_MESSAGE_MAP
        MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WMDropFiles)
        END_MESSAGE_MAP(TForm);


};
//---------------------------------------------------------------------------
extern PACKAGE TFormPrincipal *FormPrincipal;
//---------------------------------------------------------------------------
#endif
