#include <burn.hpp>
/*
A simple contract, to which users send DOP, HUB or DMD, and then the contract burns the coins.
*/

// Need to have another function that handles the receiving of the xTokens.

[[eosio::on_notify("dop.efi::transfer")]]
void burncontrak::burndop(const name& owner_account, const name& to, const asset& burn_quantity_dop, std::string memo)
{
    /*
        We'll have to check some things on every one of these transfers.
        Need to have some variables that show if the xToken season is open or not.
        Need to not allow people to burn their coins if there are no more xTokens to give out.
        xseason_burn_open = True or False
        If it's open, but there's no xTokens to give out, the whole TX fails with an error message informing the user.

        Now, we'll also need to listen for incoming xToken transfers, if the season is open, then the contract will accept the xToken transfer,
          and send it to another account, `funds.efi` immediately and adds the user to the xToken Stakers table.

        The xTokens staking table is fairly simple, we'll just have owner_account and the amount of xTokens staked.
        As soon as the xseason_burn_open becomes False, then we can start the actual Staking event. It is assumed that at this point, 
         everyone has burned their coins and send xTokens to the contract to be registered and ready for staking. No more xTokens can be staked after that, so the rewards
         will always be consistent from issue() to issue(), no new xTokens are being staked.
         The issue() will basically be the same as we had in the stake.efi contract with the worker.efi calling it. Calculated by the second.
         The worker will check in every issue() if the user still has the NFT in their account... If so, they'll get double the rewards for that cycle,
           no additional calculations needed, just rewards*2 if they hold an NFT.
    */
    if (to != get_self() || owner_account == get_self())
    {
        print("*these are not the droids you are looking for*");
        return;
    }

    totaltable burnerdata(get_self(), "totals"_n.value); 

    auto total_it = burnerdata.find("totals"_n.value);

    check(total_it != burnerdata.end(), "error: totals table is not initiated.");
    check(total_it->xseason_burn_open == true, "error: can not burn DOP for xDOP at this time, the time window is closed.");


    check(owner_account == "funds.efi"_n, "error: the burning is not yet open to the public.");
    check(burn_quantity_dop.amount >= 100000, "error: must burn a minimum of 10 DOP");
    check(burn_quantity_dop.symbol == dop_symbol, "error: these are not the droids you are looking for.");

    std::string custom_memo;

    if (burn_quantity_dop.amount <= 1000000)
        custom_memo = "A pitiful sacrifice!";
    else if ((burn_quantity_dop.amount >= 1000000) && (burn_quantity_dop.amount < 10000000))
        custom_memo = "A worthy sacrifice to the Market Gods!";
    else if ((burn_quantity_dop.amount >= 10000000) && (burn_quantity_dop.amount < 100000000))
        custom_memo = "An incredible sacrifice to the Market Gods!";
    else if ((burn_quantity_dop.amount >= 100000000) && (burn_quantity_dop.amount < 1000000000))
        custom_memo = "A godlike sacrifice to the Market Gods.";
    else
        custom_memo = "Wesa goen tada moon, sers!";

    burncontrak::inline_burndop(get_self(), burn_quantity_dop, custom_memo);

    // Here we will give the user the same amount of xTokens as the initial coins he had burned:
    asset xdop_send_amount = asset(burn_quantity_dop.amount, symbol("XDOP", 4)); // should test with xhub_symbol instyead of symbol("XDOP", 4) and see if that works.
    burncontrak::inline_transferxtoken(get_self(), owner_account, xdop_send_amount, "I've burned my DOP to get xDOP!")
    // At this point, the user has the xTokens. He has to wait until the time window closes, then he can start staking the xDOP.
}

[[eosio::on_notify("hub.efi::transfer")]]
void burncontrak::burnhub(const name& owner_account, const name& to, const asset& burn_quantity_hub, std::string memo)
{
    if (to != get_self() || owner_account == get_self())
    {
        print("*these are not the droids you are looking for*");
        return;
    }

    totaltable burnerdata(get_self(), "totals"_n.value); 

    auto total_it = burnerdata.find("totals"_n.value);

    check(total_it != burnerdata.end(), "error: totals table is not initiated.");
    check(total_it->xseason_burn_open == true, "error: can not burn HUB for xHUB at this time, the time window is closed.");


    check(owner_account == "funds.efi"_n, "error: the burning is not yet open to the public.");
    check(burn_quantity_hub.amount >= 100000, "error: must burn a minimum of 10 HUB");
    check(burn_quantity_hub.symbol == hub_symbol, "error: these are not the droids you are looking for.");

    std::string custom_memo;

    if (burn_quantity_hub.amount <= 1000000)
        custom_memo = "A pitiful sacrifice!";
    else if ((burn_quantity_hub.amount >= 1000000) && (burn_quantity_hub.amount < 10000000))
        custom_memo = "A worthy sacrifice to the Market Gods!";
    else if ((burn_quantity_hub.amount >= 10000000) && (burn_quantity_hub.amount < 100000000))
        custom_memo = "An incredible sacrifice to the Market Gods!";
    else if ((burn_quantity_hub.amount >= 100000000) && (burn_quantity_hub.amount < 1000000000))
        custom_memo = "A godlike sacrifice to the Market Gods.";
    else
        custom_memo = "Wesa goen tada moon, sers!";

    burncontrak::inline_burnhub(get_self(), burn_quantity_hub, custom_memo);

    // Here we will give the user the same amount of xTokens as the initial coins he had burned:
    asset xhub_send_amount = asset(burn_quantity_hub.amount, symbol("XHUB", 4)); // should test with xhub_symbol instead of symbol("XHUB", 4) and see if that works.
    burncontrak::inline_transferxtoken(get_self(), owner_account, xhub_send_amount, "I've burned my HUB to get xHUB!")
}

