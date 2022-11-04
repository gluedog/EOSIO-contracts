#include <swap.hpp>
/* f 
This is the EFi V2 Swap Contract.
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
void swapcontrak::set(const asset& initial_hub, const asset& initial_dop, const asset& initial_dmd, uint8_t initial_bonus)
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

    addwhitelistdmd("anameisapple"_n, asset(2440756, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("angcheewei33"_n, asset(40451, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("bitcoinzakfr"_n, asset(262900, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("chenghao5555"_n, asset(2004844, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("ddhwsw521.tp"_n, asset(257776, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("depurantsave"_n asset(510000, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("dxj123dxj123"_n, asset(48000, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("e1314.e"_n, asset(16718, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("earart.ftw"_n, asset(14168, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("eoskidultlab"_n, asset(60421, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("eoslsftomoon"_n, asset(70892, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("eosworld1115"_n, asset(7342770, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("ez2eosfamily"_n, asset(1839988, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("flybottle123"_n, asset(206020, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("gary22212345"_n, asset(27712, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("gi2dmobxguge"_n, asset(30780, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("gm3tmmjshege"_n, asset(128209, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("gu3tanrthege"_n, asset(3469452, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("gu4dombugmge"_n, asset(98502, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("gy2dqmzwhege"_n, asset(97568, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("gyztimztg4ge"_n, asset(113901, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("ha4dqnzsgmge"_n, asset(98982, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("hejjooneosvc"_n, asset(72309, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("hekey5134qih"_n, asset(51875, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("hosageosbitp"_n, asset(100000, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("huimouyixiao"_n, asset(200000, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("iknownothing"_n, asset(200000, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("jiaosheyinyi"_n, asset(20000, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("liyuxuan1135"_n, asset(15000, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("maltaeosprog"_n, asset(185257, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("oracle1q1w1e"_n, asset(15886, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("qjyailcm1314"_n, asset(79407, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("rha222222thm"_n, asset(99999, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("rivaldo11111"_n, asset(15424, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("robita123451"_n, asset(11743, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("stenzhou1234"_n, asset(33058, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("unfabibenwei"_n, asset(73147, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("uvytoz5fnyx5"_n, asset(120407, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("voicelessone"_n, asset(40916, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("wangmingwang"_n, asset(118906, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("xianyang1533"_n, asset(10128, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("xieqianlong4"_n, asset(36300, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("xingguoda123"_n, asset(25500, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("xjuoj4smvfmu"_n, asset(18000, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("yangmingm.tp"_n, asset(28800, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("yingshudeeos"_n, asset(120000, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("zhour1234.tp"_n, asset(100000, "DMD"), asset(0,"DMD") );
    addwhitelistdmd("zyzlovejf.tp"_n, asset(78616, "DMD"), asset(0,"DMD") );

    addwhitelistdop("1coin1beauty"_n, asset(3200000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("1home4myeos1"_n, asset(2455040, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("33qmwwghf2xx"_n, asset(54796738, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("4f45wnnxblym"_n, asset(1418803, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("4mxdyvoxg4b2"_n, asset(14624979, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("agentrmars12"_n, asset(11070155, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("alecz.ftw"_n, asset(5509320, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("angcheewei33"_n, asset(17660000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("araksara4332"_n, asset(1588160, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("bauernhofk1b"_n, asset(1451700, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("bnr431334523"_n, asset(5422176, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("bon235.ftw"_n, asset(2000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("bpndewia4amw"_n, asset(1376732, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("calvaniz.ftw"_n, asset(10000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("chenghao5555"_n, asset(391161700, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("chenqiji1234"_n, asset(1647811, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("chickengreat"_n, asset(5000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("comefromhere"_n, asset(828590984, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("crypto5.ftw"_n, asset(5543827, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("dappdapp5555"_n, asset(3477816, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("datguyprinze"_n, asset(4498019, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("dawannardis1"_n, asset(1575843, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("depurantsave"_n, asset(35950000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("dhedgeinvest"_n, asset(3700000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("dickyguo3333"_n, asset(3670000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("dllo4211.ftw"_n, asset(1540586, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("doinggoodjob"_n, asset(83045119, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("downthehatch"_n, asset(26488790, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("e1314.e"_n, asset(3000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("earart.ftw"_n, asset(13716531, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("easytoeosshk"_n, asset(57007282, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("eos152152lch"_n, asset(3066091, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("eosairuqixue"_n, asset(3176051, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("eosdefi22333"_n, asset(219847680, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("eosgateio222"_n, asset(50000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("eosisrealbtc"_n, asset(1623563, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("eosjunying12"_n, asset(13890000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("eoskidultlab"_n, asset(3237284, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("eosmillion12"_n, asset(1525114, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("eosotcotc111"_n, asset(2389000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("eospluser.tp"_n, asset(15300000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("eoswallet342"_n, asset(3880000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("eoswanglawhu"_n, asset(13105288, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("eosworld1115"_n, asset(73119100, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("eosxhmxhm123"_n, asset(1184000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("eosxhmxhm321"_n, asset(12716300, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("eszhengzhike"_n, asset(1715754, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("ez2eosfamily"_n, asset(35219573, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("fanliting111"_n, asset(7000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("g44tgmzwgage"_n, asset(22558838, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("geydknrqg4ge"_n, asset(2242502, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("gezdcobzgqge"_n, asset(10793700, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("gm3tmmjshege"_n, asset(10600829, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("gmytinzrgage"_n, asset(154891185, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("gotoswimming"_n, asset(22727702, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("guydombzgage"_n, asset(36508221, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("guydqnbzgmge"_n, asset(3322558, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("gy4danzsgige"_n, asset(7727335, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("gyzdqojugmge"_n, asset(5565184, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("gyztimztg4ge"_n, asset(31730135, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("ha2timjqgige"_n, asset(5000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("ha4dqnzsgmge"_n, asset(332653875, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("hekey5134qih"_n, asset(3055683, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("herroyuycary"_n, asset(400362831, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("herroyuyjinh"_n, asset(2297492, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("heydenjugmge"_n, asset(3280000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("jaicrytpo.tp"_n, asset(55000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("jamzbondz123"_n, asset(1864627, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("jawarido.ftw"_n, asset(9640000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("jcortez24412"_n, asset(20727843, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("jdghhdyssrso"_n, asset(4600000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("jianglinxi11"_n, asset(12000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("jlbcnt1wg.tp"_n, asset(43941603, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("joah12345333"_n, asset(1496000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("johanewa1234"_n, asset(3878928, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("juarjol.ftw"_n, asset(1924692, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("kapo.ftw"_n, asset(7865107, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("kranthi3.ftw"_n, asset(1340000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("lepyotlo.ftw"_n, asset(15459629, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("lovehuangmin"_n, asset(298901783, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("lovenieflsky"_n, asset(26282923, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("lovetomorrow"_n, asset(500157673, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("maltaeosprog"_n, asset(95200706, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("maoc1414.ftw"_n, asset(10826179, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("mariaioa.ftw"_n, asset(1630613, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("mazak.ftw"_n, asset(1000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("mosstheboss1"_n, asset(88408687, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("mykeyhulu525"_n, asset(3904858, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("mysteorrrrrr"_n, asset(2000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("nanjinshitou"_n, asset(1000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("pen1s.ftw"_n, asset(1278025, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("piemuran1234"_n, asset(15000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("pq2ketd4cdxr"_n, asset(1497732, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("primenumber2"_n, asset(21378627, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("realdonnypie"_n, asset(3200534, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("reyhubeosacc"_n, asset(1676395, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("rha222222thm"_n, asset(40000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("savindolphin"_n, asset(1099026, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("shaojun11111"_n, asset(1887472, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("shikongdeeos"_n, asset(38678200, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("tabuzah13.m"_n, asset(38957910, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("throne2thorn"_n, asset(2000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("timer11.ftw"_n, asset(2000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("uf55555cttyc"_n, asset(7345418, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("uirbnrjyc2v2"_n, asset(1669230, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("vrs5nhk2t4e4"_n, asset(4000097, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("wangmingwang"_n, asset(1927405, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("whatisaname1"_n, asset(169757883, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("xiaoyezi5555"_n, asset(602920000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("xiaoyuxiaoyu"_n, asset(50000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("yangyijun123"_n, asset(6000000, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("youcuozhijia"_n, asset(4974375, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("yucel1453145"_n, asset(22295322, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("zengzhiwei22"_n, asset(91150405, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("zyzlovejf.tp"_n, asset(4439917, "DOP"), asset(0,"DOP")) ;
    addwhitelistdop("zztaoyong.tp"_n, asset(1230000, "DOP"), asset(0,"DOP")) ;
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

    uint8_t bonus = total_it->bonus;
    totalswapped.modify(total_it, get_self(), [&](auto& row) 
    {
        row.dop_total_swapped += swap_quantity_dop; 
    });

    swaptable swapped(get_self(), get_self().value);

    auto swapped_it = swapped.find(owner_account.value);
    if(swapped_it == swapped.end()) 
    {
        swapped.emplace(get_self(), [&](auto& row)
        {
            row.owner_account = owner_account;
            row.dop_swapped_amount = swap_quantity_dop; 
        });
    } 
    else 
    {
        swapped.modify(swapped_it, get_self(), [&](auto& row) 
        {   
            if (row.dop_swapped_amount.amount == 0)
                row.dop_swapped_amount = swap_quantity_dop;
            else
                row.dop_swapped_amount += swap_quantity_dop;
        });
    }

    auto swapped_it2 = swapped.find(owner_account.value);
    check(swapped_it2 != swapped.end(), "Critical Table Error in setting received_amount"); 

    /* Calculate the rewards by looking at the bonus, if bonus == 0 then just: */
    if (bonus == 0)
    {
        asset new_swap_quantity = swap_quantity_dop;
        new_swap_quantity.amount = swap_quantity_dop.amount/10000;
        new_swap_quantity.symbol = symbol("DOP", 4);
        swapcontrak::inline_transferdop(get_self(), owner_account, new_swap_quantity, "Sending rewards with zero bonus.");
        /* Modify received_amount in the swapped table */
        swapped.modify(swapped_it2, get_self(), [&](auto& row) 
        {   
            if (row.dop_received_amount.amount == 0)
                row.dop_received_amount = new_swap_quantity; // Let's see if we can delete this if else here and just add the +=
            else
                row.dop_received_amount += new_swap_quantity;
        });
    }
    /* Otherwise, we send him more coins */
    else
    {
        /* Send the rewards + the bonuses */
        asset new_swap_quantity = swap_quantity_dop;
        new_swap_quantity.amount = swap_quantity_dop.amount/10000;
        new_swap_quantity.symbol = symbol("DOP", 4);

        asset new_swap_quantity2 = new_swap_quantity + (new_swap_quantity * bonus/100); // Give the appropiate bonus.
        
        swapcontrak::inline_transferdop(get_self(), owner_account, new_swap_quantity2, "Sending rewards with bonus.");
        /* Modify received_amount in the swapped table */
        swapped.modify(swapped_it2, get_self(), [&](auto& row) 
        {   
            if (row.dop_received_amount.amount == 0)
                row.dop_received_amount = new_swap_quantity2;
            else
                row.dop_received_amount += new_swap_quantity2;
        });
    }

    // \|/

    /* Check to see if the owner account can actually swap the requested amount of DOP. Look into the whitelist table. */
    /* Check here if the owner_account is found in the whitelist table and if their swap quantity is lower or equal to their remaining_swappable_quantity. */

    /* If everything checks out, we will substract the remaining_qty, add to the claimed_qty (without the bonuses) */
    /* Then we will send him the rewards + whatever bonus may be, the bonuses are not recorded anywhere and we will use the base values for the whitelist table */

    // /|\

   
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
    entrytable userwhitelist( get_self(), get_self().value ); 
    auto whitelist_it = userwhitelist.find( owner_account.value );
    check(whitelist_it != userwhitelist.end(), "error: user is not whitelisted.");
    check(whitelist_it->whitelist_remaining_dmd.quantity >= swap_quantity_dmd.quantity);
    /* <------------> */
    /* <------------> */

    // At this point, everything should be OK. We will substract the user's remaining and claimed from the whitelist table, and modify the totaltable;

    totalswapped.modify(total_it, get_self(), [&](auto& row) 
    {
        row.dmd_total_swapped += converted_swap_qty; 
    });

    swaptable swapped(get_self(), get_self().value);
    
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
