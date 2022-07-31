#include <stake.hpp>
/*
This is the EFi V2 Steak Contract.
*/
void stake::setlocked(bool locked)
{ /* This function lets us set the 'locked' variable true or false for the 31 day staking. */
    require_auth(get_self());

    totaltable totalstaked(get_self(), "totals"_n.value); 
    auto total_it = totalstaked.find("totals"_n.value);
    if(total_it != totalstaked.end()) 
    {
        uint32_t now = current_time_point().sec_since_epoch();
        totalstaked.modify(total_it, get_self(), [&]( auto& row ) 
        {
            row.locked = locked;
            row.last_reward_time = now;
        });
    } 
    else 
        return;
}
/* Setter function that needs to be called after the contract is deployed to initialize the totals table. */
void stake::set(const asset& total_staked_hub, const asset& total_staked_dop, const asset& total_staked_dmd, 
            uint32_t hub_issue_frequency, uint32_t dop_issue_frequency, uint32_t dmd_issue_frequency)
{
    require_auth(get_self());
    totaltable totalstaked(get_self(), "totals"_n.value);

    uint32_t now = current_time_point().sec_since_epoch();

    auto total_it = totalstaked.find("totals"_n.value);
    if(total_it == totalstaked.end()) 
    {
        totalstaked.emplace(get_self(), [&](auto& row) 
        {
            row.key                   = "totals"_n;
            row.hub_total_staked      = total_staked_hub;
            row.dop_total_staked      = total_staked_dop;
            row.dmd_total_staked      = total_staked_dmd;
            /* We'll always set the locked to zero (false) on set(). We have to do two things to initialize the staking */
            row.locked                = 0; 
            /* How many coins are issued per second (will be multiplied by 1000 to fit the precision) */
            row.hub_issue_frequency   = hub_issue_frequency;
            row.dop_issue_frequency   = dop_issue_frequency;
            row.dmd_issue_frequency   = dmd_issue_frequency;
            row.last_reward_time = now;
        });
    } 
    else 
        totalstaked.modify(total_it, get_self(),[&]( auto& row) 
        {
            row.hub_total_staked      = total_staked_hub;
            row.dop_total_staked      = total_staked_dop;
            row.dmd_total_staked      = total_staked_dmd;
            /* We'll always set the locked to zero on set(). We have to do two things to initialize the staking */
            row.locked                = 0; 
            /* How many coins are issued per second (will be multiplied by 1000 to fit the precision) */
            row.hub_issue_frequency   = hub_issue_frequency;
            row.dop_issue_frequency   = dop_issue_frequency;
            row.dmd_issue_frequency   = dmd_issue_frequency;
            row.last_reward_time = now;
        });
}
// For added security, we can feed the staking contract with a bit of coins every day, so we don't have to give it the maximum number of coins.
void stake::issue()
{/* -> Looks at the time passed since last issue() call.
    -> The issue() action will need to calculate what to add to the "unclaimed_rewards" field value. 
    -> The "unclaimed_rewards" are zeroed when the user calls claim() successfully and receives all unclaimed rewards.
    -> The issue() action will issue the same amount of tokens for every second that passed since the last_reward_time field value. 
    -> The new tokens are issued to that period's pool. Then, it will update the unclaimed_reward field for each user. 
    -> Looks at what percentage of the period's pool the user has and then, it adds the appropiate value in the unclaimed_reward(). */
    require_auth( "worker.efi"_n ); /* issue() is called by the efi worker */

    totaltable totalstaked(get_self(), "totals"_n.value); 
    auto total_it = totalstaked.find("totals"_n.value);
    check(total_it != totalstaked.end(), "error: totals table is not initiated."); 

    bool locked = total_it->locked;
    check(locked == true, "error: staking has ended.");

    uint32_t last_reward_time     = total_it->last_reward_time;
    uint16_t issue_precision      = 10000; // This means that if we set ("issue_frequency" == 100): we release 0.01 token per second.
                                           //          and if we set ("issue_frequency" == 1):   we release 0.0001 tokens per second.

    // Coins issued every second. Multiplied by 10000 for tokens with precision 4. Divided by "issue_precision" for extra control:
    uint32_t hub_issue_frequency  = total_it->hub_issue_frequency*10000/issue_precision;
    uint32_t dop_issue_frequency  = total_it->dop_issue_frequency*10000/issue_precision;
    uint32_t dmd_issue_frequency  = total_it->dmd_issue_frequency*10000/issue_precision;
    uint64_t hub_total_staked     = total_it->hub_total_staked.amount;
    uint64_t dop_total_staked     = total_it->dop_total_staked.amount;
    uint64_t dmd_total_staked     = total_it->dmd_total_staked.amount;

    // How many seconds have passed until now and last_reward_time:
    uint32_t now = current_time_point().sec_since_epoch();
    uint32_t seconds_passed = now - last_reward_time;
    eosio::print_f("Seconds passed since last issue(): [%] \n",seconds_passed);

    // Determine how much total reward should be issued in this period for each of the coins: hub, dmd, dop.
    uint64_t total_hub_released = seconds_passed * hub_issue_frequency;
    uint64_t total_dop_released = seconds_passed * dop_issue_frequency;
    uint64_t total_dmd_released = seconds_passed * dmd_issue_frequency;

    // We will iterate through the "staketable staked" table, and update each user's unclaimed_amount() field
    // Original iteration code: https://developers.eos.io/manuals/eosio.cdt/v1.8/how-to-guides/key-value-api/kv_table/how-to-iterate-kv-table
    staketable staked(get_self(), get_self().value);

    auto begin_itr = staked.begin();
    auto end_itr = staked.end();

    while (begin_itr != end_itr)
    {
        eosio::print_f("Checking staking information for: [%]\n",begin_itr->owner_account);
        staked.modify(begin_itr, get_self(), [&]( auto& row ) 
        { /* Modify the table entries to increase user's "unclaimed_amount" */
            if (row.hub_staked_amount.amount > 0){
                float hub_percentage = float(row.hub_staked_amount.amount)/float(hub_total_staked) * 100; 
                row.hub_unclaimed_amount.amount += (hub_percentage*total_hub_released)/0.01/10000;} /* (divided by 10000) for coins with precision 4 */

            if (row.dop_staked_amount.amount > 0){
                float dop_percentage = float(row.dop_staked_amount.amount)/float(dop_total_staked) * 100;
                row.dop_unclaimed_amount.amount += (dop_percentage*total_dop_released)/0.01/10000;} /* (divided by 10000) for coins with precision 4 */

            if (row.dmd_staked_amount.amount > 0){
                float dmd_percentage = float(row.dmd_staked_amount.amount)/float(dmd_total_staked) * 100;
                row.dmd_unclaimed_amount.amount += (dmd_percentage*total_dmd_released)/0.01/10000;} /* (divided by 10000) for coins with precision 4 */
        });
        ++ begin_itr;
    }
    /* Log the last reward time */
    totalstaked.modify(total_it, get_self(), [&](auto& row) 
    {
        row.last_reward_time = now;
    });
}
/* This is the claim() action that the users call to claim their unclaimed HUB rewards */
void stake::claimhub(const name& owner_account)
{
    require_auth(owner_account);

    staketable staked(get_self(), get_self().value);
    auto staked_it = staked.find(owner_account.value);
    check(staked_it != staked.end(), "error: no deposits detected. stake some coins first."); 

    asset hub_unclaimed = staked_it->hub_unclaimed_amount;
    check(hub_unclaimed.amount != 0,"error: no HUB available to claim.");
    /* Transfer the user his hub, and then set his "hub_unclaimed_amount" field to zero and also updates "hub_claimed_amount" */
    stake::inline_transferhub(get_self(), owner_account, hub_unclaimed, "I'm claiming HUB!!!");
    staked.modify(staked_it, get_self(), [&](auto& row) 
    {   
        row.hub_unclaimed_amount.amount = 0;
        row.hub_claimed_amount.amount += hub_unclaimed.amount;
    });
}
/* This is the claim() action that the users call to claim their unclaimed DOP rewards */
void stake::claimdop(const name& owner_account)
{
    require_auth(owner_account);

    staketable staked(get_self(), get_self().value);
    auto staked_it = staked.find( owner_account.value);
    check(staked_it != staked.end(), "error: no deposits detected. stake some coins first."); 

    asset dop_unclaimed = staked_it->dop_unclaimed_amount;
    check(dop_unclaimed.amount != 0,"error: no DOP available to claim.");
    /* Transfer the user his dop, and then set his "dop_unclaimed_amount" field to zero and also updates "dop_claimed_amount" */
    stake::inline_transferdop(get_self(), owner_account, dop_unclaimed, "I'm claiming DOP!!!");
    staked.modify(staked_it, get_self(), [&](auto& row) 
    {   
        row.dop_unclaimed_amount.amount = 0;
        row.dop_claimed_amount.amount += dop_unclaimed.amount;
    });
}
/* This is the claim() action that the users call to claim their unclaimed DMD rewards */
void stake::claimdmd(const name& owner_account)
{
    require_auth(owner_account);

    staketable staked(get_self(), get_self().value);
    auto staked_it = staked.find(owner_account.value);
    check(staked_it != staked.end(), "error: no deposits detected. stake some coins first."); 

    asset dmd_unclaimed = staked_it->dmd_unclaimed_amount;
    check(dmd_unclaimed.amount != 0,"error: no DMD available to claim.");
    /* Transfer the user his dmd, and then set his "dmd_unclaimed_amount" field to zero and also updates "dmd_claimed_amount" */
    stake::inline_transferdmd(get_self(), owner_account, dmd_unclaimed, "I'm claiming DMD!!!");
    staked.modify(staked_it, get_self(), [&](auto& row) 
    {   
        row.dmd_unclaimed_amount.amount = 0;
        row.dmd_claimed_amount.amount += dmd_unclaimed.amount;
    });
}

