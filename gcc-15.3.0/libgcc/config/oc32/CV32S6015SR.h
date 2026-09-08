//---------------------------------------------------------------;
// CV32S6015SR.h                                                 ;
// Header file for CV32S6015 SR                                  ;
//---------------------------------------------------------------;

#ifndef CV32S6015_SR_H
#define CV32S6015_SR_H

//---------------------------------------------------------------;
// CV32S6015 Register definition for ASM                         ;
//---------------------------------------------------------------;
// Internal Core Registers
//
#define PID 0x0000
#define CMODE 0x0004
// 				0x0008
// 				0x000C
// 				0x0010
// 				0x0014
// 				0x0018
// 				0x001C
// 				0x0020
// 				0x0024
// 				0x0028
// 				0x002C
// 				0x0030
// 				0x0034
// 				0x0038
// 				0x003C
#define HIE 0x0040
#define HIPRM 0x0044
#define HIPL0 0x0048
#define HIPL1 0x004C

//  External Peripheral Registers
//
#define HRF 0x8000
#define ALMCON 0x8004
#define BPCON 0x8008
#define DMACON 0x800C
#define VTSCON 0x8010
#define LVDCON 0x8014
#define CLKCON 0x8018
#define IOSCCON 0x801C
#define FLCCON 0x8020
#define FLUCON 0x8024
#define FLUADR 0x8028
#define FLUDATA0 0x802C
#define FLUDATA1 0x8030
#define FLUDATA2 0x8034
#define FLUDATA3 0x8038
#define FLURRDA 0x803C
#define PA 0x8040
#define PAOE 0x8044
#define PAIE 0x8048
#define PAPU 0x804C
#define PARIE 0x8050
#define PAFIE 0x8054
#define PAIF 0x8058
#define PAIFC 0x805C
#define PAFS0 0x8060
#define PAFS1 0x8064
#define PALS 0x8068
#define PAPE 0x806C
// 				0x8070
// 				0x8074
// 				0x8078
// 				0x807C
#define PB 0x8080
#define PBOE 0x8084
#define PBIE 0x8088
#define PBPU 0x808C
#define PBRIE 0x8090
#define PBFIE 0x8094
#define PBIF 0x8098
#define PBIFC 0x809C
#define PBFS0 0x80A0
#define PBFS1 0x80A4
#define PBLS 0x80A8
#define PBPE 0x80AC
// 				0x80B0
// 				0x80B4
// 				0x80B8
// 				0x80BC
// 				0x80C0
// 				0x80C4
// 				0x80C8
// 				0x80CC
// 				0x80D0
// 				0x80D4
// 				0x80D8
// 				0x80DC
// 				0x80E0
// 				0x80E4
// 				0x80E8
// 				0x80EC
// 				0x80F0
// 				0x80F4
// 				0x80F8
// 				0x80FC
// 				0x8100
// 				0x8104
// 				0x8108
// 				0x810C
// 				0x8110
// 				0x8114
// 				0x8118
// 				0x811C
// 				0x8120
// 				0x8124
// 				0x8128
// 				0x812C
// 				0x8130
// 				0x8134
// 				0x8138
// 				0x813C
#define ADC0CON 0x8140
#define ADC0E0CC 0x8144
#define ADC0E1CC 0x8148
#define ADC0E2CC 0x814C
#define ADC0E3CC 0x8150
#define ADC0E4CC 0x8154
#define ADC0E5CC 0x8158
#define ADC0E6CC 0x815C
#define ADC0E7CC 0x8160
#define ADC0DA 0x8164
#define ADC0DC0 0x8168
#define ADC0DC1 0x816C
#define ADC0WC0 0x8170
#define ADC0WW0 0x8174
#define ADC0WC1 0x8178
#define ADC0WW1 0x817C
#define ADC1CON 0x8180
#define ADC1E0CC 0x8184
#define ADC1E1CC 0x8188
#define ADC1E2CC 0x818C
#define ADC1E3CC 0x8190
#define ADC1E4CC 0x8194
#define ADC1E5CC 0x8198
#define ADC1E6CC 0x819C
#define ADC1E7CC 0x81A0
#define ADC1DA 0x81A4
#define ADC1DC0 0x81A8
#define ADC1DC1 0x81AC
#define ADC1WC0 0x81B0
#define ADC1WW0 0x81B4
#define ADC1WC1 0x81B8
#define ADC1WW1 0x81BC
#define CCP0CON 0x81C0
#define CCP0VAL 0x81C4
#define CCP0OUT 0x81C8
#define CCP0GAT 0x81CC
#define CCP1CON 0x81D0
#define CCP1VAL 0x81D4
#define CCP1OUT 0x81D8
#define CCP1GAT 0x81DC
#define CCP2CON 0x81E0
#define CCP2VAL 0x81E4
#define CCP2OUT 0x81E8
#define CCP2GAT 0x81EC
#define CCP3CON 0x81F0
#define CCP3VAL 0x81F4
#define CCP3OUT 0x81F8
#define CCP3GAT 0x81FC
#define CCP4CON 0x8200
#define CCP4VAL 0x8204
#define CCP4OUT 0x8208
#define CCP4GAT 0x820C
#define CCP5CON 0x8210
#define CCP5VAL 0x8214
#define CCP5OUT 0x8218
#define CCP5GAT 0x821C
#define MP0CON 0x8220
#define MP0DTY01 0x8224
#define MP0DTY23 0x8228
#define MP0DTY45 0x822C
#define MP0DTY67 0x8230
#define MP0LDA 0x8234
#define MP0LDC 0x8238
#define MP1CON 0x823C
#define MP1DTY01 0x8240
#define MP1DTY23 0x8244
#define MP1DTY45 0x8248
#define MP1DTY67 0x824C
#define MP1LDA 0x8250
#define MP1LDC 0x8254
#define MP2CON 0x8258
#define MP2DTY01 0x825C
#define MP2DTY23 0x8260
#define MP2DTY45 0x8264
#define MP2DTY67 0x8268
#define MP2LDA 0x826C
#define MP2LDC 0x8270
#define ABUFCON 0x8274
#define WDT0CON 0x8278
#define WDT1CON 0x827C
#define SPI0CON 0x8280
#define SPI0TBUF 0x8284
#define SPI0RBUF 0x8288
#define SPI0TDA 0x828C
#define SPI0RDA 0x8290
#define SPI0TBC 0x8294
// 				0x8298
// 				0x829C
#define SPI1CON 0x82A0
#define SPI1TBUF 0x82A4
#define SPI1RBUF 0x82A8
#define SPI1TDA 0x82AC
#define SPI1RDA 0x82B0
#define SPI1TBC 0x82B4
#define CMP0TW 0x82B8
#define CMP1TW 0x82BC
#define UT0CON 0x82C0
#define UT0TBUF 0x82C4
#define UT0RBUF 0x82C8
#define UT0TDA 0x82CC
#define UT0RDA 0x82D0
#define UT0TBC 0x82D4
#define UT0RBC 0x82D8
#define CMP2TW 0x82DC
#define UT1CON 0x82E0
#define UT1TBUF 0x82E4
#define UT1RBUF 0x82E8
#define UT1TDA 0x82EC
#define UT1RDA 0x82F0
#define UT1TBC 0x82F4
#define UT1RBC 0x82F8
#define PLLCON 0x82FC
#define IICCON 0x8300
#define IICTBUF 0x8304
#define IICRBUF 0x8308
#define IICADR 0x830C
#define IICDA 0x8310
#define IICBC 0x8314
#define CMP0CON 0x8318
#define CMP1CON 0x831C
#define CRCCON 0x8320
#define CRCPOLY 0x8324
#define CRCIN 0x8328
#define CRCREG 0x832C
#define CRCOUT 0x8330
#define CRCDA 0x8334
#define CRCBC 0x8338
#define AMPCON 0x833C
#define T0CON 0x8340
#define T0CNT 0x8344
#define T0ARL 0x8348
#define T0ECON 0x834C
#define T1CON 0x8350
#define T1CNT 0x8354
#define T1ARL 0x8358
#define T1ECON 0x835C
#define T2CON 0x8360
#define T2CNT 0x8364
#define T2ARL 0x8368
#define T2ECON 0x836C
#define T3CON 0x8370
#define T3CNT 0x8374
#define T3ARL 0x8378
#define T3ECON 0x837C
#define T4CON 0x8380
#define T4CNT 0x8384
#define T4ARL 0x8388
#define T4ECON 0x838C
#define T5CON 0x8390
#define T5CNT 0x8394
#define T5ARL 0x8398
#define T5ECON 0x839C
#define T6CON 0x83A0
#define T6CNT 0x83A4
#define T6ARL 0x83A8
#define T6ECON 0x83AC
#define T7CON 0x83B0
#define T7CNT 0x83B4
#define T7ARL 0x83B8
#define T7ECON 0x83BC
#define UBCON 0x83C0
#define UFCON 0x83C4
#define UE0CON 0x83C8
#define UE1CON 0x83CC
#define UE2CON 0x83D0
#define UE3CON 0x83D4
#define UE4CON 0x83D8
#define CMP2CON 0x83DC
#define DAC0CON 0x83E0
#define DAC0BUF 0x83E4
#define DAC0DA 0x83E8
#define DAC0DC 0x83EC
#define DAC1CON 0x83F0
#define DAC1BUF 0x83F4
#define DAC1DA 0x83F8
#define DAC1DC 0x83FC

