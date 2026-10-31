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
// CV32S6015 Register bit definition                             ;
//---------------------------------------------------------------;

//---------------------------------------------------------------;
// CV32S6015 Register bit definition                             ;
//---------------------------------------------------------------;
#define ENABLE          1
#define DISABLE         0
#define HIGH            1
#define LOW             0


//-------------------
//    CMODE
#define UWSRP_POS       0U
#define HISE_POS        1U
#define SRME_POS        2U
#define SFBR_POS        3U
#define HIAS_POS        4U
#define UWSRP           (0x01U << UWSRP_POS)
#define HISE            (0x01U << HISE_POS)
#define SRME            (0x01U << SRME_POS)
#define SFBR            (0x01U << SFBR_POS)
#define HIAS            (0x0FU << HIAS_POS)

//-------------------
//    HIE
#define SERRHIE_POS     0U
#define CMP0HIE_POS     1U
#define CMP1HIE_POS     2U
#define CCP0HIE_POS     3U
#define CCP1HIE_POS     4U
#define CCP2HIE_POS     5U
#define CCP3HIE_POS     6U
#define CCP4HIE_POS     7U
#define CCP5HIE_POS     8U
#define T0HIE_POS       9U
#define T1HIE_POS       10U
#define T2HIE_POS       11U
#define T3HIE_POS       12U
#define T4HIE_POS       13U
#define T5HIE_POS       14U
#define T6HIE_POS       15U
#define T7HIE_POS       16U
#define WDTHIE_POS      17U
#define ADC0HIE_POS     18U
#define ADC1HIE_POS     19U
#define UT0HIE_POS      20U
#define UT1HIE_POS      21U
#define SPI0HIE_POS     22U
#define SPI1HIE_POS     23U
#define IICHIE_POS      24U
#define MPWM0HIE_POS    25U
#define MPWM1HIE_POS    26U
#define MPWM2HIE_POS    27U
#define DACHIE_POS      28U
#define FLCRCHIE_POS    29U
#define PAHIE_POS       30U
#define PBHIE_POS       31U
#define SERRHIE         (0x01U << SERRHIE_POS)
#define CMP0HIE         (0x01U << CMP0HIE_POS)
#define CMP1HIE         (0x01U << CMP1HIE_POS)
#define CCP0HIE         (0x01U << CCP0HIE_POS)
#define CCP1HIE         (0x01U << CCP1HIE_POS)
#define CCP2HIE         (0x01U << CCP2HIE_POS)
#define CCP3HIE         (0x01U << CCP3HIE_POS)
#define CCP4HIE         (0x01U << CCP4HIE_POS)
#define CCP5HIE         (0x01U << CCP5HIE_POS)
#define T0HIE           (0x01U << T0HIE_POS)
#define T1HIE           (0x01U << T1HIE_POS)
#define T2HIE           (0x01U << T2HIE_POS)
#define T3HIE           (0x01U << T3HIE_POS)
#define T4HIE           (0x01U << T4HIE_POS)
#define T5HIE           (0x01U << T5HIE_POS)
#define T6HIE           (0x01U << T6HIE_POS)
#define T7HIE           (0x01U << T7HIE_POS)
#define WDTHIE          (0x01U << WDTHIE_POS)
#define ADC0HIE         (0x01U << ADC0HIE_POS)
#define ADC1HIE         (0x01U << ADC1HIE_POS)
#define UT0HIE          (0x01U << UT0HIE_POS)
#define UT1HIE          (0x01U << UT1HIE_POS)
#define SPI0HIE         (0x01U << SPI0HIE_POS)
#define SPI1HIE         (0x01U << SPI1HIE_POS)
#define IICHIE          (0x01U << IICHIE_POS)
#define MPWM0HIE        (0x01U << MPWM0HIE_POS)
#define MPWM1HIE        (0x01U << MPWM1HIE_POS)
#define MPWM2HIE        (0x01U << MPWM2HIE_POS)
#define DACHIE          (0x01U << DACHIE_POS)
#define FLCRCHIE        (0x01U << FLCRCHIE_POS)
#define PAHIE           (0x01U << PAHIE_POS)
#define PBHIE           (0x01U << PBHIE_POS)

//-------------------
//    HIPRM
#define SERRHIPRM_POS     0U
#define CMP0HIPRM_POS     1U
#define CMP1HIPRM_POS     2U
#define CCP0HIPRM_POS     3U
#define CCP1HIPRM_POS     4U
#define CCP2HIPRM_POS     5U
#define CCP3HIPRM_POS     6U
#define CCP4HIPRM_POS     7U
#define CCP5HIPRM_POS     8U
#define T0HIPRM_POS       9U
#define T1HIPRM_POS       10U
#define T2HIPRM_POS       11U
#define T3HIPRM_POS       12U
#define T4HIPRM_POS       13U
#define T5HIPRM_POS       14U
#define T6HIPRM_POS       15U
#define T7HIPRM_POS       16U
#define WDTHIPRM_POS      17U
#define ADC0HIPRM_POS     18U
#define ADC1HIPRM_POS     19U
#define UT0HIPRM_POS      20U
#define UT1HIPRM_POS      21U
#define SPI0HIPRM_POS     22U
#define SPI1HIPRM_POS     23U
#define IICHIPRM_POS      24U
#define MPWM0HIPRM_POS    25U
#define MPWM1HIPRM_POS    26U
#define MPWM2HIPRM_POS    27U
#define DACHIPRM_POS      28U
#define FLCRCHIPRM_POS    29U
#define PAHIPRM_POS       30U
#define PBHIPRM_POS       31U
#define SERRHIPRM         (0x01U << SERRHIPRM_POS)
#define CMP0HIPRM         (0x01U << CMP0HIPRM_POS)
#define CMP1HIPRM         (0x01U << CMP1HIPRM_POS)
#define CCP0HIPRM         (0x01U << CCP0HIPRM_POS)
#define CCP1HIPRM         (0x01U << CCP1HIPRM_POS)
#define CCP2HIPRM         (0x01U << CCP2HIPRM_POS)
#define CCP3HIPRM         (0x01U << CCP3HIPRM_POS)
#define CCP4HIPRM         (0x01U << CCP4HIPRM_POS)
#define CCP5HIPRM         (0x01U << CCP5HIPRM_POS)
#define T0HIPRM           (0x01U << T0HIPRM_POS)
#define T1HIPRM           (0x01U << T1HIPRM_POS)
#define T2HIPRM           (0x01U << T2HIPRM_POS)
#define T3HIPRM           (0x01U << T3HIPRM_POS)
#define T4HIPRM           (0x01U << T4HIPRM_POS)
#define T5HIPRM           (0x01U << T5HIPRM_POS)
#define T6HIPRM           (0x01U << T6HIPRM_POS)
#define T7HIPRM           (0x01U << T7HIPRM_POS)
#define WDTHIPRM          (0x01U << WDTHIPRM_POS)
#define ADC0HIPRM         (0x01U << ADC0HIPRM_POS)
#define ADC1HIPRM         (0x01U << ADC1HIPRM_POS)
#define UT0HIPRM          (0x01U << UT0HIPRM_POS)
#define UT1HIPRM          (0x01U << UT1HIPRM_POS)
#define SPI0HIPRM         (0x01U << SPI0HIPRM_POS)
#define SPI1HIPRM         (0x01U << SPI1HIPRM_POS)
#define IICHIPRM          (0x01U << IICHIPRM_POS)
#define MPWM0HIPRM        (0x01U << MPWM0HIPRM_POS)
#define MPWM1HIPRM        (0x01U << MPWM1HIPRM_POS)
#define MPWM2HIPRM        (0x01U << MPWM2HIPRM_POS)
#define DACHIPRM          (0x01U << DACHIPRM_POS)
#define FLCRCHIPRM        (0x01U << FLCRCHIPRM_POS)
#define PAHIPRM           (0x01U << PAHIPRM_POS)
#define PBHIPRM           (0x01U << PBHIPRM_POS)

//-------------------
//    HIPL0
#define LVDHIPL0_POS    0U
#define HRFHIPL0_POS    0U
#define PLLHIPL0_POS    0U
#define CMP0HIPL0_POS   1U
#define CMP1HIPL0_POS   2U
#define CCP0HIPL0_POS   3U
#define CCP1HIPL0_POS   4U
#define CCP2HIPL0_POS   5U
#define CCP3HIPL0_POS   6U
#define CCP4HIPL0_POS   7U
#define CCP5HIPL0_POS   8U
#define T0HIPL0_POS     9U
#define T1HIPL0_POS     10U
#define T2HIPL0_POS     11U
#define T3HIPL0_POS     12U
#define T4HIPL0_POS     13U
#define T5HIPL0_POS     14U
#define T6HIPL0_POS     15U
#define T7HIPL0_POS     16U
#define WDTHIPL0_POS    17U
#define ADC0HIPL0_POS   18U
#define ADC1HIPL0_POS   19U
#define UT0HIPL0_POS    20U
#define UT1HIPL0_POS    21U
#define SPI0HIPL0_POS   22U
#define SPI1HIPL0_POS   23U
#define IICHIPL0_POS    24U
#define MPWM0HIPL0_POS  25U
#define MPWM1HIPL0_POS  26U
#define MPWM2HIPL0_POS  27U
#define DACHIPL0_POS    28U
#define FLHIPL0_POS     29U
#define CRCHIPL0_POS    29U
#define PAHIPL0_POS     30U
#define PBHIPL0_POS     31U
#define LVDHIPL0        (0x01U << LVDHIPL0_POS)
#define HRFHIPL0        (0x01U << HRFHIPL0_POS)
#define PLLHIPL0        (0x01U << PLLHIPL0_POS)
#define CMP0HIPL0       (0x01U << CMP0HIPL0_POS)
#define CMP1HIPL0       (0x01U << CMP1HIPL0_POS)
#define CCP0HIPL0       (0x01U << CCP0HIPL0_POS)
#define CCP1HIPL0       (0x01U << CCP1HIPL0_POS)
#define CCP2HIPL0       (0x01U << CCP2HIPL0_POS)
#define CCP3HIPL0       (0x01U << CCP3HIPL0_POS)
#define CCP4HIPL0       (0x01U << CCP4HIPL0_POS)
#define CCP5HIPL0       (0x01U << CCP5HIPL0_POS)
#define T0HIPL0         (0x01U << T0HIPL0_POS)
#define T1HIPL0         (0x01U << T1HIPL0_POS)
#define T2HIPL0         (0x01U << T2HIPL0_POS)
#define T3HIPL0         (0x01U << T3HIPL0_POS)
#define T4HIPL0         (0x01U << T4HIPL0_POS)
#define T5HIPL0         (0x01U << T5HIPL0_POS)
#define T6HIPL0         (0x01U << T6HIPL0_POS)
#define T7HIPL0         (0x01U << T7HIPL0_POS)
#define WDTHIPL0        (0x01U << WDTHIPL0_POS)
#define ADC0HIPL0       (0x01U << ADC0HIPL0_POS)
#define ADC1HIPL0       (0x01U << ADC1HIPL0_POS)
#define UT0HIPL0        (0x01U << UT0HIPL0_POS)
#define UT1HIPL0        (0x01U << UT1HIPL0_POS)
#define SPI0HIPL0       (0x01U << SPI0HIPL0_POS)
#define SPI1HIPL0       (0x01U << SPI1HIPL0_POS)
#define IICHIPL0        (0x01U << IICHIPL0_POS)
#define MPWM0HIPL0      (0x01U << MPWM0HIPL0_POS)
#define MPWM1HIPL0      (0x01U << MPWM1HIPL0_POS)
#define MPWM2HIPL0      (0x01U << MPWM2HIPL0_POS)
#define DACHIPL0        (0x01U << DACHIPL0_POS)
#define FLHIPL0         (0x01U << FLHIPL0_POS)
#define CRCHIPL0        (0x01U << CRCHIPL0_POS)
#define PAHIPL0         (0x01U << PAHIPL0_POS)
#define PBHIPL0         (0x01U << PBHIPL0_POS)

