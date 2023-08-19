object FormListagem: TFormListagem
  Left = 192
  Top = 127
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsDialog
  Caption = 'Listagem'
  ClientHeight = 420
  ClientWidth = 640
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 625
    Height = 401
    TabOrder = 0
    object BotaoOk: TButton
      Left = 486
      Top = 352
      Width = 75
      Height = 25
      Caption = '&Ok'
      TabOrder = 0
      OnClick = BotaoOkClick
    end
    object Memo: TMemo
      Left = 8
      Top = 8
      Width = 601
      Height = 321
      ScrollBars = ssVertical
      TabOrder = 1
    end
    object BotaoSalvarListagem: TButton
      Left = 368
      Top = 352
      Width = 91
      Height = 25
      Caption = '&Salvar...'
      TabOrder = 2
      OnClick = BotaoSalvarListagemClick
    end
  end
end