void stake::withdraw(const name& owner_account)
{      /* Lets the user withdraw his "staked_amount" */
/* Only proceed if locked == false (the staking period is over) */
    require_auth(owner_account);

    totaltable totalstaked(get_self(), "totals"_n.value);
    auto total_it = totalstaked.find("totals"_n.value);
    check(total_it != totalstaked.end(), "error: totals table is not initiated."); 
    check(total_it->locked == false, "error: you can not withdraw your stake before the locking period ends.");

    staketable staked(get_self(), get_self().value);
    auto staked_it = staked.find(owner_account.value);
    check(staked_it != staked.end(), "error: no deposits detected for user.");

    bool empty = false;
    if (staked_it->hub_staked_amount.amount == 0 && staked_it->dop_staked_amount.amount == 0 && staked_it->dmd_staked_amount.amount == 0)
        empty = true;

    check(!empty, "error: no deposits detected for user*.");

    if (staked_it->hub_staked_amount.amount > 0)
    {
        stake::inline_transferhub(get_self(), owner_account, staked_it->hub_staked_amount, "I'm withdrawing HUB from V2 staking!!!");

        totalstaked.modify(total_it, get_self(), [&](auto& row){
            row.hub_total_staked -= staked_it->hub_staked_amount;
        });
        staked.modify(staked_it, get_self(), [&](auto& row){   
            row.hub_staked_amount.amount = 0;
        });
    }

    if (staked_it->dop_staked_amount.amount > 0)
    {
        stake::inline_transferdop(get_self(), owner_account, staked_it->dop_staked_amount, "I'm withdrawing DOP from V2 staking!!!");

        totalstaked.modify(total_it, get_self(), [&](auto& row){
            row.dop_total_staked -= staked_it->dop_staked_amount;
        });
        staked.modify(staked_it, get_self(), [&](auto& row){   
            row.dop_staked_amount.amount = 0;
        });
    }

    if (staked_it->dmd_staked_amount.amount > 0){
        stake::inline_transferdmd(get_self(), owner_account, staked_it->dmd_staked_amount, "I'm withdrawing DMD from V2 staking!!!");

        totalstaked.modify(total_it, get_self(), [&](auto& row) {
            row.dmd_total_staked -= staked_it->dmd_staked_amount;
        });
        staked.modify(staked_it, get_self(), [&](auto& row){   
            row.dmd_staked_amount.amount = 0;
        });}
}
/* This is what happens when users send DMD , HUB or DOP to the V2 Staking contract or when they click Stake on the website. */
[[eosio::on_notify("hub.efi::transfer")]]
void stake::stakehub(const name& owner_account, const name& to, const asset& stake_quantity_hub, std::string memo)
{
    if (to != get_self() || owner_account == get_self())
    {
        print("error: these are not the droids you are looking for.");
        return;
    }

    if (owner_account == "efi"_n)
        return;  /* This account can always send HUB to the staking contract to top it off */

    check(stake_quantity_hub.amount >= 500000, "error: must stake a minimum of 50 HUB!");
    check(stake_quantity_hub.symbol == hub_symbol, "error: these are not the droids you are looking for.");

    totaltable totalstaked(get_self(), "totals"_n.value);
    auto total_it = totalstaked.find("totals"_n.value);
    check(total_it != totalstaked.end(), "error: totals table is not initiated."); 
    /* We have to check to see if we can get the value of locked right here */
    /* We also have to check to see if locked == false, and if so, we won't allow anyone to send EOS */
    bool locked = total_it->locked;
    check(locked == true, "error: the staking event has ended.");

    /* Update the total staked variable in the table */
    totalstaked.modify(total_it, get_self(), [&](auto& row) 
    {
        row.hub_total_staked += stake_quantity_hub;
    });

    staketable staked(get_self(), get_self().value);

    auto staked_it = staked.find(owner_account.value);
    if(staked_it == staked.end())
    { /* First time depositing anything into staking */
        staked.emplace(get_self(), [&](auto& row)
        {
            row.owner_account = owner_account;
            row.hub_staked_amount = stake_quantity_hub;
            row.hub_unclaimed_amount = stake_quantity_hub;
            row.hub_claimed_amount = stake_quantity_hub;
            row.hub_unclaimed_amount.amount = 0;
            row.hub_claimed_amount.amount = 0;
        });
    }
    else if (staked_it->hub_staked_amount.amount == 0)
    { /* First time depositing HUB, but he has deposited another type of coin before */
        staked.modify(staked_it, get_self(), [&](auto& row) 
        {   
            row.hub_staked_amount = stake_quantity_hub;
            row.hub_unclaimed_amount = stake_quantity_hub;
            row.hub_claimed_amount = stake_quantity_hub;
            row.hub_unclaimed_amount.amount = 0;
            row.hub_claimed_amount.amount = 0;
        });
    }
    else
    { /* It's not his first time depositing this type of coin */
        staked.modify(staked_it, get_self(), [&](auto& row) 
        {   
            row.hub_staked_amount.amount += stake_quantity_hub.amount;
        });
    }
}