//-------------------
//    HIPL1
#define LVDHIPL1_POS    0U
#define HRFHIPL1_POS    0U
#define PLLHIPL1_POS    0U
#define CMP0HIPL1_POS   1U
#define CMP1HIPL1_POS   2U
#define CCP0HIPL1_POS   3U
#define CCP1HIPL1_POS   4U
#define CCP2HIPL1_POS   5U
#define CCP3HIPL1_POS   6U
#define CCP4HIPL1_POS   7U
#define CCP5HIPL1_POS   8U
#define T0HIPL1_POS     9U
#define T1HIPL1_POS     10U
#define T2HIPL1_POS     11U
#define T3HIPL1_POS     12U
#define T4HIPL1_POS     13U
#define T5HIPL1_POS     14U
#define T6HIPL1_POS     15U
#define T7HIPL1_POS     16U
#define WDTHIPL1_POS    17U
#define ADC0HIPL1_POS   18U
#define ADC1HIPL1_POS   19U
#define UT0HIPL1_POS    20U
#define UT1HIPL1_POS    21U
#define SPI0HIPL1_POS   22U
#define SPI1HIPL1_POS   23U
#define IICHIPL1_POS    24U
#define MPWM0HIPL1_POS  25U
#define MPWM1HIPL1_POS  26U
#define MPWM2HIPL1_POS  27U
#define DACHIPL1_POS    28U
#define FLHIPL1_POS     29U
#define CRCHIPL1_POS    29U
#define PAHIPL1_POS     30U
#define PBHIPL1_POS     31U
#define LVDHIPL1        (0x01U << LVDHIPL1_POS)
#define HRFHIPL1        (0x01U << HRFHIPL1_POS)
#define PLLHIPL1        (0x01U << PLLHIPL1_POS)
#define CMP0HIPL1       (0x01U << CMP0HIPL1_POS)
#define CMP1HIPL1       (0x01U << CMP1HIPL1_POS)
#define CCP0HIPL1       (0x01U << CCP0HIPL1_POS)
#define CCP1HIPL1       (0x01U << CCP1HIPL1_POS)
#define CCP2HIPL1       (0x01U << CCP2HIPL1_POS)
#define CCP3HIPL1       (0x01U << CCP3HIPL1_POS)
#define CCP4HIPL1       (0x01U << CCP4HIPL1_POS)
#define CCP5HIPL1       (0x01U << CCP5HIPL1_POS)
#define T0HIPL1         (0x01U << T0HIPL1_POS)
#define T1HIPL1         (0x01U << T1HIPL1_POS)
#define T2HIPL1         (0x01U << T2HIPL1_POS)
#define T3HIPL1         (0x01U << T3HIPL1_POS)
#define T4HIPL1         (0x01U << T4HIPL1_POS)
#define T5HIPL1         (0x01U << T5HIPL1_POS)
#define T6HIPL1         (0x01U << T6HIPL1_POS)
#define T7HIPL1         (0x01U << T7HIPL1_POS)
#define WDTHIPL1        (0x01U << WDTHIPL1_POS)
#define ADC0HIPL1       (0x01U << ADC0HIPL1_POS)
#define ADC1HIPL1       (0x01U << ADC1HIPL1_POS)
#define UT0HIPL1        (0x01U << UT0HIPL1_POS)
#define UT1HIPL1        (0x01U << UT1HIPL1_POS)
#define SPI0HIPL1       (0x01U << SPI0HIPL1_POS)
#define SPI1HIPL1       (0x01U << SPI1HIPL1_POS)
#define IICHIPL1        (0x01U << IICHIPL1_POS)
#define MPWM0HIPL1      (0x01U << MPWM0HIPL1_POS)
#define MPWM1HIPL1      (0x01U << MPWM1HIPL1_POS)
#define MPWM2HIPL1      (0x01U << MPWM2HIPL1_POS)
#define DACHIPL1        (0x01U << DACHIPL1_POS)
#define FLHIPL1         (0x01U << FLHIPL1_POS)
#define CRCHIPL1        (0x01U << CRCHIPL1_POS)
#define PAHIPL1         (0x01U << PAHIPL1_POS)
#define PBHIPL1         (0x01U << PBHIPL1_POS)

//-------------------
//    HRF
#define VTSAFCLR_POS    14U
#define VTSAF_POS       15U
#define WDT0RFCLR_POS   18U
#define WDT0RF_POS      19U
#define WDT1RFCLR_POS   20U
#define WDT1RF_POS      21U
#define LVDRFCLR_POS    22U
#define LVDRF_POS       23U
#define DBGRFCLR_POS    24U
#define DBGRF_POS       25U
#define IERFCLR_POS     26U
#define IERF_POS        27U
#define PORFCLR_POS     30U
#define PORF_POS        31U
#define VTSAFCLR        (0x01U << VTSAFCLR_POS)
#define VTSAF           (0x01U << VTSAF_POS)
#define WDT0RFCLR       (0x01U << WDT0RFCLR_POS)
#define WDT0RF          (0x01U << WDT0RF_POS)
#define WDT1RFCLR       (0x01U << WDT1RFCLR_POS)
#define WDT1RF          (0x01U << WDT1RF_POS)
#define LVDRFCLR        (0x01U << LVDRFCLR_POS)
#define LVDRF           (0x01U << LVDRF_POS)
#define DBGRFCLR        (0x01U << DBGRFCLR_POS)
#define DBGRF           (0x01U << DBGRF_POS)
#define IERFCLR         (0x01U << IERFCLR_POS)
#define IERF            (0x01U << IERF_POS)
#define PORFCLR         (0x01U << PORFCLR_POS)
#define PORF            (0x01U << PORF_POS)

//-------------------
//    ALMCON
#define UXARE_POS       12U
#define VTSARE_POS      13U
#define UXARE           (0x01U << UXARE_POS)
#define VTSARE          (0x01U << VTSARE_POS)

//-------------------
//    BPCON
#define BTC_POS         0U
#define BVC_POS         8U
#define REG15C_POS      16U
#define REG33C_POS      24U
#define HOLD_POS        30U
#define SLEEP_POS       31U
#define BTC             (0x0FU << BTC_POS)
#define BVC             (0x0FU << BVC_POS)
#define REG15C          (0x1FU << REG15C_POS)
#define REG33C          (0x1FU << REG33C_POS)
#define HOLD            (0x01U << HOLD_POS)
#define SLEEP           (0x01U << SLEEP_POS)

//-------------------
//    DMACON
#define DMACH0_POS      0U
#define DMACH1_POS      4U
#define DMACH2_POS      8U
#define DMACH3_POS      12U
#define DMACH0          (0x0FU << DMACH0_POS)
#define DMACH1          (0x0FU << DMACH1_POS)
#define DMACH2          (0x0FU << DMACH2_POS)
#define DMACH3          (0x0FU << DMACH3_POS)

//-------------------
//    VTSCON
#define VTSE_POS        0U
#define VTST_POS        4U
#define VTSE            (0x01U << VTSE_POS)
#define VTST            (0x1FU << VTST_POS)

//-------------------
//    LVDCON
#define LVDE_POS        0U
#define LVDRE_POS       1U
#define LVDWE_POS       2U
#define LVDIE_POS       3U
#define LVDVP_POS       4U
#define LVDECLR_POS     30U
#define LVDEF_POS       31U
#define LVDE            (0x01U << LVDE_POS)
#define LVDRE           (0x01U << LVDRE_POS)
#define LVDWE           (0x01U << LVDWE_POS)
#define LVDIE           (0x01U << LVDIE_POS)
#define LVDVP           (0x0FU << LVDVP_POS)
#define LVDECLR         (0x01U << LVDECLR_POS)
#define LVDEF           (0x01U << LVDEF_POS)

//-------------------
//    CLKCON
#define IOSCH_POS       0U
#define IOSCUTE_POS     1U
#define IOSCUTL_POS     2U
#define UXCDRE_POS      3U
#define PLLE_POS        4U
#define PLLM_POS        5U
#define XHOSCE_POS      6U
#define CSF_POS         7U
#define CSWT_POS        8U
#define FLMCG_POS       15U
#define CRCMCG_POS      16U
#define USBMCG_POS      17U
#define ADCMCG_POS      18U
#define DACMCG_POS      20U
#define IOSCH           (0x01U << IOSCH_POS)
#define IOSCUTE         (0x01U << IOSCUTE_POS)
#define IOSCUTL         (0x01U << IOSCUTL_POS)
#define UXCDRE          (0x01U << UXCDRE_POS)
#define PLLE            (0x01U << PLLE_POS)
#define PLLM            (0x01U << PLLM_POS)
#define XHOSCE          (0x01U << XHOSCE_POS)
#define CSF             (0x01U << CSF_POS)
#define CSWT            (0x7FU << CSWT_POS)
#define FLMCG           (0x01U << FLMCG_POS)
#define CRCMCG          (0x01U << CRCMCG_POS)
#define USBMCG          (0x01U << USBMCG_POS)
#define ADCMCG          (0x01U << ADCMCG_POS)
#define DACMCG          (0x01U << DACMCG_POS)

//-------------------
//    PLLCON
#define PFM_POS         0U
#define PFD_POS         8U
#define PFCNT_POS       16U
#define PFOM_POS        29U
#define PFOCLR_POS      30U
#define PFO_POS         31U
#define PFM             (0xFFU << PFM_POS)
#define PFD             (0x07U << PFD_POS)
#define PFCNT           (0x3FFU << PFCNT_POS)
#define PFOM            (0x01U << PFOM_POS)
#define PFOCLR          (0x01U << PFOCLR_POS)
#define PFO             (0x01U << PFO_POS)