//---------------------------------------------------------------;
// CV32S6015 Register bit definition for ASM                     ;
//---------------------------------------------------------------;
//-------------------
//	CMODE
#define UWSRP 0
#define HISE 1
#define SRME 2

//-------------------
//	HRF
#define VTSAFCLR 14
#define VTSAF 15
#define WDT0RFCLR 18
#define WDT0RF 19
#define WDT1RFCLR 20
#define WDT1RF 21
#define LVDRFCLR 22
#define LVDRF 23
#define DBGRFCLR 24
#define DBGRF 25
#define IERFCLR 26
#define IERF 27
#define PORFCLR 30
#define PORF 31

//-------------------
//	ALMCON
#define VTSARE 14

//-------------------
//	BPCON
#define HOLD 30
#define SLEEP 31

//-------------------
//	CLKCON
#define IOSCH 0
#define IOSCUTE 1
#define IOSCUTL 2
#define PLLE 4
#define PLLM 5
#define XHOSCE 6
#define CSF 7
#define FLMCG 15
#define CRCMCG 16
#define ADCMCG 18

//-------------------
//	FLUCON
#define FLUKS 15
#define FLUWIPC 20
#define FLUSC 21
#define FLUSS 22
#define FLURUCS 23
#define FLURRE 27
#define FLDCTP 28
#define FLUIE 29
#define FLUOCLR 30
#define FLUOPF 31