[[eosio::on_notify("dmd.efi::transfer")]]
void burncontrak::burndmd(const name& owner_account, const name& to, const asset& burn_quantity_dmd, std::string memo)
{
    if (to != get_self() || owner_account == get_self())
    {
        print("*these are not the droids you are looking for*");
        return;
    }

    totaltable burnerdata(get_self(), "totals"_n.value); 

    auto total_it = burnerdata.find("totals"_n.value);

    check(total_it != burnerdata.end(), "error: totals table is not initiated.");
    check(total_it->xseason_burn_open == true, "error: can not burn DMD for xDMD at this time, the time window is closed.");


    check(owner_account == "funds.efi"_n, "error: the burning is not yet open to the public.");
    check(burn_quantity_dmd.amount >= 100000, "error: must burn a minimum of 10 DMD");
    check(burn_quantity_dmd.symbol == dmd_symbol, "error: these are not the droids you are looking for.");

    std::string custom_memo;

    if (burn_quantity_dmd.amount <= 1000000)
        custom_memo = "A pitiful sacrifice!";
    else if ((burn_quantity_dmd.amount >= 1000000) && (burn_quantity_dmd.amount < 10000000))
        custom_memo = "A worthy sacrifice to the Market Gods!";
    else if ((burn_quantity_dmd.amount >= 10000000) && (burn_quantity_dmd.amount < 100000000))
        custom_memo = "An incredible sacrifice to the Market Gods!";
    else if ((burn_quantity_dmd.amount >= 100000000) && (burn_quantity_dmd.amount < 1000000000))
        custom_memo = "A godlike sacrifice to the Market Gods.";
    else
        custom_memo = "Wesa goen tada moon, sers!";

    burncontrak::inline_burndmd(get_self(), burn_quantity_dmd, custom_memo);

    // Here we will give the user the same amount of xTokens as the initial coins he had burned:
    asset xdmd_send_amount = asset(burn_quantity_dmd.amount, symbol("XDMD", 4)); // should test with xdmd_symbol instead of symbol("XDMD", 4) and see if that works.
    burncontrak::inline_transferxtoken(get_self(), owner_account, xdmd_send_amount, "I've burned my DMD to get xDMD!")
}

void burncontrak::set(bool xseason_burn_open)
{   /* Never implemented the xToken burning */
    require_auth(get_self());
    totaltable burnerdata(get_self(), "totals"_n.value); 

    auto total_it = burnerdata.find("totals"_n.value);
    if(total_it == burnerdata.end()) 
    {
        burnerdata.emplace(get_self(), [&]( auto& row) 
        {
            row.key = "totals"_n;     // Three stages to the xToken Burning seasons:
            row.xseason_burn_open     = xseason_burn_open;      // This tells us if users are allowed to burn their coins and receive xTokens.
            row.xseason_stake_open    = xseason_stake_open;    //  This tells us if the users are allowed to stake their xTokens.
            row.xseason_rewards_open  = xseason_rewards_open; //   This tells us if rewards are being given out to users.
            // Only one can be true at any given time, either that, or none are true. 
            // Will need to implement sanity checks to ensure that only one is true at any given time, or none. Otherwise everything will be locked.
        });
    } 
    else
    { 
        totalswapped.modify(total_it, get_self(), [&](auto& row) 
        {
            row.xseason_burn_open     = xseason_burn_open;      // This tells us if users are allowed to burn their coins and receive xTokens.
            row.xseason_stake_open    = xseason_stake_open;    //  This tells us if the users are allowed to stake their xTokens.
            row.xseason_rewards_open  = xseason_rewards_open; //   This tells us if rewards are being given out to users.
        });
    }
}