//-------------------
//    IOSCCON
#define IOSCHFT_POS     0U
#define IOSCHRT_POS     12U
#define IOSCLFT_POS     16U
#define IOSCLRT_POS     28U
#define IOSCHFT         (0x3FFU << IOSCHFT_POS)
#define IOSCHRT         (0x03U << IOSCHRT_POS)
#define IOSCLFT         (0x3FFU << IOSCLFT_POS)
#define IOSCLRT         (0x03U << IOSCLRT_POS)

//-------------------
//    FLCCON
#define FLCI_POS        0U
#define FLCC_POS        8U
#define FLCDL_POS       10U
#define FLCLC_POS       12U
#define FLTSHSL_POS     16U
#define FLCAC_POS       20U
#define FLBANK0V_POS    28U
#define FLBANK1V_POS    29U
#define FLBANK2V_POS    30U
#define FLBANK3V_POS    31U
#define FLCI            (0xFFU << FLCI_POS)
#define FLCC            (0x03U << FLCC_POS)
#define FLCDL           (0x03U << FLCDL_POS)
#define FLCLC           (0x0FU << FLCLC_POS)
#define FLTSHSL         (0x0FU << FLTSHSL_POS)
#define FLCAC           (0x1FU << FLCAC_POS)
#define FLBANK0V        (0x01U << FLBANK0V_POS)
#define FLBANK1V        (0x01U << FLBANK1V_POS)
#define FLBANK2V        (0x01U << FLBANK2V_POS)
#define FLBANK3V        (0x01U << FLBANK3V_POS)

//-------------------
//    FLUCON
#define FLUI_POS        0U
#define FLUC_POS        8U
#define FLUDC_POS       10U
#define FLUT_POS        12U
#define FLUKS_POS       15U
#define FLULC_POS       16U
#define FLUWIPC_POS     20U
#define FLUSC_POS       21U
#define FLUSS_POS       22U
#define FLURUCS_POS     23U
#define FLURRC_POS      24U
#define FLURRE_POS      27U
#define FLDCTP_POS      28U
#define FLUIE_POS       29U
#define FLUOCLR_POS     30U
#define FLUOPF_POS      31U

#define FLUI            (0xFFU << FLUI_POS)
#define FLUC            (0x03U << FLUC_POS)
#define FLUDC           (0x03U << FLUDC_POS)
#define FLUT            (0x07U << FLUT_POS)
#define FLUKS           (0x01U << FLUKS_POS)
#define FLULC           (0x0FU << FLULC_POS)
#define FLUWIPC         (0x01U << FLUWIPC_POS)
#define FLUSC           (0x01U << FLUSC_POS)
#define FLUSS           (0x01U << FLUSS_POS)
#define FLURUCS         (0x01U << FLURUCS_POS)
#define FLURRC          (0x07U << FLURRC_POS)
#define FLURRE          (0x01U << FLURRE_POS)
#define FLDCTP          (0x01U << FLDCTP_POS)
#define FLUIE           (0x01U << FLUIE_POS)
#define FLUOCLR         (0x01U << FLUOCLR_POS)
#define FLUOPF          (0x01U << FLUOPF_POS)

//-------------------
//    ADCxCON
#define ADCE_POS        0U
#define ADCDMADA_POS    1U
#define ADCDMABM_POS    2U
#define ADCCCD_POS      4U
#define ADCUCS_POS      7U
#define ADCRS_POS       8U
#define ADCP0ES_POS     12U
#define ADCP0REE_POS    17U
#define ADCP0FEE_POS    18U
#define ADCP1ES_POS     19U
#define ADCP1REE_POS    24U
#define ADCP1FEE_POS    25U
#define ADCESDLYP_POS   27U
#define ADCESDLYU_POS   28U
#define ADCE            (0x01U << ADCE_POS)
#define ADCDMADA        (0x01U << ADCDMADA_POS)
#define ADCDMABM        (0x03U << ADCDMABM_POS)
#define ADCCCD          (0x07U << ADCCCD_POS)
#define ADCUCS          (0x01U << ADCUCS_POS)
#define ADCRS           (0x0FU << ADCRS_POS)
#define ADCP0ES         (0x1FU << ADCP0ES_POS)
#define ADCP0REE        (0x01U << ADCP0REE_POS)
#define ADCP0FEE        (0x01U << ADCP0FEE_POS)
#define ADCP1ES         (0x1FU << ADCP1ES_POS)
#define ADCP1REE        (0x01U << ADCP1REE_POS)
#define ADCP1FEE        (0x01U << ADCP1FEE_POS)
#define ADCESDLYP       (0x01U << ADCESDLYP_POS)
#define ADCESDLYU       (0x0FU << ADCESDLYU_POS)

//-------------------
//    ADCxExCC
#define ADCED_POS       0U
#define ADCEIS_POS      12U
#define ADCEES_POS      16U
#define ADCESDLY_POS    20U
#define ADCEPL_POS      24U
#define ADCEST_POS      27U
#define ADCECCLR_POS    30U
#define ADCECF_POS      31U
#define ADCED           (0xFFFU << ADCED_POS)
#define ADCEIS          (0x0FU << ADCEIS_POS)
#define ADCEES          (0x0FU << ADCEES_POS)
#define ADCESDLY        (0x0FU << ADCESDLY_POS)
#define ADCEPL          (0x07U << ADCEPL_POS)
#define ADCEST          (0x07U << ADCEST_POS)
#define ADCECCLR        (0x01U << ADCECCLR_POS)
#define ADCECF          (0x01U << ADCECF_POS)

//-------------------
//    ADCDC0
#define ADCE0DC_POS     0U
#define ADCE0DMAE_POS   7U
#define ADCE1DC_POS     8U
#define ADCE1DMAE_POS   15U
#define ADCE2DC_POS     16U
#define ADCE2DMAE_POS   23U
#define ADCE3DC_POS     24U
#define ADCE3DMAE_POS   31U
#define ADCE0DC         (0x7FU << ADCE0DC_POS)
#define ADCE0DMAE       (0x01U << ADCE0DMAE_POS)
#define ADCE1DC         (0x7FU << ADCE1DC_POS)
#define ADCE1DMAE       (0x01U << ADCE1DMAE_POS)
#define ADCE2DC         (0x7FU << ADCE2DC_POS)
#define ADCE2DMAE       (0x01U << ADCE2DMAE_POS)
#define ADCE3DC         (0x7FU << ADCE3DC_POS)
#define ADCE3DMAE       (0x01U << ADCE3DMAE_POS)

//-------------------
//    ADCDC1
#define ADCE4DC_POS     0U
#define ADCE4DMAE_POS   7U
#define ADCE5DC_POS     8U
#define ADCE5DMAE_POS   15U
#define ADCE6DC_POS     16U
#define ADCE6DMAE_POS   23U
#define ADCE7DC_POS     24U
#define ADCE7DMAE_POS   31U
#define ADCE4DC         (0x7FU << ADCE4DC_POS)
#define ADCE4DMAE       (0x01U << ADCE4DMAE_POS)
#define ADCE5DC         (0x7FU << ADCE5DC_POS)
#define ADCE5DMAE       (0x01U << ADCE5DMAE_POS)
#define ADCE6DC         (0x7FU << ADCE6DC_POS)
#define ADCE6DMAE       (0x01U << ADCE6DMAE_POS)
#define ADCE7DC         (0x7FU << ADCE7DC_POS)
#define ADCE7DMAE       (0x01U << ADCE7DMAE_POS)

//-------------------
//    ADCWW0
#define ADCWW0T_POS     0U
#define ADCE0IB_POS     12U
#define ADCE1IB_POS     13U
#define ADCE2IB_POS     14U
#define ADCE3IB_POS     15U
#define ADCWW0B_POS     16U
#define ADCE4IB_POS     28U
#define ADCE5IB_POS     29U
#define ADCE6IB_POS     30U
#define ADCE7IB_POS     31U
#define ADCWW0T         (0xFFFU << ADCWW0T_POS)
#define ADCE0IB         (0x01U << ADCE0IB_POS)
#define ADCE1IB         (0x01U << ADCE1IB_POS)
#define ADCE2IB         (0x01U << ADCE2IB_POS)
#define ADCE3IB         (0x01U << ADCE3IB_POS)
#define ADCWW0B         (0xFFFU << ADCWW0B_POS)
#define ADCE4IB         (0x01U << ADCE4IB_POS)
#define ADCE5IB         (0x01U << ADCE5IB_POS)
#define ADCE6IB         (0x01U << ADCE6IB_POS)
#define ADCE7IB         (0x01U << ADCE7IB_POS)

//-------------------
//    ADCWW1
#define ADCWW1T_POS     0U
#define ADCWW1B_POS     16U
#define ADCRC_POS       30U
#define ADCWW1T         (0xFFFU << ADCWW1T_POS)
#define ADCWW1B         (0xFFFU << ADCWW1B_POS)
#define ADCRC           (0x3U << ADCRC_POS)

//-------------------
//    ADCWC0
#define ADCW0E0E_POS    0U
#define ADCW0E1E_POS    1U
#define ADCW0E2E_POS    2U
#define ADCW0E3E_POS    3U
#define ADCW0E4E_POS    4U
#define ADCW0E5E_POS    5U
#define ADCW0E6E_POS    6U
#define ADCW0E7E_POS    7U
#define ADCE0OV_POS     8U
#define ADCE1OV_POS     9U
#define ADCE2OV_POS     10U
#define ADCE3OV_POS     11U
#define ADCE4OV_POS     12U
#define ADCE5OV_POS     13U
#define ADCE6OV_POS     14U
#define ADCE7OV_POS     15U
#define ADCW0E0CLR_POS  16U
#define ADCW0E1CLR_POS  18U
#define ADCW0E2CLR_POS  20U
#define ADCW0E3CLR_POS  22U
#define ADCW0E4CLR_POS  24U
#define ADCW0E5CLR_POS  26U
#define ADCW0E6CLR_POS  28U
#define ADCW0E7CLR_POS  30U
#define ADCW0E0F_POS    17U
#define ADCW0E1F_POS    19U
#define ADCW0E2F_POS    21U
#define ADCW0E3F_POS    23U
#define ADCW0E4F_POS    25U
#define ADCW0E5F_POS    27U
#define ADCW0E6F_POS    29U
#define ADCW0E7F_POS    31U