//-------------------
//	ADC0CON
#define ADC0E 0
#define ADC0DMADA 1
#define ADC0UCS 7
#define ADC0P0REE 17
#define ADC0P0FEE 18
#define ADC0P1REE 24
#define ADC0P1FEE 25
#define ADC0ESDLYP 27

//-------------------
//	ADC0E0CC
#define ADC0E0CCLR 30
#define ADC0E0CF 31

//-------------------
//	ADC0E1CC
#define ADC0E1CCLR 30
#define ADC0E1CF 31

//-------------------
//	ADC0E2CC
#define ADC0E2CCLR 30
#define ADC0E2CF 31

//-------------------
//	ADC0E3CC
#define ADC0E3CCLR 30
#define ADC0E3CF 31

//-------------------
//	ADC0E4CC
#define ADC0E4CCLR 30
#define ADC0E4CF 31

//-------------------
//	ADC0E5CC
#define ADC0E5CCLR 30
#define ADC0E5CF 31

//-------------------
//	ADC0E6CC
#define ADC0E6CCLR 30
#define ADC0E6CF 31

//-------------------
//	ADC0E7CC
#define ADC0E7CCLR 30
#define ADC0E7CF 31

//-------------------
//	ADC0DC0
#define ADC0E0DMAE 7
#define ADC0E1DMAE 15
#define ADC0E2DMAE 23
#define ADC0E3DMAE 31

//-------------------
//	ADC0DC1
#define ADC0E4DMAE 7
#define ADC0E5DMAE 15
#define ADC0E6DMAE 23
#define ADC0E7DMAE 31

//-------------------
//	ADC0WW0
#define ADC0E0IB 12
#define ADC0E1IB 13
#define ADC0E2IB 14
#define ADC0E3IB 15
#define ADC0E4IB 28
#define ADC0E5IB 29
#define ADC0E6IB 30
#define ADC0E7IB 31

//-------------------
//	ADC0WC0
#define ADC0W0E0E 0
#define ADC0W0E1E 1
#define ADC0W0E2E 2
#define ADC0W0E3E 3
#define ADC0W0E4E 4
#define ADC0W0E5E 5
#define ADC0W0E6E 6
#define ADC0W0E7E 7
#define ADC0E0OV 8
#define ADC0E1OV 9
#define ADC0E2OV 10
#define ADC0E3OV 11
#define ADC0E4OV 12
#define ADC0E5OV 13
#define ADC0E6OV 14
#define ADC0E7OV 15
#define ADC0W0E0CLR 16
#define ADC0W0E1CLR 18
#define ADC0W0E2CLR 20
#define ADC0W0E3CLR 22
#define ADC0W0E4CLR 24
#define ADC0W0E5CLR 26
#define ADC0W0E6CLR 28
#define ADC0W0E7CLR 30
#define ADC0W0E0F 17
#define ADC0W0E1F 19
#define ADC0W0E2F 21
#define ADC0W0E3F 23
#define ADC0W0E4F 25
#define ADC0W0E5F 27
#define ADC0W0E6F 29
#define ADC0W0E7F 31

//-------------------
//	ADC0WC1
#define ADC0W1E0E 0
#define ADC0W1E1E 1
#define ADC0W1E2E 2
#define ADC0W1E3E 3
#define ADC0W1E4E 4
#define ADC0W1E5E 5
#define ADC0W1E6E 6
#define ADC0W1E7E 7
#define ADC0E0IE 8
#define ADC0E1IE 9
#define ADC0E2IE 10
#define ADC0E3IE 11
#define ADC0E4IE 12
#define ADC0E5IE 13
#define ADC0E6IE 14
#define ADC0E7IE 15
#define ADC0W1E0CLR 16
#define ADC0W1E1CLR 18
#define ADC0W1E2CLR 20
#define ADC0W1E3CLR 22
#define ADC0W1E4CLR 24
#define ADC0W1E5CLR 26
#define ADC0W1E6CLR 28
#define ADC0W1E7CLR 30
#define ADC0W1E0F 17
#define ADC0W1E1F 19
#define ADC0W1E2F 21
#define ADC0W1E3F 23
#define ADC0W1E4F 25
#define ADC0W1E5F 27
#define ADC0W1E6F 29
#define ADC0W1E7F 31

