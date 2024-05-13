#pragma once
#include <type_traits>       // std::is_same_v, std::is_integral_v, std::is_unsigned_v, std::make_unsigned_t, std::common_type_t
#include <bit>               // std::countr_zero, std::bit_width
#include <initializer_list>  // std::initializer_list
#include <gsh/TypeDef.hpp>   // gsh::itype
#include <gsh/Modint.hpp>    // gsh::DynamicModint

namespace gsh {

//@brief Find the largest x for which x * x <= n (https://math.stackexchange.com/questions/2469446/what-is-a-fast-algorithm-for-finding-the-integer-square-root)
constexpr itype::u32 IntSqrt(const itype::u32 x) {
    if (x < 2) return x;
    const itype::u32 log2x = std::bit_width(x) - 1;
    const itype::u32 log2y = log2x / 2u;
    itype::u32 y = 1 << log2y;
    itype::u32 y_squared = 1 << (2u * log2y);
    itype::i32 sqr_diff = x - y_squared;
    y += (sqr_diff / 3u) >> log2y;
    y_squared = y * y;
    sqr_diff = x - y_squared;
    y += sqr_diff / (2 * y);
    y_squared = y * y;
    sqr_diff = x - y_squared;
    if (sqr_diff >= 0) return y;
    y -= (-sqr_diff / (2 * y)) + 1;
    y_squared = y * y;
    sqr_diff = x - y_squared;
    y -= (sqr_diff < 0);
    return y;
}

// @brief Find the greatest common divisor as in std::gcd. (https://lpha-z.hatenablog.com/entry/2020/05/24/231500)
// @return std::common_type_t<T, U>
template<class T, class U> constexpr auto GCD(T x, U y) {
    static_assert(!std::is_same_v<T, bool> && !std::is_same_v<U, bool> && std::is_integral_v<T> && std::is_integral_v<U>, "GCD / The input must be an integral type.");
    if constexpr (std::is_same_v<T, U>) {
        if constexpr (std::is_unsigned_v<T>) {
            return internal::calc_gcd(x, y);
        } else {
            return static_cast<T>(GCD<std::make_unsigned_t<T>, std::make_unsigned<T>>((x < 0 ? -x : x), (y < 0 ? -y : y)));
        }
    } else {
        return GCD<std::common_type_t<T, U>, std::common_type_t<T, U>>(x, y);
    }
}
// @brief Find the greatest common divisor of multiple numbers.
template<class T, class... Args> auto GCD(T x, Args... y) {
    return GCD(x, GCD(y...));
}
// @brief Find the  least common multiple as in std::lcm.
template<class T, class U> auto LCM(T x, U y) {
    return static_cast<std::common_type_t<T, U>>(x < 0 ? -x : x) / GCD(x, y) * static_cast<std::common_type_t<T, U>>(y < 0 ? -y : y);
}
// @brief Find the least common multiple of multiple numbers.
template<class T, class... Args> auto LCM(T x, Args... y) {
    return LCM(x, LCM(y...));
}

namespace internal {