#define ADCW0E0E        (0x01U << ADCW0E0E_POS)
#define ADCW0E1E        (0x01U << ADCW0E1E_POS)
#define ADCW0E2E        (0x01U << ADCW0E2E_POS)
#define ADCW0E3E        (0x01U << ADCW0E3E_POS)
#define ADCW0E4E        (0x01U << ADCW0E4E_POS)
#define ADCW0E5E        (0x01U << ADCW0E5E_POS)
#define ADCW0E6E        (0x01U << ADCW0E6E_POS)
#define ADCW0E7E        (0x01U << ADCW0E7E_POS)
#define ADCE0OV         (0x01U << ADCE0OV_POS)
#define ADCE1OV         (0x01U << ADCE1OV_POS)
#define ADCE2OV         (0x01U << ADCE2OV_POS)
#define ADCE3OV         (0x01U << ADCE3OV_POS)
#define ADCE4OV         (0x01U << ADCE4OV_POS)
#define ADCE5OV         (0x01U << ADCE5OV_POS)
#define ADCE6OV         (0x01U << ADCE6OV_POS)
#define ADCE7OV         (0x01U << ADCE7OV_POS)
#define ADCW0E0CLR      (0x01U << ADCW0E0CLR_POS)
#define ADCW0E1CLR      (0x01U << ADCW0E1CLR_POS)
#define ADCW0E2CLR      (0x01U << ADCW0E2CLR_POS)
#define ADCW0E3CLR      (0x01U << ADCW0E3CLR_POS)
#define ADCW0E4CLR      (0x01U << ADCW0E4CLR_POS)
#define ADCW0E5CLR      (0x01U << ADCW0E5CLR_POS)
#define ADCW0E6CLR      (0x01U << ADCW0E6CLR_POS)
#define ADCW0E7CLR      (0x01U << ADCW0E7CLR_POS)
#define ADCW0E0F        (0x01U << ADCW0E0F_POS)
#define ADCW0E1F        (0x01U << ADCW0E1F_POS)
#define ADCW0E2F        (0x01U << ADCW0E2F_POS)
#define ADCW0E3F        (0x01U << ADCW0E3F_POS)
#define ADCW0E4F        (0x01U << ADCW0E4F_POS)
#define ADCW0E5F        (0x01U << ADCW0E5F_POS)
#define ADCW0E6F        (0x01U << ADCW0E6F_POS)
#define ADCW0E7F        (0x01U << ADCW0E7F_POS)

//-------------------
//    ADCWC1
#define ADCW1E0E_POS    0U
#define ADCW1E1E_POS    1U
#define ADCW1E2E_POS    2U
#define ADCW1E3E_POS    3U
#define ADCW1E4E_POS    4U
#define ADCW1E5E_POS    5U
#define ADCW1E6E_POS    6U
#define ADCW1E7E_POS    7U
#define ADCE0IE_POS     8U
#define ADCE1IE_POS     9U
#define ADCE2IE_POS     10U
#define ADCE3IE_POS     11U
#define ADCE4IE_POS     12U
#define ADCE5IE_POS     13U
#define ADCE6IE_POS     14U
#define ADCE7IE_POS     15U
#define ADCW1E0CLR_POS  16U
#define ADCW1E1CLR_POS  18U
#define ADCW1E2CLR_POS  20U
#define ADCW1E3CLR_POS  22U
#define ADCW1E4CLR_POS  24U
#define ADCW1E5CLR_POS  26U
#define ADCW1E6CLR_POS  28U
#define ADCW1E7CLR_POS  30U
#define ADCW1E0F_POS    17U
#define ADCW1E1F_POS    19U
#define ADCW1E2F_POS    21U
#define ADCW1E3F_POS    23U
#define ADCW1E4F_POS    25U
#define ADCW1E5F_POS    27U
#define ADCW1E6F_POS    29U
#define ADCW1E7F_POS    31U
#define ADCW1E0E        (0x01U << ADCW1E0E_POS)
#define ADCW1E1E        (0x01U << ADCW1E1E_POS)
#define ADCW1E2E        (0x01U << ADCW1E2E_POS)
#define ADCW1E3E        (0x01U << ADCW1E3E_POS)
#define ADCW1E4E        (0x01U << ADCW1E4E_POS)
#define ADCW1E5E        (0x01U << ADCW1E5E_POS)
#define ADCW1E6E        (0x01U << ADCW1E6E_POS)
#define ADCW1E7E        (0x01U << ADCW1E7E_POS)
#define ADCE0IE         (0x01U << ADCE0IE_POS)
#define ADCE1IE         (0x01U << ADCE1IE_POS)
#define ADCE2IE         (0x01U << ADCE2IE_POS)
#define ADCE3IE         (0x01U << ADCE3IE_POS)
#define ADCE4IE         (0x01U << ADCE4IE_POS)
#define ADCE5IE         (0x01U << ADCE5IE_POS)
#define ADCE6IE         (0x01U << ADCE6IE_POS)
#define ADCE7IE         (0x01U << ADCE7IE_POS)
#define ADCW1E0CLR      (0x01U << ADCW1E0CLR_POS)
#define ADCW1E1CLR      (0x01U << ADCW1E1CLR_POS)
#define ADCW1E2CLR      (0x01U << ADCW1E2CLR_POS)
#define ADCW1E3CLR      (0x01U << ADCW1E3CLR_POS)
#define ADCW1E4CLR      (0x01U << ADCW1E4CLR_POS)
#define ADCW1E5CLR      (0x01U << ADCW1E5CLR_POS)
#define ADCW1E6CLR      (0x01U << ADCW1E6CLR_POS)
#define ADCW1E7CLR      (0x01U << ADCW1E7CLR_POS)
#define ADCW1E0F        (0x01U << ADCW1E0F_POS)
#define ADCW1E1F        (0x01U << ADCW1E1F_POS)
#define ADCW1E2F        (0x01U << ADCW1E2F_POS)
#define ADCW1E3F        (0x01U << ADCW1E3F_POS)
#define ADCW1E4F        (0x01U << ADCW1E4F_POS)
#define ADCW1E5F        (0x01U << ADCW1E5F_POS)
#define ADCW1E6F        (0x01U << ADCW1E6F_POS)
#define ADCW1E7F        (0x01U << ADCW1E7F_POS)

//-------------------
//    CMPxCON
#define CMPS_POS        0U
#define CMPE_POS        1U
#define CMPHS_POS       2U
#define CMPFS_POS       4U
#define CMPRFE_POS      6U
#define CMPFFE_POS      7U
#define CMPIS_POS       8U
#define CMPOE_POS       11U
#define CMPRS_POS       12U
#define CMPIE_POS       15U
#define CMPOT_POS       16U
#define CMPTM_POS       20U
#define CMPECLR_POS     30U
#define CMPEF_POS       31U
#define CMPS            (0x01U << CMPS_POS)
#define CMPE            (0x01U << CMPE_POS)
#define CMPHS           (0x03U << CMPHS_POS)
#define CMPFS           (0x03U << CMPFS_POS)
#define CMPRFE          (0x01U << CMPRFE_POS)
#define CMPFFE          (0x01U << CMPFFE_POS)
#define CMPIS           (0x07U << CMPIS_POS)
#define CMPOE           (0x01U << CMPOE_POS)
#define CMPRS           (0x07U << CMPRS_POS)
#define CMPIE           (0x01U << CMPIE_POS)
#define CMPOT           (0x0FU << CMPOT_POS)
#define CMPTM           (0x07U << CMPTM_POS)
#define CMPECLR         (0x01U << CMPECLR_POS)
#define CMPEF           (0x01U << CMPEF_POS)

//-------------------
//    CMPxTW
#define CMPTWS_POS      0U
#define CMPTWE_POS      16U
#define CMPTWS          (0xFFFFU << CMPTWS_POS)
#define CMPTWE          (0xFFFFU << CMPTWE_POS)

//-------------------
//    AMPCON
#define AMP0GC_POS      0U
#define AMP0E_POS       3U
#define AMP0OT_POS      4U
#define AMP1GC_POS      8U
#define AMP1E_POS       11U
#define AMP1OT_POS      12U
#define AMP2GC_POS      16U
#define AMP2E_POS       19U
#define AMP2OT_POS      20U
#define AMP3GC_POS      24U
#define AMP3E_POS       27U
#define AMP3OT_POS      28U
#define AMP0GC          (0x07U << AMP0GC_POS)
#define AMP0E           (0x01U << AMP0E_POS)
#define AMP0OT          (0x0FU << AMP0OT_POS)
#define AMP1GC          (0x07U << AMP1GC_POS)
#define AMP1E           (0x01U << AMP1E_POS)
#define AMP1OT          (0x0FU << AMP1OT_POS)
#define AMP2GC          (0x07U << AMP2GC_POS)
#define AMP2E           (0x01U << AMP2E_POS)
#define AMP2OT          (0x0FU << AMP2OT_POS)
#define AMP3GC          (0x07U << AMP3GC_POS)
#define AMP3E           (0x01U << AMP3E_POS)
#define AMP3OT          (0x0FU << AMP3OT_POS)

//-------------------
//    CCPxCON
#define CCPE_POS        0U
#define CCPUT_POS       1U
#define CCPUM_POS       2U
#define CCPM_POS        4U
#define CCPCIC_POS      6U
#define CCPCI_POS       8U
#define CCPVFE_POS      11U
#define CCPT_POS        12U
#define CCPIE_POS       15U
#define CCPFC0I_POS     16U
#define CCPFC1I_POS     18U
#define CCPFC0L_POS     20U
#define CCPFC1L_POS     21U
#define CCPFC0E_POS     22U
#define CCPFC1E_POS     23U
#define CCPFC_POS       24U
#define CCPCOV_POS      25U
#define CCPFCLR_POS     26U
#define CCPFF_POS       27U
#define CCPGCLR_POS     28U
#define CCPGF_POS       29U
#define CCPCCLR_POS     30U
#define CCPCF_POS       31U
#define CCPE            (0x01U << CCPE_POS)
#define CCPUT           (0x01U << CCPUT_POS)
#define CCPUM           (0x03U << CCPUM_POS)
#define CCPM            (0x03U << CCPM_POS)
#define CCPCIC          (0x03U << CCPCIC_POS)
#define CCPCI           (0x07U << CCPCI_POS)
#define CCPVFE          (0x01U << CCPVFE_POS)
#define CCPT            (0x07U << CCPT_POS)
#define CCPIE           (0x01U << CCPIE_POS)
#define CCPFC0I         (0x03U << CCPFC0I_POS)
#define CCPFC1I         (0x03U << CCPFC1I_POS)
#define CCPFC0L         (0x01U << CCPFC0L_POS)
#define CCPFC1L         (0x01U << CCPFC1L_POS)
#define CCPFC0E         (0x01U << CCPFC0E_POS)
#define CCPFC1E         (0x01U << CCPFC1E_POS)
#define CCPFC           (0x01U << CCPFC_POS)
#define CCPCOV          (0x01U << CCPCOV_POS)
#define CCPFCLR         (0x01U << CCPFCLR_POS)
#define CCPFF           (0x01U << CCPFF_POS)
#define CCPGCLR         (0x01U << CCPGCLR_POS)
#define CCPGF           (0x01U << CCPGF_POS)
#define CCPCCLR         (0x01U << CCPCCLR_POS)
#define CCPCF           (0x01U << CCPCF_POS)