//-------------------
//	ADC1CON
#define ADC1E 0
#define ADC1DMADA 1
#define ADC1UCS 7
#define ADC1P0REE 17
#define ADC1P0FEE 18
#define ADC1P1REE 24
#define ADC1P1FEE 25
#define ADC1ESDLYP 27

//-------------------
//	ADC1E0CC
#define ADC1E0CCLR 30
#define ADC1E0CF 31

//-------------------
//	ADC1E1CC
#define ADC1E1CCLR 30
#define ADC1E1CF 31

//-------------------
//	ADC1E2CC
#define ADC1E2CCLR 30
#define ADC1E2CF 31

//-------------------
//	ADC1E3CC
#define ADC1E3CCLR 30
#define ADC1E3CF 31

//-------------------
//	ADC1E4CC
#define ADC1E4CCLR 30
#define ADC1E4CF 31

//-------------------
//	ADC1E5CC
#define ADC1E5CCLR 30
#define ADC1E5CF 31

//-------------------
//	ADC1E6CC
#define ADC1E6CCLR 30
#define ADC1E6CF 31

//-------------------
//	ADC1E7CC
#define ADC1E7CCLR 30
#define ADC1E7CF 31

//-------------------
//	ADC1DC0
#define ADC1E0DMAE 7
#define ADC1E1DMAE 15
#define ADC1E2DMAE 23
#define ADC1E3DMAE 31

//-------------------
//	ADC1DC1
#define ADC1E4DMAE 7
#define ADC1E5DMAE 15
#define ADC1E6DMAE 23
#define ADC1E7DMAE 31

//-------------------
//	ADC1WC0
#define ADC1W0E0E 0
#define ADC1W0E1E 1
#define ADC1W0E2E 2
#define ADC1W0E3E 3
#define ADC1W0E4E 4
#define ADC1W0E5E 5
#define ADC1W0E6E 6
#define ADC1W0E7E 7
#define ADC1E0OV 8
#define ADC1E1OV 9
#define ADC1E2OV 10
#define ADC1E3OV 11
#define ADC1E4OV 12
#define ADC1E5OV 13
#define ADC1E6OV 14
#define ADC1E7OV 15
#define ADC1W0E0CLR 16
#define ADC1W0E1CLR 18
#define ADC1W0E2CLR 20
#define ADC1W0E3CLR 22
#define ADC1W0E4CLR 24
#define ADC1W0E5CLR 26
#define ADC1W0E6CLR 28
#define ADC1W0E7CLR 30
#define ADC1W0E0F 17
#define ADC1W0E1F 19
#define ADC1W0E2F 21
#define ADC1W0E3F 23
#define ADC1W0E4F 25
#define ADC1W0E5F 27
#define ADC1W0E6F 29
#define ADC1W0E7F 31

//-------------------
//	ADC1WC1
#define ADC1W1E0E 0
#define ADC1W1E1E 1
#define ADC1W1E2E 2
#define ADC1W1E3E 3
#define ADC1W1E4E 4
#define ADC1W1E5E 5
#define ADC1W1E6E 6
#define ADC1W1E7E 7
#define ADC1E0IE 8
#define ADC1E1IE 9
#define ADC1E2IE 10
#define ADC1E3IE 11
#define ADC1E4IE 12
#define ADC1E5IE 13
#define ADC1E6IE 14
#define ADC1E7IE 15
#define ADC1W1E0CLR 16
#define ADC1W1E1CLR 18
#define ADC1W1E2CLR 20
#define ADC1W1E3CLR 22
#define ADC1W1E4CLR 24
#define ADC1W1E5CLR 26
#define ADC1W1E6CLR 28
#define ADC1W1E7CLR 30
#define ADC1W1E0F 17
#define ADC1W1E1F 19
#define ADC1W1E2F 21
#define ADC1W1E3F 23
#define ADC1W1E4F 25
#define ADC1W1E5F 27
#define ADC1W1E6F 29
#define ADC1W1E7F 31

//-------------------
//	VTSCON
#define VTSE 0

//-------------------
//	LVDCON
#define LVDE 0
#define LVDRE 1
#define LVDWE 2
#define LVDIE 3
#define LVDECLR 30
#define LVDEF 31

//-------------------
//	CMP0CON
#define CMP0S 0
#define CMP0E 1
#define CMP0RFE 6
#define CMP0FFE 7
#define CMP0OE 11
#define CMP0IE 15
#define CMP0ECLR 30
#define CMP0EF 31