    bool isPrime16(const itype::u16 x) {
        // clang-format off
        const static itype::u64 flag_table[512] = {
0x816d129a64b4cb6eu,0x2196820d864a4c32u,0xa48961205a0434c9u,0x4a2882d129861144u,0x834992132424030u,0x148a48844225064bu,0xb40b4086c304205u,0x65048928125108a0u,0x80124496804c3098u,0xc02104c941124221u,0x804490000982d32u,0x220825b082689681u,0x9004265940a28948u,0x6900924430434006u,0x12410da408088210u,0x86122d22400c060u,0x110d301821b0484u,0x14916022c044a002u,0x92094d204a6400cu,0x4ca2100800522094u,0xa48b081051018200u,0x34c108144309a25u,0x2084490880522502u,0x241140a218003250u,0xa41a00101840128u,0x2926000836004512u,0x10100480c0618283u,0xc20c26584822006du,0x4520582024894810u,0x10c0250219002488u,0x802832ca01140868u,0x60901300264b0400u,
0x32100100d0258082u,0x430800112186430cu,0x92900c10480424u,0x24880906002d2043u,0x530082090932c040u,0x4000814196800880u,0x2058489608481048u,0x926094022080c329u,0x5a0104422812000u,0xa042049019040u,0xc02c801348348924u,0x800084524002982u,0x4d0048452043698u,0x1865328244908a00u,0x28024001020a0090u,0x861104309204a440u,0xc90804522c004208u,0x4424990912486084u,0x1000211403002400u,0x4040208805321a01u,0x6030014084c30906u,0xa2020c9011680218u,0x8224148929860004u,0x880190480084102u,0x20004a442681210u,0x120100100c061061u,0x6512422194032010u,0x140128040a0c9418u,0x14000d040a40a29u,0x4882402d20410490u,0x24080130100020c1u,0x8229020024845904u,
0x4816814802586100u,0xa0ca000611210010u,0x4200b09104000240u,0x2514480906810c04u,0x860a00a011252092u,0x84520004802c10cu,0x22130406980032u,0x1282441481480482u,0xd028804340101824u,0x2c00d86424812004u,0x20000a241081209u,0x180110c04120ca41u,0x20941220a41804a4u,0x48044320240a083u,0x8a6086400c001800u,0x82010512886400u,0x4096110c101a24au,0x840b40160008801u,0x494400880030106u,0x2520c028029208au,0x264848000844201u,0x2122404430004832u,0x20d004a0c3080200u,0x5228004040161840u,0x810180114820890u,0x809320a00a408209u,0x10500522000c008u,0x820c06114010u,0x908028009a44904bu,0x28024309064a04u,0x4480096500180134u,0x1448618202240003u,
0x5108340028120041u,0x6084892890120504u,0x8249402610491012u,0x8840240a01109100u,0x2ca2500004104c10u,0x125001b00a489040u,0x9228a00904a40008u,0x4120022110430002u,0x520c0408003281u,0x8101021020844921u,0x6984010122404810u,0x884402c80130c1u,0x6112c02d02010cu,0x812014030c000a0u,0x840140948000200bu,0xb00841000320040u,0x41848a2906010024u,0x80034c9408081080u,0x5020204140964001u,0x20a44040a2892522u,0x104a212001288602u,0x4225044008140008u,0x2100920410432102u,0x84030922184ca011u,0x124228204108941u,0x900c10884080814u,0x368000028a41b042u,0x200009124a04904u,0x806080102924194u,0x80892816d0010009u,0x500c900168000060u,0x4130424080400120u,
0x49400681252000u,0x1820a00049120108u,0x28241000a6010530u,0x12880020c8200200u,0x420126020092900cu,0x102422404004916u,0x1008801a0c8088u,0x1169008844940260u,0x841324a0120830u,0x30002810c0650082u,0xc801061101200304u,0xc82100820c20080u,0xb0004006520c0213u,0x1004869801104061u,0x4180416014920884u,0x204140228104101au,0x1060340841005229u,0x884004010012800u,0x252040448209042u,0xd820004200800u,0x4020480510024082u,0xc0240601000099u,0x844101221048268u,0x916d020a6400004u,0x92090c20024124c9u,0x4309004000001240u,0x24110102982084u,0x3041089003002443u,0x100882804c205824u,0x2010094106812524u,0x244a001080441018u,0xc00030802894010du,
0x900020c84106002u,0x20c2041008018202u,0x1100001804060968u,0xc028221100b0890u,0x24100260008b610u,0x8024201a21244a01u,0x2402d00024400u,0xa69020001020948bu,0x16186112c001340u,0x4830810402104180u,0x108a218050282048u,0x4248101009100804u,0x520c06092820ca0u,0x82080400014020d2u,0x484180480002822du,0x84030404910010u,0x22c06400006804c2u,0x9100860944320840u,0x2400486400012802u,0x8652210043009010u,0x8808204020908b41u,0x6084020020134404u,0x1008003040249081u,0x4320041001020808u,0x4c800168129040b4u,0x10404912c0080018u,0x104c248941001a24u,0x41204a0910520400u,0x610081411692248u,0x4000100028848024u,0x2806480826080110u,0x200a048442011400u,
0x1224820008820100u,0x4109040a0404004u,0x10802c2010402290u,0x8101005804004328u,0x4832120094810u,0xa0106c000044a442u,0xc948808300804844u,0x4b0100502000000u,0x408409210290413u,0x1900201900228244u,0x41008a6090810120u,0xa2020004104502c0u,0x4201204921104009u,0x422014414002c30u,0x1080210489089202u,0x4804140200105u,0x1325864b0400912u,0x80c1090441009008u,0x124009a00900861u,0x806820526020812u,0x2418002048200008u,0x9001100020348u,0x4009801104a0184u,0x80812000c0008618u,0x4a0cb40005301004u,0x4420002802912982u,0xa2014080912c00c0u,0x80020c309041200u,0x2c00000422100c02u,0x32120000c0008611u,0x5005024040808940u,0x4d120a60a4826086u,
0x1402098012089080u,0x9044008a20240148u,0x12d10002010404u,0x248121320040040au,0x8908040220841908u,0x4482186802022480u,0x8001280040210042u,0x20c801140208245u,0x2020400190402400u,0x2009400019282050u,0x820804060048008u,0x2424110034094930u,0x2920400c2410082u,0x100a0020c008024u,0x100d02104416006u,0x1291048412480001u,0x1841120044240008u,0x2004520080410c26u,0x218482090240009u,0x8a0014d009a20300u,0x40149820004a2584u,0x144000000005a200u,0x90084802c205801u,0x41b0020802912020u,0x218001009003008u,0x844240000020221u,0xc021244b2006012u,0x20500420c84080c0u,0x5329040b04b00005u,0x2920820030486100u,0x1043202253001600u,0x4000d204800048u,
0x8040029800344a2u,0x84092830406404c0u,0xc000920221805044u,0x800822886010u,0x2081009683048418u,0x5100848845000205u,0x944b4186512020u,0x80584c2011080080u,0x805008920060304u,0x982004000900522u,0x20c241a000000050u,0xd021264008160008u,0x4402004190810890u,0x49009860a0c1008u,0x8920300804a0c800u,0x800402c22110084u,0x200901024801b002u,0x4260028000040304u,0x20944104a2130u,0xa480218212002401u,0x1840a09104021020u,0x500096906020004u,0x480000010258u,0xc801340020920300u,0x2080420830084820u,0x212400401689091u,0x1100a00108120061u,0xc00922404482104u,0x9612010000048401u,0x8828228841a00140u,0x114122480424400u,0x108104101a609042u,
0x240028329060848u,0x4010800510806424u,0x2009018442080202u,0x1340301160005004u,0x4520080900810402u,0x2080c269061104au,0x200040260009121u,0x884480806080c00u,0x205a00a480000211u,0x9000204048800u,0x400c82014490814u,0x101200805940a091u,0x4000065808000u,0x6084032100194080u,0x808061121a2404c0u,0x820124209040208u,0xa0010120900434u,0x340240929108000bu,0x4000021961108840u,0x2104086880c02504u,0x84010ca000042280u,0x8a20008a08004120u,0x882110404884800u,0x100040a449098640u,0x800c805004a20101u,0x41121801a0824800u,0x1240041480401u,0x168000200148800u,0x808308224a0820u,0x34000000c2010489u,0x4a41020228820004u,0x424800902820590u,
0x1401288092010041u,0x4304b0104c205000u,0x44000201049021a4u,0x2042000608640048u,0x5020004a01920208u,0x800090422902532u,0x3200051001218011u,0xc10d240808948808u,0x4121840200b4080u,0x82c1052610402200u,0x841220224300100u,0x2812d225001a4824u,0x200413040040042u,0x890884d124201300u,0xa4184400520480u,0x2042091091200600u,0x4040840028304024u,0x4004080904100880u,0x8000000219002208u,0x402090012102022cu,0x120584834000c00u,0x90001480200443u,0x30020400000116du,0x65004a0530884010u,0x8003288418082410u,0x1969100040b04220u,0x4c20480000004u,0x9608252200050001u,0x12910d000220204u,0x44160104100860a0u,0x8440488202280210u,0x4000048028229020u,
0x6010032980002404u,0x205100a081000048u,0x920420410100d10cu,0x504420092100000u,0x2052201080408601u,0xd000020a48100021u,0x4800000480484112u,0x1043002400042209u,0x82c201244000a60u,0x806400984004420u,0x12980020804000c1u,0xc048840020a21048u,0x82980812902010u,0xc328000304a00au,0x40040804104244u,0x480032100100500u,0x408040010691288u,0x1820044948840204u,0x2010830806402u,0x1088412008491252u,0xd005860100340848u,0x4102402184830000u,0x5120a240488010u,0x1840209001004900u,0x880400522024002u,0x8201050018201082u,0x129908104005840u,0xa20140220064a0u,0x94806000000d0418u,0x120c30800d108260u,0x2120c04012000020u,0x203448010410258u,
0xc044000829901304u,0x1801a0026002100u,0x320020140a201413u,0x8009204240000861u,0x6800426080810106u,0x8002048042088290u,0x810c009800040b09u,0x92032884484406u,0x2810c000a408001u,0x920029028045108u,0xca0810900006010u,0x208028020009a400u,0x4148104020200u,0x120406012110904u,0x860a080011403048u,0xd001048160040000u,0x200a0090184102u,0x10ca6480080106c1u,0x5020820148809904u,0x22902084804890u,0x8610242018040019u,0x4410122400c240u,0x106120024100816u,0x80104d0212009008u,0x1104300225040u,0x140100000a2130u,0xa2910c1048410u,0x490c120120008a01u,0x6004014800810420u,0x44a4810080c1280u,0x5045844028001028u,0x980014406106010u,
0x9000a042018600u,0x8008004140229005u,0x4930580100c00802u,0x80020c0241001409u,0x9005100824008940u,0x61120008820a4032u,0x2410042200210400u,0x4020001001040a08u,0x12902022880484u,0x140b400401240653u,0x80c90100d028260u,0x2480800914000920u,0x2001440201400082u,0x41100a4084400cu,0x2020084480090530u,0x2000212043490002u,0x208044008b60100u,0x2410084080410180u,0x12c0098612042000u,0x8920020004148121u,0x6900d100801244b4u,0x418001242008040u,0x228040221064900u,0x820006810c00184u,0x2481011091080040u,0x100086884c10d204u,0x40908a0014020c80u,0x245800a480212018u,0x484130c160101020u,0x502094000094802u,0x21824204a208211u,0x300040a0c22100cu,
0x2100020404484806u,0x12020c0018008480u,0x8941108205140001u,0x48840121a4400812u,0x1400280240601002u,0xc200125120a04008u,0x4c128940301a0100u,0xa001011400008002u,0x140061821221821u,0x430024804900080u,0x448082488050008u,0x8000060224u,0x4820a0090116510u,0x2920424486004c3u,0x8029061840808844u,0x2110c84400000110u,0x141001a04b003089u,0x65200040940200u,0x2012812022400ca2u,0x88080010010482u,0x4140804204801100u,0x424802c32400014u,0x83200091000019u,0x4040840109204005u,0x2090414000112020u,0x618489290400000u,0x1024340148808108u,0x2d06180420000420u,0x220a009000011090u,0x101841100220001u,0x122004400882000u,0x1120060240a600u,
0x1928008a04a0c801u,0x9121224a0520080u,0x2400040048012408u,0x4048040008840240u,0x8148801220a6090u,0x90c02000d3080201u,0xa08b00100001024u,0x20000901008000a0u,0x8402042400250252u,0x40a00240921024u,0x22010804110822u,0x3000219009001442u,0x900922000c00006cu,0x20c02000402810u,0x1212058201400090u,0x812802806104c109u,0x2986100804490024u,0x908849300a218041u,0x941808129044100u,0x4010004010124000u,0x2040210280050248u,0x48900060205800u,0x4400004880c02880u,0x212000609000280u,0x1245108308100001u,0x2020004404082c00u,0x20c80500012010c0u,0x224001008109804u,0x2412886100884016u,0x61008004200a680u,0x8104205000a04048u,0x1801008001840a4u};
        // clang-format on
        return x == 2 || (x % 2 == 1 && (flag_table[x / 128] & (1ull << (x % 128 / 2))));
    }
    bool isPrime32(const itype::u32 x) {
        if (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0 || x % 11 == 0 || x % 13 == 0 || x % 17 == 0) return false;
        const static itype::u16 bases[] = { 1216, 1836,  8885,  4564, 10978, 5228, 15613, 13941, 1553, 173,   3615, 3144, 10065, 9259,  233,  2362, 6244,  6431, 10863, 5920, 6408, 6841, 22124, 2290,  45597, 6935,  4835, 7652, 1051, 445,  5807, 842,  1534, 22140, 1282, 1733, 347,   6311,  14081, 11157, 186,  703,  9862,  15490, 1720, 17816, 10433, 49185, 2535, 9158,  2143,  2840,  664,  29074, 24924, 1035, 41482, 1065,  10189, 8417,  130,  4551,  5159,  48886,
                                            786,  1938,  1013,  2139, 7171,  2143, 16873, 188,   5555, 42007, 1045, 3891, 2853,  23642, 148,  3585, 3027,  280,  3101,  9918, 6452, 2716, 855,   990,   1925,  13557, 1063, 6916, 4965, 4380, 587,  3214, 1808, 1036,  6356, 8191, 6783,  14424, 6929,  1002,  840,  422,  44215, 7753,  5799, 3415,  231,   2013,  8895, 2081,  883,   3855,  5577, 876,   3574,  1925, 1192,  865,   7376,  12254, 5952, 2516,  20463, 186,
                                            5411, 35353, 50898, 1084, 2127,  4305, 115,   7821,  1265, 16169, 1705, 1857, 24938, 220,   3650, 1057, 482,   1690, 2718,  4309, 7496, 1515, 7972,  3763,  10954, 2817,  3430, 1423, 714,  6734, 328,  2581, 2580, 10047, 2797, 155,  5951,  3817,  54850, 2173,  1318, 246,  1807,  2958,  2697, 337,   4871,  2439,  736,  37112, 1226,  527,   7531, 5418,  7242,  2421, 16135, 7015,  8432,  2605,  5638, 5161,  11515, 14949,
                                            748,  5003,  9048,  4679, 1915,  7652, 9657,  660,   3054, 15469, 2910, 775,  14106, 1749,  136,  2673, 61814, 5633, 1244,  2567, 4989, 1637, 1273,  11423, 7974,  7509,  6061, 531,  6608, 1088, 1627, 160,  6416, 11350, 921,  306,  18117, 1238,  463,   1722,  996,  3866, 6576,  6055,  130,  24080, 7331,  3922,  8632, 2706,  24108, 32374, 4237, 15302, 287,   2296, 1220,  20922, 3350,  2089,  562,  11745, 163,   11951 };
        using mint = DynamicModint32<-1>;
        mint::set_mod(x);
        const itype::u32 h = x * 0xad625b89;
        itype::u32 d = x - 1;
        mint cur = bases[h >> 24];
        itype::i32 s = std::countr_zero(d);
        d >>= s;
        cur = cur.pow(d);
        if (cur.val() == 1) return true;
        while (--s && cur.val() != x - 1) cur *= cur;
        return cur.val() == x - 1;
    }

