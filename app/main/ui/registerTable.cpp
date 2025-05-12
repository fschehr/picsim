#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
// #include "ftxui-grid-container/grid-container.hpp"

ftxui::Component RegisterTable() {
    using namespace ftxui;

    std::string registerValues[256] = {
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00",
        "00", "00", "00", "00", "00", "00", "00", "00"
    };

    Component input_08 = Input(&registerValues[0]);
    Component input_09 = Input(&registerValues[1]);
    Component input_0A = Input(&registerValues[2]);
    Component input_0B = Input(&registerValues[3]);
    Component input_0C = Input(&registerValues[4]);
    Component input_0D = Input(&registerValues[5]);
    Component input_0E = Input(&registerValues[6]);
    Component input_0F = Input(&registerValues[7]);
    Component input_10 = Input(&registerValues[8]);
    Component input_11 = Input(&registerValues[9]);
    Component input_12 = Input(&registerValues[10]);
    Component input_13 = Input(&registerValues[11]);
    Component input_14 = Input(&registerValues[12]);
    Component input_15 = Input(&registerValues[13]);
    Component input_16 = Input(&registerValues[14]);
    Component input_17 = Input(&registerValues[15]);
    Component input_18 = Input(&registerValues[16]);
    Component input_19 = Input(&registerValues[17]);
    Component input_1A = Input(&registerValues[18]);
    Component input_1B = Input(&registerValues[19]);
    Component input_1C = Input(&registerValues[20]);
    Component input_1D = Input(&registerValues[21]);
    Component input_1E = Input(&registerValues[22]);
    Component input_1F = Input(&registerValues[23]);
    Component input_20 = Input(&registerValues[24]);
    Component input_21 = Input(&registerValues[25]);
    Component input_22 = Input(&registerValues[26]);
    Component input_23 = Input(&registerValues[27]);
    Component input_24 = Input(&registerValues[28]);
    Component input_25 = Input(&registerValues[29]);
    Component input_26 = Input(&registerValues[30]);
    Component input_27 = Input(&registerValues[31]);
    Component input_28 = Input(&registerValues[32]);
    Component input_29 = Input(&registerValues[33]);
    Component input_2A = Input(&registerValues[34]);
    Component input_2B = Input(&registerValues[35]);
    Component input_2C = Input(&registerValues[36]);
    Component input_2D = Input(&registerValues[37]);
    Component input_2E = Input(&registerValues[38]);
    Component input_2F = Input(&registerValues[39]);
    Component input_30 = Input(&registerValues[40]);
    Component input_31 = Input(&registerValues[41]);
    Component input_32 = Input(&registerValues[42]);
    Component input_33 = Input(&registerValues[43]);
    Component input_34 = Input(&registerValues[44]);
    Component input_35 = Input(&registerValues[45]);
    Component input_36 = Input(&registerValues[46]);
    Component input_37 = Input(&registerValues[47]);
    Component input_38 = Input(&registerValues[48]);
    Component input_39 = Input(&registerValues[49]);
    Component input_3A = Input(&registerValues[50]);
    Component input_3B = Input(&registerValues[51]);
    Component input_3C = Input(&registerValues[52]);
    Component input_3D = Input(&registerValues[53]);
    Component input_3E = Input(&registerValues[54]);
    Component input_3F = Input(&registerValues[55]);
    Component input_40 = Input(&registerValues[56]);
    Component input_41 = Input(&registerValues[57]);
    Component input_42 = Input(&registerValues[58]);
    Component input_43 = Input(&registerValues[59]);
    Component input_44 = Input(&registerValues[60]);
    Component input_45 = Input(&registerValues[61]);
    Component input_46 = Input(&registerValues[62]);
    Component input_47 = Input(&registerValues[63]);
    Component input_48 = Input(&registerValues[64]);
    Component input_49 = Input(&registerValues[65]);
    Component input_4A = Input(&registerValues[66]);
    Component input_4B = Input(&registerValues[67]);
    Component input_4C = Input(&registerValues[68]);
    Component input_4D = Input(&registerValues[69]);
    Component input_4E = Input(&registerValues[70]);
    Component input_4F = Input(&registerValues[71]);
    Component input_50 = Input(&registerValues[72]);
    Component input_51 = Input(&registerValues[73]);
    Component input_52 = Input(&registerValues[74]);
    Component input_53 = Input(&registerValues[75]);
    Component input_54 = Input(&registerValues[76]);
    Component input_55 = Input(&registerValues[77]);
    Component input_56 = Input(&registerValues[78]);
    Component input_57 = Input(&registerValues[79]);
    Component input_58 = Input(&registerValues[80]);
    Component input_59 = Input(&registerValues[81]);
    Component input_5A = Input(&registerValues[82]);
    Component input_5B = Input(&registerValues[83]);
    Component input_5C = Input(&registerValues[84]);
    Component input_5D = Input(&registerValues[85]);
    Component input_5E = Input(&registerValues[86]);
    Component input_5F = Input(&registerValues[87]);
    Component input_60 = Input(&registerValues[88]);
    Component input_61 = Input(&registerValues[89]);
    Component input_62 = Input(&registerValues[90]);
    Component input_63 = Input(&registerValues[91]);
    Component input_64 = Input(&registerValues[92]);
    Component input_65 = Input(&registerValues[93]);
    Component input_66 = Input(&registerValues[94]);
    Component input_67 = Input(&registerValues[95]);
    Component input_68 = Input(&registerValues[96]);
    Component input_69 = Input(&registerValues[97]);
    Component input_6A = Input(&registerValues[98]);
    Component input_6B = Input(&registerValues[99]);
    Component input_6C = Input(&registerValues[100]);
    Component input_6D = Input(&registerValues[101]);
    Component input_6E = Input(&registerValues[102]);
    Component input_6F = Input(&registerValues[103]);
    Component input_70 = Input(&registerValues[104]);
    Component input_71 = Input(&registerValues[105]);
    Component input_72 = Input(&registerValues[106]);
    Component input_73 = Input(&registerValues[107]);
    Component input_74 = Input(&registerValues[108]);
    Component input_75 = Input(&registerValues[109]);
    Component input_76 = Input(&registerValues[110]);
    Component input_77 = Input(&registerValues[111]);
    Component input_78 = Input(&registerValues[112]);
    Component input_79 = Input(&registerValues[113]);
    Component input_7A = Input(&registerValues[114]);
    Component input_7B = Input(&registerValues[115]);
    Component input_7C = Input(&registerValues[116]);
    Component input_7D = Input(&registerValues[117]);
    Component input_7E = Input(&registerValues[118]);
    Component input_7F = Input(&registerValues[119]);
    Component input_80 = Input(&registerValues[120]);
    Component input_81 = Input(&registerValues[121]);
    Component input_82 = Input(&registerValues[122]);
    Component input_83 = Input(&registerValues[123]);
    Component input_84 = Input(&registerValues[124]);
    Component input_85 = Input(&registerValues[125]);
    Component input_86 = Input(&registerValues[126]);
    Component input_87 = Input(&registerValues[127]);
    Component input_88 = Input(&registerValues[128]);
    Component input_89 = Input(&registerValues[129]);
    Component input_8A = Input(&registerValues[130]);
    Component input_8B = Input(&registerValues[131]);
    Component input_8C = Input(&registerValues[132]);
    Component input_8D = Input(&registerValues[133]);
    Component input_8E = Input(&registerValues[134]);
    Component input_8F = Input(&registerValues[135]);
    Component input_90 = Input(&registerValues[136]);
    Component input_91 = Input(&registerValues[137]);
    Component input_92 = Input(&registerValues[138]);
    Component input_93 = Input(&registerValues[139]);
    Component input_94 = Input(&registerValues[140]);
    Component input_95 = Input(&registerValues[141]);
    Component input_96 = Input(&registerValues[142]);
    Component input_97 = Input(&registerValues[143]);
    Component input_98 = Input(&registerValues[144]);
    Component input_99 = Input(&registerValues[145]);
    Component input_9A = Input(&registerValues[146]);
    Component input_9B = Input(&registerValues[147]);
    Component input_9C = Input(&registerValues[148]);
    Component input_9D = Input(&registerValues[149]);
    Component input_9E = Input(&registerValues[150]);
    Component input_9F = Input(&registerValues[151]);
    Component input_A0 = Input(&registerValues[152]);
    Component input_A1 = Input(&registerValues[153]);
    Component input_A2 = Input(&registerValues[154]);
    Component input_A3 = Input(&registerValues[155]);
    Component input_A4 = Input(&registerValues[156]);
    Component input_A5 = Input(&registerValues[157]);
    Component input_A6 = Input(&registerValues[158]);
    Component input_A7 = Input(&registerValues[159]);
    Component input_A8 = Input(&registerValues[160]);
    Component input_A9 = Input(&registerValues[161]);
    Component input_AA = Input(&registerValues[162]);
    Component input_AB = Input(&registerValues[163]);
    Component input_AC = Input(&registerValues[164]);
    Component input_AD = Input(&registerValues[165]);
    Component input_AE = Input(&registerValues[166]);
    Component input_AF = Input(&registerValues[167]);
    Component input_B0 = Input(&registerValues[168]);
    Component input_B1 = Input(&registerValues[169]);
    Component input_B2 = Input(&registerValues[170]);
    Component input_B3 = Input(&registerValues[171]);
    Component input_B4 = Input(&registerValues[172]);
    Component input_B5 = Input(&registerValues[173]);
    Component input_B6 = Input(&registerValues[174]);
    Component input_B7 = Input(&registerValues[175]);
    Component input_B8 = Input(&registerValues[176]);
    Component input_B9 = Input(&registerValues[177]);
    Component input_BA = Input(&registerValues[178]);
    Component input_BB = Input(&registerValues[179]);
    Component input_BC = Input(&registerValues[180]);
    Component input_BD = Input(&registerValues[181]);
    Component input_BE = Input(&registerValues[182]);
    Component input_BF = Input(&registerValues[183]);
    Component input_C0 = Input(&registerValues[184]);
    Component input_C1 = Input(&registerValues[185]);
    Component input_C2 = Input(&registerValues[186]);
    Component input_C3 = Input(&registerValues[187]);
    Component input_C4 = Input(&registerValues[188]);
    Component input_C5 = Input(&registerValues[189]);
    Component input_C6 = Input(&registerValues[190]);
    Component input_C7 = Input(&registerValues[191]);
    Component input_C8 = Input(&registerValues[192]);
    Component input_C9 = Input(&registerValues[193]);
    Component input_CA = Input(&registerValues[194]);
    Component input_CB = Input(&registerValues[195]);
    Component input_CC = Input(&registerValues[196]);
    Component input_CD = Input(&registerValues[197]);
    Component input_CE = Input(&registerValues[198]);
    Component input_CF = Input(&registerValues[199]);
    Component input_D0 = Input(&registerValues[200]);
    Component input_D1 = Input(&registerValues[201]);
    Component input_D2 = Input(&registerValues[202]);
    Component input_D3 = Input(&registerValues[203]);
    Component input_D4 = Input(&registerValues[204]);
    Component input_D5 = Input(&registerValues[205]);
    Component input_D6 = Input(&registerValues[206]);
    Component input_D7 = Input(&registerValues[207]);
    Component input_D8 = Input(&registerValues[208]);
    Component input_D9 = Input(&registerValues[209]);
    Component input_DA = Input(&registerValues[210]);
    Component input_DB = Input(&registerValues[211]);
    Component input_DC = Input(&registerValues[212]);
    Component input_DD = Input(&registerValues[213]);
    Component input_DE = Input(&registerValues[214]);
    Component input_DF = Input(&registerValues[215]);
    Component input_E0 = Input(&registerValues[216]);
    Component input_E1 = Input(&registerValues[217]);
    Component input_E2 = Input(&registerValues[218]);
    Component input_E3 = Input(&registerValues[219]);
    Component input_E4 = Input(&registerValues[220]);
    Component input_E5 = Input(&registerValues[221]);
    Component input_E6 = Input(&registerValues[222]);
    Component input_E7 = Input(&registerValues[223]);
    Component input_E8 = Input(&registerValues[224]);
    Component input_E9 = Input(&registerValues[225]);
    Component input_EA = Input(&registerValues[226]);
    Component input_EB = Input(&registerValues[227]);
    Component input_EC = Input(&registerValues[228]);
    Component input_ED = Input(&registerValues[229]);
    Component input_EE = Input(&registerValues[230]);
    Component input_EF = Input(&registerValues[231]);
    Component input_F0 = Input(&registerValues[232]);
    Component input_F1 = Input(&registerValues[233]);
    Component input_F2 = Input(&registerValues[234]);
    Component input_F3 = Input(&registerValues[235]);
    Component input_F4 = Input(&registerValues[236]);
    Component input_F5 = Input(&registerValues[237]);
    Component input_F6 = Input(&registerValues[238]);
    Component input_F7 = Input(&registerValues[239]);
    Component input_F8 = Input(&registerValues[240]);
    Component input_F9 = Input(&registerValues[241]);
    Component input_FA = Input(&registerValues[242]);
    Component input_FB = Input(&registerValues[243]);
    Component input_FC = Input(&registerValues[244]);
    Component input_FD = Input(&registerValues[245]);
    Component input_FE = Input(&registerValues[246]);
    Component input_FF = Input(&registerValues[247]);

    // auto grid = GridContainer({
    //     { text(""), text("00"), text("01"), text("02"), text("03"), text("04"), text("05"), text("06"), text("07") },
    //     { text("08"), input_08, input_09, input_0A, input_0B, input_0C, input_0D, input_0E, input_0F },
    //     { text("10"), input_10, input_11, input_12, input_13, input_14, input_15, input_16, input_17 },
    //     { text("18"), input_18, input_19, input_1A, input_1B, input_1C, input_1D, input_1E, input_1F },
    //     { text("20"), input_20, input_21, input_22, input_23, input_24, input_25, input_26, input_27 },
    //     { text("28"), input_28, input_29, input_2A, input_2B, input_2C, input_2D, input_2E, input_2F },
    //     { text("30"), input_30, input_31, input_32, input_33, input_34, input_35, input_36, input_37 },
    //     { text("38"), input_38, input_39, input_3A, input_3B, input_3C, input_3D, input_3E, input_3F },
    //     { text("40"), input_40, input_41, input_42, input_43, input_44, input_45, input_46, input_47 },
    //     { text("48"), input_48, input_49, input_4A, input_4B, input_4C, input_4D, input_4E, input_4F },
    //     { text("50"), input_50, input_51, input_52, input_53, input_54, input_55, input_56, input_57 },
    //     { text("58"), input_58, input_59, input_5A, input_5B, input_5C, input_5D, input_5E, input_5F },
    //     { text("60"), input_60, input_61, input_62, input_63, input_64, input_65, input_66, input_67 },
    //     { text("68"), input_68, input_69, input_6A, input_6B, input_6C, input_6D, input_6E, input_6F },
    //     { text("70"), input_70, input_71, input_72, input_73, input_74, input_75, input_76, input_77 },
    //     { text("78"), input_78, input_79, input_7A, input_7B, input_7C, input_7D, input_7E, input_7F },
    //     { text("80"), input_80, input_81, input_82, input_83, input_84, input_85, input_86, input_87 },
    //     { text("88"), input_88, input_89, input_8A, input_8B, input_8C, input_8D, input_8E, input_8F },
    //     { text("90"), input_90, input_91, input_92, input_93, input_94, input_95, input_96, input_97 },
    //     { text("98"), input_98, input_99, input_9A, input_9B, input_9C, input_9D, input_9E, input_9F },
    //     { text("A0"), input_A0, input_A1, input_A2, input_A3, input_A4, input_A5, input_A6, input_A7 },
    //     { text("A8"), input_A8, input_A9, input_AA, input_AB, input_AC, input_AD, input_AE, input_AF },
    //     { text("B0"), input_B0, input_B1, input_B2, input_B3, input_B4, input_B5, input_B6, input_B7 },
    //     { text("B8"), input_B8, input_B9, input_BA, input_BB, input_BC, input_BD, input_BE, input_BF },
    //     { text("C0"), input_C0, input_C1, input_C2, input_C3, input_C4, input_C5, input_C6, input_C7 },
    //     { text("C8"), input_C8, input_C9, input_CA, input_CB, input_CC, input_CD, input_CE, input_CF },
    //     { text("D0"), input_D0, input_D1, input_D2, input_D3, input_D4, input_D5, input_D6, input_D7 },
    //     { text("D8"), input_D8, input_D9, input_DA, input_DB, input_DC, input_DD, input_DE, input_DF },
    //     { text("E0"), input_E0, input_E1, input_E2, input_E3, input_E4, input_E5, input_E6, input_E7 },
    //     { text("E8"), input_E8, input_E9, input_EA, input_EB, input_EC, input_ED, input_EE, input_EF },
    //     { text("F0"), input_F0, input_F1, input_F2, input_F3, input_F4, input_F5, input_F6, input_F7 },
    //     { text("F8"), input_F8, input_F9, input_FA, input_FB, input_FC, input_FD, input_FE, input_FF }
    // });

    auto container = Container::Vertical({

    });

    auto registerTable_renderer = Renderer(container, [=] {
        return window(
            text(" Register Table "),
            center(
                hbox({
                    text("Register Table content goes here..."),
                })
            )
        );
    });

    return registerTable_renderer;
}