//-------------------
//    CCPxOUT
#define CCPPOE_POS      0U
#define CCPNOE_POS      1U
#define CCPFPO_POS      2U
#define CCPFNO_POS      3U
#define CCPPIO_POS      4U
#define CCPNIO_POS      5U
#define CCPROC_POS      6U
#define CCPGM_POS       8U
#define CCPGUE_POS      10U
#define CCPGDE_POS      11U
#define CCPGINV_POS     12U
#define CCPPFM_POS      13U
#define CCPDCD_POS      16U
#define CCPSBR_POS      19U
#define CCPCOL_POS      20U
#define CCPGFM_POS      22U
#define CCPPOE          (0x01U << CCPPOE_POS)
#define CCPNOE          (0x01U << CCPNOE_POS)
#define CCPFPO          (0x01U << CCPFPO_POS)
#define CCPFNO          (0x01U << CCPFNO_POS)
#define CCPPIO          (0x01U << CCPPIO_POS)
#define CCPNIO          (0x01U << CCPNIO_POS)
#define CCPROC          (0x03U << CCPROC_POS)
#define CCPGM           (0x03U << CCPGM_POS)
#define CCPGUE          (0x01U << CCPGUE_POS)
#define CCPGDE          (0x01U << CCPGDE_POS)
#define CCPGINV         (0x01U << CCPGINV_POS)
#define CCPPFM          (0x01U << CCPPFM_POS)
#define CCPDCD          (0x07U << CCPDCD_POS)
#define CCPSBR          (0x01U << CCPSBR_POS)
#define CCPCOL          (0x03U << CCPCOL_POS)
#define CCPGFM          (0x01U << CCPGFM_POS)

//-------------------
//    CCPxVAL
#define CCPCV_POS       0U
#define CCPPSC_POS      16U
#define CCPDT_POS       24U
#define CCPCV           (0xFFFFU << CCPCV_POS)
#define CCPPSC          (0xFFU << CCPPSC_POS)
#define CCPDT           (0xFFU << CCPDT_POS)

//-------------------
//    CCPxGAT
#define CCPGSV_POS      0U
#define CCPGEV_POS      16U
#define CCPGSV          (0xFFFFU << CCPGSV_POS)
#define CCPGEV          (0xFFFFU << CCPGEV_POS)

//-------------------
//    MPxCON
#define MPW0E_POS       0U
#define MPW1E_POS       1U
#define MPW2E_POS       2U
#define MPW3E_POS       3U
#define MPW4E_POS       4U
#define MPW5E_POS       5U
#define MPW6E_POS       6U
#define MPW7E_POS       7U
#define MPT_POS         8U
#define MPCO_POS        11U
#define MPLSM_POS       12U
#define MPLSKS_POS      13U
#define MPIE_POS        14U
#define MPW0CLR_POS     16U
#define MPW0F_POS       17U
#define MPW1CLR_POS     18U
#define MPW1F_POS       19U
#define MPW2CLR_POS     20U
#define MPW2F_POS       21U
#define MPW3CLR_POS     22U
#define MPW3F_POS       23U
#define MPW4CLR_POS     24U
#define MPW4F_POS       25U
#define MPW5CLR_POS     26U
#define MPW5F_POS       27U
#define MPW6CLR_POS     28U
#define MPW6F_POS       29U
#define MPW7CLR_POS     30U
#define MPW7F_POS       31U
#define MPW0E           (0x01U << MPW0E_POS)
#define MPW1E           (0x01U << MPW1E_POS)
#define MPW2E           (0x01U << MPW2E_POS)
#define MPW3E           (0x01U << MPW3E_POS)
#define MPW4E           (0x01U << MPW4E_POS)
#define MPW5E           (0x01U << MPW5E_POS)
#define MPW6E           (0x01U << MPW6E_POS)
#define MPW7E           (0x01U << MPW7E_POS)
#define MPT             (0x07U << MPT_POS)
#define MPCO            (0x01U << MPCO_POS)
#define MPLSM           (0x01U << MPLSM_POS)
#define MPLSKS          (0x01U << MPLSKS_POS)
#define MPIE            (0x01U << MPIE_POS)
#define MPW0CLR         (0x01U << MPW0CLR_POS)
#define MPW0F           (0x01U << MPW0F_POS)
#define MPW1CLR         (0x01U << MPW1CLR_POS)
#define MPW1F           (0x01U << MPW1F_POS)
#define MPW2CLR         (0x01U << MPW2CLR_POS)
#define MPW2F           (0x01U << MPW2F_POS)
#define MPW3CLR         (0x01U << MPW3CLR_POS)
#define MPW3F           (0x01U << MPW3F_POS)
#define MPW4CLR         (0x01U << MPW4CLR_POS)
#define MPW4F           (0x01U << MPW4F_POS)
#define MPW5CLR         (0x01U << MPW5CLR_POS)
#define MPW5F           (0x01U << MPW5F_POS)
#define MPW6CLR         (0x01U << MPW6CLR_POS)
#define MPW6F           (0x01U << MPW6F_POS)
#define MPW7CLR         (0x01U << MPW7CLR_POS)
#define MPW7F           (0x01U << MPW7F_POS)

//-------------------
//    ABUFCON
#define ABUFIS_POS      0U
#define ABUFE_POS       5U
#define ABUFM_POS       6U
#define ABUFOT_POS      8U
#define ADCTM_POS       16U
#define ABUFIS          (0x1FU << ABUFIS_POS)
#define ABUFE           (0x01U << ABUFE_POS)
#define ABUFM           (0x03U << ABUFM_POS)
#define ABUFOT          (0x0FU << ABUFOT_POS)
#define ADCTM           (0xFFU << ADCTM_POS)

//-------------------
//    WDTxCON
#define WDTE_POS        0U
#define WDTRE_POS       1U
#define WDTWE_POS       2U
#define WDTIE_POS       3U
#define WDTCD_POS       4U
#define WDTCLR_POS      8U
#define WDTRLV_POS      16U
#define WDTTO_POS       31U
#define WDTE            (0x01U << WDTE_POS)
#define WDTRE           (0x01U << WDTRE_POS)
#define WDTWE           (0x01U << WDTWE_POS)
#define WDTIE           (0x01U << WDTIE_POS)
#define WDTCD           (0x07U << WDTCD_POS)
#define WDTCLR          (0xFFU << WDTCLR_POS)
#define WDTRLV          (0xFFU << WDTRLV_POS)
#define WDTTO           (0x01U << WDTTO_POS)

//-------------------
//    SPIxCON
#define SPIE_POS        0U
#define SPIKS_POS       1U
#define SPICIH_POS      2U
#define SPIDSP_POS      3U
#define SPIMCD_POS      4U
#define SPIS_POS        8U
#define SPIHD_POS       9U
#define SPIHDR_POS      10U
#define SPILF_POS       11U
#define SPIDMAE_POS     12U
#define SPIMSOE_POS     13U
#define SPISIOP_POS     14U
#define SPIPORTS_POS    15U
#define SPIIE_POS       16U
#define SPITCLR_POS     30U
#define SPITBV_POS      31U
#define SPIE            (0x01U << SPIE_POS)
#define SPIKS           (0x01U << SPIKS_POS)
#define SPICIH          (0x01U << SPICIH_POS)
#define SPIDSP          (0x01U << SPIDSP_POS)
#define SPIMCD          (0x0FU << SPIMCD_POS)
#define SPIS            (0x01U << SPIS_POS)
#define SPIHD           (0x01U << SPIHD_POS)
#define SPIHDR          (0x01U << SPIHDR_POS)
#define SPILF           (0x01U << SPILF_POS)
#define SPIDMAE         (0x01U << SPIDMAE_POS)
#define SPIMSOE         (0x01U << SPIMSOE_POS)
#define SPISIOP         (0x01U << SPISIOP_POS)
#define SPIPORTS        (0x01U << SPIPORTS_POS)
#define SPIIE           (0x01U << SPIIE_POS)
#define SPITCLR         (0x01U << SPITCLR_POS)
#define SPITBV          (0x01U << SPITBV_POS)

//-------------------
//    UTxCON
#define UTE_POS         0U
#define UTTKS_POS       1U
#define UTM_POS         2U
#define UTBRA_POS       4U
#define UTRE_POS        7U
#define UTBR_POS        8U
#define UTHD_POS        20U
#define UTTDMAE_POS     21U
#define UTRDMAE_POS     22U
#define UTPORTS_POS     23U
#define UTIE_POS        24U
#define UTPAR_POS       25U
#define UTRERR_POS      27U
#define UTTCLR_POS      28U
#define UTTBV_POS       29U
#define UTRCLR_POS      30U
#define UTRBV_POS       31U
#define UTE             (0x01U << UTE_POS)
#define UTTKS           (0x01U << UTTKS_POS)
#define UTM             (0x03U << UTM_POS)
#define UTBRA           (0x07U << UTBRA_POS)
#define UTRE            (0x01U << UTRE_POS)
#define UTBR            (0xFFFU << UTBR_POS)
#define UTHD            (0x01U << UTHD_POS)
#define UTTDMAE         (0x01U << UTTDMAE_POS)
#define UTRDMAE         (0x01U << UTRDMAE_POS)
#define UTPORTS         (0x01U << UTPORTS_POS)
#define UTIE            (0x01U << UTIE_POS)
#define UTPAR           (0x03U << UTPAR_POS)
#define UTRERR          (0x01U << UTRERR_POS)
#define UTTCLR          (0x01U << UTTCLR_POS)
#define UTTBV           (0x01U << UTTBV_POS)
#define UTRCLR          (0x01U << UTRCLR_POS)
#define UTRBV           (0x01U << UTRBV_POS)