    template<bool Prob> bool isPrime64(const itype::u64 x) {
        if (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0 || x % 11 == 0 || x % 13 == 0 || x % 17 == 0) return false;
        using mint = DynamicModint64<-1>;
        mint::set_mod(x);
        const itype::i32 s = std::countr_zero(x - 1);
        const itype::u64 d = (x - 1) >> s;
        if constexpr (!Prob) {
            auto test = [&](itype::u64 a) -> bool {
                mint cur = mint(a).pow(d);
                if (cur.val() <= 1) return true;
                itype::i32 i = s;
                while (--i && cur.val() != x - 1) cur *= cur;
                return cur.val() == x - 1;
            };
            if (x < 585226005592931977ull) {
                if (x < 7999252175582851ull) {
                    if (x < 350269456337ull) return test(4230279247111683200ull) && test(14694767155120705706ull) && test(16641139526367750375ull);
                    else if (x < 55245642489451ull) return test(2ull) && test(141889084524735ull) && test(1199124725622454117ull) && test(11096072698276303650ull);
                    else return test(2ull) && test(4130806001517ull) && test(149795463772692060ull) && test(186635894390467037ull) && test(3967304179347715805ull);
                } else return test(2ull) && test(123635709730000ull) && test(9233062284813009ull) && test(43835965440333360ull) && test(761179012939631437ull) && test(1263739024124850375ull);
            } else return test(2ull) && test(325ull) && test(9375ull) && test(28178ull) && test(450775ull) && test(9780504ull) && test(1795265022ull);
        } else {
#include <gsh/internal/MRbase.txt>
            auto test = [&](itype::u16 a) -> bool {
                mint cur = mint::raw(a).pow(d);
                if (cur.val() <= 1) return true;
                itype::i32 i = s;
                while (--i && cur.val() != x - 1) cur *= cur;
                return cur.val() == x - 1;
            };
            if (x < 684630005672341) {
                return test(2) && test(base49[(0x3ac69a35u * (itype::u32) x) >> 21] + 3);
            } else {
                if (!test(2)) return false;
                const itype::u16 mask = base64[(0x3ac69a35u * (itype::u32) x) >> 18];
                return test((mask & 0x7fff) + 3) && test((mask & 0x8000) ? 26460 : 9375);
            }
        }
    }

}  // namespace internal

// @brief Prime number determination
template<bool Prob = false> bool isPrime(const itype::u64 x) {
    if (x <= 2147483647) {
        if (x <= 65535) return internal::isPrime16(x);
        else return internal::isPrime32(x);
    } else return internal::isPrime64<Prob>(x);
}


}  // namespace gsh
