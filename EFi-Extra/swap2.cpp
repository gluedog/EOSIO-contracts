#include <swap.hpp>
/* 
This is the EFi V2 Swap2 Contract.
by gluedog
*/


/* We will manually add the DMD and DOP users to the tables */

void swapcontrak::addwhitelistdop( const name& owner_account, const asset& whitelist_remaining_dop, const asset& whitelist_claimed_dop )
{
    require_auth( get_self() );
    entrytable userwhitelist(get_self(), owner_account.value); 

    auto it = userwhitelist.find(owner_account.value);
    if( it != userwhitelist.end() ) 
    {
        userwhitelist.modify( it, get_self(), [&]( auto& row ) 
        {
            row.whitelist_remaining_dop = whitelist_remaining_dop;
            row.whitelist_claimed_dop = whitelist_claimed_dop;
        });
    } 
    else 
        userwhitelist.emplace( get_self(), [&]( auto& row ) 
        {
            row.owner_account = owner_account;
            row.whitelist_remaining_dop = whitelist_remaining_dop;
            row.whitelist_claimed_dop = whitelist_claimed_dop;
        });
}

void swapcontrak::addwhitelistdmd( const name& owner_account, const asset& whitelist_remaining_dmd, const asset& whitelist_claimed_dmd )
{
    require_auth( get_self() );
    entrytable userwhitelist(get_self(), owner_account.value); 

    auto it = userwhitelist.find(owner_account.value);
    if( it != userwhitelist.end() ) 
    {
        userwhitelist.modify( it, get_self(), [&]( auto& row ) 
        {
            row.whitelist_remaining_dmd = whitelist_remaining_dmd;
            row.whitelist_claimed_dmd = whitelist_claimed_dmd;
        });
    } 
    else 
        userwhitelist.emplace( get_self(), [&]( auto& row ) 
        {
            row.owner_account = owner_account;
            row.whitelist_remaining_dmd = whitelist_remaining_dmd;
            row.whitelist_claimed_dmd = whitelist_claimed_dmd;
        });
}

void swapcontrak::setbonus(uint8_t bonus)
{
    require_auth(get_self());

    totaltable totalswapped(get_self(), "totals"_n.value); 
    auto total_it = totalswapped.find("totals"_n.value);
    if(total_it != totalswapped.end()) 
    {
        totalswapped.modify(total_it, get_self(), [&]( auto& row) 
        {
            row.bonus = bonus;
        });
    } 
    else 
        return;
}

void swapcontrak::setlocked(bool locked)
{ /* This function lets us set the 'locked' variable true or false for the 31 day staking. */
    require_auth(get_self());

    totaltable totalswapped(get_self(), "totals"_n.value); 
    auto total_it = totalswapped.find("totals"_n.value);
    check(total_it != totalswapped.end(), "error: totals table is not initiated.");

    totalswapped.modify(total_it, get_self(), [&]( auto& row) 
    {
        row.locked = locked;
    });
}
/* Setter function that needs to be called after the contract is deployed to initialize the totals table (with 0, preferably, except the bonus, maybe.) */
void swapcontrak::set(const asset& initial_dop, const asset& initial_dmd, uint8_t initial_bonus)
{
    require_auth(get_self());
    totaltable totalswapped(get_self(), "totals"_n.value); 

    auto total_it = totalswapped.find("totals"_n.value);
    if(total_it == totalswapped.end())
    {
        totalswapped.emplace(get_self(), [&](auto& row) 
        {
            row.key = "totals"_n;
            row.dmd_total_swapped = initial_dmd;
            row.dop_total_swapped = initial_dop;
            row.bonus             = initial_bonus;
            row.locked            = 1; // We'll always set the swap to locked == true when setting the contract.
        });
    } 
    else
    { 
        totalswapped.modify(total_it,get_self(), [&](auto& row) 
        {
            row.dmd_total_swapped = initial_dmd;
            row.dop_total_swapped = initial_dop;
            row.bonus             = initial_bonus;
            row.locked            = 1; // We'll always set the swap to locked == true when setting the contract.
        });
    }
}