//-------------------
//    IICCON
#define IICE_POS        0U
#define IICKS_POS       1U
#define IICS_POS        2U
#define IICTX_POS       3U
#define IICTXC_POS      4U
#define IICHE_POS       6U
#define IICSAM_POS      7U
#define IICMCD_POS      8U
#define IICSCL_POS      16U
#define IICDMAE_POS     17U
#define IICPORTS_POS    18U
#define IICIE_POS       19U
#define IICBSTA_POS     20U
#define IICSLE_POS      21U
#define IICBSTART_POS   22U
#define IICBSTOP_POS    23U
#define IICCCLR_POS     24U
#define IICCLTO_POS     25U
#define IICHCLR_POS     26U
#define IICHNACK_POS    27U
#define IICTCLR_POS     28U
#define IICTBV_POS      29U
#define IICRCLR_POS     30U
#define IICRBV_POS      31U
#define IICE            (0x01U << IICE_POS)
#define IICKS           (0x01U << IICKS_POS)
#define IICS            (0x01U << IICS_POS)
#define IICTX           (0x01U << IICTX_POS)
#define IICTXC          (0x03U << IICTXC_POS)
#define IICHE           (0x01U << IICHE_POS)
#define IICSAM          (0x01U << IICSAM_POS)
#define IICMCD          (0xFFU << IICMCD_POS)
#define IICSCL          (0x01U << IICSCL_POS)
#define IICDMAE         (0x01U << IICDMAE_POS)
#define IICPORTS        (0x01U << IICPORTS_POS)
#define IICIE           (0x01U << IICIE_POS)
#define IICBSTA         (0x01U << IICBSTA_POS)
#define IICSLE          (0x01U << IICSLE_POS)
#define IICBSTART       (0x01U << IICBSTART_POS)
#define IICBSTOP        (0x01U << IICBSTOP_POS)
#define IICCCLR         (0x01U << IICCCLR_POS)
#define IICCLTO         (0x01U << IICCLTO_POS)
#define IICHCLR         (0x01U << IICHCLR_POS)
#define IICHNACK        (0x01U << IICHNACK_POS)
#define IICTCLR         (0x01U << IICTCLR_POS)
#define IICTBV          (0x01U << IICTBV_POS)
#define IICRCLR         (0x01U << IICRCLR_POS)
#define IICRBV          (0x01U << IICRBV_POS)

//-------------------
//    CRCCON
#define CRCREGL_POS     0U
#define CRCREFI_POS     5U
#define CRCREFO_POS     6U
#define CRCCPLO_POS     7U
#define CRCIDM_POS      8U
#define CRCPDC_POS      10U
#define CRCKS_POS       12U
#define CRCIE_POS       13U
#define CRCRVB_POS      14U
#define CRCCCLR_POS     30U
#define CRCCF_POS       31U

#define CRCREGL         (0x1FU << CRCREGL_POS)
#define CRCREFI         (0x01U << CRCREFI_POS)
#define CRCREFO         (0x01U << CRCREFO_POS)
#define CRCCPLO         (0x01U << CRCCPLO_POS)
#define CRCIDM          (0x03U << CRCIDM_POS)
#define CRCPDC          (0x03U << CRCPDC_POS)
#define CRCKS           (0x01U << CRCKS_POS)
#define CRCIE           (0x01U << CRCIE_POS)
#define CRCRVB          (0x01U << CRCRVB_POS)
#define CRCCCLR         (0x01U << CRCCCLR_POS)
#define CRCCF           (0x01U << CRCCF_POS)

//-------------------
//    TxCON
#define TE_POS          0U
#define TUT_POS         1U
#define TUM_POS         2U
#define TCM_POS         4U
#define TECC_POS        6U
#define TCS_POS         8U
#define TOIE_POS        11U
#define TERS_POS        12U
#define TERC_POS        16U
#define TRS_POS         18U
#define TRGE_POS        19U
#define TENC0_POS       20U
#define TENC1_POS       22U
#define TENCM_POS       24U
#define TENCP_POS       26U
#define TOFM_POS        27U
#define TCD_POS         29U
#define TOCLR_POS       30U
#define TOV_POS         31U
#define TE              (0x01U << TE_POS)
#define TUT             (0x01U << TUT_POS)
#define TUM             (0x03U << TUM_POS)
#define TCM             (0x03U << TCM_POS)
#define TECC            (0x03U << TECC_POS)
#define TCS             (0x07U << TCS_POS)
#define TOIE            (0x01U << TOIE_POS)
#define TERS            (0x0FU << TERS_POS)
#define TERC            (0x03U << TERC_POS)
#define TRS             (0x01U << TRS_POS)
#define TRGE            (0x01U << TRGE_POS)
#define TENC0           (0x03U << TENC0_POS)
#define TENC1           (0x03U << TENC1_POS)
#define TENCM           (0x03U << TENCM_POS)
#define TENCP           (0x01U << TENCP_POS)
#define TOFM            (0x03U << TOFM_POS)
#define TCD             (0x01U << TCD_POS)
#define TOCLR           (0x01U << TOCLR_POS)
#define TOV             (0x01U << TOV_POS)

//-------------------
//    TxARL
#define TARL_POS        0U
#define TPSC_POS        16U
#define TARL            (0xFFFFU << TARL_POS)
#define TPSC            (0xFFFFU << TPSC_POS)

//-------------------
//    TxECON
#define TPEC_POS        0U
#define TETS_POS        8U
#define TETC_POS        12U
#define TET_POS         13U
#define TACT_POS        14U
#define TEPS_POS        16U
#define TEPC_POS        19U
//#define TPS_POS         20U
#define TPGE_POS        21U
#define TCSC_POS        22U
#define TSBR_POS        24U
#define TEIE_POS        25U
#define TECLR_POS       30U
#define TEF_POS         31U

#define TPEC            (0xFFU << TPEC_POS)
#define TETS            (0x0FU << TETS_POS)
#define TETC            (0x01U << TETC_POS)
#define TET             (0x01U << TET_POS)
#define TACT            (0x03U << TACT_POS)
#define TEPS            (0x07U << TEPS_POS)
#define TEPC            (0x01U << TEPC_POS)
//#define TPS             (0x01U << TPS_POS)
#define TPGE            (0x01U << TPGE_POS)
#define TCSC            (0x03U << TCSC_POS)
#define TSBR            (0x01U << TSBR_POS)
#define TEIE            (0x01U << TEIE_POS)
#define TECLR           (0x01U << TECLR_POS)
#define TEF             (0x01U << TEF_POS)

//-------------------
//    UBCON
#define UE_POS          0U
#define USRSM_POS       1U
#define UFS_POS         2U
#define ULT_POS         4U
#define UDPPU_POS       6U
#define UDNPU_POS       7U
#define UDADR_POS       8U
#define UXM_POS         16U
#define UIE_POS         17U
#define URSBF_POS       18U
#define UBRSM_POS       27U
#define UBSPD_POS       29U
#define UBCLR_POS       30U
#define UBRST_POS       31U

#define UE              (0x01U << UE_POS)
#define USRSM           (0x01U << USRSM_POS)
#define UFS             (0x01U << UFS_POS)
#define ULT             (0x01U << ULT_POS)
#define UDPPU           (0x01U << UDPPU_POS)
#define UDNPU           (0x01U << UDNPU_POS)
#define UDADR           (0x7FU << UDADR_POS)
#define UXM             (0x01U << UXM_POS)
#define UIE             (0x01U << UIE_POS)
#define URSBF           (0x01U << URSBF_POS)
#define UBRSM           (0x01U << UBRSM_POS)
#define UBSPD           (0x01U << UBSPD_POS)
#define UBCLR           (0x01U << UBCLR_POS)
#define UBRST           (0x01U << UBRST_POS)

//-------------------
//    UFCON
#define UFNUM_POS       0U
#define UFCRC_POS       11U
#define USOFE_POS       16U
#define UMISS_POS       29U
#define USCLR_POS       30U
#define USOF_POS        31U
#define UFNUM           (0x7FFU << UFNUM_POS)
#define UFCRC           (0x1FU << UFCRC_POS)
#define USOFE           (0x01U << USOFE_POS)
#define UMISS           (0x01U << UMISS_POS)
#define USCLR           (0x01U << USCLR_POS)
#define USOF            (0x01U << USOF_POS)

//-------------------
//    UE0CON
#define UEOPS_POS       0U
#define UEIPS_POS       8U
#define UE0DOS_POS      16U
#define UE0DIS_POS      17U
#define UE0SOS_POS      18U
#define UE0SIS_POS      19U
#define UE0STALL_POS    20U
#define ULTS_POS        21U
#define UECERRH_POS     22U
#define UECTP_POS       23U
#define UENCLR_POS      24U
#define UEINACK_POS     25U
#define UECCLR_POS      26U
#define UECERR_POS      27U
#define UEOCLR_POS      28U
#define UEOPF_POS       29U
#define UEICLR_POS      30U
#define UEIPF_POS       31U
#define UEOPS           (0xFFU << UEOPS_POS)
#define UEIPS           (0xFFU << UEIPS_POS)
#define UE0DOS          (0x01U << UE0DOS_POS)
#define UE0DIS          (0x01U << UE0DIS_POS)
#define UE0SOS          (0x01U << UE0SOS_POS)
#define UE0SIS          (0x01U << UE0SIS_POS)
#define UE0STALL        (0x01U << UE0STALL_POS)
#define ULTS            (0x01U << ULTS_POS)
#define UECERRH         (0x01U << UECERRH_POS)
#define UECTP           (0x01U << UECTP_POS)
#define UENCLR          (0x01U << UENCLR_POS)
#define UEINACK         (0x01U << UEINACK_POS)
#define UECCLR          (0x01U << UECCLR_POS)
#define UECERR          (0x01U << UECERR_POS)
#define UEOCLR          (0x01U << UEOCLR_POS)
#define UEOPF           (0x01U << UEOPF_POS)
#define UEICLR          (0x01U << UEICLR_POS)
#define UEIPF           (0x01U << UEIPF_POS)

//-------------------
//    UExCON
#define UEOS_POS        16U
#define UEIS_POS        17U
#define UEODATA0_POS    18U
#define UEIDATA0_POS    19U
#define UEOSTALL_POS    20U
#define UEISTALL_POS    21U
#define UENOHS_POS      23U
// #define UEOPS //same as E0
// #define UEIPS //same as E0
#define UEOS            (0x01U << UEOS_POS)
#define UEIS            (0x01U << UEIS_POS)
#define UEODATA0        (0x01U << UEODATA0_POS)
#define UEIDATA0        (0x01U << UEIDATA0_POS)
#define UEOSTALL        (0x01U << UEOSTALL_POS)
#define UEISTALL        (0x01U << UEISTALL_POS)
// #define UECERRH //same as E0
#define UENOHS          (0x01U << UENOHS_POS)
// #define UENCLR  //same as E0
// #define UEINAC  //same as E0
// #define UECCLR  //same as E0
// #define UECERR  //same as E0
// #define UEOCLR  //same as E0
// #define UEOPF   //same as E0
// #define UEICLR  //same as E0
// #define UEIPF   //same as E0

//-------------------
//    DACxCON
#define DACCCD_POS      0U
#define DACE_POS        16U
#define DACIE_POS       17U
#define DACDMAE_POS     18U
#define DACDMAL_POS     19U
#define DACCS_POS       20U
#define DACIM_POS       21U
#define DACTE_POS       22U
#define DACCCLR_POS     30U
#define DACCF_POS       31U