//-------------------
//	CMP1CON
#define CMP1S 0
#define CMP1E 1
#define CMP1RFE 6
#define CMP1FFE 7
#define CMP1OE 11
#define CMP1IE 15
#define CMP1ECLR 30
#define CMP1EF 31

//-------------------
//	CMP2CON
#define CMP2S 0
#define CMP2E 1
#define CMP2RFE 6
#define CMP2FFE 7
#define CMP2OE 11
#define CMP2IE 15
#define CMP2ECLR 30
#define CMP2EF 31

//-------------------
//	AMPCON
#define AMP0E 3
#define AMP1E 11
#define AMP2E 19
#define AMP3E 27

//-------------------
//	CCP0CON
#define CCP0E 0
#define CCP0UT 1
#define CCP0VFE 11
#define CCP0IE 15
#define CCP0FC0L 20
#define CCP0FC1L 21
#define CCP0FC0E 22
#define CCP0FC1E 23
#define CCP0FC 24
#define CCP0COV 25
#define CCP0FCLR 26
#define CCP0FF 27
#define CCP0GCLR 28
#define CCP0GF 29
#define CCP0CCLR 30
#define CCP0CF 31

//-------------------
//	CCP0OUT
#define CCP0POE 0
#define CCP0NOE 1
#define CCP0FPO 2
#define CCP0FNO 3
#define CCP0PIO 4
#define CCP0NIO 5
#define CCP0GUE 10
#define CCP0GDE 11
#define CCP0GINV 12
#define CCP0CFM 13
#define CCP0SBR 19
#define CCP0GFM 22

//-------------------
//	CCP1CON
#define CCP1E 0
#define CCP1UT 1
#define CCP1VFE 11
#define CCP1IE 15
#define CCP1FC0L 20
#define CCP1FC1L 21
#define CCP1FC0E 22
#define CCP1FC1E 23
#define CCP1FC 24
#define CCP1COV 25
#define CCP1FCLR 26
#define CCP1FF 27
#define CCP1GCLR 28
#define CCP1GF 29
#define CCP1CCLR 30
#define CCP1CF 31

//-------------------
//	CCP1OUT
#define CCP1POE 0
#define CCP1NOE 1
#define CCP1FPO 2
#define CCP1FNO 3
#define CCP1PIO 4
#define CCP1NIO 5
#define CCP1GUE 10
#define CCP1GDE 11
#define CCP1GINV 12
#define CCP1CFM 13
#define CCP1SBR 19
#define CCP1GFM 22

//-------------------
//	CCP2CON
#define CCP2E 0
#define CCP2UT 1
#define CCP2VFE 11
#define CCP2IE 15
#define CCP2FC0L 20
#define CCP2FC1L 21
#define CCP2FC0E 22
#define CCP2FC1E 23
#define CCP2FC 24
#define CCP2COV 25
#define CCP2FCLR 26
#define CCP2FF 27
#define CCP2GCLR 28
#define CCP2GF 29
#define CCP2CCLR 30
#define CCP2CF 31

//-------------------
//	CCP2OUT
#define CCP2POE 0
#define CCP2NOE 1
#define CCP2FPO 2
#define CCP2FNO 3
#define CCP2PIO 4
#define CCP2NIO 5
#define CCP2GUE 10
#define CCP2GDE 11
#define CCP2GINV 12
#define CCP2CFM 13
#define CCP2SBR 19
#define CCP2GFM 22

//-------------------
//	CCP3CON
#define CCP3E 0
#define CCP3UT 1
#define CCP3VFE 11
#define CCP3IE 15
#define CCP3FC0L 20
#define CCP3FC1L 21
#define CCP3FC0E 22
#define CCP3FC1E 23
#define CCP3FC 24
#define CCP3COV 25
#define CCP3FCLR 26
#define CCP3FF 27
#define CCP3GCLR 28
#define CCP3GF 29
#define CCP3CCLR 30
#define CCP3CF 31

//-------------------
//	CCP3OUT
#define CCP3POE 0
#define CCP3NOE 1
#define CCP3FPO 2
#define CCP3FNO 3
#define CCP3PIO 4
#define CCP3NIO 5
#define CCP3GUE 10
#define CCP3GDE 11
#define CCP3GINV 12
#define CCP3CFM 13
#define CCP3SBR 19
#define CCP3GFM 22

//-------------------
//	CCP4CON
#define CCP4E 0
#define CCP4UT 1
#define CCP4VFE 11
#define CCP4IE 15
#define CCP4FC0L 20
#define CCP4FC1L 21
#define CCP4FC0E 22
#define CCP4FC1E 23
#define CCP4FC 24
#define CCP4COV 25
#define CCP4FCLR 26
#define CCP4FF 27
#define CCP4GCLR 28
#define CCP4GF 29
#define CCP4CCLR 30
#define CCP4CF 31