/* This function will populate the whitelist with hardcoded values inside the source code, right here */
void swapcontrak::populate()
{
    require_auth( get_self() );

    addwhitelistdmd("anameisapple"_n, asset(2440756, dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("angcheewei33"_n, asset(40451,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("bitcoinzakfr"_n, asset(262900,  dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("chenghao5555"_n, asset(2004844, dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("ddhwsw521.tp"_n, asset(257776,  dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("depurantsave"_n, asset(510000,  dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("dxj123dxj123"_n, asset(48000,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("e1314.e"_n,      asset(16718,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("earart.ftw"_n,   asset(14168,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("eoskidultlab"_n, asset(60421,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("eoslsftomoon"_n, asset(70892,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("eosworld1115"_n, asset(7342770, dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("ez2eosfamily"_n, asset(1839988, dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("flybottle123"_n, asset(206020,  dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("gary22212345"_n, asset(27712,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("gi2dmobxguge"_n, asset(30780,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("gm3tmmjshege"_n, asset(128209,  dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("gu3tanrthege"_n, asset(3469452, dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("gu4dombugmge"_n, asset(98502,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("gy2dqmzwhege"_n, asset(97568,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("gyztimztg4ge"_n, asset(113901,  dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("ha4dqnzsgmge"_n, asset(98982,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("hejjooneosvc"_n, asset(72309,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("hekey5134qih"_n, asset(51875,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("hosageosbitp"_n, asset(100000,  dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("huimouyixiao"_n, asset(200000,  dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("iknownothing"_n, asset(200000,  dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("jiaosheyinyi"_n, asset(20000,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("liyuxuan1135"_n, asset(15000,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("maltaeosprog"_n, asset(185257,  dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("oracle1q1w1e"_n, asset(15886,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("qjyailcm1314"_n, asset(79407,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("rha222222thm"_n, asset(99999,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("rivaldo11111"_n, asset(15424,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("robita123451"_n, asset(11743,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("stenzhou1234"_n, asset(33058,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("unfabibenwei"_n, asset(73147,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("uvytoz5fnyx5"_n, asset(120407,  dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("voicelessone"_n, asset(40916,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("wangmingwang"_n, asset(118906,  dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("xianyang1533"_n, asset(10128,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("xieqianlong4"_n, asset(36300,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("xingguoda123"_n, asset(25500,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("xjuoj4smvfmu"_n, asset(18000,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("yangmingm.tp"_n, asset(28800,   dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("yingshudeeos"_n, asset(120000,  dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("zhour1234.tp"_n, asset(100000,  dmd2_symbol), asset(0, dmd2_symbol) );
    addwhitelistdmd("zyzlovejf.tp"_n, asset(78616,   dmd2_symbol), asset(0, dmd2_symbol) );

    addwhitelistdop("1coin1beauty"_n, asset(3200000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("1home4myeos1"_n, asset(2455040,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("33qmwwghf2xx"_n, asset(54796738,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("4f45wnnxblym"_n, asset(1418803,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("4mxdyvoxg4b2"_n, asset(14624979,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("agentrmars12"_n, asset(11070155,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("alecz.ftw"_n,    asset(5509320,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("angcheewei33"_n, asset(17660000,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("araksara4332"_n, asset(1588160,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("bauernhofk1b"_n, asset(1451700,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("bnr431334523"_n, asset(5422176,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("bon235.ftw"_n,   asset(2000000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("bpndewia4amw"_n, asset(1376732,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("calvaniz.ftw"_n, asset(10000000,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("chenghao5555"_n, asset(391161700, dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("chenqiji1234"_n, asset(1647811,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("chickengreat"_n, asset(5000000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("comefromhere"_n, asset(828590984, dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("crypto5.ftw"_n,  asset(5543827,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("dappdapp5555"_n, asset(3477816,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("datguyprinze"_n, asset(4498019,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("dawannardis1"_n, asset(1575843,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("depurantsave"_n, asset(35950000,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("dhedgeinvest"_n, asset(3700000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("dickyguo3333"_n, asset(3670000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("dllo4211.ftw"_n, asset(1540586,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("doinggoodjob"_n, asset(83045119,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("downthehatch"_n, asset(26488790,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("e1314.e"_n,      asset(3000000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("earart.ftw"_n,   asset(13716531,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("easytoeosshk"_n, asset(57007282,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("eos152152lch"_n, asset(3066091,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("eosairuqixue"_n, asset(3176051,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("eosdefi22333"_n, asset(219847680, dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("eosgateio222"_n, asset(50000000,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("eosisrealbtc"_n, asset(1623563,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("eosjunying12"_n, asset(13890000,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("eoskidultlab"_n, asset(3237284,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("eosmillion12"_n, asset(1525114,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("eosotcotc111"_n, asset(2389000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("eospluser.tp"_n, asset(15300000,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("eoswallet342"_n, asset(3880000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("eoswanglawhu"_n, asset(13105288,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("eosworld1115"_n, asset(73119100,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("eosxhmxhm123"_n, asset(1184000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("eosxhmxhm321"_n, asset(12716300,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("eszhengzhike"_n, asset(1715754,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("ez2eosfamily"_n, asset(35219573,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("fanliting111"_n, asset(7000000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("g44tgmzwgage"_n, asset(22558838,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("geydknrqg4ge"_n, asset(2242502,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("gezdcobzgqge"_n, asset(10793700,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("gm3tmmjshege"_n, asset(10600829,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("gmytinzrgage"_n, asset(154891185, dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("gotoswimming"_n, asset(22727702,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("guydombzgage"_n, asset(36508221,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("guydqnbzgmge"_n, asset(3322558,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("gy4danzsgige"_n, asset(7727335,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("gyzdqojugmge"_n, asset(5565184,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("gyztimztg4ge"_n, asset(31730135,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("ha2timjqgige"_n, asset(5000000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("ha4dqnzsgmge"_n, asset(332653875, dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("hekey5134qih"_n, asset(3055683,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("herroyuycary"_n, asset(400362831, dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("herroyuyjinh"_n, asset(2297492,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("heydenjugmge"_n, asset(3280000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("jaicrytpo.tp"_n, asset(55000000,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("jamzbondz123"_n, asset(1864627,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("jawarido.ftw"_n, asset(9640000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("jcortez24412"_n, asset(20727843,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("jdghhdyssrso"_n, asset(4600000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("jianglinxi11"_n, asset(12000000,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("jlbcnt1wg.tp"_n, asset(43941603,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("joah12345333"_n, asset(1496000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("johanewa1234"_n, asset(3878928,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("juarjol.ftw"_n,  asset(1924692,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("kapo.ftw"_n,     asset(7865107,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("kranthi3.ftw"_n, asset(1340000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("lepyotlo.ftw"_n, asset(15459629,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("lovehuangmin"_n, asset(298901783, dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("lovenieflsky"_n, asset(26282923,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("lovetomorrow"_n, asset(500157673, dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("maltaeosprog"_n, asset(95200706,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("maoc1414.ftw"_n, asset(10826179,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("mariaioa.ftw"_n, asset(1630613,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("mazak.ftw"_n,    asset(1000000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("mosstheboss1"_n, asset(88408687,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("mykeyhulu525"_n, asset(3904858,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("mysteorrrrrr"_n, asset(2000000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("nanjinshitou"_n, asset(1000000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("pen1s.ftw"_n,    asset(1278025,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("piemuran1234"_n, asset(15000000,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("pq2ketd4cdxr"_n, asset(1497732,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("primenumber2"_n, asset(21378627,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("realdonnypie"_n, asset(3200534,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("reyhubeosacc"_n, asset(1676395,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("rha222222thm"_n, asset(40000000,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("savindolphin"_n, asset(1099026,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("shaojun11111"_n, asset(1887472,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("shikongdeeos"_n, asset(38678200,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("tabuzah13.m"_n,  asset(38957910,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("throne2thorn"_n, asset(2000000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("timer11.ftw"_n,  asset(2000000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("uf55555cttyc"_n, asset(7345418,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("uirbnrjyc2v2"_n, asset(1669230,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("vrs5nhk2t4e4"_n, asset(4000097,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("wangmingwang"_n, asset(1927405,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("xiaoyezi5555"_n, asset(602920000, dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("xiaoyuxiaoyu"_n, asset(50000000,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("yangyijun123"_n, asset(6000000,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("youcuozhijia"_n, asset(4974375,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("yucel1453145"_n, asset(22295322,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("zengzhiwei22"_n, asset(91150405,  dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("zyzlovejf.tp"_n, asset(4439917,   dop2_symbol), asset(0, dop2_symbol)) ;
    addwhitelistdop("zztaoyong.tp"_n, asset(1230000,   dop2_symbol), asset(0, dop2_symbol)) ;
}

[[eosio::on_notify("dolphintoken::transfer")]]
void swapcontrak::registerswapdop(const name& owner_account, const name& to, const asset& swap_quantity_dop, std::string memo)
{
    if (to != get_self() || owner_account == get_self())
    {
        print("*these are not the droids you are looking for*");
        return;
    }

    check(swap_quantity_dop.amount >= 5000000000, "error: must swap a minimum of 50 DOP");
    check(swap_quantity_dop.symbol == dop_symbol, "error: these are not the droids you are looking for.");

    totaltable totalswapped(get_self(), "totals"_n.value); 
    auto total_it = totalswapped.find("totals"_n.value);
    check(total_it != totalswapped.end(), "error: totals table is not initiated.");
    check(total_it->locked == false,"error: the EFi V2 swap is currently locked (inactive).");

    asset converted_swap_qty  = swap_quantity_dop;
    converted_swap_qty.amount = swap_quantity_dop.amount/10000;
    converted_swap_qty.symbol = symbol("DOP", 4);

    /* <------------> */
    /* <------------> */
    /* Here we should check and see if the user is found in the whitelist and if his remaining quantity is greater than or equal to quantity sent */
    entrytable userwhitelist(get_self(), owner_account.value); 
    auto whitelist_it = userwhitelist.find( owner_account.value );
    check(whitelist_it != userwhitelist.end(), "error: user is not whitelisted.");
    check(converted_swap_qty.amount <= whitelist_it->whitelist_remaining_dop.amount, "error: swap quantity would exceed maximum allowed for user.");
    /* <------------> */
    /* <------------> */

    // At this point, everything should be OK. We will substract the user's remaining and claimed from the whitelist table, and modify the totaltable;

    totalswapped.modify(total_it, get_self(), [&](auto& row) 
    {
        row.dop_total_swapped += converted_swap_qty; 
    });
    
    // Get Bonus
    uint8_t bonus = total_it->bonus;
    /* Calculate the rewards by looking at the bonus, if bonus == 0 then just: */
    if (bonus == 0)
    {
        swapcontrak::inline_transferdop(get_self(), owner_account, converted_swap_qty, "Sending rewards with zero bonus.");
        /* Modify received_amount in the swapped table */
        userwhitelist.modify(whitelist_it, get_self(), [&](auto& row) 
        {   
            row.whitelist_remaining_dop -= converted_swap_qty;
            row.whitelist_claimed_dop += converted_swap_qty;
        });
    }
    /* Otherwise, we send him more coins */
    else
    {
        /* Send the rewards + the bonus */
        asset swap_qty_withbonus = converted_swap_qty + (converted_swap_qty * bonus/100); // Give the appropiate bonus.
        swapcontrak::inline_transferdop(get_self(), owner_account, swap_qty_withbonus, "Sending rewards with bonus.");
        /* Modify remaining and claimed in the whitelist table, we won't add the bonus here. */
        userwhitelist.modify(whitelist_it, get_self(), [&](auto& row) 
        {   
            row.whitelist_remaining_dop -= converted_swap_qty;
            row.whitelist_claimed_dop += converted_swap_qty;
        });
    }
}

[[eosio::on_notify("eosdmdtokens::transfer")]]
void swapcontrak::registerswapdmd(const name& owner_account, const name& to, const asset& swap_quantity_dmd, std::string memo)
{
    if (to != get_self() || owner_account == get_self())
    {
        print("*these are not the droids you are looking for*");
        return;
    }

    check(swap_quantity_dmd.amount >= 5000000000, "error: must swap a minimum of 0.5 DMD!");
    check(swap_quantity_dmd.symbol == dmd_symbol, "error: these are not the droids you are looking for.");

    totaltable totalswapped(get_self(), "totals"_n.value); 
    auto total_it = totalswapped.find("totals"_n.value);
    check(total_it != totalswapped.end(), "error: totals table is not initiated.");
    check(total_it->locked == false,"error: the EFi V2 swap is currently locked (inactive).");

    asset converted_swap_qty  = swap_quantity_dmd;
    converted_swap_qty.amount = swap_quantity_dmd.amount/1000000;
    converted_swap_qty.symbol = symbol("DMD", 4);

    /* <------------> */
    /* <------------> */
    /* Here we should check and see if the user is found in the whitelist and if his remaining quantity is greater than or equal to quantity sent */
    entrytable userwhitelist(get_self(), owner_account.value); 
    auto whitelist_it = userwhitelist.find( owner_account.value );
    check(whitelist_it != userwhitelist.end(), "error: user is not whitelisted.");
    check(converted_swap_qty.amount <= whitelist_it->whitelist_remaining_dmd.amount, "error: swap quantity would exceed maximum allowed for user.");
    /* <------------> */
    /* <------------> */

    // At this point, everything should be OK. We will substract the user's remaining and claimed from the whitelist table, and modify the totaltable;

    totalswapped.modify(total_it, get_self(), [&](auto& row) 
    {
        row.dmd_total_swapped += converted_swap_qty; 
    });
    
    // Get Bonus
    uint8_t bonus = total_it->bonus;
    /* Calculate the rewards by looking at the bonus, if bonus == 0 then just: */
    if (bonus == 0)
    {
        swapcontrak::inline_transferdmd(get_self(), owner_account, converted_swap_qty, "Sending rewards with zero bonus.");
        /* Modify received_amount in the swapped table */
        userwhitelist.modify(whitelist_it, get_self(), [&](auto& row) 
        {   
            row.whitelist_remaining_dmd -= converted_swap_qty;
            row.whitelist_claimed_dmd += converted_swap_qty;
        });
    }
    /* Otherwise, we send him more coins */
    else
    {
        /* Send the rewards + the bonus */
        asset swap_qty_withbonus = converted_swap_qty + (converted_swap_qty * bonus/100); // Give the appropiate bonus.
        swapcontrak::inline_transferdmd(get_self(), owner_account, swap_qty_withbonus, "Sending rewards with bonus.");
        /* Modify remaining and claimed in the whitelist table, we won't add the bonus here. */
        userwhitelist.modify(whitelist_it, get_self(), [&](auto& row) 
        {   
            row.whitelist_remaining_dmd -= converted_swap_qty;
            row.whitelist_claimed_dmd += converted_swap_qty;
        });
    }
}