#define DACCCD          (0xFFFFU << DACCCD_POS)
#define DACE            (0x01U << DACE_POS)
#define DACIE           (0x01U << DACIE_POS)
#define DACDMAE         (0x01U << DACDMAE_POS)
#define DACDMAL         (0x01U << DACDMAL_POS)
#define DACCS           (0x01U << DACCS_POS)
#define DACIM           (0x01U << DACIM_POS)
#define DACTE           (0x01U << DACTE_POS)
#define DACCCLR         (0x01U << DACCCLR_POS)
#define DACCF           (0x01U << DACCF_POS)


//-------------------
//    PORTx_PD (Port Data)
#define P0_POS          0U
#define P1_POS          1U
#define P2_POS          2U
#define P3_POS          3U
#define P4_POS          4U
#define P5_POS          5U
#define P6_POS          6U
#define P7_POS          7U
#define P8_POS          8U
#define P9_POS          9U
#define P10_POS         10U
#define P11_POS         11U
#define P12_POS         12U
#define P13_POS         13U
#define P14_POS         14U
#define P15_POS         15U
#define P16_POS         16U
#define P17_POS         17U
#define P18_POS         18U
#define P19_POS         19U
#define P20_POS         20U
#define P21_POS         21U
#define P22_POS         22U
#define P23_POS         23U
#define P24_POS         24U
#define P25_POS         25U
#define P26_POS         26U
#define P27_POS         27U
#define P28_POS         28U
#define P29_POS         29U
#define P30_POS         30U
#define P31_POS         31U
#define P0D             (0x01U << P0_POS)
#define P1D             (0x01U << P1_POS)
#define P2D             (0x01U << P2_POS)
#define P3D             (0x01U << P3_POS)
#define P4D             (0x01U << P4_POS)
#define P5D             (0x01U << P5_POS)
#define P6D             (0x01U << P6_POS)
#define P7D             (0x01U << P7_POS)
#define P8D             (0x01U << P8_POS)
#define P9D             (0x01U << P9_POS)
#define P10D            (0x01U << P10_POS)
#define P11D            (0x01U << P11_POS)
#define P12D            (0x01U << P12_POS)
#define P13D            (0x01U << P13_POS)
#define P14D            (0x01U << P14_POS)
#define P15D            (0x01U << P15_POS)
#define P16D            (0x01U << P16_POS)
#define P17D            (0x01U << P17_POS)
#define P18D            (0x01U << P18_POS)
#define P19D            (0x01U << P19_POS)
#define P20D            (0x01U << P20_POS)
#define P21D            (0x01U << P21_POS)
#define P22D            (0x01U << P22_POS)
#define P23D            (0x01U << P23_POS)
#define P24D            (0x01U << P24_POS)
#define P25D            (0x01U << P25_POS)
#define P26D            (0x01U << P26_POS)
#define P27D            (0x01U << P27_POS)
#define P28D            (0x01U << P28_POS)
#define P29D            (0x01U << P29_POS)
#define P30D            (0x01U << P30_POS)
#define P31D            (0x01U << P31_POS)

//-------------------
//    PORTx_POE (Port Output Enable)
#define P0OE            (0x01U << P0_POS)
#define P1OE            (0x01U << P1_POS)
#define P2OE            (0x01U << P2_POS)
#define P3OE            (0x01U << P3_POS)
#define P4OE            (0x01U << P4_POS)
#define P5OE            (0x01U << P5_POS)
#define P6OE            (0x01U << P6_POS)
#define P7OE            (0x01U << P7_POS)
#define P8OE            (0x01U << P8_POS)
#define P9OE            (0x01U << P9_POS)
#define P10OE           (0x01U << P10_POS)
#define P11OE           (0x01U << P11_POS)
#define P12OE           (0x01U << P12_POS)
#define P13OE           (0x01U << P13_POS)
#define P14OE           (0x01U << P14_POS)
#define P15OE           (0x01U << P15_POS)
#define P16OE           (0x01U << P16_POS)
#define P17OE           (0x01U << P17_POS)
#define P18OE           (0x01U << P18_POS)
#define P19OE           (0x01U << P19_POS)
#define P20OE           (0x01U << P20_POS)
#define P21OE           (0x01U << P21_POS)
#define P22OE           (0x01U << P22_POS)
#define P23OE           (0x01U << P23_POS)
#define P24OE           (0x01U << P24_POS)
#define P25OE           (0x01U << P25_POS)
#define P26OE           (0x01U << P26_POS)
#define P27OE           (0x01U << P27_POS)
#define P28OE           (0x01U << P28_POS)
#define P29OE           (0x01U << P29_POS)
#define P30OE           (0x01U << P30_POS)
#define P31OE           (0x01U << P31_POS)

//-------------------
//    PORTx_PIE (Port Input Enable)
#define P0IE            (0x01U << P0_POS)
#define P1IE            (0x01U << P1_POS)
#define P2IE            (0x01U << P2_POS)
#define P3IE            (0x01U << P3_POS)
#define P4IE            (0x01U << P4_POS)
#define P5IE            (0x01U << P5_POS)
#define P6IE            (0x01U << P6_POS)
#define P7IE            (0x01U << P7_POS)
#define P8IE            (0x01U << P8_POS)
#define P9IE            (0x01U << P9_POS)
#define P10IE           (0x01U << P10_POS)
#define P11IE           (0x01U << P11_POS)
#define P12IE           (0x01U << P12_POS)
#define P13IE           (0x01U << P13_POS)
#define P14IE           (0x01U << P14_POS)
#define P15IE           (0x01U << P15_POS)
#define P16IE           (0x01U << P16_POS)
#define P17IE           (0x01U << P17_POS)
#define P18IE           (0x01U << P18_POS)
#define P19IE           (0x01U << P19_POS)
#define P20IE           (0x01U << P20_POS)
#define P21IE           (0x01U << P21_POS)
#define P22IE           (0x01U << P22_POS)
#define P23IE           (0x01U << P23_POS)
#define P24IE           (0x01U << P24_POS)
#define P25IE           (0x01U << P25_POS)
#define P26IE           (0x01U << P26_POS)
#define P27IE           (0x01U << P27_POS)
#define P28IE           (0x01U << P28_POS)
#define P29IE           (0x01U << P29_POS)
#define P30IE           (0x01U << P30_POS)
#define P31IE           (0x01U << P31_POS)

//-------------------
//    PORTx_PPU (Port Pull-Up)
#define P0PU            (0x01U << P0_POS)
#define P1PU            (0x01U << P1_POS)
#define P2PU            (0x01U << P2_POS)
#define P3PU            (0x01U << P3_POS)
#define P4PU            (0x01U << P4_POS)
#define P5PU            (0x01U << P5_POS)
#define P6PU            (0x01U << P6_POS)
#define P7PU            (0x01U << P7_POS)
#define P8PU            (0x01U << P8_POS)
#define P9PU            (0x01U << P9_POS)
#define P10PU           (0x01U << P10_POS)
#define P11PU           (0x01U << P11_POS)
#define P12PU           (0x01U << P12_POS)
#define P13PU           (0x01U << P13_POS)
#define P14PU           (0x01U << P14_POS)
#define P15PU           (0x01U << P15_POS)
#define P16PU           (0x01U << P16_POS)
#define P17PU           (0x01U << P17_POS)
#define P18PU           (0x01U << P18_POS)
#define P19PU           (0x01U << P19_POS)
#define P20PU           (0x01U << P20_POS)
#define P21PU           (0x01U << P21_POS)
#define P22PU           (0x01U << P22_POS)
#define P23PU           (0x01U << P23_POS)
#define P24PU           (0x01U << P24_POS)
#define P25PU           (0x01U << P25_POS)
#define P26PU           (0x01U << P26_POS)
#define P27PU           (0x01U << P27_POS)
#define P28PU           (0x01U << P28_POS)
#define P29PU           (0x01U << P29_POS)
#define P30PU           (0x01U << P30_POS)
#define P31PU           (0x01U << P31_POS)

//-------------------
//    PORTx_PRIE (Port Rising Edge IE)
#define P0RIE           (0x01U << P0_POS)
#define P1RIE           (0x01U << P1_POS)
#define P2RIE           (0x01U << P2_POS)
#define P3RIE           (0x01U << P3_POS)
#define P4RIE           (0x01U << P4_POS)
#define P5RIE           (0x01U << P5_POS)
#define P6RIE           (0x01U << P6_POS)
#define P7RIE           (0x01U << P7_POS)
#define P8RIE           (0x01U << P8_POS)
#define P9RIE           (0x01U << P9_POS)
#define P10RIE          (0x01U << P10_POS)
#define P11RIE          (0x01U << P11_POS)
#define P12RIE          (0x01U << P12_POS)
#define P13RIE          (0x01U << P13_POS)
#define P14RIE          (0x01U << P14_POS)
#define P15RIE          (0x01U << P15_POS)
#define P16RIE          (0x01U << P16_POS)
#define P17RIE          (0x01U << P17_POS)
#define P18RIE          (0x01U << P18_POS)
#define P19RIE          (0x01U << P19_POS)
#define P20RIE          (0x01U << P20_POS)
#define P21RIE          (0x01U << P21_POS)
#define P22RIE          (0x01U << P22_POS)
#define P23RIE          (0x01U << P23_POS)
#define P24RIE          (0x01U << P24_POS)
#define P25RIE          (0x01U << P25_POS)
#define P26RIE          (0x01U << P26_POS)
#define P27RIE          (0x01U << P27_POS)
#define P28RIE          (0x01U << P28_POS)
#define P29RIE          (0x01U << P29_POS)
#define P30RIE          (0x01U << P30_POS)
#define P31RIE          (0x01U << P31_POS)

//-------------------
//    PORTx_PFIE (Port Falling Edge IE)
#define P0FIE           (0x01U << P0_POS)
#define P1FIE           (0x01U << P1_POS)
#define P2FIE           (0x01U << P2_POS)
#define P3FIE           (0x01U << P3_POS)
#define P4FIE           (0x01U << P4_POS)
#define P5FIE           (0x01U << P5_POS)
#define P6FIE           (0x01U << P6_POS)
#define P7FIE           (0x01U << P7_POS)
#define P8FIE           (0x01U << P8_POS)
#define P9FIE           (0x01U << P9_POS)
#define P10FIE          (0x01U << P10_POS)
#define P11FIE          (0x01U << P11_POS)
#define P12FIE          (0x01U << P12_POS)
#define P13FIE          (0x01U << P13_POS)
#define P14FIE          (0x01U << P14_POS)
#define P15FIE          (0x01U << P15_POS)
#define P16FIE          (0x01U << P16_POS)
#define P17FIE          (0x01U << P17_POS)
#define P18FIE          (0x01U << P18_POS)
#define P19FIE          (0x01U << P19_POS)
#define P20FIE          (0x01U << P20_POS)
#define P21FIE          (0x01U << P21_POS)
#define P22FIE          (0x01U << P22_POS)
#define P23FIE          (0x01U << P23_POS)
#define P24FIE          (0x01U << P24_POS)
#define P25FIE          (0x01U << P25_POS)
#define P26FIE          (0x01U << P26_POS)
#define P27FIE          (0x01U << P27_POS)
#define P28FIE          (0x01U << P28_POS)
#define P29FIE          (0x01U << P29_POS)
#define P30FIE          (0x01U << P30_POS)
#define P31FIE          (0x01U << P31_POS)