[[eosio::on_notify("dop.efi::transfer")]]
void stake::stakedop(const name& owner_account, const name& to, const asset& stake_quantity_dop, std::string memo)
{
    if (to != get_self() || owner_account == get_self())
    {
        print("error: these are not the droids you are looking for.");
        return;
    }

    if (owner_account == "efi"_n)
        return;  /* This account can always send DOP to the staking contract to top it off */

    check(stake_quantity_dop.amount >= 500000, "error: must stake a minimum of 50 DOP!");
    check(stake_quantity_dop.symbol == dop_symbol, "error: these are not the droids you are looking for.");

    totaltable totalstaked(get_self(), "totals"_n.value);
    auto total_it = totalstaked.find("totals"_n.value);
    check(total_it != totalstaked.end(), "error: totals table is not initiated."); 
    /* We have to check to see if we can get the value of locked right here */
    /* We also have to check to see if locked == false, and if so, we won't allow anyone to send EOS */
    bool locked = total_it->locked;
    check(locked == true, "error: the staking event has ended.");

    /* Update the total staked variable in the table */
    totalstaked.modify(total_it, get_self(), [&](auto& row) 
    {
        row.dop_total_staked += stake_quantity_dop;
    });

    staketable staked(get_self(), get_self().value);

    auto staked_it = staked.find(owner_account.value);
    if(staked_it == staked.end())
    { /* First time depositing anything into staking */
        staked.emplace(get_self(), [&](auto& row)
        {
            row.owner_account = owner_account;
            row.dop_staked_amount = stake_quantity_dop;
            row.dop_unclaimed_amount = stake_quantity_dop;
            row.dop_claimed_amount = stake_quantity_dop;
            row.dop_unclaimed_amount.amount = 0;
            row.dop_claimed_amount.amount = 0;
        });
    }
    else if (staked_it->dop_staked_amount.amount == 0)
    { /* First time depositing DOP, but he has deposited another type of coin before */
        staked.modify(staked_it, get_self(), [&](auto& row) 
        {   
            row.dop_staked_amount = stake_quantity_dop;
            row.dop_unclaimed_amount = stake_quantity_dop;
            row.dop_claimed_amount = stake_quantity_dop;
            row.dop_unclaimed_amount.amount = 0;
            row.dop_claimed_amount.amount = 0;
        });
    }
    else
    { /* It's not his first time depositing this type of coin */
        staked.modify(staked_it, get_self(), [&](auto& row) 
        {   
            row.dop_staked_amount.amount += stake_quantity_dop.amount;
        });
    }
}