//-------------------
//	CCP4OUT
#define CCP4POE 0
#define CCP4NOE 1
#define CCP4FPO 2
#define CCP4FNO 3
#define CCP4PIO 4
#define CCP4NIO 5
#define CCP4GUE 10
#define CCP4GDE 11
#define CCP4GINV 12
#define CCP4CFM 13
#define CCP4SBR 19
#define CCP4GFM 22

//-------------------
//	CCP5CON
#define CCP5E 0
#define CCP5UT 1
#define CCP5VFE 11
#define CCP5IE 15
#define CCP5FC0L 20
#define CCP5FC1L 21
#define CCP5FC0E 22
#define CCP5FC1E 23
#define CCP5FC 24
#define CCP5COV 25
#define CCP5FCLR 26
#define CCP5FF 27
#define CCP5GCLR 28
#define CCP5GF 29
#define CCP5CCLR 30
#define CCP5CF 31

//-------------------
//	CCP5OUT
#define CCP5POE 0
#define CCP5NOE 1
#define CCP5FPO 2
#define CCP5FNO 3
#define CCP5PIO 4
#define CCP5NIO 5
#define CCP5GUE 10
#define CCP5GDE 11
#define CCP5GINV 12
#define CCP5CFM 13
#define CCP5SBR 19
#define CCP5GFM 22

//-------------------
//	MP0CON
#define MP0W0E 0
#define MP0W1E 1
#define MP0W2E 2
#define MP0W3E 3
#define MP0W4E 4
#define MP0W5E 5
#define MP0W6E 6
#define MP0W7E 7
#define MP0CO 11
#define MP0LSM 12
#define MP0LSKS 13
#define MP0IE 14
#define MP0W0CLR 16
#define MP0W0F 17
#define MP0W1CLR 18
#define MP0W1F 19
#define MP0W2CLR 20
#define MP0W2F 21
#define MP0W3CLR 22
#define MP0W3F 23
#define MP0W4CLR 24
#define MP0W4F 25
#define MP0W5CLR 26
#define MP0W5F 27
#define MP0W6CLR 28
#define MP0W6F 29
#define MP0W7CLR 30
#define MP0W7F 31

//-------------------
//	MP1CON
#define MP1W0E 0
#define MP1W1E 1
#define MP1W2E 2
#define MP1W3E 3
#define MP1W4E 4
#define MP1W5E 5
#define MP1W6E 6
#define MP1W7E 7
#define MP1CO 11
#define MP1LSM 12
#define MP1LSKS 13
#define MP1IE 14
#define MP1W0CLR 16
#define MP1W0F 17
#define MP1W1CLR 18
#define MP1W1F 19
#define MP1W2CLR 20
#define MP1W2F 21
#define MP1W3CLR 22
#define MP1W3F 23
#define MP1W4CLR 24
#define MP1W4F 25
#define MP1W5CLR 26
#define MP1W5F 27
#define MP1W6CLR 28
#define MP1W6F 29
#define MP1W7CLR 30
#define MP1W7F 31

//-------------------
//	MP2CON
#define MP2W0E 0
#define MP2W1E 1
#define MP2W2E 2
#define MP2W3E 3
#define MP2W4E 4
#define MP2W5E 5
#define MP2W6E 6
#define MP2W7E 7
#define MP2CO 11
#define MP2LSM 12
#define MP2LSKS 13
#define MP2IE 14
#define MP2W0CLR 16
#define MP2W0F 17
#define MP2W1CLR 18
#define MP2W1F 19
#define MP2W2CLR 20
#define MP2W2F 21
#define MP2W3CLR 22
#define MP2W3F 23
#define MP2W4CLR 24
#define MP2W4F 25
#define MP2W5CLR 26
#define MP2W5F 27
#define MP2W6CLR 28
#define MP2W6F 29
#define MP2W7CLR 30
#define MP2W7F 31

//-------------------
//	ABUFCON
#define ABUFE 5

//-------------------
//	WDT0CON
#define WDT0E 0
#define WDT0RE 1
#define WDT0WE 2
#define WDT0IE 3
#define WDT0TO 31

//-------------------
//	WDT1CON
#define WDT1E 0
#define WDT1RE 1
#define WDT1WE 2
#define WDT1IE 3
#define WDT1TO 31

//-------------------
//	SPI0CON
#define SPI0E 0
#define SPI0KS 1
#define SPI0CIH 2
#define SPI0DSP 3
#define SPI0S 8
#define SPI0HD 9
#define SPI0HDR 10
#define SPI0LF 11
#define SPI0DMAE 12
#define SPI0MSOE 13
#define SPI0SIOP 14
#define SPI0PORTS 15
#define SPI0IE 17
#define SPI0TCLR 30
#define SPI0TBV 31