//-------------------
//    PORTx_PIF (Port Interrupt Flag)
#define P0IF            (0x01U << P0_POS)
#define P1IF            (0x01U << P1_POS)
#define P2IF            (0x01U << P2_POS)
#define P3IF            (0x01U << P3_POS)
#define P4IF            (0x01U << P4_POS)
#define P5IF            (0x01U << P5_POS)
#define P6IF            (0x01U << P6_POS)
#define P7IF            (0x01U << P7_POS)
#define P8IF            (0x01U << P8_POS)
#define P9IF            (0x01U << P9_POS)
#define P10IF           (0x01U << P10_POS)
#define P11IF           (0x01U << P11_POS)
#define P12IF           (0x01U << P12_POS)
#define P13IF           (0x01U << P13_POS)
#define P14IF           (0x01U << P14_POS)
#define P15IF           (0x01U << P15_POS)
#define P16IF           (0x01U << P16_POS)
#define P17IF           (0x01U << P17_POS)
#define P18IF           (0x01U << P18_POS)
#define P19IF           (0x01U << P19_POS)
#define P20IF           (0x01U << P20_POS)
#define P21IF           (0x01U << P21_POS)
#define P22IF           (0x01U << P22_POS)
#define P23IF           (0x01U << P23_POS)
#define P24IF           (0x01U << P24_POS)
#define P25IF           (0x01U << P25_POS)
#define P26IF           (0x01U << P26_POS)
#define P27IF           (0x01U << P27_POS)
#define P28IF           (0x01U << P28_POS)
#define P29IF           (0x01U << P29_POS)
#define P30IF           (0x01U << P30_POS)
#define P31IF           (0x01U << P31_POS)

//-------------------
//    PORTx_PIFC (Port Interrupt Flag Clear)
#define P0IFC           (0x01U << P0_POS)
#define P1IFC           (0x01U << P1_POS)
#define P2IFC           (0x01U << P2_POS)
#define P3IFC           (0x01U << P3_POS)
#define P4IFC           (0x01U << P4_POS)
#define P5IFC           (0x01U << P5_POS)
#define P6IFC           (0x01U << P6_POS)
#define P7IFC           (0x01U << P7_POS)
#define P8IFC           (0x01U << P8_POS)
#define P9IFC           (0x01U << P9_POS)
#define P10IFC          (0x01U << P10_POS)
#define P11IFC          (0x01U << P11_POS)
#define P12IFC          (0x01U << P12_POS)
#define P13IFC          (0x01U << P13_POS)
#define P14IFC          (0x01U << P14_POS)
#define P15IFC          (0x01U << P15_POS)
#define P16IFC          (0x01U << P16_POS)
#define P17IFC          (0x01U << P17_POS)
#define P18IFC          (0x01U << P18_POS)
#define P19IFC          (0x01U << P19_POS)
#define P20IFC          (0x01U << P20_POS)
#define P21IFC          (0x01U << P21_POS)
#define P22IFC          (0x01U << P22_POS)
#define P23IFC          (0x01U << P23_POS)
#define P24IFC          (0x01U << P24_POS)
#define P25IFC          (0x01U << P25_POS)
#define P26IFC          (0x01U << P26_POS)
#define P27IFC          (0x01U << P27_POS)
#define P28IFC          (0x01U << P28_POS)
#define P29IFC          (0x01U << P29_POS)
#define P30IFC          (0x01U << P30_POS)
#define P31IFC          (0x01U << P31_POS)

//-------------------
//    PORTx_PFS0 (Port Filter Select 0)
#define P0FS0           (0x01U << P0_POS)
#define P1FS0           (0x01U << P1_POS)
#define P2FS0           (0x01U << P2_POS)
#define P3FS0           (0x01U << P3_POS)
#define P4FS0           (0x01U << P4_POS)
#define P5FS0           (0x01U << P5_POS)
#define P6FS0           (0x01U << P6_POS)
#define P7FS0           (0x01U << P7_POS)
#define P8FS0           (0x01U << P8_POS)
#define P9FS0           (0x01U << P9_POS)
#define P10FS0          (0x01U << P10_POS)
#define P11FS0          (0x01U << P11_POS)
#define P12FS0          (0x01U << P12_POS)
#define P13FS0          (0x01U << P13_POS)
#define P14FS0          (0x01U << P14_POS)
#define P15FS0          (0x01U << P15_POS)
#define P16FS0          (0x01U << P16_POS)
#define P17FS0          (0x01U << P17_POS)
#define P18FS0          (0x01U << P18_POS)
#define P19FS0          (0x01U << P19_POS)
#define P20FS0          (0x01U << P20_POS)
#define P21FS0          (0x01U << P21_POS)
#define P22FS0          (0x01U << P22_POS)
#define P23FS0          (0x01U << P23_POS)
#define P24FS0          (0x01U << P24_POS)
#define P25FS0          (0x01U << P25_POS)
#define P26FS0          (0x01U << P26_POS)
#define P27FS0          (0x01U << P27_POS)
#define P28FS0          (0x01U << P28_POS)
#define P29FS0          (0x01U << P29_POS)
#define P30FS0          (0x01U << P30_POS)
#define P31FS0          (0x01U << P31_POS)

//-------------------
//    PORTx_PFS1 (Port Filter Select 1)
#define P0FS1           (0x01U << P0_POS)
#define P1FS1           (0x01U << P1_POS)
#define P2FS1           (0x01U << P2_POS)
#define P3FS1           (0x01U << P3_POS)
#define P4FS1           (0x01U << P4_POS)
#define P5FS1           (0x01U << P5_POS)
#define P6FS1           (0x01U << P6_POS)
#define P7FS1           (0x01U << P7_POS)
#define P8FS1           (0x01U << P8_POS)
#define P9FS1           (0x01U << P9_POS)
#define P10FS1          (0x01U << P10_POS)
#define P11FS1          (0x01U << P11_POS)
#define P12FS1          (0x01U << P12_POS)
#define P13FS1          (0x01U << P13_POS)
#define P14FS1          (0x01U << P14_POS)
#define P15FS1          (0x01U << P15_POS)
#define P16FS1          (0x01U << P16_POS)
#define P17FS1          (0x01U << P17_POS)
#define P18FS1          (0x01U << P18_POS)
#define P19FS1          (0x01U << P19_POS)
#define P20FS1          (0x01U << P20_POS)
#define P21FS1          (0x01U << P21_POS)
#define P22FS1          (0x01U << P22_POS)
#define P23FS1          (0x01U << P23_POS)
#define P24FS1          (0x01U << P24_POS)
#define P25FS1          (0x01U << P25_POS)
#define P26FS1          (0x01U << P26_POS)
#define P27FS1          (0x01U << P27_POS)
#define P28FS1          (0x01U << P28_POS)
#define P29FS1          (0x01U << P29_POS)
#define P30FS1          (0x01U << P30_POS)
#define P31FS1          (0x01U << P31_POS)

//-------------------
//    PORTx_PLS (Port Low Speed)
#define P0LS            (0x01U << P0_POS)
#define P1LS            (0x01U << P1_POS)
#define P2LS            (0x01U << P2_POS)
#define P3LS            (0x01U << P3_POS)
#define P4LS            (0x01U << P4_POS)
#define P5LS            (0x01U << P5_POS)
#define P6LS            (0x01U << P6_POS)
#define P7LS            (0x01U << P7_POS)
#define P8LS            (0x01U << P8_POS)
#define P9LS            (0x01U << P9_POS)
#define P10LS           (0x01U << P10_POS)
#define P11LS           (0x01U << P11_POS)
#define P12LS           (0x01U << P12_POS)
#define P13LS           (0x01U << P13_POS)
#define P14LS           (0x01U << P14_POS)
#define P15LS           (0x01U << P15_POS)
#define P16LS           (0x01U << P16_POS)
#define P17LS           (0x01U << P17_POS)
#define P18LS           (0x01U << P18_POS)
#define P19LS           (0x01U << P19_POS)
#define P20LS           (0x01U << P20_POS)
#define P21LS           (0x01U << P21_POS)
#define P22LS           (0x01U << P22_POS)
#define P23LS           (0x01U << P23_POS)
#define P24LS           (0x01U << P24_POS)
#define P25LS           (0x01U << P25_POS)
#define P26LS           (0x01U << P26_POS)
#define P27LS           (0x01U << P27_POS)
#define P28LS           (0x01U << P28_POS)
#define P29LS           (0x01U << P29_POS)
#define P30LS           (0x01U << P30_POS)
#define P31LS           (0x01U << P31_POS)

//-------------------
//    PORTx_PPE (Port Protect Enable)
#define P0PE            (0x01U << P0_POS)
#define P1PE            (0x01U << P1_POS)
#define P2PE            (0x01U << P2_POS)
#define P3PE            (0x01U << P3_POS)
#define P4PE            (0x01U << P4_POS)
#define P5PE            (0x01U << P5_POS)
#define P6PE            (0x01U << P6_POS)
#define P7PE            (0x01U << P7_POS)
#define P8PE            (0x01U << P8_POS)
#define P9PE            (0x01U << P9_POS)
#define P10PE           (0x01U << P10_POS)
#define P11PE           (0x01U << P11_POS)
#define P12PE           (0x01U << P12_POS)
#define P13PE           (0x01U << P13_POS)
#define P14PE           (0x01U << P14_POS)
#define P15PE           (0x01U << P15_POS)
#define P16PE           (0x01U << P16_POS)
#define P17PE           (0x01U << P17_POS)
#define P18PE           (0x01U << P18_POS)
#define P19PE           (0x01U << P19_POS)
#define P20PE           (0x01U << P20_POS)
#define P21PE           (0x01U << P21_POS)
#define P22PE           (0x01U << P22_POS)
#define P23PE           (0x01U << P23_POS)
#define P24PE           (0x01U << P24_POS)
#define P25PE           (0x01U << P25_POS)
#define P26PE           (0x01U << P26_POS)
#define P27PE           (0x01U << P27_POS)
#define P28PE           (0x01U << P28_POS)
#define P29PE           (0x01U << P29_POS)
#define P30PE           (0x01U << P30_POS)
#define P31PE           (0x01U << P31_POS)

#endif // CV32S6015_SR_H
