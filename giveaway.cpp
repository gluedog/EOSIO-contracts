#include <nftgiveaway.hpp>
/*
NFT Giveaway contrak.

Users send 0.1 EOS to the contract to be enrolled in the giveaway.

We will use current_blocktime for each on_notify tx to create the seed for the pseudo RNG.

We will have a locked and unlocked variable which determines the time limit when users can enroll in the giveaway.
*/

void nftgiveaway::setlocked(bool locked)
{ /* This function lets us set the 'locked' variable true or false to determine if people can still send EOS and be enrolled in the giveaway */
    require_auth(get_self());

    totaltable global(get_self(), "totals"_n.value); 
    auto total_it = global.find("totals"_n.value);
    if (total_it == global.end())
    {
        global.emplace(get_self(), [&]( auto& row) 
        {
            row.key    = "totals"_n;
            row.locked = locked;
        });
    }
    else
    {
        global.modify(total_it, get_self(), [&]( auto& row) 
        {
            row.locked = locked;
        });
    }
}

[[eosio::on_notify("eosio.token::transfer")]]
void nftgiveaway::registergiveaway(const name& owner_account, const name& to, const asset& amount_eos_sent, std::string memo)
{
    check(amount_eos_sent.symbol == eos_symbol, "error: these are not the droids you are looking for.");
    // Checks how much EOS has been sent and what the memo is.
    // Registers the user in the buyers and mints tables. Updates the total table.
    if (to != get_self() || owner_account == get_self())
    {
        print("error: these are not the droids you are looking for.");
        return;
    }

    uint16_t giveaway_entry_price = 1000; // 0.1 EOS

    check(amount_eos_sent.amount >= giveaway_entry_price, "error: you must send 0.1 EOS to participate in the giveaway.");

    playertable players(get_self(), get_self().value);
    auto players_it = players.find(owner_account.value);
    check(players_it == players.end(), "error: account has already been registered in the giveaway."); 

    eosio::print_f("User has sent: [%] EOS \n",amount_eos_sent.amount);
    eosio::print_f("Entry Price:   [%]\n",giveaway_entry_price);

    // Update the table(s):
    uint32_t now = current_time_point().sec_since_epoch();
    players.emplace(get_self(), [&](auto& row) 
    {
        row.owner_account = owner_account;
        row.seed = now;
    });

    uint32_t refund_amount = amount_eos_sent.amount - giveaway_entry_price;

    eosio::print_f("Refund for user: [%]\n",refund_amount);
    if (refund_amount > 0)
    {
        asset refund_asset = amount_eos_sent;
        refund_asset.amount = refund_amount;
        nftgiveaway::inline_transfereos(get_self(), owner_account, refund_asset, "Sending refund for the NFT giveaway entry.");
    }
}


/*
TODO:
Finish getwinners function.
Add the locked variable into play to stop registrations once the contract is locked.

*/

/*

void nftgiveaway::getwinners()
{
    staketable staked(get_self(), get_self().value);

    auto begin_itr = staked.begin();
    auto end_itr = staked.end();

    uint16_t current_iteration = 0;
    while (begin_itr != end_itr)
    {
        eosio::print_f("Checking staking information for: [%]\n",begin_itr->owner_account);
        staked.modify(begin_itr, get_self(), [&]( auto& row ) 
        { // Modify the table entries to increase user's "unclaimed_amount"
            if (row.hub_staked_amount.amount > 0)
            {
            }
        });
        ++ begin_itr;
        ++ current_iteration;
    }

*/    

/*
array = [18246363, 18246812, 18246444, 18246123, 18246263, 18246991, 18246223, 18246483, 18246618, 18246231, 1824298]


# 11 numbers, let's try and select 5 positions out of these 11

maxwinners = 5
winnerpositions = []
counter = 0

flag = False

for seed in array:
    position = seed%len(array)

    for winner in winnerpositions:
        if position == winner:
            flag = True

    if flag == True:
        # Get new position
        continue
    else:
        winnerpositions.append(position)

    if len(winnerpositions) == 5:
        break


print(winnerpositions)

        Get all the seeds from the users.
        Just use those to seed the RNG.
        Refund all the users their 0.1 EOS.

        So basically what we have, is the current blocktime at which everyone sent 0.1 EOS, so we can iterate over those to get some seeds.
        We also have the blocktime at which we call getwinners().
        So we need this function to pick 25 winners out of.... however many there are in total.
        So we can do 25 loops, that each select a number between 1 and max_players.
        The algorithm would be pseudo_random_seed % max_players. Check for duplicates, if duplicate, add another step.
    */