//-------------------
//	SPI1CON
#define SPI1E 0
#define SPI1KS 1
#define SPI1CIH 2
#define SPI1DSP 3
#define SPI1S 8
#define SPI1HD 9
#define SPI1HDR 10
#define SPI1LF 11
#define SPI1DMAE 12
#define SPI1MSOE 13
#define SPI1SIOP 14
#define SPI1PORTS 15
#define SPI1IE 17
#define SPI1TCLR 30
#define SPI1TBV 31

//-------------------
//	UT0CON
#define UT0E 0
#define UT0TKS 1
#define UT0RE 7
#define UT0HD 20
#define UT0TDMAE 21
#define UT0RDMAE 22
#define UT0PORTS 23
#define UT0IE 24
#define UT0RERR 27
#define UT0TCLR 28
#define UT0TBV 29
#define UT0RCLR 30
#define UT0RBV 31

//-------------------
//	UT1CON
#define UT1E 0
#define UT1TKS 1
#define UT1RE 7
#define UT1HD 20
#define UT1TDMAE 21
#define UT1RDMAE 22
#define UT1PORTS 23
#define UT1IE 24
#define UT1RERR 27
#define UT1TCLR 28
#define UT1TBV 29
#define UT1RCLR 30
#define UT1RBV 31

//-------------------
//	PLLCON
#define PFOM 29
#define PFOCLR 30
#define PFO 31

//-------------------
//	IICCON
#define IICE 0
#define IICKS 1
#define IICS 2
#define IICTX 3
#define IICHE 6
#define IICSAM 7
#define IICSCL 16
#define IICDMAE 17
#define IICPORTS 18
#define IICIE 19
#define IICBSTA 20
#define IICSLE 21
#define IICBSTART 22
#define IICBSTOP 23
#define IICCCLR 24
#define IICCLTO 25
#define IICHCLR 26
#define IICHNACK 27
#define IICTCLR 28
#define IICTBV 29
#define IICRCLR 30
#define IICRBV 31

//-------------------
//	CRCCON
#define CRCREFI 5
#define CRCREFO 6
#define CRCCPLO 7
#define CRCKS 12
#define CRCIE 13
#define CRCRVB 14
#define CRCCCLR 30
#define CRCCF 31

//-------------------
//	T0CON
#define T0E 0
#define T0UT 1
#define T0OIE 11
#define T0RS 18
#define T0RGE 19
#define T0ENCP 26
#define T0CD 29
#define T0OCLR 30
#define T0OV 31

//-------------------
//	T0ECON
#define T0ETC 12
#define T0ET 13
#define T0EPC 19
#define T0PS 20
#define T0PGE 21
#define T0SBR 24
#define T0EIE 25
#define T0ECLR 30
#define T0EF 31

//-------------------
//	T1CON
#define T1E 0
#define T1UT 1
#define T1OIE 11
#define T1RS 18
#define T1RGE 19
#define T1ENCP 26
#define T1CD 29
#define T1OCLR 30
#define T1OV 31

//-------------------
//	T1ECON
#define T1ETC 12
#define T1ET 13
#define T1EPC 19
#define T1PS 20
#define T1PGE 21
#define T1SBR 24
#define T1EIE 25
#define T1ECLR 30
#define T1EF 31

//-------------------
//	T2CON
#define T2E 0
#define T2UT 1
#define T2OIE 11
#define T2RS 18
#define T2RGE 19
#define T2ENCP 26
#define T2CD 29
#define T2OCLR 30
#define T2OV 31

//-------------------
//	T2ECON
#define T2ETC 12
#define T2ET 13
#define T2EPC 19
#define T2PS 20
#define T2PGE 21
#define T2SBR 24
#define T2EIE 25
#define T2ECLR 30
#define T2EF 31

//-------------------
//	T3CON
#define T3E 0
#define T3UT 1
#define T3OIE 11
#define T3RS 18
#define T3RGE 19
#define T3ENCP 26
#define T3CD 29
#define T3OCLR 30
#define T3OV 31

//-------------------
//	T3ECON
#define T3ETC 12
#define T3ET 13
#define T3EPC 19
#define T3PS 20
#define T3PGE 21
#define T3SBR 24
#define T3EIE 25
#define T3ECLR 30
#define T3EF 31

//-------------------
//	T4CON
#define T4E 0
#define T4UT 1
#define T4OIE 11
#define T4RS 18
#define T4RGE 19
#define T4ENCP 26
#define T4CD 29
#define T4OCLR 30
#define T4OV 31

//-------------------
//	T4ECON
#define T4ETC 12
#define T4ET 13
#define T4EPC 19
#define T4PS 20
#define T4PGE 21
#define T4SBR 24
#define T4EIE 25
#define T4ECLR 30
#define T4EF 31

//-------------------
//	T5CON
#define T5E 0
#define T5UT 1
#define T5OIE 11
#define T5RS 18
#define T5RGE 19
#define T5ENCP 26
#define T5CD 29
#define T5OCLR 30
#define T5OV 31