[[eosio::on_notify("dmd.efi::transfer")]]
void stake::stakedmd(const name& owner_account, const name& to, const asset& stake_quantity_dmd, std::string memo)
{
    if (to != get_self() || owner_account == get_self())
    {
        print("error: these are not the droids you are looking for.");
        return;
    }

    if (owner_account == "efi"_n)
        return;  /* This account can always send DMD to the staking contract to top it off */

    check(stake_quantity_dmd.amount >= 5000, "error: must stake a minimum of 0.5 DMD!");
    check(stake_quantity_dmd.symbol == dmd_symbol, "error: these are not the droids you are looking for.");

    totaltable totalstaked(get_self(), "totals"_n.value);
    auto total_it = totalstaked.find("totals"_n.value);
    check(total_it != totalstaked.end(), "error: totals table is not initiated."); 
    /* We have to check to see if we can get the value of locked right here */
    /* We also have to check to see if locked == false, and if so, we won't allow anyone to send EOS */
    bool locked = total_it->locked;
    check(locked == true, "error: the staking event has ended.");

    /* Update the total staked variable in the table */
    totalstaked.modify(total_it, get_self(), [&](auto& row) 
    {
        row.dmd_total_staked += stake_quantity_dmd;
    });

    staketable staked(get_self(), get_self().value);

    auto staked_it = staked.find(owner_account.value);
    if(staked_it == staked.end())
    { /* First time depositing anything into staking */
        staked.emplace(get_self(), [&](auto& row)
        {
            row.owner_account = owner_account;
            row.dmd_staked_amount = stake_quantity_dmd;
            row.dmd_unclaimed_amount = stake_quantity_dmd;
            row.dmd_claimed_amount = stake_quantity_dmd;
            row.dmd_unclaimed_amount.amount = 0;
            row.dmd_claimed_amount.amount = 0;
        });
    }
    else if (staked_it->dmd_staked_amount.amount == 0)
    { /* First time depositing DMD, but he has deposited another type of coin before */
        staked.modify(staked_it, get_self(), [&](auto& row) 
        {   
            row.dmd_staked_amount = stake_quantity_dmd;
            row.dmd_unclaimed_amount = stake_quantity_dmd;
            row.dmd_claimed_amount = stake_quantity_dmd;
            row.dmd_unclaimed_amount.amount = 0;
            row.dmd_claimed_amount.amount = 0;
        });
    }
    else
    { /* It's not his first time depositing this type of coin */
        staked.modify(staked_it, get_self(), [&](auto& row) 
        {   
            row.dmd_staked_amount.amount += stake_quantity_dmd.amount;
        });
    }
}