//-------------------
//	T5ECON
#define T5ETC 12
#define T5ET 13
#define T5EPC 19
#define T5PS 20
#define T5PGE 21
#define T5SBR 24
#define T5EIE 25
#define T5ECLR 30
#define T5EF 31

//-------------------
//	T6CON
#define T6E 0
#define T6UT 1
#define T6OIE 11
#define T6RS 18
#define T6RGE 19
#define T6ENCP 26
#define T6CD 29
#define T6OCLR 30
#define T6OV 31

//-------------------
//	T6ECON
#define T6ETC 12
#define T6ET 13
#define T6EPC 19
#define T6PS 20
#define T6PGE 21
#define T6SBR 24
#define T6EIE 25
#define T6ECLR 30
#define T6EF 31

//-------------------
//	T7CON
#define T7E 0
#define T7UT 1
#define T7OIE 11
#define T7RS 18
#define T7RGE 19
#define T7ENCP 26
#define T7CD 29
#define T7OCLR 30
#define T7OV 31

//-------------------
//	T7ECON
#define T7ETC 12
#define T7ET 13
#define T7EPC 19
#define T7PS 20
#define T7PGE 21
#define T7SBR 24
#define T7EIE 25
#define T7ECLR 30
#define T7EF 31

//-------------------
//	UBCON
#define UE 0
#define USRSM 1
#define UFS 2
#define ULT 4
#define UDPPU 6
#define UDNPU 7
#define UXM 16
#define UIE 17
#define URSBF 18
#define UBRSM 27
#define UBSPD 29
#define UBCLR 30
#define UBRST 31

//-------------------
//	UFCON
#define USOFE 16
#define UMISS 29
#define USCLR 30
#define USOF 31

//-------------------
//	UE0CON
#define UE0DOS 16
#define UE0DIS 17
#define UE0SOS 18
#define UE0SIS 19
#define UE0STALL 20
#define ULTS 21
#define UE0CERRH 22
#define UE0CTP 23
#define UE0NCLR 24
#define UE0INACK 25
#define UE0CCLR 26
#define UE0CERR 27
#define UE0OCLR 28
#define UE0OPF 29
#define UE0ICLR 30
#define UE0IPF 31

//-------------------
//	UE1CON
#define UE1OS 16
#define UE1IS 17
#define UE1ODATA0 18
#define UE1IDATA0 19
#define UE1OSTALL 20
#define UE1ISTALL 21
#define UE1CERRH 22
#define UE1NOHS 23
#define UE1NCLR 24
#define UE1INACK 25
#define UE1CCLR 26
#define UE1CERR 27
#define UE1OCLR 28
#define UE1OPF 29
#define UE1ICLR 30
#define UE1IPF 31

//-------------------
//	UE2CON
#define UE2OS 16
#define UE2IS 17
#define UE2ODATA0 18
#define UE2IDATA0 19
#define UE2OSTALL 20
#define UE2ISTALL 21
#define UE2CERRH 22
#define UE2NOHS 23
#define UE2NCLR 24
#define UE2INACK 25
#define UE2CCLR 26
#define UE2CERR 27
#define UE2OCLR 28
#define UE2OPF 29
#define UE2ICLR 30
#define UE2IPF 31

//-------------------
//	UE3CON
#define UE3OS 16
#define UE3IS 17
#define UE3ODATA0 18
#define UE3IDATA0 19
#define UE3OSTALL 20
#define UE3ISTALL 21
#define UE3CERRH 22
#define UE3NOHS 23
#define UE3NCLR 24
#define UE3INACK 25
#define UE3CCLR 26
#define UE3CERR 27
#define UE3OCLR 28
#define UE3OPF 29
#define UE3ICLR 30
#define UE3IPF 31

//-------------------
//	UE4CON
#define UE4OS 16
#define UE4IS 17
#define UE4ODATA0 18
#define UE4IDATA0 19
#define UE4OSTALL 20
#define UE4ISTALL 21
#define UE4CERRH 22
#define UE4NOHS 23
#define UE4NCLR 24
#define UE4INACK 25
#define UE4CCLR 26
#define UE4CERR 27
#define UE4OCLR 28
#define UE4OPF 29
#define UE4ICLR 30
#define UE4IPF 31

//-------------------
//	DAC0CON
#define DAC0E 16
#define DAC0IE 17
#define DAC0DMAE 18
#define DAC0DMAL 19
#define DAC0CS 20
#define DAC0IM 21
#define DAC0TE 22
#define DAC0CCLR 30
#define DAC0CF 31

//-------------------
//	DAC1CON
#define DAC1E 16
#define DAC1IE 17
#define DAC1DMAE 18
#define DAC1DMAL 19
#define DAC1CS 20
#define DAC1IM 21
#define DAC1TE 22
#define DAC1CCLR 30
#define DAC1CF 31

#endif // CV32S6015_SR_H
